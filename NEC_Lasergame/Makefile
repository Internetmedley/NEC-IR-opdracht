PROJECT := main

SEARCH :=

HEADERS := keypad_listener.hpp
HEADERS += init_game.hpp
HEADERS += ZRX543.hpp
HEADERS += reg_game_para.hpp
HEADERS += run_game.hpp
HEADERS += ir_sender.hpp
HEADERS += pause_detector.hpp
HEADERS += pause_listener.hpp
HEADERS += msg_decoder.hpp
HEADERS += msg_listener.hpp
HEADERS += msg_logger.hpp

SOURCES := ir_sender.cpp
SOURCES += msg_decoder.cpp
SOURCES += pause_detector.cpp

TARGET := arduino_due

SERIAL_PORT := COM3

HEAP := BMPTK

RUN_PAUSE := echo "Run complete"

include $(RTOS)/makefile.inc
include $(HWLIB)/makefile.inc