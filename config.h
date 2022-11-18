/**
  ******************************************************************************
  * @file           : config.h
  * @brief          : configuration
  ******************************************************************************
  * @attention
  *
  ******************************************************************************
  */
#ifndef CONFIG_h
#define CONFIG_h

#define ZERO_TORQUE  500 //zero torque [ms]
#define RLONG_TIME  10 //右クリック長押し判定時間 [50ms]

//period percent
static float g_width[] = {
    5.0, 10.0, 16.0, 20.0, 25.0,
    32.0, 40.0, 50.0, 80.0, 100.0
};

//voltage percent
//static float g_height[] = {
//    10.0, 20.0, 30.0, 40.0, 50.0,
//    60.0, 70.0, 80.0, 90.0, 100.0
//};

#endif/*CONFIG_h*/

