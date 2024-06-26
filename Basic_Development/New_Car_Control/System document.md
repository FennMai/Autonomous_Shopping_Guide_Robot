# New car control system

**goal:**

1. build a capacity closed-loop pd contorl system for car. 
2. solve the open-loop system bug from old system.


### brainstrom
1. will not use the qt(cost too much time)
--> detect by output 
--> build `car_sys_base.cpp` `car_sys_base.h` `test.cpp`
2. remote control(by the key??? can have a try / by the bluetooth handle???)
3. find the source: reporsitories and video(30mins)

### **Extended board info analysis**
    Don't have encoder motor control program

![alt text](<assets/System document/image.png>)


![alt text](<assets/System document/image-2.png>)

Raspi_i2c --> Emakefun_MotorDriver -->

### some basic
直流减速编码器电机MC520是直流有刷电机
![alt text](<assets/System document/image-1.png>)

    another plan
    compare MG310

    https://doc.embedfire.com/motor/motor_tutorial/zh/latest/improve_part/PID_parameter_tuning.html
### diary 

20240528: 
1. goal: learn the hardware and how to connect the pin for drive the motor

20240529: 
1. goal: writing code 

20240607:
1. pos-vol-pid control test
2. motor2 still have bug
3. the pos and pulse have bug

20240626：
1. 缠了黑色带子的电机--以我的视觉方向平行车头朝向，视觉上是是右边
2. 现在的代码，可以测试可以运行
3. 但是！一个很重要的问题，有个编码器没有变化
4. 