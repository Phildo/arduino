ARDUINO_DIR = /usr/share/arduino

USER_LIB_PATH = $(HOME)/sketchbook/libraries

#ARDUINO_PORT = /dev/ttyACM*
#BOARD_TAG = uno
#BOARD_SUB = atmega328

ARDUINO_PORT = /dev/ttyUSB0
BOARD_TAG = nano328
BOARD_SUB = atmega328old

include /usr/share/arduino/Arduino.mk

