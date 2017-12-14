#******************************************************************************

#

# Makefile - Rules for building the driver library.

#

# Copyright (c) 2005-2015 Texas Instruments Incorporated.  All rights reserved.

# Software License Agreement





#

# The base directory for TivaWare.

#

ROOT=..



#

# Include the common make definitions.

#

include ${ROOT}/makedefs



#

# Where to find header files that do not live in the source directory.

#

IPATH=../CMSIS/Device/ST/STM32F4xx/Include

IIPATH=../CMSIS/Include

OPATH=/home/talha/STM32F4xx_DSP_StdPeriph_Lib_V1.4.0/Libraries/STM32-F1-F2-F4-L1_I2C_CPAL_V1.2.0/Libraries/STM32_CPAL_Driver/devices/stm32f4xx
OOPATH=/STM32F4xx_DSP_StdPeriph_Lib_V1.4.0/Libraries/STM32-F1-F2-F4-L1_I2C_CPAL_V1.2.0/Libraries/STM32_CPAL_Driver/inc/

#

# The default rule, which causes the driver library to be built.

#

all: ${COMPILER}

all: ${COMPILER}/libdriver.a



#

# The rule to clean out all the build products.

#

clean:

	@rm -rf ${COMPILER} ${wildcard *~}



#

# The rule to create the target directory.

#

${COMPILER}:

	@mkdir -p ${COMPILER}



#

# Rules for building the driver library.

#
${COMPILER}/libdriver.a: ${COMPILER}/system_stm32f4xx.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_adc.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_can.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_crc.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_cryp_aes.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_cryp_des.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_cryp_tdes.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_dac.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_flash.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_dbgmcu.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_gpio.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_dcmi.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_i2c.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_dma2d.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_ltdc.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_dma.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_exti.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_pwr.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_flash_ramfunc.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_hash_md5.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_hash_sha1.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_sai.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_fmc.o

#${COMPILER}/libdriver: ${COMPILER}/stm32f4xx_fsmc.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_syscfg.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_iwdg.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_rcc.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_tim.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_usart.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_rng.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_rtc.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_sdio.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_spi.o

${COMPILER}/libdriver.a: ${COMPILER}/stm32f4xx_wwdg.o

${COMPILER}/libdriver.a: ${COMPILER}/misc.o

#${COMPILER}/libdriver.a: /home/talha/STM32F4xx_DSP_StdPeriph_Lib_V1.4.0/Libraries/STM32-F1-F2-F4-L1_I2C_CPAL_V1.2.0/Libraries/STM32_CPAL_Driver/cpal_i2c.o

#${COMPILER}/libdriver.a: /home/talha/STM32F4xx_DSP_StdPeriph_Lib_V1.4.0/Libraries/STM32-F1-F2-F4-L1_I2C_CPAL_V1.2.0/Libraries/STM32_CPAL_Driver/cpal_usercallback_template.o

#${COMPILER}/libdriver.a: /home/talha/STM32F4xx_DSP_StdPeriph_Lib_V1.4.0/Libraries/STM32-F1-F2-F4-L1_I2C_CPAL_V1.2.0/Libraries/STM32_CPAL_Driver/cpal_hal.o

#${COMPILER}/libdriver.a: /home/talha/STM32F4xx_DSP_StdPeriph_Lib_V1.4.0/Libraries/STM32-F1-F2-F4-L1_I2C_CPAL_V1.2.0/Libraries/STM32_CPAL_Driver/cpal_i2c_hal_stm32f4xx.o

#

# Include the automatically generated dependency files.

#

ifneq (${MAKECMDGOALS},clean)

-include ${wildcard ${COMPILER}/*.d} __dummy__

endif
