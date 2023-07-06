## Run your code on Proteus

For Proteus to be able to run your code, it requires your code to be in a .hex file format. You can generate this file while compiling your project in STM32CubeIDE. To do this, select your project from the project list on the left side, then click on the 'Project' menu in the navigation bar and select the 'Properties' option at the bottom. In the opened window, click on the 'C/C++ Build' option in the list on the left side. Switch to the 'Tool Settings' tab on the right side. Then, enable the 'Convert to Intel Hex file (-O ihex)' option under 'MCU Post build outputs'. Now, after compiling your code, a .hex file will be created inside the 'Debug' folder.

![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/stm32cubeide_properties.png)

To generate the .hex file, you first need to compile our code once. Now, to make the simulation work, you need to assign this file to the STM32F401VE board that you have added to your circuit in Proteus. To do this, double-click on the board in Proteus. In the opened window, click on the small file button next to the 'Program File' entry. In the dialog box that appears, browse and select the .hex file. After designing the circuit according to your code, you can start the simulation.

![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/proteus_properties.png)
