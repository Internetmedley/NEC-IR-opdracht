#############################################################################
#
# Project Makefile
#
# (c) Wouter van Ooijen (www.voti.nl) 2016
#
# This file is in the public domain.
# 
#############################################################################

# source files in this project (main.cpp is automatically assumed)
SOURCES := pause_detector.cpp msg_decoder.cpp msg_logger.cpp ir_sender.cpp

# header files in this project
HEADERS += pause_detector.hpp pause_listener.hpp msg_decoder.hpp msg_listener.hpp msg_logger.hpp

HEADERS += init_game.hpp ir_sender.hpp keypad_listener.hpp reg_game_para.hpp run_game.hpp ZRX543.hpp 

# other places to look for files for this project
SEARCH  := 

# set REATIVE to the next higher directory 
# and defer to the Makefile.due there
RELATIVE := $(RELATIVE)../../
include $(RELATIVE)Makefile.due