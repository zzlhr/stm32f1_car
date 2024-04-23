//
// Created by 刘浩然 on 2024/4/1.
// 用于通讯
//

#ifndef YT_2_YT_COMM_H
#define YT_2_YT_COMM_H


#include <stdbool.h>
#include "../yk_core/yk_typedef.h"
#include "../application.h"

typedef void (*send_msg)(uint8_t *par, int size);

/**
 * 初始化函数
 * @return
 */
T_YkReturnCode Yt_Comm_Init(void);

/**
 * 任务启动函数
 * @return
 */
void Yt_Comm_Start(void);




/**
 * 消息派发器
 * @return
 */
#if USE_ETH_CONTROL
T_YkReturnCode Yt_Comm_Msg_Dispath(Yk_Comm_Typedef *msg, send_msg send_msg);

#elif USE_UART_CONTROL

T_YkReturnCode Yt_Comm_Msg_Dispath(Yk_Comm_Typedef *msg);

#endif


/**
 * 消息解析
 * @return
 */
bool Yt_Recv_Msg_Byte_Parse(uint8_t data, Yk_Comm_Typedef *msg);


/**
 * 发送函数
 * @return
 */
T_YkReturnCode Yt_Comm_Msg_Send(void);


/**
 * 接收消息
 * @return
 */
T_YkReturnCode Yt_Comm_Msg_Recv(void);


/**
 * 接收消息
 * @return
 */
T_YkReturnCode Yt_Comm_Msg_Recv_CRC(void);

#endif //YT_2_YT_COMM_H
