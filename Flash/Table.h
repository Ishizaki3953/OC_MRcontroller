/**
  ******************************************************************************
  * @file           : Table.h
  * @brief          : table access
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef TABLE_h
#define TABLE_h

#include "mbed.h"
#include "Flash.h"

#define POSI_MAX  1024

typedef union{
/* USER CODE BEGIN -----------------------------------------------------------*/
    //uint8_t d[128*1024];//sector11 max size
    struct {
        int8_t pwmA;//default is FFh(Flash Erase Value)
        int8_t pwmB;
        int8_t pwmC;
        int8_t pwmD;
    } s[POSI_MAX];
/* USER CODE END -------------------------------------------------------------*/
} table_t;

class Table{
private:
    Flash _setting;
    static const int SECTOR=11;
    table_t _table;
    uint8_t *_p;
public:
    Table();
    void begin_write();
    bool write(uint8_t c);
    void end_write();
    
    void begin_read();
    int read(uint8_t *data, int size);
    void end_read();
};

#endif/*TABLE_h*/

