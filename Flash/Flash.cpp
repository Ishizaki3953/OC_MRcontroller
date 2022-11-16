/**
  ******************************************************************************
  * @file           : Flash.cpp
  * @brief          : flash access
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#include "Flash.h"

/**
  * @brief flash erase
  * @param  sector number
  * @retval none
  */
void Flash::erase(int sector){
    SOFBlock::format(sector);
}
/**
  * @brief flash begin write
  * @param  sector: number  erase: true is erase
  * @retval none
  */
void Flash::begin_write(int sector, bool erase){
    if(erase) SOFBlock::format(sector);
    _w.open(sector);
}
/**
  * @brief flash write
  * @param  data: datas  size:data size
  * @retval none
  */
void Flash::write(uint8_t *data, int size){
    _w.write_data(data, size);
}
/**
  * @brief flash write
  * @param  data: data
  * @retval none
  */
void Flash::write(uint8_t data){
    _w.write_data(&data, 1);
}
/**
  * @brief flash end write
  * @param  none
  * @retval none
  */
void Flash::end_write(){
    _w.close();
}

/**
  * @brief flash begin read
  * @param  sector: number
  * @retval none
  */
void Flash::begin_read(int sector){
    _r.open(sector);
    _size = _r.get_data_size();
    _pr = _r.get_physical_data_addr();
}
/**
  * @brief flash read
  * @param  data:datas   size:data size
  * @retval none
  */
int Flash::read(uint8_t *data, int size){
    if(_size == 0){
        //no data
        *data = NULL;
        return 0;
    }
    if(_size >= size){
        memcpy(data, _pr, size);
        _pr += size;
        _size -= size;
        return size;
    }
    //amari data
    int n = _size;
    memcpy(data, _pr, _size);
    _pr += _size;
    _size = 0;
    return n; 
}
/**
  * @brief flash end read
  * @param  none
  * @retval none
  */
void Flash::end_read(){
    _r.close();
}

