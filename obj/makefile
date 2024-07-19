################################################################################
# MRS Version: 1.9.1
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include rtthread/src/subdir.mk
-include rtthread/libcpu/risc-v/common/subdir.mk
-include rtthread/components/utilities/ulog/backend/subdir.mk
-include rtthread/components/utilities/ulog/subdir.mk
-include rtthread/components/rt_vsnprintf_full/subdir.mk
-include rtthread/components/net/at/src/subdir.mk
-include rtthread/components/finsh/subdir.mk
-include rtthread/components/drivers/serial/subdir.mk
-include rtthread/components/drivers/misc/subdir.mk
-include rtthread/subdir.mk
-include drivers/subdir.mk
-include common/rt_kprintf_threadsafe/subdir.mk
-include common/agile_led/subdir.mk
-include common/LwUtil/subdir.mk
-include common/LwRB/subdir.mk
-include common/FlexibleButton/subdir.mk
-include common/AT\ Command/subdir.mk
-include bsp/src/subdir.mk
-include User/subdir.mk
-include Startup/subdir.mk
-include Peripheral/src/subdir.mk
-include Debug/subdir.mk
-include Core/subdir.mk
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
SCV_ControlBox_CH32V303RCT6_BC260.hex \

SECONDARY_LIST += \
SCV_ControlBox_CH32V303RCT6_BC260.lst \

SECONDARY_SIZE += \
SCV_ControlBox_CH32V303RCT6_BC260.siz \


# 所有目标
all: SCV_ControlBox_CH32V303RCT6_BC260.elf secondary-outputs

# 工具调用
SCV_ControlBox_CH32V303RCT6_BC260.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-elf-gcc -march=rv32imacxw -mabi=ilp32 -msmall-data-limit=8 -msave-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -Wunused -Wuninitialized  -g -T "F:\BaiduSyncdisk\SXDT\IoT_SCV\V2.0\SCV_ControlBox_CH32V303RCT6_BC260\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -Xlinker --print-memory-usage -Wl,-Map,"SCV_ControlBox_CH32V303RCT6_BC260.map" --specs=nano.specs --specs=nosys.specs -o "SCV_ControlBox_CH32V303RCT6_BC260.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
SCV_ControlBox_CH32V303RCT6_BC260.hex: SCV_ControlBox_CH32V303RCT6_BC260.elf
	@	riscv-none-elf-objcopy -O ihex "SCV_ControlBox_CH32V303RCT6_BC260.elf"  "SCV_ControlBox_CH32V303RCT6_BC260.hex"
	@	@
SCV_ControlBox_CH32V303RCT6_BC260.lst: SCV_ControlBox_CH32V303RCT6_BC260.elf
	@	riscv-none-elf-objdump --all-headers --demangle --disassemble -M xw "SCV_ControlBox_CH32V303RCT6_BC260.elf" > "SCV_ControlBox_CH32V303RCT6_BC260.lst"
	@	@
SCV_ControlBox_CH32V303RCT6_BC260.siz: SCV_ControlBox_CH32V303RCT6_BC260.elf
	@	riscv-none-elf-size --format=berkeley "SCV_ControlBox_CH32V303RCT6_BC260.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) SCV_ControlBox_CH32V303RCT6_BC260.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
