//
// Created by 刘浩然 on 2024/4/1.
//

#include <stdio.h>
#include "yt_comm.h"
#include "usart.h"
#include "../yk_core/yk_error_code.h"
#include "../yk_core/yk_log.h"
#include "yk_msg.h"
#include "../yk_model/car_control.h"
#include <stdlib.h>

#define MSG_HEADER 0x8D
#define MAX_MSG_LEN 128
#define MAX_LEN 1
#define READ_TIMEOUT (MAX_LEN * 10)


uint16_t parse_index = 0;


T_YkReturnCode Yt_Comm_Init(void) {
#if USE_ETH_CONTROL
    //    TCP_Echo_Init();
#endif
    return YK_SUCCESS;
}


void Yt_Comm_Start(void) {
    uint8_t recv_data[MAX_LEN];
    Yk_Comm_Typedef msg;
    YK_LOG_INFO("Yt_Comm_Start ...");

#if USE_UART_CONTROL
    uint8_t result;
    while (1) {
        result = HAL_UART_Receive_IT(&huart2, recv_data, MAX_LEN);
        if (result != HAL_OK) {
            if (result == HAL_TIMEOUT) {
//                YK_LOG_WARN("read serial data time out....");
            }
            if (result == HAL_ERROR) {
//                YK_LOG_WARN("read serial data error....");
            }
            if (result == HAL_BUSY) {
//                YK_LOG_WARN("read serial data busy....");
            }
//            HAL_Delay(100);
            continue;
        }
        printf("recv_data: ");
        for (int i = 0; i < MAX_LEN; ++i) {
            printf("%02X ", recv_data[i]);
            if (Yt_Recv_Msg_Byte_Parse(recv_data[i], &msg)) {
                // 解析成功调用派发器处理
                printf("\r\n");
                Yt_Comm_Msg_Dispath(&msg);
            }
        }
//        HAL_Delay(100);
    }
#endif


}


bool Yt_Recv_Msg_Byte_Parse(uint8_t data, Yk_Comm_Typedef *msg) {
    // 解析协议，将数据存储在
    if (data == MSG_HEADER && parse_index == 0) {
        // 协议首帧
        YK_LOG_INFO("1");
        msg = (Yk_Comm_Typedef *) malloc(MAX_MSG_LEN);
        msg->Head = MSG_HEADER;
        parse_index++;
    } else if (parse_index == 1) {
        YK_LOG_INFO("2");
        msg->Len = data;
        parse_index++;
    } else if (parse_index == 2) {
        YK_LOG_INFO("3");
        msg->Msg_Id = data;
        parse_index++;
    } else if (parse_index > 1 && msg->Len > 0 && parse_index < msg->Len + 3) {
        // 数据帧
        YK_LOG_INFO("4");
        if (parse_index == 3) {
            // 初始化内存
            msg->Payload = (uint8_t *) malloc(msg->Len);
        }
        msg->Payload[parse_index - 3] = data;
        parse_index++;
    } else if (parse_index > 1 && parse_index == msg->Len + 3) {
        YK_LOG_INFO("5");
        // 校验帧
        //TODO: 校验
        msg->crc = data;
        // 清理变量
        parse_index = 0;
        return true;
    }

    return false;


}


#if USE_ETH_CONTROL

T_YkReturnCode Yt_Comm_Msg_Dispath(Yk_Comm_Typedef *msg, send_msg send_msg) {
    YK_LOG_INFO("recv data: header: %02X, Len :%02X, Msg_Id: %02X, crc:%02X",
                msg->Head, msg->Len, msg->Msg_Id, msg->crc);
    for (int i = 0; i < msg->Len - 3; i++) {
        printf("%02X ", msg->Payload[i]);
    }
    uint8_t *status;

    switch (msg->Msg_Id) {
        case SERVO_STATUS:
            Yk_Servo_Get_Status(&status);
            send_msg(status, 8);
//            tcp_write(tpcb, &status, 8, 1);
            break;
        case SERVO_CONTROL:
            if (msg->Payload[0]) {
                Yk_Servo_Control_Open(1);
            } else {
                Yk_Servo_Control_Open(0);
            }
            break;
        default:

            break;
    }


    return YK_SUCCESS;
}

#elif USE_UART_CONTROL

T_YkReturnCode Yt_Comm_Msg_Dispath(Yk_Comm_Typedef *msg) {
    YK_LOG_INFO("recv data: header: %02X, Len :%02X, Msg_Id: %02X, crc:%02X",
                msg->Head, msg->Len, msg->Msg_Id, msg->crc);
    for (int i = 0; i < sizeof(msg->Payload); i++) {
        printf("%02X ", msg->Payload[i]);
    }

    uint8_t *res;
    switch (msg->Msg_Id) {
        case CAR_CONTROL_ENABLE: {
            res = malloc(sizeof(uint8_t) * 4);
            res[0] = 0x8d;
            res[1] = 0x00;
            res[2] = 0x10;
            res[3] = 0x00;
            car_control_enable();
            HAL_UART_Transmit(&huart1, res, 4, 0xff);
            break;
        }
        case CAR_CONTROL_CLOSE: {
            res = malloc(sizeof(uint8_t) * 4);
            res[0] = 0x8d;
            res[1] = 0x00;
            res[2] = 0x11;
            res[3] = 0x00;
            car_control_close();
            HAL_UART_Transmit(&huart1, res, 4, 0xff);
            break;
        }
        case CAR_GO: {
            res = malloc(sizeof(uint8_t) * 4);
            res[0] = 0x8d;
            res[1] = 0x00;
            res[2] = 0x12;
            res[3] = 0x00;
            car_go(msg->Payload[0]);
            HAL_UART_Transmit(&huart1, res, 4, 0xff);

            break;
        }
        case CAR_TURN: {
            res = malloc(sizeof(uint8_t) * 4);
            res[0] = 0x8d;
            res[1] = 0x00;
            res[2] = 0x13;
            res[3] = 0x00;
            turn(msg->Payload[0]);
            HAL_UART_Transmit(&huart1, res, 4, 0xff);
            break;
        }
        default: {
            break;
        }

    }

    return YK_SUCCESS;
}

#endif


/*******************************网口通讯***************************************/
