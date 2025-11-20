/**
  ******************************************************************************
  * @file           : ads1220.c
  * @brief          : source file for the ADS1220
  * @author         : henri busch
  * @date           :
  * @project        :
  ******************************************************************************
  * @attention
  * TODO: chip select continuously on
  * TODO: All Data ready signals sending over a the data line
  * TODO: Other system for channel -> Set the Ports and Pins once in a definde, and then only change it over a function call
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "stm32wlxx_hal.h"
#include "stm32wlxx_hal_def.h"
#include "spi.h"
#include "ads1220.h"
#include "usart.h"
#include "inttypes.h"

/* variables -----------------------------------------------------------------*/
GPIO_TypeDef* PORT_ADS1220_CS[3] = {GPIOA, GPIOB, GPIOC};
uint8_t PIN_ADS1220_CS[3] = {GPIO_PIN_1,GPIO_PIN_2, GPIO_PIN_2};
SPI_HandleTypeDef* SPIChannel = &hspi1;

/* function ------------------------------------------------------------------*/
/*
 * @brief: Transmitting and receiving data over the SPI bus
 * @param hspi: A pointer to a SPI_HandleTypeDef struct that defines the SPI peripheral to use for the transmission.
 * @param RxData: A pointer to a buffer that will hold the received data.
 * @param arLength: The length of the data to transmit and receive.
 * @param TxData: The data to transmit over the SPI bus.
 */
void SPI_ReadWriteByte(SPI_HandleTypeDef *hspi, uint8_t* RxData, int arLength, uint8_t* TxData)
{
	// Delay for a minimum of d(CSSC)
	HAL_Delay(T_CSSC);
	// Sends
	HAL_SPI_TransmitReceive(&hspi1, TxData, RxData, arLength, HAL_MAX_DELAY); // Wait for 1000 ms = 1s
	// Delay for a minimum of t d(SCCS)
	HAL_Delay(T_SCCS);
}

void SPI_ReadByte(SPI_HandleTypeDef* hspi, uint8_t* RxData, int arLength) {
	// Delay for a minimum of d(CSSC)
	HAL_Delay(T_CSSC);
	// Sends
	HAL_SPI_Receive(&hspi1, RxData, arLength, HAL_MAX_DELAY); // Wait for 1000 ms = 1s
	// Delay for a minimum of t d(SCCS)
	HAL_Delay(T_SCCS);
}


//void SPI_ReadWriteByte(SPI_HandleTypeDef *hspi, uint8_t* RxData, int arLength, uint8_t TxData)
//{
//	// Delay for a minimum of d(CSSC)
//	HAL_Delay(T_CSSC);
//	uint8_t ar[arLength];
//	ar[0] = TxData;
//	for(int i = 1; i < arLength; i++){
//		ar[i] = TxData + i;
//	}
//	// Sends
//	//TODO: maybe 3*8
////	HAL_SPI_TransmitReceive(&hspi1, ar, RxData, arLength, HAL_MAX_DELAY); // Wait for 1000 ms = 1s
//	HAL_SPI_Receive(&hspi1, RxData, arLength, HAL_MAX_DELAY);
//	// Delay for a minimum of t d(SCCS)
//	HAL_Delay(T_SCCS);
//}
/*
 * @brief:  Initializes the Chip Select (CS) pin of the ADS1220 ADC
 * @param status: The desired state of the CS pin (GPIO_PIN_RESET or GPIO_PIN_SET)
 */
void CS_Select_Init(GPIO_PinState status)
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, (GPIO_PinState) status); //HAL_GPIO_WritePin(PORT_ADS1220_CS[channel], PIN_ADS1220_CS[channel], (GPIO_PinState)status);
}

/*
 * @brief: Initializes the Chip Select (CS) pin of a device
 * @param type: Pointer to the GPIO port used for the CS pin (e.g., GPIOA, GPIOB, etc.)
 * @param gpioPin: The GPIO pin number used for the CS pin (e.g., GPIO_PIN_1, GPIO_PIN_2, etc.)
 * @param status: The desired state of the CS pin (GPIO_PIN_RESET or GPIO_PIN_SET)
 *
 * TODO: Use the function instead of the CS_Select_Init()
 */
void CS_Select_Init_2(GPIO_TypeDef* type, uint16_t gpioPin, GPIO_PinState status)

// GPIO_TypeDef
{
	HAL_GPIO_WritePin(type, gpioPin, (GPIO_PinState)status); //HAL_GPIO_WritePin(PORT_ADS1220_CS[channel], PIN_ADS1220_CS[channel], (GPIO_PinState)status);
}


/*
 * @brief: Performs a reset of the ADS1220 ADC using the SPI protocol
 */
void ADS1220_RESET(void)
{
	//Pull down the chip select and start transferring data
	CS_Select_Init(0);
	HAL_Delay(T_CSSC);

	uint8_t dataBuf[1];
	uint8_t transBuf[1];
	transBuf[0] = ADS1220_CMD_RESET;
	//ADS1220 reset command 0x06
	SPI_ReadWriteByte(SPIChannel, dataBuf, 1, transBuf);

	//Pull up chip selection to stop data transfer
	CS_Select_Init(1);

}

/*
 * @brief: Configures the ADS1220 ADC by writing values to its registers and setting it to continuous conversion mode
 */
// TODO: Continuous conversion mode on an then working with interrupts
// TODO: WATCH!!!! the timing with the
void ADS1220_Config(void)
{
	//TODO: Check
	uint8_t CH_cfg[4] = {
			ADS1220_MUX_0_1 | ADS1220_GAIN_32 | ADS1220_PGA_BYPASS_ENABLE,                    //register 0 configuration
			ADS1220_DR_20 | ADS1220_CC_NORMAL | ADS1220_CONTINUOUS  , //register 1 configuration
			ADS1220_VREF_INT | ADS1220_PSW_ON ,                                              //register 2 configuration
			ADS1220_IDAC_OFF | ADS1220_IDAC_OFF                                              //register 3 configuration
			};

	////reset ADS1220 chip (0x06)
	//ADS1220_RESET();

	//delay
	HAL_Delay(5);

	// ADS1220_Write_Register(uint8_t channel, uint8_t RegAddr, uint8_t Qty, uint8_t* pData)

	//write register WREG instruction 0x43, while configuring registers 0 - 3
	ADS1220_Write_Register(REG_CONFIG_0, 4, CH_cfg);

	// Delay
	HAL_Delay(5);

	////In continuous conversion mode, send START/SYNC command (08h) to start conversion
	//ADS1220_START_SYNC_Init();
}

/*
 * @brief: Configures the ADS1220 registers to enable temperature mode
 */
void ADS1220_Enable_Temperature_Mode(void){
	uint8_t CH_cfg[4] = {
	ADS1220_MUX_0_1 | ADS1220_GAIN_32 | ADS1220_PGA_BYPASS_ENABLE,                    //register 0 configuration
	ADS1220_DR_20 | ADS1220_CC_NORMAL | ADS1220_CONTINUOUS | ADS1220_Temperature_on , //register 1 configuration
	ADS1220_VREF_INT | ADS1220_PSW_ON ,                                              //register 2 configuration
	ADS1220_IDAC_OFF | ADS1220_IDAC_OFF                                              //register 3 configuration
	};
	ADS1220_Write_Register(REG_CONFIG_0, 4, CH_cfg);
	HAL_Delay(50);
}
/*
 * @brief: Configures the ADS1220 registers to disable temperature mode
 */
void ADS1220_Disable_Temperature_Mode(void){
	uint8_t CH_cfg[4] = {
	ADS1220_MUX_0_1 | ADS1220_GAIN_32 | ADS1220_PGA_BYPASS_ENABLE,                    //register 0 configuration
	ADS1220_DR_20 | ADS1220_CC_NORMAL | ADS1220_CONTINUOUS , //register 1 configuration
	ADS1220_VREF_INT | ADS1220_PSW_ON ,                                              //register 2 configuration
	ADS1220_IDAC_OFF | ADS1220_IDAC_OFF                                              //register 3 configuration
	};
	ADS1220_Write_Register(REG_CONFIG_0, 4, CH_cfg);
	HAL_Delay(50);
}

/*
 * @brief: Write a configuration register in the ADS1220 ADC.
 * @param RegAddr: The address of the register to write to.
 * @param Qty: The number of bytes to write (must be 1-4).
 * @param pData: Pointer to the buffer containing the data to write.
 *
 * @note The chip select is pulled down and data is transferred using SPI. The write register instruction is sent first,
 * followed by the address of the register to write to, and the number of bytes to write. Then, the data is sent byte by byte.
 * After all the data has been transferred, the chip select is pulled up to stop the data transfer.
 */
void ADS1220_Write_Register(uint8_t RegAddr, uint8_t regs, uint8_t* pData)
{
	uint8_t i = 0;

	// pull down the chip select and start transferring data
	CS_Select_Init(0);

	//Send the write register instruction, 0x43
	uint8_t dataBuf[1];
	uint8_t transBuff[1];
	transBuff[0] = ADS1220_CMD_WREG | (((RegAddr << 2) & 0x0c) | ((regs - 1) & 0x03);
	HAL_Delay(10);
	SPI_ReadWriteByte(SPIChannel, dataBuf, 1, transBuff));
	HAL_Delay(10);
	//Send data
	while(regs--)
	{
		SPI_ReadWriteByte(SPIChannel, dataBuf, 1, pData[i++]);
		HAL_Delay(10);
	}

	// Pull up the chip select to stop data transfer
	CS_Select_Init(1);
}

//void ADS1220_ReadRegister(int StartAddress, int NumRegs, unsigned * pData)
//{
//   int i;
//
//   // Pull up the chip select to stop data transfer
//   	CS_Select_Init(0);
//
//    // send the command byte
//    // SendByte(ADS1220_CMD_RREG | (((StartAddress<<2) & 0x0c) |((NumRegs-1)&0x03)));
//   	uint8_t dataBuf[1];
//    SPI_ReadWriteByte(SPIChannel, dataBuf,1, ADS1220_CMD_RREG | (((StartAddress<<2) & 0x0c) |((NumRegs-1)&0x03)));
//
//    // get the register content
//    for (i=0; i < NumRegs; i++)
//    {
//        SPI_ReadWriteByte(SPIChannel, dataBuf, 1, pData[i++]);
//    }
//
//    // Pull up the chip select to stop data transfer
//    CS_Select_Init(1);
//
//    return;
//}


/*
 * @brief:  Initializes the ADS1220 in continuous conversion mode by sending the START/SYNC command (08h) to start conversion.
 */
void ADS1220_START_SYNC_Init()
{
	// Pull down the chip select and start transmitting data
	CS_Select_Init(0);

	uint8_t dataBuf[1];
	uint8_t transBuff[1];
	transBuff[0] = ADS1220_CMD_SYNC;
	//Send start or restart instruction 0x08

	SPI_ReadWriteByte(SPIChannel, dataBuf, 1, transBuff);

	// Pull up the chip select to stop data transfer
	CS_Select_Init(1);
}

/*
 * @brief: Reads temperature data from the ADS1220
 * @param buffer: character array to store debug output
 * @return: temperature data as a floating-point value
 */
// TODO: change output type to float
// TODO: Try with 18 shift maybe?
float ADS1220_Read_Temp_Data(char buffer[50])
{
    int temp;
    int tmp2;
    uint32_t data;
    float final;

    uint8_t dataBuf[3];

    // pull down the chip select to start transferring data
    CS_Select_Init(0);

    // read data by command
    // TODO: use constant for command instead of hard-coded value
    HAL_Delay(1);
	SPI_ReadByte(SPIChannel, dataBuf, 3);  //SPI_ReadWriteByte(SPIChannel, dataBuf, 3, ADS1220_CMD_RREG | (((StartAddress<<2) & 0x0c) |((4-1)&0x03)));

    uint8_t data0 = dataBuf[0];
    uint8_t data1 = dataBuf[1];
    uint8_t data2 = dataBuf[2];

    data = data0; // 16
    data = data << 8 | data1; // 8
    data = data << 8 | data2;

    temp = data;
    sprintf(buffer, "Daten:          %d\r\n", temp);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

    data = data >> 10;

    tmp2 = data;
    sprintf(buffer, "Daten2:         %d\r\n", tmp2);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), HAL_MAX_DELAY);

    if (data >> 13) // is the MSB
    {
        data = ~(data-1) & 0x3fff;
        final = data * (-0.03125);
    }
    else {
        final = data * (0.03125);
    }

    // Pull up the chip select to stop data transfer
    CS_Select_Init(1);
    HAL_Delay(1);

    return final;
}
