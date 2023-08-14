# Temperature Sensor and ADC

In this project, an STM32F103C4 microcontroller was used instead of the STM32F401VET6 microcontroller. This is because in the Proteus program, the ADC does not work during simulation for STM32F4x series microcontrollers. If the source code is compiled for STM32F4x microcontrollers, it will work.

Data from the LM35 temperature sensor is read using the ADC through the 0th pin of port A. The LM35 sensor measures temperatures in the range of -55°C to 150°C. The Vin pin of the sensor should be supplied with power between 4V and 30V (recommended 6V). The GND pin should be connected to 0V, but when this pin is connected to 0V, negative voltage is applied to the Vout pin at temperatures below zero. The microcontroller we are using cannot read negative voltages via the ADC. Therefore, in order to start the Vout voltage range from 0V, the GND pin is connected to 0.55V. This provides approximately 0V for -55°C.

The LEDs on the right-hand side operate as follows:
Blue LED (PB0): Turns on if the temperature is 0 or below
Green LED (PB1): Turns on if the temperature is 10 or above
Yellow LED (PB0): Turns on if the temperature is 20 or above
Orange LED (PB2): Turns on if the temperature is 30 or above
Red LED (PB3): Turns on if the temperature is 40 or above

The ADC operates in continuous conversion mode using interrupts.

![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/007_TEMPERATURE_SENSOR_AND_ADC/007_TEMPERATURE_SENSOR_AND_ADC.gif)