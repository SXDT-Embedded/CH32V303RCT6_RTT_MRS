################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include rt-thread/src/subdir.mk
-include rt-thread/components/utilities/ulog/backend/subdir.mk
-include rt-thread/components/utilities/ulog/subdir.mk
-include rt-thread/components/finsh/subdir.mk
-include rt-thread/components/drivers/serial/subdir.mk
-include rt-thread/components/drivers/misc/subdir.mk
-include rt-thread/components/drivers/ipc/subdir.mk
-include libraries/hal_libraries/ch32v303_hal/source/subdir.mk
-include libraries/hal_libraries/bmsis/source/subdir.mk
-include libraries/hal_drivers/subdir.mk
-include libcpu/cpu/subdir.mk
-include board/subdir.mk
-include applications/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
SCV_ControlBox_CH32V303RCT6_BC260.hexAndbin \

SECONDARY_LIST += \
SCV_ControlBox_CH32V303RCT6_BC260.lst \

SECONDARY_SIZE += \
SCV_ControlBox_CH32V303RCT6_BC260.siz \


# 所有目标
all: SCV_ControlBox_CH32V303RCT6_BC260.elf secondary-outputs

# 工具调用
SCV_ControlBox_CH32V303RCT6_BC260.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-elf-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -T "F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage -Wl,-Map,"SCV_ControlBox_CH32V303RCT6_BC260.map" --specs=nano.specs --specs=nosys.specs -o "SCV_ControlBox_CH32V303RCT6_BC260.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
SCV_ControlBox_CH32V303RCT6_BC260.hexAndbin: SCV_ControlBox_CH32V303RCT6_BC260.elf
	@	riscv-none-elf-objcopy -O ihex "SCV_ControlBox_CH32V303RCT6_BC260.elf"  "SCV_ControlBox_CH32V303RCT6_BC260.hex" && riscv-none-elf-objcopy -O binary "SCV_ControlBox_CH32V303RCT6_BC260.elf"  "SCV_ControlBox_CH32V303RCT6_BC260.bin"
	@	@
SCV_ControlBox_CH32V303RCT6_BC260.lst: SCV_ControlBox_CH32V303RCT6_BC260.elf
	@	riscv-none-elf-objdump --source --all-headers --demangle --disassemble --line-numbers --wide -M xw "SCV_ControlBox_CH32V303RCT6_BC260.elf" > "SCV_ControlBox_CH32V303RCT6_BC260.lst"
	@	@
SCV_ControlBox_CH32V303RCT6_BC260.siz: SCV_ControlBox_CH32V303RCT6_BC260.elf
	@	riscv-none-elf-size --format=berkeley "SCV_ControlBox_CH32V303RCT6_BC260.elf"
	@	@
	$(MAKE) --no-print-directory post-build

# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) SCV_ControlBox_CH32V303RCT6_BC260.elf
	-@
post-build:
	-riscv-none-elf-objcopy -O ihex "SCV_ControlBox_CH32V303RCT6_BC260.elf" "SCV_ControlBox_CH32V303RCT6_BC260.hex" && riscv-none-elf-objcopy -O binary "SCV_ControlBox_CH32V303RCT6_BC260.elf" "SCV_ControlBox_CH32V303RCT6_BC260.bin"
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
