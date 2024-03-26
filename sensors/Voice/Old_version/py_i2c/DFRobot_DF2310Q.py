import logging
import smbus
import time

# 初始化日志记录器
logger = logging.getLogger()
logger.setLevel(logging.FATAL)    # 只显示错误信息
ph = logging.StreamHandler()
formatter = logging.Formatter("%(asctime)s - [%(filename)s %(funcName)s]:%(lineno)d - %(levelname)s: %(message)s")
ph.setFormatter(formatter)
logger.addHandler(ph)

# I2C地址及寄存器定义
DF2301Q_I2C_ADDR = 0x64
DF2301Q_I2C_REG_CMDID = 0x02
DF2301Q_I2C_REG_PLAY_CMDID = 0x03
DF2301Q_I2C_REG_SET_MUTE = 0x04
DF2301Q_I2C_REG_SET_VOLUME = 0x05
DF2301Q_I2C_REG_WAKE_TIME = 0x06
DF2301Q_I2C_MSG_TAIL = 0x5A

class DFRobot_DF2301Q(object):
    '''!
      @brief 定义DFRobot_DF2301Q基础类
    '''
    def __init__(self):
        '''!
          @brief 模块初始化
        '''
        pass

class DFRobot_DF2301Q_I2C(DFRobot_DF2301Q):
    '''!
      @brief 定义DFRobot_DF2301Q_I2C基础类
    '''
    def __init__(self, i2c_addr=DF2301Q_I2C_ADDR, bus=1):
        '''!
          @brief 模块I2C通信初始化
          @param i2c_addr - I2C通信地址
          @param bus - I2C总线
        '''
        self._addr = i2c_addr
        self._i2c = smbus.SMBus(bus)
        super(DFRobot_DF2301Q_I2C, self).__init__()

    def get_CMDID(self):
        '''!
          @brief 获取命令字对应的ID
          @return 获取到的命令字ID，返回0表示未获取到有效ID
        '''
        time.sleep(0.05)  # 防止访问速率干扰声音模块的其他功能
        return self._read_reg(DF2301Q_I2C_REG_CMDID)

    def play_by_CMDID(self, CMDID):
        '''!
          @brief 根据命令字ID播放对应的回复音频
          @param CMDID - 命令字ID
          @note 通过ID-1在I2C模式下可以进入唤醒状态
        '''
        self._write_reg(DF2301Q_I2C_REG_PLAY_CMDID, CMDID)
        time.sleep(1)

    def get_wake_time(self):
        '''!
          @brief 获取唤醒时长
          @return 当前设置的唤醒周期
        '''
        return self._read_reg(DF2301Q_I2C_REG_WAKE_TIME)

    def set_wake_time(self, wake_time):
        '''!
          @brief 设置唤醒时长
          @param wake_time - 唤醒时长(0-255)
        '''
        wake_time = wake_time & 0xFF
        self._write_reg(DF2301Q_I2C_REG_WAKE_TIME, wake_time)

    def set_volume(self, vol):
        '''!
          @brief 设置音量
          @param vol - 音量值(1~7)
        '''
        self._write_reg(DF2301Q_I2C_REG_SET_VOLUME, vol)

    def set_mute_mode(self, mode):
        '''!
          @brief 设置静音模式
          @param mode - 静音模式；设置值 1: 静音, 0: 非静音
        '''
        if (0 != mode):
            mode = 1
        self._write_reg(DF2301Q_I2C_REG_SET_MUTE, mode)

    def _write_reg(self, reg, data):
        '''!
          @brief 向寄存器写入数据
          @param reg -
          寄存器地址
          @param data - 要写入的数据
        '''
        if isinstance(data, int):
            data = [data]
        self._i2c.write_i2c_block_data(self._addr, reg, data)

    def _read_reg(self, reg):
        '''!
          @brief 从寄存器读取数据
          @param reg - 寄存器地址
          @return 读取到的数据
        '''
        data = self._i2c.read_i2c_block_data(self._addr, reg, 1)
        return data[0]
