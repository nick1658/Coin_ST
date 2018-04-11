#ifndef __fm25w256_H
#define __fm25w256_H
#ifdef __cplusplus
extern "C" {
#endif
 
//#include "stm32f1xx_hal.h"
 
void framWriteOneByte(uint16_t Address, uint8_t Data);
uint8_t framReadOneByte(uint16_t Address);
uint8_t framReadStatus (void);
uint8_t framWriteEnable (void);
uint8_t framWriteDisable (void);
uint8_t framReadBytes(uint16_t Address, uint8_t *buf, uint16_t length);
uint8_t framWriteBytes(uint16_t Address, uint8_t *buf, uint16_t length);
 
#ifdef __cplusplus
}
#endif
#endif



