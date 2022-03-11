#ifndef PIN_H
#define PIN_H

typedef uint8_t pin_t;

const pin_t IO1 = 27;
const pin_t IO2 = 24;
const pin_t IO3 = 29;
const pin_t IO4 = 23;
const pin_t IO5 = 26;
const pin_t IO6 = 22;
const pin_t IO7 = 2;
const pin_t IO8 = 3;
const pin_t IO9 = 6;
const pin_t IO10 = 7;
const pin_t IO11 = 8;
const pin_t IO12 = 9;
const pin_t IO13 = 4;
const pin_t IO14 = 5;
const pin_t IO15 = 40;
const pin_t IO16 = 39;
const pin_t IO17 = 36;
const pin_t IO18 = 38;
const pin_t IO19 = 43;
const pin_t IO20 = 41;
const pin_t IO21 = 42;
const pin_t IO22 = 44;
const pin_t IO23 = A12;
const pin_t IO24 = A11;
const pin_t IO25 = A13;
const pin_t IO26 = A9;
const pin_t IO27 = A6;
const pin_t IO28 = A4;
const pin_t IO29 = A10;
const pin_t IO30 = A5;
const pin_t IO31 = 49;
const pin_t IO32 = 51;
const pin_t IO33 = 46;
const pin_t IO34 = 48;
const pin_t IO35 = 50;
const pin_t IO36 = 53;
const pin_t IO37 = 47;
const pin_t IO38 = 52;
const pin_t IO39 = 28;
const pin_t IO40 = 25;
                    // 1     2    3    4    5    6    7    8     9    10   11    12    13    14    15    16    17    18    19    20     21   22    23    24    25    26     27   28    29    30    31    32    33    34    35    36    37    38    39   40
const pin_t IO[40] = {IO1, IO40, IO6, IO3, IO4, IO2, IO8, IO7, IO13 , IO14, IO9, IO11, IO12, IO10, IO21, IO22, IO16, IO15, IO17, IO19, IO18, IO20, IO23, IO29, IO30, IO24, IO25, IO27, IO28, IO26, IO37, IO31, IO38, IO32, IO33, IO35, IO36, IO34, IO39, IO5};

const pin_t MUXR_A = A14;
const pin_t MUXR_B = A8;
const pin_t MUXR_C = A7;
const pin_t MUXL_A = 18;
const pin_t MUXL_B = 17;
const pin_t MUXL_C = 19;

const pin_t MUX1_CTR = 10;
const pin_t MUX2_CTR = 11;
const pin_t MUX3_CTR = 34;
const pin_t MUX4_CTR = 32;
const pin_t MUX5_CTR = 45;

const pin_t MUX1_ADC = A2;
const pin_t MUX2_ADC = A3;
const pin_t MUX3_ADC = A1;
const pin_t MUX4_ADC = A0;
const pin_t MUX5_ADC = A15;

const pin_t MUX_INHIBIT_CTR = 31;

const pin_t BUTTON1 = 16;
const pin_t BUTTON2 = 15;
const pin_t BUTTON3 = 12;
const pin_t BUTTON4 = 13;

const pin_t LEDR = 33;
const pin_t LEDG = 35;
const pin_t LEDB = 37;

const pin_t HEADER1 = 14;
const pin_t HEADER2 = 30;

#endif