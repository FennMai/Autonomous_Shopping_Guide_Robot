/*!
 * @file  DFRobot_DF2301Q.h
 * @brief  Basic structure of DFRobot_DF2301Q class
 * @copyright  Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * @licence  The MIT License (MIT)
 * @author  [qsjhyy](yihuan.huang@dfrobot.com)
 * @version  V1.0
 * @date  2022-12-30
 * @url  https://github.com/DFRobot/DFRobot_DF2301Q
*/
#ifndef DFRobot_DF2301Q_H
#define DFRobot_DF2301Q_H

#include <Arduino.h>
#include <Wire.h>
#if defined(ARDUINO_AVR_UNO)||defined(ESP8266)
#include <SoftwareSerial.h>
#endif

// #define ENABLE_DBG   //!< Open this macro and you can see the details of the program
#ifdef ENABLE_DBG
  #define DBG(...) {Serial.print("[");Serial.print(__FUNCTION__); Serial.print("(): "); Serial.print(__LINE__); Serial.print(" ] "); Serial.println(__VA_ARGS__);}
#else
  #define DBG(...)
#endif

#define DF2301Q_I2C_ADDR                 uint8_t(0x64)   //!< I2C address

#define DF2301Q_I2C_REG_CMDID            uint8_t(0x02)   //!< Address of the register for requesting command word ID
#define DF2301Q_I2C_REG_PLAY_CMDID       uint8_t(0x03)   //!< Address of the register for playing audio by command word ID
#define DF2301Q_I2C_REG_SET_MUTE         uint8_t(0x04)   //!< Register for setting mute mode
#define DF2301Q_I2C_REG_SET_VOLUME       uint8_t(0x05)   //!< Register for setting volume
#define DF2301Q_I2C_REG_WAKE_TIME        uint8_t(0x06)   //!< Address of the register for wake-up time

#define DF2301Q_UART_BAUDRATE            long(9600)      //!< UART baud rate
#define DF2301Q_UART_MSG_DATA_MAX_SIZE   uint8_t(8)      //!< Maximum data length of serial data frame

/*header*/
#define DF2301Q_UART_MSG_HEAD_LOW    uint8_t(0xF4)
#define DF2301Q_UART_MSG_HEAD_HIGH   uint8_t(0xF5)
#define DF2301Q_UART_MSG_HEAD        ((DF2301Q_UART_MSG_HEAD_HIGH<<8)|DF2301Q_UART_MSG_HEAD_LOW)
/*tail*/
#define DF2301Q_UART_MSG_TAIL        uint8_t(0xFB)
/*msgType*/
#define DF2301Q_UART_MSG_TYPE_CMD_UP          uint8_t(0xA0)
#define DF2301Q_UART_MSG_TYPE_CMD_DOWN        uint8_t(0xA1)
#define DF2301Q_UART_MSG_TYPE_ACK             uint8_t(0xA2)
#define DF2301Q_UART_MSG_TYPE_NOTIFY          uint8_t(0xA3)
/*msgCmd*/
#define DF2301Q_UART_MSG_CMD_ASR_RESULT       uint8_t(0x91)           //Report voice recognition results
#define DF2301Q_UART_MSG_CMD_PLAY_VOICE       uint8_t(0x92)           //Play local broadcast audio
#define DF2301Q_UART_MSG_CMD_GET_FLASHUID     uint8_t(0x93)           //Read the serial number of FLASH
#define DF2301Q_UART_MSG_CMD_GET_VERSION      uint8_t(0x94)           //Read version number
#define DF2301Q_UART_MSG_CMD_RESET_MODULE     uint8_t(0x95)           //Reset the voice module
#define DF2301Q_UART_MSG_CMD_SET_CONFIG       uint8_t(0x96)           //Settings
#define DF2301Q_UART_MSG_CMD_ENTER_OTA_MODE   uint8_t(0x97)          //Enter update mode
#define DF2301Q_UART_MSG_CMD_NOTIFY_STATUS    uint8_t(0x9A)           //Event notification
#define DF2301Q_UART_MSG_CMD_ACK_COMMON       uint8_t(0xAA)
/* !!! if user want add please add form DF2301Q_UART_MSG_CMD_USER_START*/
#define DF2301Q_UART_MSG_CMD_USER_START       uint8_t(0xB0)
/*msgData  msgCmd:DF2301Q_UART_MSG_CMD_PLAY_VOICE*/
#define DF2301Q_UART_MSG_DATA_PLAY_START      uint8_t(0x80)
#define DF2301Q_UART_MSG_DATA_PLAY_PAUSE      uint8_t(0x81)
#define DF2301Q_UART_MSG_DATA_PLAY_RESUME     uint8_t(0x82)
#define DF2301Q_UART_MSG_DATA_PLAY_STOP       uint8_t(0x83)
#define DF2301Q_UART_MSG_DATA_PLAY_BY_VOICEID       uint8_t(0x90)
#define DF2301Q_UART_MSG_DATA_PLAY_BY_SEMANTIC_ID   uint8_t(0x91)
#define DF2301Q_UART_MSG_DATA_PLAY_BY_CMD_ID        uint8_t(0x92)
/*msgData  msg_cmd:DF2301Q_UART_MSG_CMD_GET_VERSION*/
#define DF2301Q_UART_MSG_DATA_VER_PROTOCOL    uint8_t(0x80)         // Serial protocol version number
#define DF2301Q_UART_MSG_DATA_VER_SDK         uint8_t(0x81)         // SDK version number
#define DF2301Q_UART_MSG_DATA_VER_ASR         uint8_t(0x82)         // ASR component version number
#define DF2301Q_UART_MSG_DATA_VER_PREPROCESS  uint8_t(0x83)         // Audio pre-processing algorithm version number
#define DF2301Q_UART_MSG_DATA_VER_PLAYER      uint8_t(0x84)         // Player version number
#define DF2301Q_UART_MSG_DATA_VER_APP         uint8_t(0x8A)         // App version number
/*msgData  msg_cmd:DF2301Q_UART_MSG_CMD_NOTIFY_STATUS*/
#define DF2301Q_UART_MSG_DATA_NOTIFY_POWERON       uint8_t(0xB0)
#define DF2301Q_UART_MSG_DATA_NOTIFY_WAKEUPENTER   uint8_t(0xB1)
#define DF2301Q_UART_MSG_DATA_NOTIFY_WAKEUPEXIT    uint8_t(0xB2)
#define DF2301Q_UART_MSG_DATA_NOTIFY_PLAYSTART     uint8_t(0xB3)
#define DF2301Q_UART_MSG_DATA_NOTIFY_PLAYEND       uint8_t(0xB4)
/*msgData msg_cmd:DF2301Q_UART_MSG_CMD_SET_CONFIG*/
#define DF2301Q_UART_MSG_CMD_SET_VOLUME        uint8_t(0x80)
#define DF2301Q_UART_MSG_CMD_SET_ENTERWAKEUP   uint8_t(0x81)
#define DF2301Q_UART_MSG_CMD_SET_PRT_MID_RST   uint8_t(0x82)
#define DF2301Q_UART_MSG_CMD_SET_MUTE          uint8_t(0x83)
#define DF2301Q_UART_MSG_CMD_SET_WAKE_TIME     uint8_t(0x84)
#define DF2301Q_UART_MSG_CMD_SET_NEEDACK       uint8_t(0x90)
#define DF2301Q_UART_MSG_CMD_SET_NEEDSTRING    uint8_t(0x91)
/*ACK error code*/
#define DF2301Q_UART_MSG_ACK_ERR_NONE          uint8_t(0x0)
#define DF2301Q_UART_MSG_ACK_ERR_CHECKSUM      uint8_t(0xff)
#define DF2301Q_UART_MSG_ACK_ERR_NOSUPPORT     uint8_t(0xfe)

class DFRobot_DF2301Q
{
public:
  /**
   * @fn DFRobot_DF2301Q
   * @brief Constructor 
   * @return None
   */
  DFRobot_DF2301Q();

  /**
   * @fn begin
   * @brief Init function
   * @return bool type, true if successful, false if error
   */
  virtual bool begin(void) = 0;

private:

};

/**************************************************************************
                          Subclass using I2C interface for communication
 **************************************************************************/

class DFRobot_DF2301Q_I2C: public DFRobot_DF2301Q
{
public:
  /**
   * @fn DFRobot_DF2301Q_I2C
   * @brief Constructor
   * @param pWire I2C bus pointer object, construct the device. You can pass a parameter to it or not, default to be Wire
   * @param address 7bits I2C address, the first three bits determine the value of the address, default to 0x50
   * @return None
   */
  DFRobot_DF2301Q_I2C(TwoWire *pWire=&Wire, uint8_t i2cAddr=DF2301Q_I2C_ADDR);

  /**
   * @fn begin
   * @brief Subclass init function
   * @return Bool type, true for success, false for failure
   */
  virtual bool begin(void);

  /**
   * @fn getCMDID
   * @brief Get the ID corresponding to the command word
   * @return Return the obtained command word ID, returning 0 means no valid ID is obtained
   */
  uint8_t getCMDID(void);

  /**
   * @fn playByCMDID
   * @brief Play the corresponding reply audio according to the command word ID
   * @param CMDID - Command word ID
   * @return None
   * @note Can enter wake-up state through ID-1 in I2C mode
   */
  void playByCMDID(uint8_t CMDID);

  /**
   * @fn getWakeTime
   * @brief Get the wake-up duration
   * @return The current set wake-up period
   */
  uint8_t getWakeTime(void);

  /**
   * @fn setWakeTime
   * @brief Set wake-up duration
   * @param wakeTime - Wake-up duration(0-255)
   * @return None
   */
  void setWakeTime(uint8_t wakeTime);

  /**
   * @fn setVolume
   * @brief Set volume
   * @param vol - Volume range(1-7)
   * @return None
   */
  void setVolume(uint8_t vol);

  /**
   * @fn setMuteMode
   * @brief Set mute mode
   * @param mode - Mute mode; set value 1: mute, 0: unmute
   * @return None
   */
  void setMuteMode(uint8_t mode);

protected:
  /**
   * @fn writeReg
   * @brief Write register value through I2C bus
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be written
   * @return None
   */
  void writeReg(uint8_t reg, const void* pBuf);

  /**
   * @fn readReg
   * @brief Read register value through I2C bus
   * @param reg  Register address 8bits
   * @param pBuf Storage and buffer for data to be read
   * @return Return the read length, returning 0 means reading failed
   */
  size_t readReg(uint8_t reg, void* pBuf);

private:
  TwoWire *_pWire;   // Pointer to I2C communication method
  uint8_t _deviceAddr;   // Address of the device for I2C communication
};
