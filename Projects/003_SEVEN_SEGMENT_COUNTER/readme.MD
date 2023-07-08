# Seven Segment Counter

Every time the button is pressed, the value of the 'counter' variable increases, and the value of the variable is displayed on two different 7-segment displays. The left 7-segment display has 4 input pins and depending on the state of the input pins, it shows the value of the variable from 0 to E in hexadecimal. The right 7-segment display has 7 input pins, and each input controls a segment (LED) on the display. While the binary value of the number is transmitted through the pins on the left display as 0 and 1, the desired segments to be illuminated on the right display are controlled through the pins.

![](https://github.com/ertrldtcu/STM32F-Projects/blob/master/Projects/003_SEVEN_SEGMENT_COUNTER/003_SEVEN_SEGMENT_COUNTER.gif)
