/**
  ******************************************************************************
  * @file           : Terminal.h
  * @brief          : Terminal (PC TeraTerm)
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef TERMINAL_h
#define TERMINAL_h

#include "mbed.h"
#include "queue.h"
#include "Table.h"
#include "Flash.h"

#define CR  0x0d
#define LF  0x0a

#define ROW_MAX  POSI_MAX //1024
#define COL_MAX  5

class Terminal{
private:
    bool _timeout;
    Timeout _t;
    Queue *_que;//serial buffer
    
    Table _table;
    
    uint8_t _csv[256];
    uint8_t *_p;//csv pointer
    int _row;
    int _col;
    
    int _pldata[8];
    int _pln;
    
    Serial *_pc;
    int _step;
private:
    void rx_irq();
    void dummy_read();
    void t_interrupt();
    void error();
public:
    Terminal(Serial *pc);
   
    bool IsIdle();
    void loop();
};

#endif/*TERMINAL_h*/

