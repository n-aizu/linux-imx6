linux-imx6
==========

Kernel tree for i.MX6.  

This repository is forked from [Boundary Devices linux-imx6 kernel tree][linux-imx6] and  
merged with [Wandboard linux kernel tree][wandboard-linux].  
This repository also based on [SolidRun linux-linaro-stable-mx6 kernel tree][solidrun-linaro].  

This kernel tree includes patches for several peripheral devices.  
Target devices are:  
 - Intel 802.11 a/b/g/n/ac PCIe mini card.
 - Atheros 802.11 a/b/g/n PCIe mini card.
 - faytech Touchscreen Monitor.
 - Mimo UM-720-S Touchscreen Monitor.
 - TechNexion EDM1-Fairy Carrier Board(PCIe only).

Branch
-----------
The latest 3.10.17 kernel is in branch [boundary-wand-hb-imx\_3.10.17\_1.0.2\_ga-rt][latest-3.10.17]  

Target
-----------
BD-SL-i.MX6(SABRE Lite)  
Wandboard  
Hummingboard  

Config
-----------
For non-RT-Preempt kernel: nitrogen6x_defconfig/wandboard_defconfig/hummingboard_defconfig  
For RT-Preempt kernel: nitrogen6x_rt_defconfig/wandboard_rt_defconfig/hummingboard_rt_defconfig  


[linux-imx6]: https://github.com/boundarydevices/linux-imx6.git "Boundary Devices Git repository"
[wandboard-linux]: https://github.com/wandboard-org/linux.git "Wandboard Git repository"
[solidrun-linaro]: https://github.com/SolidRun/linux-linaro-stable-mx6.git "SolidRun Git repository"
[latest-3.10.17]: https://github.com/n-aizu/linux-imx6/tree/boundary-wand-hb-imx_3.10.17_1.0.2_ga-rt "3.10.17 kernel tree"

