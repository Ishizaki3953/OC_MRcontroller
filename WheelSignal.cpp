/**
  ******************************************************************************
  * @file           : WheelSignal.cpp
  * @brief          : WheelSignal Class
  ******************************************************************************
  * @attention
  * ホイール回転の信号を生成するクラス
  ******************************************************************************
  */
#include "WheelSignal.h"

/**
  * @brief constructor
  * @param  signal a and b
  * @retval none
  */
WheelSignal::WheelSignal(DigitalOut *a, DigitalOut *b){
    _signal_a = a;
    _signal_b = b;

    for(int i = 0; i < POSI_MAX; i++){//1024
        //パターンA
        if(i<512) signal_pat_a[i] = 0;
        else signal_pat_a[i] = 1;
        //パターンB
        if(i<256) signal_pat_b[i] = 0;
        else if (i<768) signal_pat_b[i] = 1;
        else signal_pat_b[i] = 0;       
    }
}
/**
  * @brief エンコーダ演算  singnal_a. singnal_b に出力
  *        1msec周期で実行すること
  * @param  周期を加味したraw値(0-1023)
  * @retval none
  */
void WheelSignal::calc(int raw_calc){//YOKO STRETCH RAW (0-1023)
    //dummy encoder
    *_signal_a = (signal_pat_a[raw_calc] > 0)? 1: 0;        
    //正転
    *_signal_b = (signal_pat_b[raw_calc] > 0)? 0: 1;

}

