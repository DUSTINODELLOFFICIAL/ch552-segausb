# ch552-segausb

Sega Genesis to USB HID controller adapter firmware for cheap CH552 board with
- low latency (controllers polled every 2ms, reports at USB high speed)
- 3 and 6 button controller compatibility
- Sega Genesis Mini compatible (note that the controller always presents as 6 button regardless of what's plugged into it though)

On a [weact CH552 board](https://www.aliexpress.us/item/3256803955611673.html) wire it up so:
```
Joy DB9    CH552 board
-------    -----------
  1          14
  2          15
  3          16
  4          17
  5          5V
  6          10
  7          32
  8          GND
  9          11
```
Flash the board by holding the P36 button down while connecting it to your PC and QUICKLY (within a few seconds) flash the gen2usb.bin with a flash tool like [wchisp](https://github.com/ch32-rs/wchisp). "make flash" is set up in the makefile for this.

If you want to play with the code, get SDCC.

The usb.c and helper macros are taken & cobbled together from [UNIT Electronics Cocket Nova examples using SDCC](https://github.com/UNIT-Electronics/CH55x_SDCC_Examples) which themselves are derived from the [wagiminator examples](https://github.com/wagiminator/MCU-Templates/tree/main/CH552). All credit to them for making this much easier.

# License
Most of this code & what it is adapted from is GPL3 with the exception of the wagiminator stuff (that was also in the Cocket Nova examples) being CC 3.0 Attribution-ShareAlike and I made some changes to it.
The CH552 specific headers are probably public domain. My code (main.c mostly) is hereby public domain
So don't use this commercially I guess.




