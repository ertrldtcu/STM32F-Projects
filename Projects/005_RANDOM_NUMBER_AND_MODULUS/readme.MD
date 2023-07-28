# Random Number and Modulus

When the button connected to pin 7 of port A is pressed, a pseudo-random number between 0 and 99 is generated and displayed on two blue seven-segment displays connected to pins 15:8 of port A. When the button connected to pin 15 of port D is pressed, the `counter` variable, ranging from 1 to 9, is incremented by 1, and it is shown on a red seven-segment display connected to pins 14:11 of port D. Whenever either of these two buttons is pressed and the variables `randomNum` or `counter` change, a modulo operation is performed. The remainder obtained by dividing `randomNum` by `counter` is displayed on a green seven-segment display connected to pins 15:12 of port E.

As the STM32F401VET6 board does not have a True Random Number Generator (T-RNG) chip, a pseudo-random number generation approach is used by performing simple mathematical operations on the `uwTick` variable. You can find more details about this in the `uint32_t timeBasedRandom(void` function inside the `main.c` file.

![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/005_RANDOM_NUMBER_AND_MODULUS/005_RANDOM_NUMBER_AND_MODULUS.gif)