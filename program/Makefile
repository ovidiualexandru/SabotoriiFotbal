DEVICE=atmega324p
DUDEDEV=-c avrispmkII -P usb -b 57600

SOURCES= main.c leds.c motors.c sensors.c serial_comm.c config.c
CFLAGS=-Wall -Os -I. -L. -I. -DF_CPU=20000000 -save-temps
LDFLAGS=-lm

HFUSE=$(subst _,,0b_1_1_0_1__0_0_0_1)
#                   ^ ^ ^ ^  ^ ^ ^ ^-- BOOTRST (no boot reset vector)
#                   | | | |  | | +---- BOOTSZ0 (2kB
#                   | | | |  | +------ BOOTSZ1      boot size)
#                   | | | |  + ------- EESAVE (preserve EEPROM over chip erase)
#                   | | | +----------- CKOPT
#                   | | +------------- SPIEN (allow serial programming)
#                   | +--------------- JTAGEN (disable JTAG, save 4 pins)
#                   +----------------- OCDEN (disable on-chip debug)
#0xD1

LFUSE=$(subst _,,0b_1_1_1_1__1_1_1_1)
#                   ^ ^ \ /  \--+--/
#                   | |  |      +----- CKSEL 3..0 (external crystal, full-swing)
#                   | |  +------------ SUT 1..0 (long startup time)
#                   | +--------------- BODEN
#                   +----------------- BODLEVEL
#0xFF

EFUSE=0xFE
LOCKB=$(subst _,,0b_0_0_0_0__0_1_0_0)
#                   ^ ^ ^ ^  ^ ^ ^ ^-- LB1 (no program write
#                   | | | |  | | +---- LB2  or read allowed)
#                   | | | |  | +------ BLB01 (allow bootloader
#                   | | | |  + ------- BLB02  w, !r application)
#                   | | | +----------- BLB11 (do not allow rw
#                   | | +------------- BLB12  to boot section)
#                   | +--------------- 1
#                   +----------------- 1

include Makefile-avr
