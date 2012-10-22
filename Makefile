DEVICE=atmega324p
DUDEDEV=-c stk500 -P COM16 -F

OBJECTS= main.o
CFLAGS=-Wall -Os -I. -L. -DF_CPU=20000000 -save-temps
LDFLAGS=-lm

#HFUSE=$(subst _,,0b_1_1_0_1__0_0_0_1)
#                   ^ ^ ^ ^  ^ ^ ^ ^-- BOOTRST (no boot reset vector)
#                   | | | |  | | +---- BOOTSZ0 (2kB
#                   | | | |  | +------ BOOTSZ1      boot size)
#                   | | | |  + ------- EESAVE (preserve EEPROM over chip erase)
#                   | | | +----------- CKOPT
#                   | | +------------- SPIEN (allow serial programming)
#                   | +--------------- JTAGEN (disable JTAG, save 4 pins)
#                   +----------------- OCDEN (disable on-chip debug)
HFUSE= 0xD1

#LFUSE=$(subst _,,0b_0_0_1_1__1_1_1_1)
#                   ^ ^ \ /  \--+--/
#                   | |  |      +----- CKSEL 3..0 (external crystal, full-swing)
#                   | |  +------------ SUT 1..0 (long startup time)
#                   | +--------------- BODEN
#                   +----------------- BODLEVEL
#should be LFUSE= 0xBF
#LFUSE= 0xc2
LFUSE= 0xBF
EFUSE= 0xFE

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
