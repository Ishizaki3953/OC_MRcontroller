/**
  ******************************************************************************
  * @file           : WheelSignal.h
  * @brief          : 
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef WHEELSIGNAL_h
#define WHEELSIGNAL_h

#include "mbed.h"
#include "common.h"
#include "Table.h"

class WheelSignal{
private:
    DigitalOut *_signal_a;
    DigitalOut *_signal_b;
    uint8_t signal_pat_a[POSI_MAX];//エンコーダAテーブル -> Wheel class
    uint8_t signal_pat_b[POSI_MAX];//エンコーダBテーブル -> Wheel class
public:
    WheelSignal(DigitalOut *a, DigitalOut *b);
    void calc(int raw_calc);

};

#endif/*WHEELSIGNAL_h*/

