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
    uint8_t _before_sct, _now_sct;//前回、現在区画格納用
    int _hys;//ヒステリシス
    int _hysl;//ヒステリシスL
    int _hysh;//ヒステリシスH
public:
    WheelSignal(DigitalOut *a, DigitalOut *b);
    void calc(int raw_calc,bool plus,bool flg);

};

#endif/*WHEELSIGNAL_h*/

