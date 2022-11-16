/**
  ******************************************************************************
  * @file           : Plotter.cpp
  * @brief          : send to Serial Plotter (PC)
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "Plotter.h"

/**
  * @brief constructor
  * @param  pc:serial port
  * @retval none
  */
Plotter::Plotter(Serial *pc){
    _pc = pc;
}
/**
  * @brief label out
  * @param  text
  * @retval none
  */
void Plotter::label(char *t1, char *t2, char *t3, char *t4){
    if(t2==NULL && t3==NULL && t4==NULL) _pc->printf("OMRON,%s\n", t1);
    else if(t3==NULL && t4==NULL) _pc->printf("OMRON,%s,%s\n", t1, t2);
    else if(t4==NULL) _pc->printf("OMRON,%s,%s,%s\n", t1, t2, t3);
    else _pc->printf("OMRON,%s,%s,%s,%s\n", t1, t2, t3, t4);
}
/**
  * @brief plot
  * @param  data:plot data  n:data size
  * @retval none
  */
void Plotter::plot(int *data, int n){
    do {
        _pc->printf("%d,", *data);
        data++;
    } while(--n > 0);
    _pc->printf("\n");
}
/**
  * @brief plot
  * @param  data:plot data  n:data size
  * @retval none
  */
void Plotter::plot(float *data, int n){
    do {
        _pc->printf("%.2f,", *data);
        data++;
    } while(--n > 0);
    _pc->printf("\n");
}
/**
  * @brief plot
  * @param  data:plot data
  * @retval none
  */
void Plotter::plot(float data){//-100,100
    plot(&data, 1);
}

