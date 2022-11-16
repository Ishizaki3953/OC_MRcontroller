/**
  ******************************************************************************
  * @file           : InterruptInEx.h
  * @brief          : interrupt chattering prevention
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef INTERRUPTINEX_h
#define INTERRUPTINEX_h

#include "mbed.h"

class InterruptInEx{
private:
    bool _repeat;
    DigitalIn _pin;
    uint8_t _last_x;
    Timeout _t;
    FunctionPointer _func, _func2;
private:
    void t_callback();
public:
    InterruptInEx(PinName pin, bool repeat);
    void loop();
    uint8_t read();

    void attach(void (*func)(void));
    template<typename T>
    void attach(T *object, void (T::*func)(void));
    void attach2(void (*func)(void));
    template<typename T>
    void attach2(T *object, void (T::*func)(void));
};
#endif/*INTERRUPTINEX_h*/

