//
// Created by 刘浩然 on 2024/4/1.
//

#include "yt_comm.h"
#include "../yk_core/yk_error_code.h"
#include "../yk_core/yk_log.h"
#include "yk_msg.h"
#include "../yk_model/car_control.h"
#include "cmsis_os2.h"
#include <stdlib.h>

#define MSG_HEADER 0x8D
#define MAX_MSG_LEN 128
#define MAX_LEN 128

uint8_t recv_data[MAX_LEN];

uint16_t parse_index = 0;

void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size) {
    uint8_t len;
    if (huart->Instance == USART2) {
        HAL_UART_DMAStop(huart);
        len = MAX_LEN - __HAL_DMA_GET_COUNTER(&hdma_usart2_rx);// 获取DMA中传输的数据个数
        YK_LOG_INFO("recv data len:%d", len);
        Yk_Comm_Typedef msg;

        for (int i = 0; i < len; ++i) {
            if (Yt_Recv_Msg_Byte_Parse(recv_data[i], &msg)) {
                // 解析成功调用派发器处理
                Yt_Comm_Msg_Dispath(&msg);
            }
        }

        HAL_UARTEx_ReceiveToIdle_DMA(huart, recv_data, MAX_LEN);        // 再次开启DMA空闲中断
    }
}

T_YkReturnCode Yt_Comm_Init(void) {
#if USE_ETH_CONTROL
    //    TCP_Echo_Init();
#endif
    return YK_SUCCESS;
}


void Yt_Comm_Start(void) {
    YK_LOG_INFO("Yt_Comm_Start ...");

#if USE_UART_CONTROL
//    uint8_t result;
    HAL_UARTEx_ReceiveToIdle_DMA(&huart2, recv_data, MAX_LEN);
    while (1);

#endif
}


bool Yt_Recv_Msg_Byte_Parse(uint8_t data, Yk_Comm_Typedef *msg) {
    // 解析协议，将数据存储在
    if (data == MSG_HEADER && parse_index == 0) {
        // 协议首帧
        msg = (Yk_Comm_Typedef *) malloc(MAX_MSG_LEN);
        msg->Head = MSG_HEADER;
        parse_index++;
    } else if (parse_index == 1) {
        msg->Len = data;
        parse_index++;
    } else if (parse_index == 2) {
        msg->Msg_Id = data;
        parse_index++;
    } else if (parse_index > 1 && msg->Len > 0 && parse_index < msg->Len + 3) {
        // 数据帧
        if (parse_index == 3) {
            // 初始化内存
            msg->Payload = (uint8_t *) malloc(msg->Len);
        }
        msg->Payload[parse_index - 3] = data;
        parse_index++;
    } else if (parse_index > 1 && parse_index == msg->Len + 3) {
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
//    YK_LOG_INFO("recv data: header: %02X , Len :%02X, Msg_Id: %02X, crc:%02X",
//                msg->Head, msg->Len, msg->Msg_Id, msg->crc);

    switch (msg->Msg_Id) {
        case CAR_CONTROL_ENABLE: {
            YK_LOG_INFO("CAR_CONTROL_ENABLE");
            car_control_enable();
            break;
        }
        case CAR_CONTROL_CLOSE: {
            YK_LOG_INFO("CAR_CONTROL_CLOSE");

            car_control_close();
            break;
        }
        case CAR_GO: {
            YK_LOG_INFO("CAR_GO");

            car_go(msg->Payload[0]);

            break;
        }
        case CAR_TURN: {
            YK_LOG_INFO("CAR_TURN");

            turn(msg->Payload[0]);
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
