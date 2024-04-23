//
// Created by 刘浩然 on 2024/4/1.
//

#ifndef YT_2_YK_TYPEDEF_H
#define YT_2_YK_TYPEDEF_H


#include <stdint.h>

typedef uint64_t T_YkReturnCode;


typedef struct Yk_Comm_Typedef {
    uint8_t Head;

    uint8_t Len;

    uint8_t Msg_Id;

    uint8_t* Payload;

    uint8_t crc;
} Yk_Comm_Typedef;

/**
 * 串口枚举
 */
typedef enum {

    YK_HAL_UART_NUM_0,

    YK_HAL_UART_NUM_1,

} Yk_HalUartNum;

#endif //YT_2_YK_TYPEDEF_H
