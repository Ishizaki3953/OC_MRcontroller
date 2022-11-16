/**
  ******************************************************************************
  * @file           : Table.cpp
  * @brief          : table access
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "Table.h"
#include "SOFBlock.h"
#include "SOF_dev.h"

/**
  * @brief constructor
  * @param  none
  * @retval none
  */
Table::Table(){
    //data load
    _setting.begin_read(SECTOR);
    _setting.read((uint8_t*)&_table, sizeof(_table));
    _setting.end_read();
}
/**
  * @brief table begin write
  * @param  none
  * @retval none
  */
void Table::begin_write(){
    _p = (uint8_t*)&_table;
}
/**
  * @brief table write
  * @param  byte data
  * @retval true is succeed
  */
bool Table::write(uint8_t c){
    if(_p < (uint8_t*)&_table + sizeof(_table)){
        *_p++ = c;
        return true;
    }
    return false;
}
/**
  * @brief table end write
  * @param  none
  * @retval none
  */
void Table::end_write(){
    _setting.begin_write(SECTOR, true);
    _setting.write((uint8_t*)&_table, sizeof(_table));
    _setting.end_write();
}


/**
  * @brief table begin read
  * @param  none
  * @retval none
  */
void Table::begin_read(){
    _setting.begin_read(SECTOR);
}
/**
  * @brief table read
  * @param  data:data  size:data size
  * @retval read size
  */
int Table::read(uint8_t *data, int size){
    return _setting.read(data, size);
}
/**
  * @brief table end read
  * @param  none
  * @retval none
  */
void Table::end_read(){
    _setting.end_read();
}

