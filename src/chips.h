#ifndef CHIPS_H
#define CHIPS_H

#include "globals.h"
//const Chip chip = {size, MOSI, MISO, CLK, RESET, XTAL, {GND1, GND2}, {VCC1, VCC2}};

const char* const chipNames[] PROGMEM = {
    "Unknown",
    "Atmega328",
    "ATTINY84",
    "ATTINY1634",
    "ATTINY85",
    "ATTINY861",
    "ATTINY167",
    "ATTINY4313",
    "ATMEGA32"
};


const Chip UnknownChip = {0, 0, 0, 0, 0, 0, {255, 255}, {255, 255}, chipNames[0]};

const Chip ATMEGA328 = {28, 25, 24, 23, 0, 8, {7, 20}, {6, 22}, chipNames[1]};
const Chip ATTINY84 = {14, 6, 13, 12, 3, 255, {7, 255}, {0, 255}, chipNames[2]};
const Chip ATTINY1634 = {20, 10, 11, 14, 16, 255, {9, 255}, {19, 255}, chipNames[3]};
const Chip ATTINY85 = {8, 7, 6, 5, 0, 255, {3, 255}, {4, 255}, chipNames[4]};
const Chip ATTINY861 = {20, 0, 1, 2, 9, 255, {5, 14}, {4, 15}, chipNames[5]};
const Chip ATTINY167 = {20, 6, 2, 7, 19, 255, {5, 14}, {4, 15}, chipNames[6]};
const Chip ATTINY4313 = {20, 13, 12, 11, 0, 255, {9, 255}, {10, 255}, chipNames[7]};
const Chip ATMEGA32 = {40, 5, 6, 7, 8, 12, {10, 29}, {9, 30}, chipNames[8]};

const Chip chips[] = {ATMEGA328, ATTINY84, ATTINY1634, ATTINY85, ATTINY861, ATTINY167, ATTINY4313, ATMEGA32};

#endif