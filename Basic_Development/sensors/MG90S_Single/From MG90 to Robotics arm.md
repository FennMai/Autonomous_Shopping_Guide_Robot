### From MG90S to Robotics arm



#### A. MG90S basic parameters

working voltage: 4.8V-6.0V

paulsewidth: 0.5-2.5ms



#### B. class & function：

```
mg90s arm_mtr1(17)
arm_mtr1.setTargetAngleAsync(45, []{ std::cout << "arm_mtr1 reached 45 degrees.\n"; });
```


 void **moveToAngleSlowly**(float targetAngle)

> **Blocking call**: Using `usleep(20000);` will cause the calling thread to sleep, which means that if your program needs to do other things at the same time (such as reading sensor data or responding to user input), it will not be able to execute, Because the program is blocked in the loop.
why???
use callback to solve?



**Asynchronous sequential control of multiple motors**

```
mmoveMotorSequentially(arm_mtr1, angle1, arm_mtr2, angle2, arm_mtr3, angle3, arm_mtr4, angle4);
```

#### C. development sequence(from 0 to)

| profile             | appendix                                      | date |
| ------------------- | --------------------------------------------- |------|
| mai_single_mg90.cpp | Synchronous, single motor, measurement only   | 0319 |
| arm_sys_basic.cpp   | 4 motor control, Async                        | 0320 |
| arm_sys.cpp         | 4 motor control, Async, sequentially function | 0320 |
| mai_single_mg90.cpp | Synchronous, single motor, measurement only   |



#### D. motion define 
> 4 joint definitions
![alt text](<assets/From MG90 to Robotics arm/image-1.png>)

| Clamp | Big arm | Lower arm | cloud platform |
|-------|---------|-----------|----------------|
| arm_mtr1(12)| arm_mtr2(20) | arm_mtr3(16) | arm_mtr4(21)|
|-90 | 0 | -90 | 0 |

![alt text](<assets/From MG90 to Robotics arm/image.png>)


Basic motion







