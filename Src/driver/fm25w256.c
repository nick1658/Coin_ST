#include "main.h"
#include "user_app.h"

uint8_t WRSR = 0x01;
uint8_t WRITE = 0x02;
uint8_t READ = 0x03;
uint8_t WRDI = 0x04;
uint8_t RDSR = 0x05;
uint8_t WREN = 0x06;
uint8_t DUMMY = 0xff;

extern void delay_us(uint32_t time);

uint8_t framWriteEnable (void)
{
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_RESET); // CS = 0
  HAL_SPI_Transmit(&hspi1, &WREN, 1, 1000);
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_SET); // CS = 1
	return 0;
}

uint8_t framWriteDisable (void)
{
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_RESET); // CS = 0
  HAL_SPI_Transmit(&hspi1, &WRDI, 1, 1000);
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_SET); // CS = 1
	return 0;
}

uint8_t framReadStatus (void)
{
  uint8_t Ret;
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_RESET); // CS = 0
  delay_us(1);
 
  HAL_SPI_Transmit(&hspi1, &RDSR, 1, 1000);
  HAL_SPI_Receive(&hspi1, &Ret, 1, 1000);
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_SET); // CS = 1
  delay_us(1);
  
  return Ret;
}

void framWriteOneByte(uint16_t Address, uint8_t Data)
{
  uint8_t AddressMSB = (uint8_t)((Address & 0xFF00) >> 8);
  uint8_t AddressLSB = (uint8_t)(Address & 0x00FF);
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_RESET); // CS = 0
  
  HAL_SPI_Transmit(&hspi1, &WRITE, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressMSB, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressLSB, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &Data, 1, 1000);
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_SET); // CS = 1
}

uint8_t framReadOneByte(uint16_t Address)
{ 
  uint8_t Ret;
  
  uint8_t AddressMSB = (uint8_t)((Address & 0xFF00) >> 8);
  uint8_t AddressLSB = (uint8_t)(Address & 0x00FF);
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_RESET); // CS = 0
 
  HAL_SPI_Transmit(&hspi1, &READ, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressMSB, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressLSB, 1, 1000);
  HAL_SPI_Receive(&hspi1, &Ret, 1, 1000);
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_SET); // CS = 1
  
  return Ret;
}

uint8_t framReadBytes(uint16_t Address, uint8_t *buf, uint16_t length)
{ 
  uint8_t Ret = 1;
  
  uint8_t AddressMSB = (uint8_t)((Address & 0xFF00) >> 8);
  uint8_t AddressLSB = (uint8_t)(Address & 0x00FF);
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_RESET); // CS = 0
 
  HAL_SPI_Transmit(&hspi1, &READ, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressMSB, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressLSB, 1, 1000);
	if (buf != NULL){
		while (length--){
			HAL_SPI_Receive(&hspi1, buf++, 1, 1000);
		}
	}else{
		Ret = 0;
	}
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_SET); // CS = 1
  
  return Ret;
}

uint8_t framWriteBytes(uint16_t Address, uint8_t *buf, uint16_t length)
{
  uint8_t Ret = 1;
  uint8_t AddressMSB = (uint8_t)((Address & 0xFF00) >> 8);
  uint8_t AddressLSB = (uint8_t)(Address & 0x00FF);
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_RESET); // CS = 0
  
  HAL_SPI_Transmit(&hspi1, &WRITE, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressMSB, 1, 1000);
  HAL_SPI_Transmit(&hspi1, &AddressLSB, 1, 1000);
	if (buf != NULL){
		while (length--){
			HAL_SPI_Transmit(&hspi1, buf++, 1, 1000);
		}
	}else{
		Ret = 0;
	}
  
  HAL_GPIO_WritePin(GPIOB, SPI1_CS_Pin, GPIO_PIN_SET); // CS = 1
  return Ret;
}
