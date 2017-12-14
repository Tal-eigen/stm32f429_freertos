TARGET=ax2550

TOOLCHAIN_PATH=/usr/bin
COMPILER=gcc
PREFIX=arm-none-eabi
BUILD_DIR=${CURDIR}/build
LIB_DIR=${CURDIR}/lib
CC=${PREFIX}-gcc


CPU=-mcpu=cortex-m4
FPU=-mfpu=fpv4-sp-d16 -mfloat-abi=softfp

vpath %.c ${CURDIR}/source \
	sys/src/cmsis/ \
	sys/src/newlib/ \
	sys/src/spl/ \
	FreeRTOS/Source \
	FreeRTOS/Source/portable/GCC/ARM_CM4F \
	FreeRTOS/Source/portable/Common/ \
	FreeRTOS/Source/portable/MemMang 

vpath %.h ${CURDIR}/include \
	sys/inc/cmsis \
	sys/inc/spl/ \
	FreeRTOS/Source/portable/GCC/ARM_CM4F/ \
	FreeRTOS/Source/include


vpath %.s ${CURDIR}/source

INC = ${CURDIR}/include
CMSIS_INC=${CURDIR}/sys/inc/cmsis/ 
SPL_INC=${CURDIR}/sys/inc/spl/
RTOS_INC=${CURDIR}/FreeRTOS/Source/include/
PORT_INC=${CURDIR}/FreeRTOS/Source/portable/GCC/ARM_CM4F/

SRC+=main.c
SRC+=stm32_hw.c
SRC+=system_stm32f4xx.c
SRC+=stm32f4xx_it.c
#SRC+=_initialize_hardware.c
SRC+=_syscalls.c

#FreeRTOS source files
SRC+=port.c
SRC+=list.c
SRC+=queue.c
SRC+=tasks.c
SRC+=event_groups.c
SRC+=timers.c
SRC+=heap_4.c
#SRC+=semphr.c

#standard peripheral library source files
SRC+=misc.c
SRC+=stm32f4xx_dcmi.c
#SRC+=stm32f4xx_hash.c
SRC+=stm32f4xx_rtc.c
SRC+=stm32f4xx_adc.c
SRC+=stm32f4xx_dma.c
#SRC+=stm32f4xx_hash_md5.c
SRC+=stm32f4xx_sai.c
SRC+=stm32f4xx_can.c
SRC+=stm32f4xx_dma2d.c
#SRC+=stm32f4xx_hash_sha1.c
SRC+=stm32f4xx_sdio.c
#SRC+=stm32f4xx_cec.c
#SRC+=stm32f4xx_dsi.c
SRC+=stm32f4xx_i2c.c
#SRC+=stm32f4xx_spdifrx.c
SRC+=stm32f4xx_crc.c
SRC+=stm32f4xx_exti.c
SRC+=stm32f4xx_iwdg.c
SRC+=stm32f4xx_spi.c
#SRC+=stm32f4xx_cryp.c
SRC+=stm32f4xx_flash.c
#SRC+=stm32f4xx_lptim.c
SRC+=stm32f4xx_syscfg.c
#SRC+=stm32f4xx_cryp_aes.c
#SRC+=stm32f4xx_flash_ramfunc.c
SRC+=stm32f4xx_ltdc.c
SRC+=stm32f4xx_tim.c
#SRC+=stm32f4xx_cryp_des.c
#SRC+=stm32f4xx_fmc.c
SRC+=stm32f4xx_pwr.c
SRC+=stm32f4xx_usart.c
#SRC+=stm32f4xx_cryp_tdes.c
#SRC+=stm32f4xx_fmpi2c.c
#SRC+=stm32f4xx_qspi.c
SRC+=stm32f4xx_wwdg.c
SRC+=stm32f4xx_dac.c
SRC+=stm32f4xx_fmc.c
SRC+=stm32f4xx_rcc.c
SRC+=stm32f4xx_dbgmcu.c
SRC+=stm32f4xx_gpio.c
SRC+=stm32f4xx_rng.c

ST_SRC+=startup_stm32f429xx.s

CFLAGS=-mthumb                  \
        -g -O0       \
        -DUSE_STDPERIPH_DRIVER  \
        ${CPU}                   \
        ${FPU}                   \
        -ffunction-sections      \
        -fdata-sections          \
        -MD                      \
        -std=c99                 \
        -Wno-error               \
        -pedantic                \
        -DSTM32F429_439xx        \
        -DSTM32F4XX              \
        -mabi=aapcs             \
        --specs=rdimon.specs    \
        -fno-builtin-memcpy     \
        -c

AFLAGS=-mthumb  \
        ${CPU}   \
        ${FPU}   \

LINKER_SCRIPT1=${CURDIR}/ldscripts/mem.ld
LINKER_SCRIPT2=${CURDIR}/ldscripts/sections.ld
LINKER_SCRIPT3=${CURDIR}/ldscripts/libs.ld

AR=${PREFIX}-ar
LD=${PREFIX}-ld
AS=${PREFIX}-as

LDFLAGS=${CPU} ${FPU} -u _scanf_float -u _printf_float -nostartfiles -fno-exceptions  -Wl,--gc-sections,-T${LINKER_SCRIPT1} -T${LINKER_SCRIPT2} -T${LINKER_SCRIPT3} \
         --entry Reset_Handler -Xlinker -Map=${BUILD_DIR}/${TARGET}.map

LIBGCC:=${shell ${CC} ${CFLAGS} -print-libgcc-file-name}

LIBC:=${shell ${CC} ${CFLAGS} -print-file-name=libc.a}

LIBM:=${shell ${CC} ${CFLAGS} -print-file-name=libm.a}

OBJCOPY=${PREFIX}-objcopy
OBJDUMP=${PREFIX}-objdump

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: %.c
	@echo [CC] $(notdir $<)
	@$(CC) $(CFLAGS) -I${INC} -I${CMSIS_INC} -I${SPL_INC} -I${PORT_INC} -I${RTOS_INC} $< -c -o $@

all:$(OBJ)
	@echo "Using the gcc version: " ${CC} --version
	@echo ${AS} -o ${ST_SRC:%.s=${BUILD_DIR}/%.o} ${CURDIR}/source/${ST_SRC}
	${AS} ${AFLAGS} ${CURDIR}/source/${ST_SRC} -o ${ST_SRC:%.s=${BUILD_DIR}/%.o}
	@echo [LD] ${TARGET}.elf ${LINKER_SCRIPT} 
	${CC} -o ${BUILD_DIR}/${TARGET}.elf ${LDFLAGS} ${OBJ} ${ST_SRC:%.s=${BUILD_DIR}/%.o} -lm -lc -lgcc -lnosys -lrdimon \
        -Xlinker -Map=${BUILD_DIR}/${TARGET}.map
	@echo [HEX] ${TARGET}.hex
	${OBJCOPY} -O ihex ${BUILD_DIR}/${TARGET}.elf ${BUILD_DIR}/${TARGET}.hex
	@echo [BIN] ${TARGET}.bin
	${OBJCOPY} -O binary ${BUILD_DIR}/${TARGET}.elf ${BUILD_DIR}/${TARGET}.bin

.PHONY: clean
clean:
	echo [RM] OBJ
	${RM} ${OBJ}
	echo [RM] BIN
	${RM} ${BUILD_DIR}/${TARGET}.elf
	${RM} ${BUILD_DIR}/${TARGET}.bin
	${RM} ${BUILD_DIR}/${TARGET}.hex

flash:
	st-flash write ${BUILD_DIR}/${TARGET}.bin 0x8000000
