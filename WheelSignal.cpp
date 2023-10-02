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
void WheelSignal::calc(int raw_calc,bool plus,bool flg){//YOKO STRETCH RAW (0-1023)
    enum Section {//区画の定義
        NONE=0, SEC_1, SEC_2, SEC_3, SEC_4,
    };
    //現在区画の確認
    uint16_t x = raw_calc;
    if(0 <= x && x < 256) _now_sct = SEC_1;
    else if(256 <= x && x < 512) _now_sct = SEC_2;
    else if(512 <= x && x < 768) _now_sct = SEC_3;
    else if(768 <= x && x < 1024) _now_sct = SEC_4;

    switch(_before_sct){
    case 0:
        _before_sct = _now_sct; //現在区画を覚えて次を待つ
        break;
    case SEC_1:
        if(flg == true) {
            if(plus == true){ //SEC_2
                //raw_calc = 384;
                _before_sct = SEC_2;
            }
            else{ //SEC_4
                //raw_calc = 896;
                _before_sct = SEC_4;
            }
        }
        else{
            _before_sct = _now_sct;
        }
        break;
    case SEC_2:
        if(flg == true) {
            if(plus == true){ //SEC_3
                //raw_calc = 640;
                _before_sct = SEC_3;
            }
            else{ //SEC_1
                //raw_calc = 128;
                _before_sct = SEC_1;
            }
        }
        else{ //SEC_2
            _before_sct = _now_sct;
        }
        break;
    case SEC_3:
        if(flg == true) {
            if(plus == true){ //SEC_4
                //raw_calc = 896;
                _before_sct = SEC_4;
            }
            else{ //SEC_2
                //raw_calc = 384;
                _before_sct = SEC_2;
            }
        }
        else{
            _before_sct = _now_sct;
        }
        break;
    case SEC_4:
        if(flg == true) {
            if(plus == true){ //SEC_1
                //raw_calc = 128;
                _before_sct = SEC_1;
            }
            else{ //SEC_3
                //raw_calc = 640;
                _before_sct = SEC_3;
            }
        }
        else{
            _before_sct = _now_sct;
        break;
        }
    }
	//hysteresis
    _hys = 32;
    _hysl = (POSI_MAX + raw_calc - _hys) % POSI_MAX;
    _hysh = (POSI_MAX + raw_calc + _hys) % POSI_MAX;
    
    //dummy encoder
    //*_signal_a = (signal_pat_a[(raw_calc] > 0)? 1: 0;
    if ((signal_pat_a[_hysl] > 0) && (signal_pat_a[_hysh] > 0)){
        *_signal_a = 1;
    } else if ((signal_pat_a[_hysl] == 0) && (signal_pat_a[_hysh] == 0)){
        *_signal_a = 0;
    } else {
        //出力変化なし
    }
    //正転
    //*_signal_b = (signal_pat_b[raw_calc] > 0)? 0: 1;
    if ((signal_pat_b[_hysl] > 0) && (signal_pat_b[_hysh] > 0)){
        *_signal_b = 0;
    } else if ((signal_pat_b[_hysl] == 0) && (signal_pat_b[_hysh] == 0)){
        *_signal_b = 1;
    } else {
        //出力変化なし
    }
}

