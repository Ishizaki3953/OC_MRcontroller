/**
  ******************************************************************************
  * @file           : Plotter.h
  * @brief          : send to Serial Plotter (PC)
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef PLOTTER_h
#define PLOTTER_h

#include "mbed.h"
#include "common.h"

class Plotter{
private:
    Serial *_pc;

public:
    Plotter(Serial *pc);
    void label(char *t1, char *t2=NULL, char *t3=NULL, char *t4=NULL);
    void plot(int *data, int n);
    void plot(float *data, int n);
    void plot(float data);
};

#endif/*PLOTTER_h*/

