/**
  ******************************************************************************
  * @file           : common.h
  * @brief          : common defines
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef COMMON_h
#define COMMON_h

#include "mbed.h"
#include "SerialEx.h"
#include "InterruptInEx.h"

typedef void (*CALLBACK)(void);

#define NOP()

extern SerialEx pc;
extern DigitalOut g_green;
extern InterruptInEx g_lclick;
extern InterruptInEx g_rclick;

extern int g_lclkCnt;
extern int g_rclkCnt;
extern int g_lrlsCnt;
extern int g_rrlsCnt;


/* UTILITY -------------------------------------------------------------------*/
/**
  * @brief get two's complement
  * @param  raw:raw  length:convert bit size
  * @retval none
  */
static void getTwosComplement(int32_t *raw, uint8_t length){
    if (*raw & ((uint32_t)1 << (length - 1))){
        *raw -= (uint32_t)1 << length;
    }
}

#endif/*COMMON_h*/

