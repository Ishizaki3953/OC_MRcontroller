/**
  ******************************************************************************
  * @file           : RotarySW.h
  * @brief          : rotary switch
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "RotarySW.h"

/**
  * @brief constructor
  * @param  pin names
  * @retval none
  */
RotarySW::RotarySW(PinName a, PinName b, PinName c, PinName d):_bus(a,b,c,d){
    //pc.printf("s=%02X\n", get_val());
    _val = get_val();
    _step = 0;
}
/**
  * @brief attach
  * @param  function pointer
  * @retval none
  */
void RotarySW::attach(void (*func)(void)){
    _func.attach(func);
}
/**
  * @brief attach
  * @param  object & function pointer
  * @retval none
  */
template<typename T>
void RotarySW::attach(T *object, void (T::*func)(void)){
    _func.attach(object, func);
}
/**
  * @brief get value
  * @param  none
  * @retval value
  */
uint8_t RotarySW::get_val(){
    //pc.printf("[%08X]", _bus);
    return _bus & 0xf;
}
/**
  * @brief loop
  * @param  none
  * @retval none
  */
void RotarySW::loop(){
    uint8_t val;
    switch(_step){
    case 0:
        if((val=get_val()) != _val){//first read
            _step = 1;
            _t.attach(this, &RotarySW::t_interrupt, 0.05);
        }
        break;
    case 1://wait
        break;
    case 2:
        if((val=get_val()) != _val){//2times read
            _val = val;
            _func.call();
            _step = 0;
        }else{
            _step = 0;
        }
        break;
    }
}
/**
  * @brief timer interrupt
  * @param  none
  * @retval none
  */
void RotarySW::t_interrupt(){
    _step = 2;
}
