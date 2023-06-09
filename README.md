# FPM383c


FPM383c on MSP430f5529lp


project could opened by TI energia for msp430f5529 lunchpad


i /* didn't */finish it's step motor controler! with a4988 // but it's enough.


i have already test it,it works .




fpm383c指纹模块与德州仪器火箭板做的指纹锁


可以记录多枚指纹(使用push2按钮,详见代码)


未开锁时红蓝交替闪烁,开锁时变为绿色

//红色灯闪烁模拟步进电机脉冲

//测试通过2023.5.22


可以正常使用..很快就要按到门上了 


添加了a4988的启停部分 2023.6.10 

sample:
![20230610000445](https://github.com/xinnie-the-pooh/FPM383c/assets/62879756/17ebbb4e-d383-442e-81e7-9a7c0fc85c20)
wire connect:
a4988 step-- lunchpad p2.0

a4988 en-- lunchpad p2.4

a4988 vi-- lunchpad 5v

a4988 dir--a4988 vi

hmc589(6)--lunchpad gnd

hmc589(5)--lunchpad p3.3

hmc589(4)--lunchpad p3.4

hmc589(3)--lunchpad 3.3v

hmc589(2)--lunchpad p6.5//i do not use this function 

hmc589(1)--lunchpad 3.3v

My step motor :36c type 0.9 degree per step,and a4988 board jumper all high ,that's mean /16, so if i want let it run 90 degree, it's need (360/0.9)*(360/90)*16 high-low 

Those buffer used in program has check sum .if need change color or others,calc them first!



