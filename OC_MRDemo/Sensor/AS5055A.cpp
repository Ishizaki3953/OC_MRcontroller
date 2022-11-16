/**
  ******************************************************************************
  * @file           : AS5055A.cpp
  * @brief          : encoder
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "AS5055A.h"

/**
  * @brief constructor
  * @param  cs:chip select  spi:spi  name:this name
  * @retval none
  */
AS5055A::AS5055A(DigitalOut *cs, SPI *spi, char *name) : ISensor(name){
    _cs = cs;
    _spi = spi;
    _spi->format(8, 1);
    _spi->frequency(10000000);//10M
}
/**
  * @brief calculate patriy
  * @param  input value
  * @retval parity
  */
int AS5055A::calculate_parity(int value){
    int cnt = 0;
    for(int i = 0; i < 16; i++){ //16bit
        if(value & 0x1) cnt++;
        value >>= 1;
    }
    return cnt & 0x1;
}
/**
  * @brief get raw(AD)
  * @param  none
  * @retval raw
  */
uint16_t AS5055A::get_raw(){
    alarm = false;
    // 15 14 13 12 11 10 09 08 07 06 05 04 03 02 01 00
    // RW ADDRESS[14:1]                             PAR
    // 1  3FF(Angular Data)                         1
    _cs->write(0);
    uint16_t result1 = _spi->write(0xff);
    uint16_t result2 = _spi->write(0xff);
    _cs->write(1);
    
    result1 = result1 << 8;
    uint16_t result = (result1 | result2);
    
    if(calculate_parity(result)){
        printf("parity error\n");
    }
    if((result & _bp_ef)){
        printf("error\n");
    }
    if((result & _bp_alarm_low)){//b13
        //pc.printf("alarm low\n");
        alarm = true;
    }
    if((result & _bp_alarm_high)){//b12
        //pc.printf("alarm high\n");
        alarm = true;
    }
    result &= 0x3FFF;//alarm remove
    result = result >> 2;
    //wait_us(500); //--> POINT!!
    return result;//12bit
}
/**
  * @brief get angle
  * @param  none
  * @retval angle
  */
float AS5055A::get_angle(){
    uint16_t raw = get_raw(); //0-4095
    float angle = raw * 360 / 4096;
    printf("[SPI] raw=%d, angle=%f\n", raw, angle);
    return angle;
}
