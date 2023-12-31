/**
  ******************************************************************************
  * @file           : Demo_asw.h
  * @brief          : Demo_asw mode
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef DEMO_ASW_h
#define DEMO_ASW_h

#include "mbed.h"
#include "ISensor.h"
#include "MotorEx.h"
#include "Plotter.h"
#include "Table.h"
#include "common.h"
#include "queueEx.h"
#include "buffer.h"
#include "WheelSignal.h"
#include <cstdint>

#if 0
typedef enum{
    PAT_A=0,
    PAT_B,
    PAT_C,
    PAT_D,
    PAT_MAX,
} PATTERN;
#endif


class Demo_asw{
private:
    QueueEx _que;//RAW格納キュー
    
    bool _plus;
    uint8_t _sct, _now_sct;
    bool fast_flg;//高速回転フラグ
    int _step;
    float _w, _h;//周期と電圧
    int _tickCnt;//タイマカウンタ
    uint16_t _raw;//current raw(AD)
    uint16_t _raw2;//convert raw
    uint16_t before_raw;//前回値
    int16_t diff_raw;//生値と前回値の差分
    bool _plus2;//ホイール回転方向格納
    Ticker _tick;//タイマ
    
    int16_t _pwm[POSI_MAX][PAT_MAX];//PWMテーブル

//    uint8_t signal_pat_a[POSI_MAX];//エンコーダAテーブル -> Wheel class
//    uint8_t signal_pat_b[POSI_MAX];//エンコーダBテーブル -> Wheel class
    
    Motor *_motor;//モータ
    ISensor *_sensor;//センサ
    Plotter *_plot;//プロッタ
    WheelSignal *_wheel;
    PATTERN _pattern;//パターンデータ
public:
#ifdef PLOT_ASW
    Demo_asw(Motor *motor, ISensor *sensor, Plotter *plot, WheelSignal *wheel);
#else
    Demo_asw(Motor *motor, ISensor *sensor, WheelSignal *wheel);
#endif
//    DigitalOut *_signal_a;
//    DigitalOut *_signal_b;
private:
//    bool calc_signal_a(uint16_t raw);
//    bool calc_signal_b(uint16_t raw);

//    void rotate_check();
    float ZeroTorqueCheck();
    void setting_load();
    float get_w();
    float get_h();
    uint16_t ad_conv(uint16_t ad, float w);
    void tick_interrupt(void);
    void setting_out();
    bool ChangeCheck(bool change);
    float calc_pwm(uint16_t raw);
public:
    void start();
    void loop();
};


#endif/*DEMO_h*/

