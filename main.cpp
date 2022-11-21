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
static PwmOut pwm(PA_8);
static DigitalOut dir(PA_7);
static Motor g_motor(&pwm, &dir);

/* SERIAL PLOTTER ------------------------------------------------------------*/
#include "Plotter.h"
static Plotter g_plot(&pc);

/* TeraTerm ------------------------------------------------------------------*/
#include "Terminal.h"
static Terminal g_term(&pc);

/* BUTTON --------------------------------------------------------------------*/
#include "InterruptInEx.h"

InterruptInEx g_lclick(PA_4, true);//左クリック(ワンショット)
InterruptInEx g_rclick(PA_3, true);//右クリック(リピートボタン)

int g_lclkCnt = 0;//左クリックカウント
int g_rclkCnt = 0;//右クリックカウント
int g_lrlsCnt = 0;//左リリースカウント
int g_rrlsCnt = 0;//右リリースカウント

/**
  * @brief left click/release interrupt
  * @param  none
  * @retval none
  */
static void lclk_interrupt(){
    g_lclkCnt++; 
    //pc.printf("L");
}
static void lrls_interrupt(){
    g_lrlsCnt++;
    //pc.printf("l"); 
}
/**
  * @brief right click/release interrupt
  * @param  none
  * @retval none
  */
static void rclk_interrupt(){
    g_rclkCnt++;
    //pc.printf("R"); 
}
static void rrls_interrupt(){
    g_rrlsCnt++;
    //pc.printf("r"); 
}

/* Wheel ---------------------------------------------------------------------*/
#include "WheelSignal.h"
static DigitalOut signal_a(PA_5);
static DigitalOut signal_b(PA_6);
static WheelSignal g_Wheel(&signal_a, &signal_b);

/* AS5601 --------------------------------------------------------------------*/
#include "AS5601.h"
static I2C i2c(PA_10, PA_9);//sda, scl
static AS5601 as5601(&i2c, "AS5601-I2C");

/* DEMO ----------------------------------------------------------------------*/
#include "Demo.h"
static Demo g_demo(&g_motor, &as5601, &g_plot, &g_Wheel);

/* Main ----------------------------------------------------------------------*/

/**
  * @brief setup
  * @param  none
  * @retval none
  */
static void setup(){
    g_plot.label("Reset!!");
    
    g_lclick.attach(&lclk_interrupt);
    g_rclick.attach(&rclk_interrupt);
    g_lclick.attach2(&lrls_interrupt);
    g_rclick.attach2(&rrls_interrupt);
}

/**
  * @brief main loop
  * @param  none
  * @retval none
  */
static void loop(){
    static int step = 0;
    
    g_lclick.loop();
    g_rclick.loop();
    
    switch(step){
    case 0://[setting mode]
        //シリアルで流し込むパターン設定を待つ
        g_term.loop();//setting only
        
        //ハードスイッチ押されたら設定モードを終える
        if(g_lclkCnt || g_rclkCnt){//click
            if(g_term.IsIdle()){//シリアル通信アイドルであれば
                g_lclkCnt = 0;
                g_rclkCnt = 0;
                g_lrlsCnt = 0;
                g_rrlsCnt = 0;

                g_demo.start();
                step++;//goto demo
            }
        }
        break;
        
    case 1://[demo mode]
        g_demo.loop();//demo forever
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


