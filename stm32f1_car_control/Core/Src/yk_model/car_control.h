//
// Created by PC on 2024/4/22.
//

#ifndef STM32F1_CAR_CONTROL_CAR_CONTROL_H
#define STM32F1_CAR_CONTROL_CAR_CONTROL_H

#define CAR_CONTROL_PWM_MAX (255)
#define CAR_CONTROL_PWM_MIN (0)


/**
 * 前后控制
 * @param speed >0 向前  <0 向后
 */
void car_go(int val);


/**
 * 左右控制
 * @param val 左右控制
 */
void turn(int val);


void car_control_enable(void);


void car_control_close(void);

void car_control_init(void);

#endif //STM32F1_CAR_CONTROL_CAR_CONTROL_H
