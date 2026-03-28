#include <stdint.h>
#include <stdbool.h>
#include "system.h"
#include "delay.h"
#include "gpio.h"
#include "usb.h"

#define PIN_AB    P10
#define PIN_SC    P11
#define PIN_UP    P14
#define PIN_DOWN  P15
#define PIN_LEFT  P16
#define PIN_RIGHT P17

#define PIN_SELECT P32

enum buttons {
  CTRL_UP,
  CTRL_DOWN,
  CTRL_LEFT,
  CTRL_RIGHT,
  CTRL_Y,
  CTRL_B,
  CTRL_A,
  CTRL_X,
  CTRL_Z,
  CTRL_C,
  CTRL_MODE,
  CTRL_START
};

const uint16_t sega_map[] = {
  0x1000,
  0x2000,
  0x4000,
  0x8000,
  0x0001, // BTN_TRIGGER (288) -> Button 1
  0x0002, // BTN_THUMB (289) -> Button 2
  0x0004, // BTN_THUMB2 (290) -> Button 3
  0x0008, // BTN_TOP (291) -> Button 4
  0x0010, // BTN_TOP2 (292) -> Button 5
  0x0020, // BTN_PINKIE (293) -> Button 6
  0x0100, // BTN_BASE3 (296) -> Button 9
  0x0200  // BTN_BASE4 (297) -> Button 10
};

const uint16_t alt_map[] = {
  0x1000,
  0x2000,
  0x4000,
  0x8000,
  0x0008,
  0x0004,
  0x0002,
  0x0001,
  0x0020,
  0x0010,
  0x0100,
  0x0200
};

uint16_t* btn_map;

void USB_interrupt(void);
void USB_ISR(void) __interrupt(INT_NO_USB) {
  USB_interrupt();
}

#pragma disable_warning 110

__xdata uint8_t pad1_report[]   = {0,0,0,0,0};

void poll_pad() {
  uint16_t pad = 0;
  bool is_6btn = false;

  pad1_report[0] = 1; // pad ID
  pad1_report[1] = 128;
  pad1_report[2] = 128;
  pad1_report[3] = 0;
  pad1_report[4] = 0;

  PIN_low(PIN_SELECT);
  DLY_us(5);
  pad |= (PIN_read(PIN_AB) ? 0 : btn_map[CTRL_A]) |
        (PIN_read(PIN_SC) ? 0 : btn_map[CTRL_START]);

  PIN_high(PIN_SELECT);
  DLY_us(5);
  pad |= (PIN_read(PIN_UP)    ? 0 : btn_map[CTRL_UP])    |
        (PIN_read(PIN_DOWN)  ? 0 : btn_map[CTRL_DOWN])  |
        (PIN_read(PIN_LEFT)  ? 0 : btn_map[CTRL_LEFT])  |
        (PIN_read(PIN_RIGHT) ? 0 : btn_map[CTRL_RIGHT]) |
        (PIN_read(PIN_AB)    ? 0 : btn_map[CTRL_B])     |
        (PIN_read(PIN_SC)    ? 0 : btn_map[CTRL_C]);

  PIN_low(PIN_SELECT);
  DLY_us(5);

  PIN_high(PIN_SELECT);
  DLY_us(5);

  PIN_low(PIN_SELECT);
  DLY_us(5);
  if (!PIN_read(PIN_UP) && !PIN_read(PIN_DOWN)) {
    is_6btn = true;
  }

  PIN_high(PIN_SELECT);
  DLY_us(5);
  if (is_6btn) {
    pad |=  (PIN_read(PIN_UP)    ? 0 : btn_map[CTRL_Z]) |
          (PIN_read(PIN_DOWN)  ? 0 : btn_map[CTRL_Y]) |
          (PIN_read(PIN_LEFT)  ? 0 : btn_map[CTRL_X]) |
          (PIN_read(PIN_RIGHT) ? 0 : btn_map[CTRL_MODE]);
  }

  PIN_low(PIN_SELECT);
  DLY_us(5);
  PIN_high(PIN_SELECT);
  DLY_us(5);

  if (pad & btn_map[CTRL_LEFT]) pad1_report[1] = 0;
  else if (pad & btn_map[CTRL_RIGHT]) pad1_report[1] = 255;

  if (pad & btn_map[CTRL_UP]) pad1_report[2] = 0;
  else if (pad & btn_map[CTRL_DOWN]) pad1_report[2] = 255;

  pad1_report[3] = (uint8_t) (pad & 0xFF);
  pad1_report[4] = (uint8_t) ((pad >> 8) & 0x03); // Buttons 9 & 10 (start & mode)

  HID_sendReport(pad1_report, sizeof(pad1_report));
}

void main(void) {
  CLK_config();
  DLY_ms(10);
  PIN_output(PIN_SELECT);
  PIN_input_PU(PIN_AB);
  PIN_input_PU(PIN_SC);
  PIN_input_PU(PIN_UP);
  PIN_input_PU(PIN_DOWN);
  PIN_input_PU(PIN_LEFT);
  PIN_input_PU(PIN_RIGHT);
  btn_map = (uint16_t *)sega_map;
  HID_init();

  while (1) {
    DLY_ms(2);
    poll_pad();
  }
}
