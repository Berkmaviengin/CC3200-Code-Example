// Standard includes
#include <stdlib.h>
#include <string.h>

// Simplelink includes
#include "simplelink.h"
#include "netcfg.h"

// Driverlib includes
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "interrupt.h"
#include "utils.h"
#include "rom.h"
#include "rom_map.h"
#include "prcm.h"
#include "pin.h"

// OS includes
#include "osi.h"

// Common interface includes
#include "gpio_if.h"
#include "uart_if.h"
#include "i2c_if.h"
#include "common.h"

// App Includes
#include "device_status.h"
#include "smartconfig.h"
#include "tmp006drv.h"
#include "bma222drv.h"
#include "pinmux.h"

#define APPLICATION_VERSION              "1.1.1"
#define AppIsim                         "Out of Box"
#define OOB_TASK_ONCELIK                 1
#define SPAWN_TASK_ONCELIK               9
#define OSI_YIGIN_BOYUTU                 2048
#define AP_SSID_LEN_MAX                 32
#define GPIO_3                          3       /* P58 - Device Mode */
#define OtoBaglanmaZamanAsimi           50      /* 5 Sec */
#define SL_DURDURMA_SURESÝ              200

typedef enum
{
  LED_OFF = 0,
  LED_ON,
  LED_BLINK
}eLEDStatus;

static const char pcDigits[] = "0123456789";
static unsigned char POST_token[] = "__SL_P_ULD";
static unsigned char GET_token_SICAKLIK[]  = "__SL_G_UTP";
static unsigned char GET_token_IVMEOLCER[]  = "__SL_G_UAC";
static unsigned char GET_token_UIC[]  = "__SL_G_UIC";
static int InternetAccess = -1;
static unsigned char KurutucuCalis = 0;
static unsigned int ModAyarlamasi = ROLE_STA; //station mode
static unsigned char LedStatus = LED_OFF;
static unsigned long  SimpleLinkStatus = 0;//SimpleLink Status
static unsigned char  SSID[SSID_LEN_MAX+1]; //Connection SSID
static unsigned char  BSSID[BSSID_LEN_MAX]; //Connection BSSID


#if defined(ccs)
extern void (* const g_pfnVectors[])(void);
#endif
#if defined(ewarm)
extern uVectorEntry __vector_table;
#endif



//***************************************************************************************
//                      FRERTOS AKTIFLESTIRME
//***************************************************************************************
#ifdef USE_FREERTOS

void vApplicationTickHook( void )
{
}

void vAssertCalled( const char *pcFile, unsigned long ulLine )
{
    while(1)
    {
    }
}

void vApplicationIdleHook( void )
{
}

void vApplicationStackOverflowHook( OsiTaskHandle *pxTask, signed char *pcTaskName)
{
    ( void ) pxTask;
    ( void ) pcTaskName;
    for( ;; );
}

void vApplicationMallocFailedHook()
{
    while(1)
  {
  }
}

#endif
//***************************************************************************************
//--------------------------------------BITIS--------------------------------------------
//***************************************************************************************



//***************************************************************************************
//                      Sicaklik ve IvmeOlcer Okuma
//***************************************************************************************
static unsigned short itoa(char cNum, char *cString)
{
    char* ptr;
    char uTemp = cNum;
    unsigned short length;

    if (cNum == 0)
    {
        length = 1;
        *cString = '0';
        return length;
    }
    length = 0;
    while (uTemp > 0)
    {
        uTemp /= 10;
        length++;
    }
    uTemp = cNum;
    ptr = cString + length;
    while (uTemp > 0)
    {
        --ptr;
        *ptr = pcDigits[uTemp % 10];
        uTemp /= 10;
    }
    return length;
}
void ReadAccSensor()
{
    const short csAccThreshold    = 5;
    signed char cAccXT1,cAccYT1,cAccZT1;
    signed char cAccXT2,cAccYT2,cAccZT2;
    signed short sDelAccX, sDelAccY, sDelAccZ;
    int iRet = -1;
    int iCount = 0;
    iRet = BMA222ReadNew(&cAccXT1, &cAccYT1, &cAccZT1);
    if(iRet)
    {
        return;
    }
    for(iCount=0;iCount<2;iCount++)
    {
        MAP_UtilsDelay((90*80*1000)); //30msec
        iRet = BMA222ReadNew(&cAccXT2, &cAccYT2, &cAccZT2);
        if(iRet)
        {
            iRet = 0;
            continue;
        }
        else
        {
            sDelAccX = abs((signed short)cAccXT2 - (signed short)cAccXT1);
            sDelAccY = abs((signed short)cAccYT2 - (signed short)cAccYT1);
            sDelAccZ = abs((signed short)cAccZT2 - (signed short)cAccZT1);
            if(sDelAccX > csAccThreshold || sDelAccY > csAccThreshold ||
               sDelAccZ > csAccThreshold)
            {
                KurutucuCalis = 1;
                break;
            }
            else
            {
                KurutucuCalis = 0;
            }
        }
    }

}
//***************************************************************************************
//--------------------------------------BITIS--------------------------------------------
//***************************************************************************************




//***************************************************************************************
//                      SIMPLELINK KULLANIMI VE TOKENLER
//***************************************************************************************
void SimpleLinkNetAppEventHandler(SlNetAppEvent_t *App) //AP icin IP ayarlamalari yapilir
{
    if(App==NULL)
    {
        UART_PRINT("NetApp pointeri bos...\n\r");
        while(1);
    }
    switch(App->Event)
    {
        case SL_NETAPP_IPV4_IPACQUIRED_EVENT: //IPV4 edilinir.
        {
            SlIpV4AcquiredAsync_t *AppEventData = NULL;
            SET_STATUS_BIT(SimpleLinkStatus,STATUS_BIT_IP_AQUIRED); //statuse edinilimis IP set edilir.
            AppEventData=&App->EventData.ipAcquiredV4; //ip verileri elde edilir.
            UART_PRINT("[NETAPP EVENT] Bulunan IP: IP=%d.%d.%d.%d ,Gateway=%d.%d.%d.%d\n\r",
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.ip,3),
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.ip,2),
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.ip,1),
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.ip,0),
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.gateway,3),
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.gateway,2),
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.gateway,1),
                        SL_IPV4_BYTE(App->EventData.ipAcquiredV4.gateway,0)); //UART'a IP ve Gateway yazilir.
            UNUSED(AppEventData);
        }
        break;

        case SL_NETAPP_IP_LEASED_EVENT: //Statik olmayan Gecici(Kiralik)IP
        {
            SET_STATUS_BIT(SimpleLinkStatus,STATUS_BIT_IP_LEASED); //Status bitini kiralik ip ile set et.
        }
        break;

        case SL_NETAPP_IP_RELEASED_EVENT: //Yayinlanan bir IP adresi icin
        {
            CLR_STATUS_BIT(SimpleLinkStatus, STATUS_BIT_IP_LEASED); //Kiralanmis biti temizle
        }
        break;

        default: //diger durumlarda
        {
            UART_PRINT("[NETAPP EVENT] Beklenmedik olay [0x%x]\n\r",App->Event); //Beklenmedik olay
        }
        break;
    }

}


void SimpleLinkWlanEventHandler(SlWlanEvent_t *WlanEvent) //Client baglandiginda
{
    if(WlanEvent==NULL)
    {
        UART_PRINT("Wlan pointeri bos...\n\r");
        while(1);
    }
    switch(WlanEvent->Event)
    {
        case SL_WLAN_CONNECT_EVENT: //Wlan'a baglanilirsa
        {
            SET_STATUS_BIT(SimpleLinkStatus, STATUS_BIT_CONNECTION); //baglanti bitiyle set et
            memcpy(SSID,WlanEvent->EventData.STAandP2PModeWlanConnected.ssid_name,
                   WlanEvent->EventData.STAandP2PModeWlanConnected.ssid_len); //SSID ismimi kopyala
            memcpy(BSSID,WlanEvent->EventData.STAandP2PModeWlanConnected.bssid,SL_BSSID_LENGTH); // MAC adresini kopyala
            UART_PRINT("[WLAN EVENT] Aygit Ap'a Baglandi: %s ,MAC: %x:%x:%x:%x:%x:%x\n\r",
                        SSID,BSSID[0],BSSID[1],BSSID[2],BSSID[3],BSSID[4],BSSID[5]);
        }
        break;

        case SL_WLAN_DISCONNECT_EVENT: //Wlandan cikilirsa
        {
            slWlanConnectAsyncResponse_t*  EventData = NULL;
            CLR_STATUS_BIT(SimpleLinkStatus, STATUS_BIT_CONNECTION); //Baglanma bitleri temizlendi
            CLR_STATUS_BIT(SimpleLinkStatus, STATUS_BIT_IP_AQUIRED); //Baglanma bitleri temizlendi
            EventData = &WlanEvent->EventData.STAandP2PModeDisconnected;

            if(SL_WLAN_DISCONNECT_USER_INITIATED_DISCONNECTION == EventData->reason_code) //Kapatma islemi geldiyse
            {
                UART_PRINT("[WLAN EVENT] Aygit A'ptan cikti : %s, MAC: %x:%x:%x:%x:%x:%x\n\r",
                           SSID,BSSID[0], BSSID[1],BSSID[2],BSSID[3],BSSID[4],BSSID[5]);
            }
            else //Cikamazsa
            {
                UART_PRINT("[WLAN ERROR] Aygit AP'dan cikamadi: %s, MAC: %x:%x:%x:%x:%x:%x\n\r",
                           SSID,BSSID[0],BSSID[1],BSSID[2],BSSID[3],BSSID[4],BSSID[5]); //Aygit cikamadi
            }
            memset(SSID,0,sizeof(SSID));
            memset(BSSID,0,sizeof(BSSID));
        }
        break;

        case SL_WLAN_STA_CONNECTED_EVENT: //Wlan'a Girilirse
        {
            UART_PRINT("[WLAN EVENT] Aygit Baglandi...\n\r");
        }
        break;

        case SL_WLAN_STA_DISCONNECTED_EVENT: //Wlan'dan cikilirsa
        {
            UART_PRINT("[WLAN EVENT] Aygit Cikti...\n\r");
        }
        break;

        case SL_WLAN_SMART_CONFIG_COMPLETE_EVENT:
        {
            //Kullanilmiyor
        }
        break;

        case SL_WLAN_SMART_CONFIG_STOP_EVENT:
        {
           //Kullanilmiyor
        }
        break;

        default: //Diger durumlarda;
        {
            UART_PRINT("[WLAN EVENT] Beklenmedik olay [0x%x]\n\r",WlanEvent->Event); //Beklenmedik olay
        }
        break;
    }
}


/************************************************************************************************/
/****************** Onemli Tokenlere veri aktarimi/Alimi Burada Yapiliyor ***********************/
/************************************************************************************************/
void SimpleLinkHttpServerCallback(SlHttpServerEvent_t *HttpServerEvent, SlHttpServerResponse_t *HttpServerResponse)
{
    switch (HttpServerEvent->Event)
    {
        case SL_NETAPP_HTTPGETTOKENVALUE_EVENT:
        {
            unsigned char *ptr;

            ptr = HttpServerResponse->ResponseData.token_value.data; //Acilan sayfanin Tokeninini al
            HttpServerResponse->ResponseData.token_value.len = 0;

            if(memcmp(HttpServerEvent->EventData.httpTokenName.data,
                      GET_token_SICAKLIK, strlen((const char *)GET_token_SICAKLIK)) == 0)//Sicaklik Tokeni secildiginde
            {
                float fSicaklik;
                TMP006DrvGetTemp(&fSicaklik); //sicakligi oku
                char cSicaklik = (char)fSicaklik;
                short sSicaklikLen = itoa(cSicaklik,(char*)ptr);
                ptr[sSicaklikLen++] = ' ';
                ptr[sSicaklikLen] = 'F';
                HttpServerResponse->ResponseData.token_value.len += sSicaklikLen; //Tokene sicakligi yaz
            }
            if(memcmp(HttpServerEvent->EventData.httpTokenName.data,
                                  GET_token_UIC, strlen((const char *)GET_token_UIC)) == 0)//Online Offline Durum kontrolu
            {
                if(InternetAccess==0) //Online ise
                {
                    strcpy((char*)HttpServerResponse->ResponseData.token_value.data,"1"); //1 yolla
                }
                else //Offline ise
                {
                    strcpy((char*)HttpServerResponse->ResponseData.token_value.data,"0"); //0 yolla
                }
                HttpServerResponse->ResponseData.token_value.len =  1;
            }
            if(memcmp(HttpServerEvent->EventData.httpTokenName.data,
                      GET_token_IVMEOLCER, strlen((const char *)GET_token_IVMEOLCER)) == 0) //ivme olcer tokeni secildiginde
            {
                ReadAccSensor(); //IvmeOlcer'i oku
                if(KurutucuCalis) //Kurutucu calisir //Burasi hirsiz alarmi ve Kurutucu icin
                {
                    strcpy((char*)HttpServerResponse->ResponseData.token_value.data,"Running");
                    HttpServerResponse->ResponseData.token_value.len += strlen("Running"); //Running yollar
                }
                else
                {
                    strcpy((char*)HttpServerResponse->ResponseData.token_value.data,"Stopped");
                    HttpServerResponse->ResponseData.token_value.len += strlen("Stopped"); //Stopped yollar.
                }
            }
        }
        break;

        case SL_NETAPP_HTTPPOSTTOKENVALUE_EVENT: //Gelen veriler
        {
            unsigned char led;
            unsigned char *ptr = HttpServerEvent->EventData.httpPostData.token_name.data; //alinan token
            if(memcmp(ptr, POST_token, strlen((const char *)POST_token)) == 0) //POST_TOKEN gelirse
            {
                ptr = HttpServerEvent->EventData.httpPostData.token_value.data;
                if(memcmp(ptr, "LED", 3) != 0)
                    break;
                ptr += 3;
                led = *ptr;
                ptr += 2;
                if(led == '1')
                {
                    if(memcmp(ptr, "ON", 2) == 0)
                    {
                        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
                        LedStatus = LED_ON;
                    }
                    else if(memcmp(ptr, "Blink", 5) == 0)
                    {
                        GPIO_IF_LedOn(MCU_RED_LED_GPIO);
                        LedStatus = LED_BLINK;
                    }
                    else
                    {
                        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
                        LedStatus = LED_OFF;
                    }
                }
                else if(led == '2')
                {
                    if(memcmp(ptr, "ON", 2) == 0)
                    {
                        GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
                    }
                    else if(memcmp(ptr, "Blink", 5) == 0)
                    {
                        GPIO_IF_LedOn(MCU_ORANGE_LED_GPIO);
                        LedStatus = 1;
                    }
                    else
                    {
                        GPIO_IF_LedOff(MCU_ORANGE_LED_GPIO);
                    }
                }
            }
        }
        break;

        default:
            break;
    }
}
/************************************************************************************************/

void SimpleLinkSockEventHandler(SlSockEvent_t *Sock)
{
    if(Sock == NULL)
    {
        UART_PRINT("Sock pointer'i bos...\n\r");
        while(1);
    }
    switch(Sock->Event )
    {
        case SL_SOCKET_TX_FAILED_EVENT: //veri aktarimi hatali olursa
        {
            switch( Sock->socketAsyncEvent.SockTxFailData.status)
            {
                case SL_ECLOSE: //Soket kapandigi durumda
                {
                    UART_PRINT("[SOCK ERROR] - socket kapandi (%d) kuyruktaki paketler iletilemedi\n\n",
                               Sock->socketAsyncEvent.SockTxFailData.sd);
                }
                break;

                default:
                {
                    UART_PRINT("[SOCK EVENT] - Beklenmedik olay [%x0x]\n\n",Sock->Event);
                }
                break;
            }
        }
    }
}


void SimpleLinkGeneralEventHandler(SlDeviceEvent_t *DevEvent)
{
    if(DevEvent == NULL)
    {
        UART_PRINT("General Pointer'i bos...\n\r");
        while(1);
    }
    UART_PRINT("[GENERAL EVENT] - ID=[%d] Sender=[%d]\n\n",DevEvent->EventData.deviceEvent.status,
               DevEvent->EventData.deviceEvent.sender);
}

//***************************************************************************************
//--------------------------------------BITIS--------------------------------------------
//***************************************************************************************





static int ModuYapilandir(int Mod)
{
    long cevap=-1;
    cevap=sl_WlanSetMode(Mod); //modu yapilandir
    ASSERT_ON_ERROR(cevap);
    cevap=sl_Stop(SL_DURDURMA_SURESÝ);
    ASSERT_ON_ERROR(cevap);
    CLR_STATUS_BIT_ALL(SimpleLinkStatus); //Butun simple link bitlerini temizle
    return sl_Start(NULL,NULL,NULL); //SimpleLink baslatmaya geri don
}


long ÝnternetBaglanti()
{
    long cevap=-1;
    unsigned int BaglanmaZamanAsimiSayaci=0;

    cevap=sl_Start(NULL,NULL,NULL); //SimpleLink baslatildi.
    ASSERT_ON_ERROR(cevap); //cevap 0dan kucukse hata verir.

    if(ROLE_STA !=cevap && ModAyarlamasi==ROLE_STA) //Aygit Ap basladi ama jumper stationda ise;
    {
        if(ROLE_AP ==cevap) //mod ap ise
        {
            while(!IS_IP_ACQUIRED(SimpleLinkStatus)) //IP alana kadar bekle
            {
            #ifndef SL_PLATFORM_MULTI_THREADED
                _SlNonOsMainLoopTask();
            #endif
            }
        }
        cevap=ModuYapilandir(ROLE_STA); //Station moduna gec.
        ASSERT_ON_ERROR(cevap); //cevap 0dan kucukse hata verir.
    }

    if(ROLE_AP !=cevap && ModAyarlamasi==ROLE_AP) //cihaz STA basladi fakat jumper AP'da ise
    {
        cevap=ModuYapilandir(ROLE_AP); //AP moduna gecti.
        ASSERT_ON_ERROR(cevap); //cevap odan kucukse hata verir.
    }

    if(cevap==ROLE_AP) //Cihaz Ap modunda basladi ve degisiklige gerek yoksa;
    {
        while(!IS_IP_ACQUIRED(SimpleLinkStatus)) //DHCP sunucusundan IP adresi alana kadar bekle
        {
        #ifndef SL_PLATFORM_MULTI_THREADED
            _SlNonOsMainLoopTask();
        #endif
        }

        cevap=sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID); //Dahili HTTP sunucusunu durdur.
        ASSERT_ON_ERROR(cevap);
        cevap=sl_NetAppStart(SL_NET_APP_HTTP_SERVER_ID); //Dahili HTTP sunucusunu baslat.
        ASSERT_ON_ERROR(cevap);

        char sayac=0;
        for(sayac=0;sayac<3;sayac++) //AP modunu belirtmek icin led 3 kere yanýp soner.
        {
            GPIO_IF_LedOn(MCU_RED_LED_GPIO);
            osi_Sleep(400);
            GPIO_IF_LedOff(MCU_RED_LED_GPIO);
            osi_Sleep(400);
        }
        char ssid[32];
        unsigned short uzunluk =32;
        unsigned short config_opt =WLAN_AP_OPT_SSID;

        sl_WlanGet(SL_WLAN_CFG_AP_ID,&config_opt,&uzunluk,(unsigned char*)ssid); //WLAN isimini alir.
        UART_PRINT("\n\r Baglanacaginiz Internet: \'%s\'\n\r\n\r",ssid);
    }

    else //Statin modunda basladi ve yapilandirmaya gerek yoksa;
    {
        cevap=sl_NetAppStop(SL_NET_APP_HTTP_SERVER_ID); //HTTP server durulur.
        ASSERT_ON_ERROR(cevap); //Hata varsa sonsuz dongu
        cevap=sl_NetAppStart(SL_NET_APP_HTTP_SERVER_ID); //HTTP server baslatilir.
        ASSERT_ON_ERROR(cevap);

        while(BaglanmaZamanAsimiSayaci<OtoBaglanmaZamanAsimi &&
                ((!IS_CONNECTED(SimpleLinkStatus))||(!IS_IP_ACQUIRED(SimpleLinkStatus)))) //Cihazin otomatik baglanmasini bekle
        {
            GPIO_IF_LedOn(MCU_RED_LED_GPIO);
            osi_Sleep(50);
            GPIO_IF_LedOff(MCU_RED_LED_GPIO);
            osi_Sleep(50);
            BaglanmaZamanAsimiSayaci++;
        }
        if(BaglanmaZamanAsimiSayaci==OtoBaglanmaZamanAsimi) //verilen zaman surecinde baglanmazsa;
        {
            GPIO_IF_LedOn(MCU_RED_LED_GPIO);
            CLR_STATUS_BIT_ALL(SimpleLinkStatus); //Simplelink baglanma verilerini temizler.

            cevap=SmartConfigConnect(); //Akilli konfigurasyon ile baglan.
            ASSERT_ON_ERROR(cevap);
            while((!IS_CONNECTED(SimpleLinkStatus))||(!IS_IP_ACQUIRED(SimpleLinkStatus))) //IP almayi bekle
            {
                MAP_UtilsDelay(500);
            }
        }
        GPIO_IF_LedOff(MCU_RED_LED_GPIO);
        InternetAccess=ConnectionTest(); //Internete ping atarak test et.
    }
    return SUCCESS; //return 0
}


static void AygitYapilandirmaOku()
{
    unsigned int GPIOPort;
    unsigned char GPIOPin;
    unsigned char PinValue;

    GPIO_IF_GetPortNPin(GPIO_3,&GPIOPort,&GPIOPin); //Hangi port hangi pinden deger okunacak.
    PinValue=GPIO_IF_Get(GPIO_3,GPIOPort,GPIOPin); //Pin_58 yani GPIO3'deki deger okunur.
    if(PinValue==1) //Pin58 lojik 1 ise;
    {
        ModAyarlamasi=ROLE_AP; //Aygit ayarlamasini Access Point yap.
    }
    else //Pin58 lojik 0 ise;
    {
        ModAyarlamasi=ROLE_STA; //Aygit ayarmasini Station yap.
    }
}


static void OOBGorev(void *Parametreler)
{
    long cevap =-1;

    AygitYapilandirmaOku(); //Aygit yapilandirma modunu okur

    cevap=ÝnternetBaglanti(); //Internete baglanir
    if(cevap<0) //Internete baglanmazsa
    {
        UART_PRINT("Internete duzgun baglanamadi.\n\r");
        UART_PRINT("Kodu tekrar yukle ve bastan basla...\n\r");
        ERR_PRINT(cevap); //Hata mesaji olarak yazdir
        while(1);
    }

    while(1)
    {
        if(LedStatus==LED_ON)
        {
            GPIO_IF_LedOn(MCU_RED_LED_GPIO);
            osi_Sleep(500);
        }
        if(LedStatus==LED_OFF)
        {
            GPIO_IF_LedOff(MCU_RED_LED_GPIO);
            osi_Sleep(500);
        }
        if(LedStatus==LED_BLINK)
        {
            GPIO_IF_LedOn(MCU_RED_LED_GPIO);
            osi_Sleep(500);
            GPIO_IF_LedOff(MCU_RED_LED_GPIO);
            osi_Sleep(500);
        }
    }
}


static void DisplayAppName(char * AppName)
{
    UART_PRINT("\n\n\n\r");
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\t\t     CC3200 %s Uygulamasi       \n\r", AppName);
    UART_PRINT("\t\t *************************************************\n\r");
    UART_PRINT("\n\n\n\r");
}


static void UygulamaDegiskenleri()
{
    SimpleLinkStatus=0;

    memset(SSID,0,sizeof(SSID));
    memset(BSSID,0,sizeof(BSSID));

    InternetAccess= -1;
    ModAyarlamasi = ROLE_STA;
    LedStatus=LED_OFF;
}


static void BoardInit(void)
{
//vektor tablosu tabanini ayarlar
#ifndef USE_TIRTOS
#if defined(ccs)
    MAP_IntVTableBaseSet((unsigned long)&g_pfnVectors[0]);
#endif
#if defined(ewarm)
    MAP_IntVTableBaseSet((unsigned long)&__vector_table);
#endif
#endif
    //Islemci Aktif
    MAP_IntMasterEnable();
    MAP_IntEnable(FAULT_SYSTICK);
    PRCMCC3200MCUInit();
}


void main()
{
    long cevap = -1;

    BoardInit();

    PinMuxConfig();
    PinConfigSet(PIN_58,PIN_STRENGTH_2MA|PIN_STRENGTH_4MA,PIN_TYPE_STD_PD);

    UygulamaDegiskenleri();

    DisplayAppName(AppIsim); //uygulama ismi uartta gosterildi

    GPIO_IF_LedConfigure(LED1); //kirmizi led pini aktif
    GPIO_IF_LedOff(MCU_RED_LED_GPIO); //kirmizi led yanmiyor

    cevap=I2C_IF_Open(I2C_MASTER_MODE_FST); //I2C master ve hizli modda baslatildi.
    if(cevap<0) //I2C acilmiyorsa
    {
        UART_PRINT("I2C duzgun baslatilamadi\n\r");
        UART_PRINT("Kodu tekrar yukle ve bastan basla...\n\r");
        ERR_PRINT(cevap); //Hata mesaji olarak yazdir
        while(1);
    }

    cevap=TMP006DrvOpen(); //Sicaklik sensoru baslatildi
    if(cevap<0) //Sicaklik sensoru duzgun baslatilamadi ise
    {
        UART_PRINT("Sicaklik sensoru duzgun baslatilamadi.\n\r");
        UART_PRINT("Kodu tekrar yukle ve bastan basla...\n\r");
        ERR_PRINT(cevap); //Hata mesaji olarak yazdir
        while(1);
    }

    cevap=BMA222Open(); //ivme sensoru acildi
    if(cevap<0) //ivme sensoru duzgun baslatilamadi ise
    {
        UART_PRINT("Ivmeolcer sensoru duzgun baslatilamadi.\n\r");
        UART_PRINT("Kodu tekrar yukle ve bastan basla...\n\r");
        ERR_PRINT(cevap); //Hata mesaji olarak yazdir
        while(1);
    }

    cevap=VStartSimpleLinkSpawnTask(SPAWN_TASK_ONCELIK); //Simplelink taslagi olusturuldu(9 onceliginde)
    if(cevap<0)
    {
        UART_PRINT("SimpleLink duzgun baslatilamadi.\n\r");
        UART_PRINT("Kodu tekrar yukle ve bastan basla...\n\r");
        ERR_PRINT(cevap); //Hata mesaji olarak yazdir
        while(1);
    }
    cevap=osi_TaskCreate(OOBGorev, (signed char*)"OOBGorev",
                         OSI_YIGIN_BOYUTU, NULL,OOB_TASK_ONCELIK, NULL ); // Osi Taslagi ayarlandi ve olusturuldu.

    if(cevap<0) //Osi taslagi olusturulamazsa
    {
        UART_PRINT("Osi taslagi olusturulamadi.\n\r");
        UART_PRINT("Kodu tekrar yukle ve bastan basla...\n\r");
        ERR_PRINT(cevap); //Hata mesaji olarak yazdir
        while(1);
    }

    osi_start(); //Osi zamanlayicisi baslatildi

    while (1)
    {

    }

}
