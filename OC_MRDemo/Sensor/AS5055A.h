/**
  ******************************************************************************
  * @file           : AS5055A.h
  * @brief          : encoder
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef AS5055A_h
#define AS5055A_h

#include "mbed.h"
#include "ISensor.h"


#define SPI_CMD_READ  0x8000
#define SPI_REG_DATA  0x7ffe
#define SPI_REG_AGC   0x7ff0
#define SPI_REG_CLRERR  0x6700

class AS5055A : public ISensor{
private:
    static const uint16_t _bp_parity       = 1 << 0;
    static const uint16_t _bp_ef           = 1 << 1;
    static const uint16_t _bp_alarm_low    = 1 << 15;
    static const uint16_t _bp_alarm_high   = 1 << 14;
    
    DigitalOut *_cs;
    SPI *_spi;
    bool alarm;
public:
    AS5055A(DigitalOut *cs, SPI *spi, char *name);
private:
    int calculate_parity(int value);
public:
    virtual uint16_t get_raw();
    virtual float get_angle();
};


#endif/*AS5055A_h*/