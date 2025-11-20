/**
  ******************************************************************************
  * @file           : ads1220.h
  * @brief          : header file for the ADS1220
  * @author         :
  * @date           :
  * @project        :
  ******************************************************************************
  * @attention
  *
  *
  ******************************************************************************
  */
#ifndef APPLICATION_USER_CORE_ADS1220_H_
#define APPLICATION_USER_CORE_ADS1220_H_

/* Includes ------------------------------------------------------------------*/
#include <stdio.h>

/* define --------------------------------------------------------------------*/
// ADS1220 commands
#define CMD_WAKEUP                0x00
#define CMD_RREG                  0x20
#define CMD_WREG                  0x40
#define CMD_SYNC                  0x08 // Start, Send the START/SYNC command (08h) to start converting in continuous conversion mode
#define CMD_RESET                 0x06 // Send the RESET command (06h) to make sure the ADS1220 is properly reset after power-up
#define CMD_PWRDN                 0x02

// ADS1220 register addresses
#define REG_CONFIG_0              0x00
#define REG_CONFIG_1              0x01
#define REG_CONFIG_2              0x02
#define REG_CONFIG_3              0x03
#define REG_AINPOS                0x04
#define REG_AINNEG                0x05

// ADS1220 input multiplexer settings
#define AIN0_AIN1                 0x00

// ADS1220 delay times
#define T_CSSC                    0.05 // Delay for a minimum of t d(CSSC) = 1 us, from Data sheet, at least 50ns
#define T_CLK                     0.015 // Delay for a minimum of t (CLK) = 1 us, , from Data sheet, at least 250ns
#define T_SCCS                    0.025 // Delay for a minimum of t d(SCCS) = 1 us, from Data sheet, at least 25ns
#define ADS1220_Temperature_on    0x02

// ADS1220 interrupt pin
#define DRDY_PIN                  GPIO_PIN_2
#define DRDY_GPIO_PORT            GPIOB

// ADS1220 chip select pin
#define CS_PIN                    GPIO_PIN_1
#define CS_GPIO_PORT              GPIOB

// Input Multiplexer Configuration Register (MUX)
#define ADS1220_MUX_0_1           0x00 // 8 Bit numbers, one is 4 Bit long
#define ADS1220_MUX_0_2           0x10
#define ADS1220_MUX_0_3           0x20
#define ADS1220_MUX_1_0           0x30
#define ADS1220_MUX_1_2           0x40
#define ADS1220_MUX_1_3           0x50
#define ADS1220_MUX_2_0           0x60
#define ADS1220_MUX_2_1           0x70
#define ADS1220_MUX_2_3           0x80
#define ADS1220_MUX_3_0           0x90
#define ADS1220_MUX_3_1           0xa0
#define ADS1220_MUX_3_2           0xb0
#define ADS1220_MUX_TEMP          0xc0
#define ADS1220_MUX_AIN0          0xd0
#define ADS1220_MUX_AIN1          0xe0
#define ADS1220_MUX_AIN2          0xA0
#define ADS1220_MUX_AIN3          0xB0

// Programmable Gain Amplifier Configuration Register (PGA)
#define ADS1220_GAIN_1            0x00
#define ADS1220_GAIN_2            0x02
#define ADS1220_GAIN_4            0x04
#define ADS1220_GAIN_8            0x06
#define ADS1220_GAIN_16           0x08
#define ADS1220_GAIN_32           0x0a
#define ADS1220_GAIN_64           0x0c
#define ADS1220_GAIN_128          0x0e

// Conversion Configuration Register (CONFIG)
#define ADS1220_DR_20             0x00
#define ADS1220_DR_45             0x20 // (config)
#define ADS1220_DR_90             0x40
#define ADS1220_DR_175            0x60
#define ADS1220_DR_330            0x80
#define ADS1220_DR_600            0xA0
#define ADS1220_DR_1000           0xC0
#define ADS1220_CC_NORMAL         0x00
#define ADS1220_CC_TURBO          0x10

// Voltage Reference Configuration Register (MUX)
#define ADS1220_VREF_INT          0x00
#define ADS1220_VREF_EX_AIN0      0x20
#define ADS1220_VREF_EX_AIN1      0x30
#define ADS1220_VREF_EX_AIN2      0x40
#define ADS1220_VREF_EX_AIN3      0x50
#define ADS1220_VREF_ANALOG       0x70
#define ADS1220_VREF_EX_DED       0x80
#define ADS1220_VREF_EX_REF0_4    0x90
#define ADS1220_VREF_EX_REF0_6    0xA0
#define ADS1220_VREF_EX_REF1_2    0xB0
#define ADS1220_VREF_EX_REF2_4    0xC0

// IDAC Configuration Register (IDAC)
#define ADS1220_IDAC_OFF          0x00
#define ADS1220_IDAC_10UA         0x01
#define ADS1220_IDAC_50UA         0x02
#define ADS1220_IDAC_100UA        0x03
#define ADS1220_IDAC_250UA        0x04
#define ADS1220_IDAC_500UA        0x05
#define ADS1220_IDAC_1000UA       0x06
#define ADS1220_IDAC_1500UA       0x07

// Internal Reference Configuration Register (REF)
#define ADS1220_REF_OFF           0x00
#define ADS1220_REF_ON            0x01
#define ADS1220_VREF_AIN          0x02

// Low-Side Switch Configuration Register (PSW)
#define ADS1220_PSW_LOW           0x00
#define ADS1220_PSW_HIGH          0x01
#define ADS1220_PSW_SW            0x02
#define ADS1220_PSW_OFF           0x00
#define ADS1220_PSW_ON            0x08  // 0000 1000

// Device ID Register (ID)
#define ADS1220_ID_1              0x72
#define ADS1220_ID_2              0x73
#define ADS1220_ID_3              0x00

// Conversation Mode
#define ADS1220_SINGESHOT         0x00
#define ADS1220_CONTINUOUS        0x04
#define ADS1220_PGA_BYPASS_ENABLE 0x01 //checked
#define ADS1220_CC                0x00
#define ADS1220_IDAC1_OFF         0x00
#define ADS1220_IDAC2_OFF         0x00
#define ADS1220_CMD_RESET         0x06 // Checked -> it fits
#define ADS1220_0_REGISTER        0x00
#define ADS1220_CMD_WREG          0x40
#define ADS1220_CMD_SYNC          0x08
#define ADS1220_CMD_RREG          0x20
#define ADS1220_CMD_RDATA         0x10

/* typedef --------------------------------------------------------------------*/
typedef struct {
    uint8_t channel; // Define a struct for the CS_Select_Init function
    uint8_t status;
} CS_Select_Init_Params_t;

/* function --------------------------------------------------------------------*/
void CS_Select_Init(GPIO_PinState status);
void ADS1220_RESET(void);
void ADS1220_Config(void);
void ADS1220_Enable_Temperature_Mode(void);
void ADS1220_Disable_Temperature_Mode(void);
void ADS1220_Write_Register(uint8_t RegAddr, uint8_t regs, uint8_t* pData);
void ADS1220_START_SYNC_Init();
void SPI_ReadWriteByte(SPI_HandleTypeDef *hspi, uint8_t* RxData, int arLength,uint8_t* TxData);
void CS_Select_Init_2(GPIO_TypeDef* type, uint16_t gpioPin, GPIO_PinState status);
float ADS1220_Read_Temp_Data(char buffer[50]);

//void ADS1220_ReadRegister(int StartAddress, int NumRegs, unsigned * pData);

#endif /* APPLICATION_USER_CORE_ADS1220_H_ */
