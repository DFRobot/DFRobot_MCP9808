# DFRobot_MCP9808
- [English Version](./README.md)

MCP9808 数字温度传感器可检测-40°C和+125°C之间的温度,提供±0.25°C/±0.5°C(典型值 / 最大值)高级精度的数据检测,以及最高+0.0625℃的用户可选择数据精度。 MCP9808具有用户可编程寄存器,可以实现关断或低功耗模式,以及温度报警窗口限制的规范和临界输出限制。 当温度变化超出规定的边界限制时,MCP9808会输出报警信号。用户可以设置比较器输出或温度报警中断输出。 这些特性使MCP9808成为精密的多区域温度监控应用的理想选择。
![正反面svg效果图](../../resources/images/SEN0435.jpg)


## 产品链接(https://www.dfrobot.com.cn/goods-2692.html)

    SKU:SEN0435

## 目录

* [概述](#概述)
* [库安装](#库安装)
* [方法](#方法)
* [兼容性](#兼容性y)
* [历史](#历史)
* [创作者](#创作者)

## 概述

- 工作电压:2.7~5.5V<br>
- 工作电流:200μA典型值<br>
- 精度:40°C 至 +125°C 时为 ±0.25°C (典型值) 20°C 至 100°C 时为 ±0.5°C (最大值) 40°C 至 +125°C 时为 ±1°C (最大值)<br>
- 用户可选测量分辨率: +0.5°C、 +0.25°C、 +0.125°C 和 +0.0625°C<br>
- 用户可编程温度报警: 温度阈值报警、临界温度报警<br>
- 通信接口:I2C(2.54mm Breakout)<br>
- 外形尺寸:18.5*23mm<br>
- 安装孔尺寸:2mm<br>
- 安装孔间距:18mm<br>

## 库安装
使用此库前,请首先下载库文件,将其粘贴到树莓派的自定义目录中,然后打开examples文件夹并在该文件夹中运行演示。

## 方法

```python
  '''!
    @brief 初始化传感器,对比传感器的芯片id 和 厂商 id
    @return  int type
    @retval  0 is init success
    @retval -1 芯片id 或者厂商id错误,请检查
  '''
  def sensor_init(self):

  '''!
    @brief 睡眠传感器模式, 功耗降低, 此时不能读取温度数据
    @return int type 
    @retval 1 is set successfully
    @retval 0 is register lock is not allowed to be modified
  '''
  def sleep_mode(self):

  '''!
    @brief 唤醒传感器模式, 此时可以读取温度数据
    @return int type
    @retval 1 is set successfully
    @retval 0 is register lock is not allowed to be modified
  '''
  def wakeup_mode(self):

  '''!
    @brief 设置电源模式
    @n 唤醒模式:该模式下,可以正常访问寄存器,能够得到正常的温度；
    @n 睡眠模式:温度测量停止,可以读取或写入寄存器,但是总线活动会使耗电升高
    @param mode
    @n     POWER_UP_MODE         唤醒模式
    @n     LOW_POWER_MODE        睡眠模式
    @return int type
    @retval 0 is set success
    @retval -1 设置失败,寄存器已经锁定,请先解锁寄存器
  '''
  def set_power_mode(self ,mode):

  '''!
    @brief 获取电源的配置模式
    @n 唤醒模式:该模式下,可以正常访问寄存器,能够得到正常的温度；
    @n 睡眠模式:温度测量停止,可以读取或写入寄存器,但是总线活动会使耗电升高
    @return int type
    @retval 0 is sleep mode
    @retval 1 is wakeup mode
  '''
  def get_power_mode(self):

  '''!
    @brief get device id
    @return device id (DEVICE_ID:0x04)
  '''
  def get_device_id(self):
  
  '''!
    @brief get manufacturer id
    @return manufacturer id (MANUFACTURER_ID:0x54)
  '''
  def get_manufacturer_id(self):
  
  '''!
    @brief 设置温度传感器的分辨率,不同的分辨率,获取的温度的精度不同
    @param resolution
    @n     RESOLUTION_0_5     获取温度的小数部分为0.5的倍数     如0.5℃ 、1.0℃、1.5℃
    @n     RESOLUTION_0_25    获取温度的小数部分为0.25的倍数    如0.25℃、0.50℃、0.75℃
    @n     RESOLUTION_0_125   获取温度的小数部分为0.125的倍数   如0.125℃、0.250℃、0.375℃
    @n     RESOLUTION_0_0625  获取温度的小数部分为0.0625的倍数  如0.0625℃、0.1250℃、0.1875℃
    @return state
    @retval 0 is set successfully
    @retval -1 设置的分辨率错误,请检查分辨率参数
  '''
  def set_resolution(self ,resolution):
    
  '''!
    @brief 获取温度传感器的分辨率 ,不同的分辨率,获取的温度的精度不同
    @return resolution 
    @retval RESOLUTION_0_5      获取温度的小数部分为0.5的倍数     如0.5℃ 、1.0℃、1.5℃
    @retval RESOLUTION_0_25     获取温度的小数部分为0.25的倍数    如0.25℃、0.50℃、0.75℃
    @retval RESOLUTION_0_125    获取温度的小数部分为0.125的倍数   如0.125℃、0.250℃、0.375℃
    @retval RESOLUTION_0_0625   获取温度的小数部分为0.0625的倍数  如0.0625℃、0.1250℃、0.1875℃
  '''
  def get_resolution(self):
  
  '''!
    @brief 获取当前的环境温度,注意设置不同分辨率能够得到的温度精度不同 单位:摄氏度
    @return 温度值为浮点数 (单位,摄氏度)
  '''
  def get_temperature(self):
  
  '''!
    @brief 获取当前比较器的状态和温度数据, 只有在比较器模式下有效
    @return 存放当前数据的列表
    @n      [0] 当前的温度
    @n      [1] 比较器状态的字符串,比较当前温度和上限阈值,下限阈,和临界值的关系
    @n      [2] 比较器状态的值,解释如下
    @n      TA 代表当前温度, TCRIT代表临界温度, TUPPER代表上限温度, TLOWER代表下限温度
    @n      1 代表 TA ≥ TCRIT, TA > TUPPER, TA < TLOWER
    @n      0 代表 TA < TCRIT, TA ≤ TUPPER, TA ≥ TLOWER
    @n      ------------------------------------
    @n      | bit7 ~ bit3 | bit2 | bit1 | bit0 |
    @n      ------------------------------------
    @n      |  reserved   |  0   |  0   |  0   |
    @n      ------------------------------------
  '''
  def get_comparator_state(self):
    
  '''!
    @brief 设置锁定模式或解锁,防止错误操作更改上限、下限、临界值的大小
    @n     锁定后只能通过断电复位解除锁定
    @param lock
    @n     CRIT_LOCK       锁定临界值,临界值的阈值不允许被修改
    @n     WIN_LOCK        锁定上限下限,上限下限的阈值不允许被修改
    @n     CRIT_WIN_LOCK   锁定临界值和上限下限,上限下限和临界值的数据都不允许被修改
    @n     NO_LOCK         不锁定寄存器
    @return state
    @retval 0x00 is set successfully
    @retval 0xFE 设置的模式错误
  '''
  def set_lock_state(self ,lock):
    
  '''!
    @brief 获取锁定的状态,来判断是否可以修改上限下限和临界值的阈值
    @return state
    @retval CRIT_LOCK      临界值锁定,临界值的阈值不允许被修改
    @retval WIN_LOCK       上限下限锁定,上限下限的阈值不允许被修改
    @retval CRIT_WIN_LOCK  临界值和窗口同时锁定,上限下限和临界值的数据都不允许被修改
    @retval NO_LOCK        没有锁定,上限下限和临界值的阈值都可以被修改
  '''
  def get_lock_state(self):

  '''!
    @brief 设置报警温度滞后的范围,在上限下限和临界值的阈值上增加一个范围,滞后功能仅适用于降温(从热至冷)
    @n     也就是说(上限/下限/临界值)减去滞后温度,ALE电平才恢复
    @n     例如:温度上限为30.0度,滞后温度为+1.5度,当前是35度ALE已经产生电平翻转,
    @n     要想ALE恢复电平,必须达到30-1.5(28.5)度,ALE引脚才能恢复电平
    @param mode
    @n     HYSTERESIS_0_0     没有滞后,就是到达指定温度就响应
    @n     HYSTERESIS_1_5     从热至冷要滞后1.5℃
    @n     HYSTERESIS_3_0     从热至冷要滞后3.0℃
    @n     HYSTERESIS_6_0     从热至冷要滞后6.0℃
    @return state
    @retval 0x00 is set successfully
    @retval -1 当前寄存器为锁定状态不允许修改
    @retval 0xFE 设置的范围错误,请检查范围
  '''  
  def set_alert_hysteresis(self ,mode):
    
  '''!
    @brief 获取滞后的温度
    @return hysteresis
    @retval HYSTERESIS_0_0    没有滞后,就是到达指定温度就响应
    @retval HYSTERESIS_1_5    从热至冷要滞后1.5℃
    @retval HYSTERESIS_3_0    从热至冷要滞后3.0℃
    @retval HYSTERESIS_6_0    从热至冷要滞后6.0℃
  '''
  def get_alert_hysteresis(self):
  
  '''!
    @brief 设置ALE引脚的极性,引脚极性为高:ALE引脚高电平为活动电平,默认为低电平,产生报警后ALE为高电平
    @n     引脚极性为低:ALE引脚低极性为活动电平,默认为高电平,产生报警后ALE为低电平
    @param polarity
    @n     POLARITY_HIGH   ALE引脚高电平为活动电平
    @n     POLARITY_LOW    ALE引脚低极性为活动电平
    @return state
    @retval 0x00 is set successfully
    @retval -1   当前寄存器为锁定状态不允许修改
    @retval 0xFE 设置的极性错误,请检查极性
  '''
  def set_polarity(self ,polarity):
    
  '''!
    @brief 获取ALE引脚的极性状态,引脚极性为高:ALE引脚高电平为活动电平,默认为低电平,产生报警后ALE为高电平
    @n     引脚极性为低:ALE引脚低极性为活动电平,默认为高电平,产生报警后ALE为低电平
    @return polarity
    @retval POLARITY_HIGH   ALE引脚高电平为活动电平
    @retval POLARITY_LOW    ALE引脚低极性为活动电平
  '''
  def get_polarity_state(self):
    
  '''!
    @brief 设置警报输出的模式,比较器输出模式不需要清除中断,中断模式需要清除中断
    @param mode
    @n     COMPARATOR_OUTPUT_MODE           比较器输出模式不需要清除中断, 
    @n      例如:设置ALE引脚为低电平活动,当超过上限警报的温度时, ALE引脚从高电平到低电平, 当温度低于上限但高于下限时, ALE引脚恢复高电平
    @n     INTERRPUT_OUTPUT_MODE            中断输出模式需要清除中断, 当产生警报时, 如果不清除中断中断一直存在, 中断模式的触发, 是从一种状态变为另一种状态, 
    @n      例如:设置了下限阈值20度, 上限阈值25度, 临界阈值30度, 当温度一直低于20度时不产生中断, 当温度超过25度时才产生中断, ALE引脚跳变, 此时应该清空中断, ALE引脚恢复, 
    @n      特殊情况, 当ALE引脚大于临界温度30度时, 中断模式失效, 清空中断也失效, 必须等温度降到30度以下, 才恢复中断模式
    @n     DISABLE_OUTPUT_MODE              禁止输出模式后不产生警报,ALE引脚失效
    @return state
    @retval 0x00 is set successfully
    @retval -1 当前寄存器为锁定状态不允许修改
    @retval 0xFE 设置的警报输出模式错误,请检查模式
  '''
  def set_alert_output_mode(self ,mode):
  
  '''!
    @brief 获取警报输出的模式
    @return mode
    @retval COMPARATOR_OUTPUT_MODE     比较器输出模式
    @retval INTERRPUT_OUTPUT_MODE      中断输出模式
    @retval DISABLE_OUTPUT_MODE        禁止输出模式
  '''
  def get_alert_output_mode(self):
  
  '''!
    @brief 使能或者禁止报警模式,使能报警模式后,ALE引脚到达报警条件后会产生跳变,禁止报警模式ALE引脚没有响应
    @param mode
    @n     ENABLE_ALERT      使能报警模式,ALE引脚到达报警条件后会产生跳变
    @n     DISABLE_ALERT     禁止报警模式,禁止报警模式ALE引脚没有响应
    @return state
    @retval 0x00 is set successfully
    @retval -1   当前寄存器为锁定状态不允许修改
    @retval 0xFE 设置的模式错误,请检查模式
  '''
  def set_alert_enable(self ,mode):
    
  
  '''!
    @brief 获取报警模式状态,得到时报警模式或者非报警模式
    @return mode
    @retval ENABLE_ALERT  报警模式
    @retval DISABLE_ALERT 非报警模式
  '''
  def get_alert_enable_state(self):
  
  '''!
    @brief 设置响应模式,响应上限下限和临界值,或者只响应临界值,只响应临界值不适用于中断模式
    @param mode
    @n     UPPER_LOWER_CRIT_RESPONSE       上限/下线和临界值 都响应,
    @n     ONLY_CRIT_RESPONSE              禁止上限下限响应,只有临界值响应
    @return state
    @retval 0x00 is set successfully
    @retval -1   当前寄存器为锁定状态不允许修改
    @retval 0xFE 设置的响应模式错误,请检查模式
  '''
  def set_alert_response_mode(self ,mode):
  
  '''!
    @brief 获取响应的模式
    @return mode
    @retval UPPER_LOWER_CRIT_RESPONSE    上限/下线和临界值 都响应
    @retval ONLY_CRIT_RESPONSE           禁止上限下限响应,只有临界值响应
  '''
  def get_alert_response_mode(self):
    
  '''!
    @brief 设置临界值和上限和下限阈值,根据配置的中断模式响应 
    @n     高于上限温度和低于下限温度和高于临界值响应中断
    @n     临界值温度必须大于上限温度
    @n     上限温度必须大于下限温度 2摄氏度
    @param crit  温度临界值, 最多两位小数, 自动处理成0.25的倍数, 范围为-40 到 +125度
    @param upper 温度上限, 最多两位小数, 自动处理成0.25的倍数, 范围为-40 到 +125度
    @param lower 温度下限, 最多两位小数, 自动处理成0.25的倍数, 范围为-40 到 +125度
    @return state
    @retval 0  is set successfully
    @retval -3 温度临界值小于上限
    @retval -2 温度上限小于下限, 或者(上限温度-下限温度 < 2 )
    @retval -1 寄存器锁定不允许操作
  '''
  def set_threshold(self, crit, upper, lower):
    
  '''!
    @brief 清空中断,只使用于中断模式下,其余模式没有效果
  '''
  def clear_interrupt(self):
```

## 兼容性

* RaspberryPi Version

| Board        | 正常运行  | 运行失败   | 未测试    | 备注
| ------------ | :-------: | :--------: | :------: | :-----: |
| RaspberryPi2 |           |            |    √     |         |
| RaspberryPi3 |           |            |    √     |         |
| RaspberryPi4 |     √     |            |          |         |

* Python版本

| Python  | 正常运行  | 运行失败   | 未测试    | 备注
| ------- | :-------: | :--------: | :------: | :-----: |
| Python2 |     √     |            |          |         |
| Python3 |     √     |            |          |         |


## 历史

- 2021/04/16 - V1.0.0 版本

## 创作者

Written by ZhixinLiu(zhixin.liu@dfrobot.com), 2021. (Welcome to our [website](https://www.dfrobot.com/))