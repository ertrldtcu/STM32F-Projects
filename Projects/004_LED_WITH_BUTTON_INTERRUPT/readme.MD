# Led With Button Interrupt

The LED connected to pin 15 of Port D is blinking when the button connected to pin 0 of Port E is pressed. However, this time we are using interrupts instead of constantly checking if the button is pressed. To do this, we enable the `EXTICR` (configuration), `IMR` (mask) and `FTSR` (falling edge trigger) registers.  We can blinking the LED in the triggered `EXTI0_IRQHandler` function.

![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/004_LED_WITH_BUTTON_INTERRUPT/004_LED_WITH_BUTTON_INTERRUPT.gif)