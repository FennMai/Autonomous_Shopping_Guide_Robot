### From MG90S to Robotics arm



#### MG90S basic parameters

working voltage: 4.8V-6.0V

paulsewidth: 0.5-2.5ms



#### function：

 void **moveToAngleSlowly**(float targetAngle)

> **Blocking call**: Using `usleep(20000);` will cause the calling thread to sleep, which means that if your program needs to do other things at the same time (such as reading sensor data or responding to user input), it will not be able to execute, Because the program is blocked in the loop.

```
mg90s arm_mtr1(17)
arm_mtr1.setTargetAngleAsync(45, []{ std::cout << "arm_mtr1 reached 45 degrees.\n"; });
```

void **moveMotorSequentially**(mg90s &motor1, mg90s &motor2, mg90s &motor3, mg90s &motor4)

> Asynchronous sequential control of multiple motors

```
moveMotorSequentially(arm_mtr1, arm_mtr2, arm_mtr3, arm_mtr4);
```



| profile           | appendix                                      |
| ----------------- | --------------------------------------------- |
| arm_sys_basic.cpp | 4 motor control, Async                        |
| arm_sys.cpp       | 4 motor control, Async, sequentially function |
|                   |                                               |







