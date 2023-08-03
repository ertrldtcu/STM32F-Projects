# Led Control With USART Interrupt

Commands sent via USART can be used to adjust the on and off times of the LED separately. The prefix ledon=xms is used to set the LED's on time, and ledoff=xms is used to set the LED's off time. Here, x represents the time duration in milliseconds. If the entered command starts with either of these two prefixes, followed by a number, and ends with the 'ms' suffix, then the command is in the correct format. Any characters after the 'ms' suffix are not checked. The code avoids using libraries to optimize it and implements string operations from scratch, which leads to some missing features.

![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/006_LED_CONTROL_WITH_USART_INTERRUPT/006_LED_CONTROL_WITH_USART_INTERRUPT_VT.png)
![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/006_LED_CONTROL_WITH_USART_INTERRUPT/006_LED_CONTROL_WITH_USART_INTERRUPT.jpg)