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
//#define PLOT_ASW
//#define PLOT_ASD

typedef enum{
    PAT_A=0,
    PAT_B,
    PAT_C,
    PAT_D,
    PAT_MAX,
} PATTERN;

extern SerialEx pc;
extern DigitalOut g_green;
extern InterruptInEx g_lclick_asw;
extern InterruptInEx g_rclick_asw;
extern InterruptInEx g_lclick_asd;
extern InterruptInEx g_rclick_asd;

extern int g_lclkCnt_asw;
extern int g_rclkCnt_asw;
extern int g_lrlsCnt_asw;
extern int g_rrlsCnt_asw;

extern int g_lclkCnt_asd;
extern int g_rclkCnt_asd;
extern int g_lrlsCnt_asd;
extern int g_rrlsCnt_asd;


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

