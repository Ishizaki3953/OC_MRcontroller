/**
  ******************************************************************************
  * @file           : RotarySW.h
  * @brief          : rotary switch
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef ROTARYSW_h
#define ROTARYSW_h

#include "mbed.h"

class RotarySW{
private:
    int _step;
    Timeout _t;
    BusIn _bus;
    uint8_t _val;
    FunctionPointer _func;
private:
    void t_interrupt();
public:
    RotarySW(PinName a, PinName b, PinName c, PinName d);
    void attach(void (*func)(void));
    template<typename T>
    void attach(T *object, void (T::*func)(void));
    uint8_t get_val();
    void loop();
};
#endif/*ROTARYSW_h*/

