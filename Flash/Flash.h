/**
  ******************************************************************************
  * @file           : Flash.h
  * @brief          : flash access
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef FLASH_h
#define FLASH_h

#include "mbed.h"
#include "SOFBlock.h"

class Flash{
private:
    SOFWriter _w;
    SOFReader _r;
    uint8_t *_pr;
    int _size;
public:
    void erase(int sector);
    void begin_write(int sector, bool erase);
    void write(uint8_t *data, int size);
    void write(uint8_t data);
    void end_write();
    
    void begin_read(int sector);
    int read(uint8_t *data, int size);
    void end_read();
};

#endif/*FLASH_h*/

