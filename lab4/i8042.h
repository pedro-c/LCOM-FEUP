#ifndef __I8042_H
#define __I8042_H


#define BIT(n) (0x01<<(n))

#define TIMER0_IRQ	        0
#define NOTIFICATION_MOUSE  1
#define NOTIFICATION_TIMER  0

#define DELAY_US 20000

#define MOUSE_IRQ           12
#define STAT_REG            0x64
#define OUT_BUF             0x60
#define IBF                 0x02
#define OBF                 0x01
#define MC                  0xD4
#define COMMAND_PORT        0x60
#define PAR_ERR             0x80
#define TO_ERR              0x40
#define DISABLE_STREAM      0xF5
#define ENABLE_SEND         0xF4
#define SET_STREAM          0xEA
#define STATUS_REQUEST      0xE9
#define ACK                 0xFA
#define NACK                0xFE
#define ENABLE              0xA8        //ENABLE MOUSE
#define W_TO_MOUSE          0xD4        //WRITE BYTE TO MOUSE
#define DISABLE_SM          0xF5        //DISABLE STREAM MODE


#endif
