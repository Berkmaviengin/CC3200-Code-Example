################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
gpio_if.obj: C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common/gpio_if.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 -me --include_path="C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="gpio_if.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 -me --include_path="C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="main.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

network_common.obj: C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common/network_common.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 -me --include_path="C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="network_common.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

pinmux.obj: ../pinmux.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 -me --include_path="C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="pinmux.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

uart_if.obj: C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common/uart_if.c $(GEN_OPTS) | $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: ARM Compiler'
	"C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/bin/armcl" -mv7M4 -me --include_path="C:/TI/ccsv7/tools/compiler/ti-cgt-arm_16.9.1.LTS/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/include" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink/source" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/example/common" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/oslib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/driverlib" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/inc" --include_path="C:/TI/CC3200SDK_1.3.0/CC3200SDK_1.3.0/cc3200-sdk/simplelink_extlib/provisioninglib" --define=ccs --define=USE_TIRTOS --define=SL_PLATFORM_MULTI_THREADED --define=cc3200 -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="uart_if.d" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


