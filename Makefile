ARCHIVE := rom.a

SRCS := \
		start.S \
		plat.c \
		main.c \


include $(SRCTREE)/build/config.mk
#FOR EVB DDR2 DRIVE
ifdef CONFIG_COMPILE_ASIC
DRAM_SRC = dramc_evb.c
else
DRAM_SRC = dramc.c
endif

SRCS  += $(DRAM_SRC) \

#FOR BARE REGISTER DRIVER OF DDR 400MHZ
#ifdef DDR_400MHZ
SRCS += dramtest_ddr2_400.c \

#endif

#FOR BARE REGISTER DRIVER OF DDR 533MHZ
#ifdef DDR_533MHZ
#SRCS += dramtest_ddr2_533.c \

#endif		

include $(BUILD_LIB)

BINARY := rom
#FOR EVB MEMORY MAP
ifdef CONFIG_COMPILE_ASIC
LINKFILE_SRC = $(CURDIR)/rom_asic.ld.S
else
LINKFILE_SRC = $(CURDIR)/rom.ld.S
endif

LIBS := \
		drv_dram.a \
		drv_serial.a \
		drv_sysm.a \
		drv_timer.a \
		lib_generic.a \
		lib_arm.a \
		lib_console.a \
		lib_rtl.a \
		rom.a

include $(BUILD_BINARY)

