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
CH32V303RCT6_RTT_MRS.hexAndbin \

SECONDARY_LIST += \
CH32V303RCT6_RTT_MRS.lst \

SECONDARY_SIZE += \
CH32V303RCT6_RTT_MRS.siz \


# 所有目标
all: CH32V303RCT6_RTT_MRS.elf secondary-outputs

# 工具调用
CH32V303RCT6_RTT_MRS.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-elf-gcc -march=rv32imafcxw -mabi=ilp32f -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g3 -T "F:\BaiduSyncdisk\learn\MCU\WCH\CH32V303-5-7\CH32V303RCT6_RTT_MRS\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage -Wl,-Map,"CH32V303RCT6_RTT_MRS.map" --specs=nano.specs --specs=nosys.specs -o "CH32V303RCT6_RTT_MRS.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
CH32V303RCT6_RTT_MRS.hexAndbin: CH32V303RCT6_RTT_MRS.elf
	@	riscv-none-elf-objcopy -O ihex "CH32V303RCT6_RTT_MRS.elf"  "CH32V303RCT6_RTT_MRS.hex" && riscv-none-elf-objcopy -O binary "CH32V303RCT6_RTT_MRS.elf"  "CH32V303RCT6_RTT_MRS.bin"
	@	@
CH32V303RCT6_RTT_MRS.lst: CH32V303RCT6_RTT_MRS.elf
	@	riscv-none-elf-objdump --source --all-headers --demangle --disassemble --line-numbers --wide -M xw "CH32V303RCT6_RTT_MRS.elf" > "CH32V303RCT6_RTT_MRS.lst"
	@	@
CH32V303RCT6_RTT_MRS.siz: CH32V303RCT6_RTT_MRS.elf
	@	riscv-none-elf-size --format=berkeley "CH32V303RCT6_RTT_MRS.elf"
	@	@
	$(MAKE) --no-print-directory post-build

# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) CH32V303RCT6_RTT_MRS.elf
	-@
post-build:
	-riscv-none-elf-objcopy -O ihex "CH32V303RCT6_RTT_MRS.elf" "CH32V303RCT6_RTT_MRS.hex" && riscv-none-elf-objcopy -O binary "CH32V303RCT6_RTT_MRS.elf" "CH32V303RCT6_RTT_MRS.bin"
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents post-build

-include ../makefile.targets
