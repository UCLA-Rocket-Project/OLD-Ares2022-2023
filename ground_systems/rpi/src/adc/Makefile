DIR_LIB = ./lib
DIR_CONFIG = $(DIR_LIB)/config
DIR_DRIVER = $(DIR_LIB)/driver
DIR_SRC = .
DIR_BUILD = ./build
DIR_BIN = $(DIR_BUILD)/bin

OBJ_C = $(wildcard ${DIR_DRIVER}/*.c ${DIR_SRC}/*.c )
OBJ_O = $(patsubst %.c,${DIR_BIN}/%.o,$(notdir ${OBJ_C}))
RPI_DEV_C = $(wildcard $(DIR_BIN)/dev_hardware_SPI.o $(DIR_BIN)/RPI_sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )
JETSON_DEV_C = $(wildcard $(DIR_BIN)/sysfs_software_spi.o $(DIR_BIN)/sysfs_gpio.o $(DIR_BIN)/DEV_Config.o )

DEBUG = -D DEBUG

USELIB_RPI = USE_BCM2835_LIB
# USELIB_RPI = USE_WIRINGPI_LIB
# USELIB_RPI = USE_DEV_LIB

ifeq ($(USELIB_RPI), USE_BCM2835_LIB)
    LIB_RPI = -lbcm2835 -lm
else ifeq ($(USELIB_RPI), USE_WIRINGPI_LIB)
    LIB_RPI = -lwiringPi -lm
else ifeq ($(USELIB_RPI), USE_DEV_LIB)
    LIB_RPI = -lm
endif
DEBUG_RPI = -D $(USELIB_RPI) -D RPI

USELIB_JETSONI = USE_DEV_LIB
# USELIB_JETSONI = USE_HARDWARE_LIB
ifeq ($(USELIB_JETSONI), USE_DEV_LIB)
    LIB_JETSONI = -lm
else ifeq ($(USELIB_JETSONI), USE_HARDWARE_LIB)
    LIB_JETSONI = -lm
endif
DEBUG_JETSONI = -D $(USELIB_JETSONI) -D JETSON

.PHONY : RPI JETSON clean

RPI:RPI_DEV RPI_epd 
JETSON: JETSON_DEV JETSON_epd

TARGET = $(DIR_BUILD)/adc
CC = gcc
MSG = -g -O0 -Wall
CFLAGS += $(MSG)

RPI_epd: ${OBJ_O} | $(DIR_BUILD)
	echo $(@)
	$(CC) $(CFLAGS) -D RPI $(OBJ_O) $(RPI_DEV_C) -o $(TARGET) $(LIB_RPI) $(DEBUG)

JETSON_epd: ${OBJ_O} | $(DIR_BUILD)
	echo $(@)
	$(CC) $(CFLAGS) $(OBJ_O) $(JETSON_DEV_C) -o $(TARGET) $(LIB_JETSONI) $(DEBUG)

${DIR_BIN}/%.o: $(DIR_SRC)/%.c | $(DIR_BIN)
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_CONFIG) -I $(DIR_DRIVER) $(DEBUG)

${DIR_BIN}/%.o: $(DIR_DRIVER)/%.c | $(DIR_BIN)
	$(CC) $(CFLAGS) -c  $< -o $@ -I $(DIR_CONFIG) $(DEBUG)

RPI_DEV: | $(DIR_BIN)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c $(DIR_CONFIG)/dev_hardware_SPI.c -o $(DIR_BIN)/dev_hardware_SPI.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c $(DIR_CONFIG)/RPI_sysfs_gpio.c -o $(DIR_BIN)/RPI_sysfs_gpio.o $(LIB_RPI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_RPI) -c $(DIR_CONFIG)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_RPI) $(DEBUG)

JETSON_DEV: | $(DIR_BIN)
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c $(DIR_CONFIG)/sysfs_software_spi.c -o $(DIR_BIN)/sysfs_software_spi.o $(LIB_JETSONI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c $(DIR_CONFIG)/sysfs_gpio.c -o $(DIR_BIN)/sysfs_gpio.o $(LIB_JETSONI) $(DEBUG)
	$(CC) $(CFLAGS) $(DEBUG_JETSONI) -c $(DIR_CONFIG)/DEV_Config.c -o $(DIR_BIN)/DEV_Config.o $(LIB_JETSONI)  $(DEBUG)

$(DIR_BIN): | $(DIR_BUILD)
	mkdir $(DIR_BIN)

$(DIR_BUILD):
	mkdir $(DIR_BUILD)

clean:
	rm -rf $(DIR_BUILD)
	rm -f $(TARGET)