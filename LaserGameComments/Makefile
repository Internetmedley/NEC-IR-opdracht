PROJECT := main

SEARCH :=

HEADERS := $(wildcard *.hpp)

SOURCES := ir_sender.cpp
SOURCES += msg_decoder.cpp
SOURCES += msg_logger.cpp
SOURCES += pause_detector.cpp

TARGET := arduino_due

SERIAL_PORT := COM9

HEAP := BMPTK

RUN_PAUSE := echo "Run complete"

include $(RTOS)/makefile.inc
include $(HWLIB)/makefile.inc