//
// Created by PC on 2024/4/22.
//

#include <stdlib.h>
#include <math.h>
#include "car_control.h"
#include "tim.h"


// �������
void turn_pros(void);

// �����ת
void turn_cons(void);


// ��ת
void turn_left(void);

// ��ת
void turn_right(void);

/**
 * ǰ�����
 * @param speed >0 ��ǰ  <0 ���
 */
void car_go(int val) {
    float f, b, o;
    // ��ȡval��Χ����f
    f = CAR_CONTROL_PWM_MAX - CAR_CONTROL_PWM_MIN;
    // ��f / pwmռ�ձȷ�Χ�����val��λ��ռ�ձȵı���b
    b = f / htim2.Init.Prescaler;
    // ��b*v���Ӧ��Ӧ�����ռ�ձ����� o
    o = b * val;
    // �ж�oΪ�������Ǹ���������ǰ�����������ˣ�ͨ�����Ƶ������ģ��IN1 IN2 �����Ƶ������ת
    if (o >= 0) {
        // ��ת
        turn_pros();
    }
    if (o < 0) {
        turn_cons();
    }

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, abs(o));

}


/**
 * ���ҿ���
 * @param val ���ҿ��� ���ٶ�
 */
void turn(int val) {
    float f, b, o;
    // ��ȡval��Χ����f
    f = CAR_CONTROL_PWM_MAX - CAR_CONTROL_PWM_MIN;
    // ��f / pwmռ�ձȷ�Χ�����val��λ��ռ�ձȵı���b
    b = f / htim2.Init.Prescaler;
    // ��b*v���Ӧ��Ӧ�����ռ�ձ����� o
    o = b * val;
    // �ж�oΪ�������Ǹ���������ǰ�����������ˣ�ͨ�����Ƶ������ģ��IN1 IN2 �����Ƶ������ת
    if (o >= 0) {
        // ��ת
        turn_left();
    }
    if (o < 0) {
        // ��ת
        turn_right();
    }

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, fabsf(o));

}


void car_control_enable(void) {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, CAR_CONTROL_PWM_MIN);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET); // ����ʹ������

}

void car_control_close(void) {
    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_4 | TIM_CHANNEL_3, CAR_CONTROL_PWM_MIN);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12 | GPIO_PIN_13, GPIO_PIN_SET); // ����ʹ������

}

void init_car_control(void) {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1 | TIM_CHANNEL_2 | TIM_CHANNEL_3 | TIM_CHANNEL_4);

    // ���Ƶ��ʹ��
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);



    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
}


// �������
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


// �����ת
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

// �������
void turn_left(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_SET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
}


// �����ת
void turn_right(void) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_12, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

