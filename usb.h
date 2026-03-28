// ===================================================================================
// USB HID Joystick -- Consolidated Header
// ===================================================================================

#pragma once
#include <stdint.h>

// ===================================================================================
// User Configuration
// ===================================================================================

#define PIN_PWM             P34         // pin connected to PWM output
#define PIN_ADC             P32         // pin connected to ADC input

// USB device descriptor
#define USB_VENDOR_ID       0x0CA3      // VID
#define USB_PRODUCT_ID      0x0024      // PID
#define USB_DEVICE_VERSION  0x0100      // v1.0 (BCD-format)

// USB configuration descriptor
#define USB_MAX_POWER_mA    150         // max power in mA

// USB descriptor strings
#define MANUFACTURER_STR    'G','e','n','e','r','i','c'
#define PRODUCT_STR         '6','B',' ','c','o','n','t','r','o','l','l','e','r'
#define SERIAL_STR          'C','H','5','5','2','x','H','I','D'
#define INTERFACE_STR       'J','o','y','s','t','i','c','k'

// ===================================================================================
// USB Standard Device Request Codes
// ===================================================================================

#define USB_GET_STATUS          0x00
#define USB_CLEAR_FEATURE       0x01
#define USB_SET_FEATURE         0x03
#define USB_SET_ADDRESS         0x05
#define USB_GET_DESCRIPTOR      0x06
#define USB_GET_CONFIGURATION   0x08
#define USB_SET_CONFIGURATION   0x09
#define USB_GET_INTERFACE       0x0A
#define USB_SET_INTERFACE       0x0B

// ===================================================================================
// USB Request Type Bits
// ===================================================================================

#define USB_REQ_TYP_MASK        0x60  // bit mask of request type
#define USB_REQ_TYP_STANDARD    0x00
#define USB_REQ_RECIP_MASK      0x1F  // bit mask of request recipient
#define USB_REQ_RECIP_DEVICE    0x00
#define USB_REQ_RECIP_ENDP      0x02

// ===================================================================================
// USB Descriptor Types
// ===================================================================================

#define USB_DESCR_TYP_DEVICE    0x01
#define USB_DESCR_TYP_CONFIG    0x02
#define USB_DESCR_TYP_STRING    0x03
#define USB_DESCR_TYP_INTERF    0x04
#define USB_DESCR_TYP_ENDP      0x05
#define USB_DESCR_TYP_HID       0x21
#define USB_DESCR_TYP_REPORT    0x22

// ===================================================================================
// USB Device Class
// ===================================================================================

#define USB_DEV_CLASS_HID       0x03

// ===================================================================================
// USB Endpoint Type and Address
// ===================================================================================

#define USB_ENDP_TYPE_INTER     0x03
#define USB_ENDP_ADDR_EP1_IN    0x81

// ===================================================================================
// USB Descriptor Structs
// ===================================================================================

typedef struct _USB_SETUP_REQ {
    uint8_t  bRequestType;
    uint8_t  bRequest;
    uint8_t  wValueL;
    uint8_t  wValueH;
    uint8_t  wIndexL;
    uint8_t  wIndexH;
    uint8_t  wLengthL;
    uint8_t  wLengthH;
} USB_SETUP_REQ, *PUSB_SETUP_REQ;
typedef USB_SETUP_REQ __xdata *PXUSB_SETUP_REQ;

typedef struct _USB_DEVICE_DESCR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdUSB;
    uint8_t  bDeviceClass;
    uint8_t  bDeviceSubClass;
    uint8_t  bDeviceProtocol;
    uint8_t  bMaxPacketSize0;
    uint16_t idVendor;
    uint16_t idProduct;
    uint16_t bcdDevice;
    uint8_t  iManufacturer;
    uint8_t  iProduct;
    uint8_t  iSerialNumber;
    uint8_t  bNumConfigurations;
} USB_DEV_DESCR, *PUSB_DEV_DESCR;
typedef USB_DEV_DESCR __xdata *PXUSB_DEV_DESCR;

typedef struct _USB_CONFIG_DESCR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t wTotalLength;
    uint8_t  bNumInterfaces;
    uint8_t  bConfigurationValue;
    uint8_t  iConfiguration;
    uint8_t  bmAttributes;
    uint8_t  MaxPower;
} USB_CFG_DESCR, *PUSB_CFG_DESCR;
typedef USB_CFG_DESCR __xdata *PXUSB_CFG_DESCR;

typedef struct _USB_INTERF_DESCR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bInterfaceNumber;
    uint8_t  bAlternateSetting;
    uint8_t  bNumEndpoints;
    uint8_t  bInterfaceClass;
    uint8_t  bInterfaceSubClass;
    uint8_t  bInterfaceProtocol;
    uint8_t  iInterface;
} USB_ITF_DESCR, *PUSB_ITF_DESCR;
typedef USB_ITF_DESCR __xdata *PXUSB_ITF_DESCR;

typedef struct _USB_ENDPOINT_DESCR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint8_t  bEndpointAddress;
    uint8_t  bmAttributes;
    uint16_t wMaxPacketSize;
    uint8_t  bInterval;
} USB_ENDP_DESCR, *PUSB_ENDP_DESCR;
typedef USB_ENDP_DESCR __xdata *PXUSB_ENDP_DESCR;

typedef struct _USB_HID_DESCR {
    uint8_t  bLength;
    uint8_t  bDescriptorType;
    uint16_t bcdHID;
    uint8_t  bCountryCode;
    uint8_t  bNumDescriptors;
    uint8_t  bDescriptorTypeX;
    uint16_t wDescriptorLength;
} USB_HID_DESCR, *PUSB_HID_DESCR;
typedef USB_HID_DESCR __xdata *PXUSB_HID_DESCR;

// ===================================================================================
// Endpoint Sizes and Addresses
// ===================================================================================

#define EP0_SIZE        8
#define EP1_SIZE        8

#define EP_BUF_SIZE(x)  (x+2<64 ? x+2 : 64)

#define EP0_BUF_SIZE    EP_BUF_SIZE(EP0_SIZE)
#define EP1_BUF_SIZE    EP_BUF_SIZE(EP1_SIZE)

#define EP0_ADDR        0
#define EP1_ADDR        (EP0_ADDR + EP0_BUF_SIZE)

// ===================================================================================
// Configuration Descriptor Structure
// ===================================================================================

typedef struct _USB_CFG_DESCR_HID {
  USB_CFG_DESCR  config;
  USB_ITF_DESCR  interface0;
  USB_HID_DESCR  hid0;
  USB_ENDP_DESCR ep1IN;
} USB_CFG_DESCR_HID, *PUSB_CFG_DESCR_HID;
typedef USB_CFG_DESCR_HID __xdata *PXUSB_CFG_DESCR_HID;

// ===================================================================================
// Descriptor Declarations
// ===================================================================================

extern __code USB_DEV_DESCR DevDescr;
extern __code USB_CFG_DESCR_HID CfgDescr;

extern __code uint8_t ReportDescr[];
extern __code uint8_t ReportDescrLen;

#define USB_REPORT_DESCR      ReportDescr
#define USB_REPORT_DESCR_LEN  ReportDescrLen

extern __code uint16_t LangDescr[];
extern __code uint16_t ManufDescr[];
extern __code uint16_t ProdDescr[];
extern __code uint16_t SerDescr[];
extern __code uint16_t InterfDescr[];

#define USB_STR_DESCR_i0    (uint8_t*)LangDescr
#define USB_STR_DESCR_i1    (uint8_t*)ManufDescr
#define USB_STR_DESCR_i2    (uint8_t*)ProdDescr
#define USB_STR_DESCR_i3    (uint8_t*)SerDescr
#define USB_STR_DESCR_i4    (uint8_t*)InterfDescr
#define USB_STR_DESCR_ix    (uint8_t*)SerDescr

// ===================================================================================
// Endpoint Buffers
// ===================================================================================

__xdata __at (EP0_ADDR) uint8_t EP0_buffer[EP0_BUF_SIZE];
__xdata __at (EP1_ADDR) uint8_t EP1_buffer[EP1_BUF_SIZE];

#define USB_setupBuf ((PUSB_SETUP_REQ)EP0_buffer)
extern uint8_t SetupReq;

// ===================================================================================
// HID Handler Functions (callbacks wired via defines below)
// ===================================================================================

void HID_setup(void);
void HID_reset(void);
void HID_EP1_IN(void);

#define USB_INIT_handler    HID_setup
#define USB_RESET_handler   HID_reset

#define EP0_SETUP_callback  USB_EP0_SETUP
#define EP0_IN_callback     USB_EP0_IN
#define EP0_OUT_callback    USB_EP0_OUT
#define EP1_IN_callback     HID_EP1_IN

// ===================================================================================
// USB Core Functions
// ===================================================================================

void USB_interrupt(void);
void USB_init(void);

// ===================================================================================
// USB HID Public API
// ===================================================================================

void HID_init(void);
void HID_sendReport(__xdata uint8_t* buf, uint8_t len);
