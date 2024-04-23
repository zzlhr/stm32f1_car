//
// Created by PC on 2024/4/22.
//

#include <stdlib.h>
#include <math.h>
#include "car_control.h"
#include "tim.h"


// 电机正传
void turn_pros(void);

// 电机反转
void turn_cons(void);


// 左转
void turn_left(void);

// 右转
void turn_right(void);

/**
 * 前后控制
 * @param speed >0 向前  <0 向后
 */
void car_go(int val) {
    float f, b, o;
    // 获取val范围数据f
    f = CAR_CONTROL_PWM_MAX - CAR_CONTROL_PWM_MIN;
    // 用f / pwm占空比范围，求出val单位与占空比的比例b
    b = f / htim2.Init.Prescaler;
    // 用b*v求出应对应输出的占空比数据 o
    o = b * val;
    // 判断o为正数还是负数，正数前进，负数后退，通过控制电机驱动模块IN1 IN2 来控制电机正反转
    if (o >= 0) {
        // 正转
        turn_pros();
    }
    if (o < 0) {
        turn_cons();
    }

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, abs(o));

}


/**
 * 左右控制
 * @param val 左右控制 的速度
 */
void turn(int val) {
    float f, b, o;
    // 获取val范围数据f
    f = CAR_CONTROL_PWM_MAX - CAR_CONTROL_PWM_MIN;
    // 用f / pwm占空比范围，求出val单位与占空比的比例b
    b = f / htim2.Init.Prescaler;
    // 用b*v求出应对应输出的占空比数据 o
    o = b * val;
    // 判断o为正数还是负数，正数前进，负数后退，通过控制电机驱动模块IN1 IN2 来控制电机正反转
    if (o >= 0) {
        // 左转
        turn_left();
    }
    if (o < 0) {
        // 右转
        turn_right();
    }

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, fabsf(o));

}


void car_control_enable(void) {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, CAR_CONTROL_PWM_MIN);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET); // 控制使能引脚

}

void car_control_close(void) {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, CAR_CONTROL_PWM_MIN);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET); // 控制使能引脚

}

void init_car_control(void) {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);

    // 控制电机使能
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);



    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}


// 电机正传
void turn_pros(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);


    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}


// 电机反转
void turn_cons(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);




    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
}

// 电机正传
void turn_left(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}


// 电机反转
void turn_right(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

