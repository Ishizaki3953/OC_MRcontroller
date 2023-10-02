/**
  ******************************************************************************
  * @file           : main.cpp
  * @brief          : OMRON MR DEMO
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "mbed.h"
#include "common.h"

/* COMMON --------------------------------------------------------------------*/
SerialEx pc(USBTX, USBRX, 115200);
DigitalOut g_green(LED1);

/* MOTOR ---------------------------------------------------------------------*/
#include "Motor.h"
//23.07.26 ishizaki change ASWの変更
static PwmOut pwm_asw(PA_8);
static DigitalOut dir_asw(PB_1);
static Motor g_motor_asw(&pwm_asw, &dir_asw);

//23.07.26 ishizaki add ASDの追加
static PwmOut pwm_asd(PA_11);
static DigitalOut dir_asd(PA_12);
static Motor g_motor_asd(&pwm_asd, &dir_asd);

/* SERIAL PLOTTER ------------------------------------------------------------*/
#include "Plotter.h"
static Plotter g_plot(&pc);

/* TeraTerm ------------------------------------------------------------------*/
#include "Terminal.h"
static Terminal g_term(&pc);

/* BUTTON --------------------------------------------------------------------*/
#include "InterruptInEx.h"
//23.07.26 ishizaki change ASWの変更
InterruptInEx g_lclick_asw(PA_4, true);//左クリック(ワンショット)
InterruptInEx g_rclick_asw(PA_3, true);//右クリック(リピートボタン)

int g_lclkCnt_asw = 0;//左クリックカウント
int g_rclkCnt_asw = 0;//右クリックカウント
int g_lrlsCnt_asw = 0;//左リリースカウント
int g_rrlsCnt_asw = 0;//右リリースカウント

/**
  * @brief left click/release interrupt
  * @param  none
  * @retval none
  */
static void lclk_interrupt_asw(){
    g_lclkCnt_asw++; 
    //pc.printf("L");
}
static void lrls_interrupt_asw(){
    g_lrlsCnt_asw++;
    //pc.printf("l"); 
}
/**
  * @brief right click/release interrupt
  * @param  none
  * @retval none
  */
static void rclk_interrupt_asw(){
    g_rclkCnt_asw++;
    //pc.printf("R"); 
}
static void rrls_interrupt_asw(){
    g_rrlsCnt_asw++;
    //pc.printf("r"); 
}

//23.07.26 ishizaki add ASDの追加
InterruptInEx g_lclick_asd(PA_1, true);//左クリック(ワンショット)
InterruptInEx g_rclick_asd(PA_0, true);//右クリック(リピートボタン)

int g_lclkCnt_asd = 0;//左クリックカウント
int g_rclkCnt_asd = 0;//右クリックカウント
int g_lrlsCnt_asd = 0;//左リリースカウント
int g_rrlsCnt_asd = 0;//右リリースカウント

/**
  * @brief left click/release interrupt
  * @param  none
  * @retval none
  */
static void lclk_interrupt_asd(){
    g_lclkCnt_asd++; 
    //pc.printf("L");
}
static void lrls_interrupt_asd(){
    g_lrlsCnt_asd++;
    //pc.printf("l"); 
}
/**
  * @brief right click/release interrupt
  * @param  none
  * @retval none
  */
static void rclk_interrupt_asd(){
    g_rclkCnt_asd++;
    //pc.printf("R"); 
}
static void rrls_interrupt_asd(){
    g_rrlsCnt_asd++;
    //pc.printf("r"); 
}

/* Wheel ---------------------------------------------------------------------*/
#include "WheelSignal.h"
//23.07.26 ishizaki change ASWの変更
static DigitalOut signal_a_asw(PA_5);
static DigitalOut signal_b_asw(PA_6);
static WheelSignal g_Wheel_asw(&signal_a_asw, &signal_b_asw);

//23.07.26 ishizaki add ASDの追加
static DigitalOut signal_a_asd(PB_5);
static DigitalOut signal_b_asd(PB_6);
static WheelSignal g_Wheel_asd(&signal_a_asd, &signal_b_asd);

/* AS5601 --------------------------------------------------------------------*/
#include "AS5601.h"
//23.07.26 ishizaki change ASWの変更
static I2C i2c_asw(PA_10, PA_9);//sda, scl
static AS5601 as5601_asw(&i2c_asw, "AS5601-I2C_ASW");

//23.07.26 ishizaki add ASDの追加
static I2C i2c_asd(PB_4, PA_7);//sda, scl
static AS5601 as5601_asd(&i2c_asd, "AS5601-I2C_ASD");

/* DEMO_ASW ------------------------------------------------------------------*/
#include "Demo_asw.h"
//23.07.26 ishizaki change ASWの変更
#ifdef PLOT_ASW
static Demo_asw g_demo_asw(&g_motor_asw, &as5601_asw, &g_plot, &g_Wheel_asw);
#else
static Demo_asw g_demo_asw(&g_motor_asw, &as5601_asw, &g_Wheel_asw);
#endif

/* DEMO_ASD ------------------------------------------------------------------*/
#include "Demo_asd.h"
//23.07.26 ishizaki add ASDの追加
#ifdef PLOT_ASD
static Demo_asd g_demo_asd(&g_motor_asd, &as5601_asd, &g_plot, &g_Wheel_asd);
#else
static Demo_asd g_demo_asd(&g_motor_asd, &as5601_asd, &g_Wheel_asd);
#endif

/* Main ----------------------------------------------------------------------*/

/**
  * @brief setup
  * @param  none
  * @retval none
  */
static void setup(){
    g_plot.label("Reset!!");
    
    //23.07.26 ishizaki change ASWの変更
    g_lclick_asw.attach(&lclk_interrupt_asw);
    g_rclick_asw.attach(&rclk_interrupt_asw);
    g_lclick_asw.attach2(&lrls_interrupt_asw);
    g_rclick_asw.attach2(&rrls_interrupt_asw);

    //23.07.26 ishizaki add ASDの追加
    g_lclick_asd.attach(&lclk_interrupt_asd);
    g_rclick_asd.attach(&rclk_interrupt_asd);
    g_lclick_asd.attach2(&lrls_interrupt_asd);
    g_rclick_asd.attach2(&rrls_interrupt_asd);
}

/**
  * @brief main loop
  * @param  none
  * @retval none
  */
static void loop(){
    static int step_asw = 0;
    static int step_asd = 0;
    
    g_lclick_asw.loop();
    g_rclick_asw.loop();

    g_lclick_asd.loop();
    g_rclick_asd.loop();
    
    switch(step_asw){
    case 0://[setting mode]
        //シリアルで流し込むパターン設定を待つ
        g_term.loop();//setting only
        
        //ハードスイッチ押されたら設定モードを終える
        if(g_lclkCnt_asw || g_rclkCnt_asw){//click
            if(g_term.IsIdle()){//シリアル通信アイドルであれば
                //23.07.26 ishizaki change ASWの変更
                g_lclkCnt_asw = 0;
                g_rclkCnt_asw = 0;
                g_lrlsCnt_asw = 0;
                g_rrlsCnt_asw = 0;

                g_demo_asw.start();
                step_asw++;
            }
        }
        break;
        
    case 1://[demo mode]
        //23.07.26 ishizaki change ASWの変更
        g_demo_asw.loop();//demo forever
        break;
        
    default://[TEST MODE]
        break;
    }

    switch(step_asd){
    case 0://[setting mode]
        //シリアルで流し込むパターン設定を待つ
        g_term.loop();//setting only
        
        //ハードスイッチ押されたら設定モードを終える
        if(g_lclkCnt_asd || g_rclkCnt_asd){//click
            if(g_term.IsIdle()){//シリアル通信アイドルであれば
                //23.07.26 ishizaki add ASDの追加
                g_lclkCnt_asd = 0;
                g_rclkCnt_asd = 0;
                g_lrlsCnt_asd = 0;
                g_rrlsCnt_asd = 0;

                g_demo_asd.start();
                step_asd++;
            }
        }
        break;
        
    case 1://[demo mode]
        //23.07.26 ishizaki add ASDの追加
        g_demo_asd.loop();//demo forever
        break;
        
    default://[TEST MODE]
        break;
    }
    
}

/**
  * @brief main
  * @param  none
  * @retval none
  */
int main(){
    setup();
    while(1){
        loop();
    }
}


