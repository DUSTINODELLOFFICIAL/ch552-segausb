// This code is cobbled together from the Cocket Nova examples from UNIT Electronics
// and wagiminator and altered to remove all the stuff not needed to present a USB
// HID Joystick. All credit to them
#include "ch552.h"
#include "usb.h"

// ===================================================================================
// Device Descriptor
// ===================================================================================

__code USB_DEV_DESCR DevDescr = {
  .bLength            = sizeof(DevDescr),       // size of the descriptor in bytes: 18
  .bDescriptorType    = USB_DESCR_TYP_DEVICE,   // device descriptor: 0x01
  .bcdUSB             = 0x0110,                 // USB specification: USB 1.1
  .bDeviceClass       = 0,                      // interface will define class
  .bDeviceSubClass    = 0,                      // unused
  .bDeviceProtocol    = 0,                      // unused
  .bMaxPacketSize0    = EP0_SIZE,               // maximum packet size for Endpoint 0
  .idVendor           = USB_VENDOR_ID,          // VID
  .idProduct          = USB_PRODUCT_ID,         // PID
  .bcdDevice          = USB_DEVICE_VERSION,     // device version
  .iManufacturer      = 1,                      // index of Manufacturer String Descr
  .iProduct           = 2,                      // index of Product String Descriptor
  .iSerialNumber      = 3,                      // index of Serial Number String Descr
  .bNumConfigurations = 1                       // number of possible configurations
};

// ===================================================================================
// Configuration Descriptor
// ===================================================================================

__code USB_CFG_DESCR_HID CfgDescr = {

  // Configuration Descriptor
  .config = {
    .bLength            = sizeof(USB_CFG_DESCR),  // size of the descriptor in bytes
    .bDescriptorType    = USB_DESCR_TYP_CONFIG,   // configuration descriptor: 0x02
    .wTotalLength       = sizeof(CfgDescr),       // total length in bytes
    .bNumInterfaces     = 1,                      // number of interfaces: 1
    .bConfigurationValue= 1,                      // value to select this configuration
    .iConfiguration     = 0,                      // no configuration string descriptor
    .bmAttributes       = 0x80,                   // attributes = bus powered, no wakeup
    .MaxPower           = USB_MAX_POWER_mA / 2    // in 2mA units
  },

  // Interface Descriptor
  .interface0 = {
    .bLength            = sizeof(USB_ITF_DESCR),
    .bDescriptorType    = USB_DESCR_TYP_INTERF,
    .bInterfaceNumber   = 0,
    .bAlternateSetting  = 0,
    .bNumEndpoints      = 1,                      // 1 endpoint (IN) for joystick
    .bInterfaceClass    = USB_DEV_CLASS_HID,
    .bInterfaceSubClass = 0,                      // no boot subclass
    .bInterfaceProtocol = 0,                      // no protocol
    .iInterface         = 4
  },

  // HID Descriptor
  .hid0 = {
    .bLength            = sizeof(USB_HID_DESCR),  // size of the descriptor in bytes: 9
    .bDescriptorType    = USB_DESCR_TYP_HID,      // HID descriptor: 0x21
    .bcdHID             = 0x0110,                 // HID class spec version (BCD: 1.1)
    .bCountryCode       = 33,                     // country code: US
    .bNumDescriptors    = 1,                      // number of report descriptors: 1
    .bDescriptorTypeX   = USB_DESCR_TYP_REPORT,   // descriptor type: report (0x22)
    .wDescriptorLength  = sizeof(ReportDescr)     // report descriptor length
  },

  // Endpoint Descriptor: Endpoint 1 (IN, Interrupt)
  .ep1IN = {
    .bLength            = sizeof(USB_ENDP_DESCR), // size of the descriptor in bytes: 7
    .bDescriptorType    = USB_DESCR_TYP_ENDP,     // endpoint descriptor: 0x05
    .bEndpointAddress   = USB_ENDP_ADDR_EP1_IN,   // endpoint: 1, direction: IN (0x81)
    .bmAttributes       = USB_ENDP_TYPE_INTER,    // transfer type: interrupt (0x03)
    .wMaxPacketSize     = EP1_SIZE,               // max packet size
    .bInterval          = 1                       // polling interval in ms
  }
};

// ===================================================================================
// HID Report Descriptor
// ===================================================================================

__code uint8_t ReportDescr[] = {
  0x05, 0x01,           // USAGE_PAGE (Generic Desktop)
  0x09, 0x04,           // USAGE (Joystick)
  0xa1, 0x01,           // COLLECTION (Application)
  0x85, 0x01,           // REPORT_ID (1) pad 1
  0xa1, 0x00,           //   COLLECTION (Physical)
  0x05, 0x01,           //     USAGE_PAGE (Generic Desktop)
  0x09, 0x30,           //     USAGE (X)
  0x09, 0x31,           //     USAGE (Y)
  0x15, 0x00,           //     LOGICAL_MINIMUM (0)
  0x26, 0xff, 0x00,     //     LOGICAL_MAXIMUM (255)
  0x75, 0x08,           //     REPORT_SIZE (8)
  0x95, 0x02,           //     REPORT_COUNT (2)
  0x81, 0x02,           //     INPUT (Data,Var,Abs)
  0x05, 0x09,           //     USAGE_PAGE (Button)
  0x19, 0x01,           //     USAGE_MINIMUM (Button 1)
  0x29, 0x0a,           //     USAGE_MAXIMUM (Button 10)
  0x15, 0x00,           //     LOGICAL_MINIMUM (0)
  0x25, 0x01,           //     LOGICAL_MAXIMUM (1)
  0x75, 0x01,           //     REPORT_SIZE (1)
  0x95, 0x0a,           //     REPORT_COUNT (10)
  0x81, 0x02,           //     INPUT (Data,Var,Abs)
  0x75, 0x01,           //     REPORT_SIZE (1)
  0x95, 0x06,           //     REPORT_COUNT (6)
  0x81, 0x03,           //     INPUT (Cnst,Var,Abs)
  0xc0,                 //   END_COLLECTION
  0xc0                  // END_COLLECTION
};

__code uint8_t ReportDescrLen = sizeof(ReportDescr);

// ===================================================================================
// String Descriptors
// ===================================================================================

// Language Descriptor (Index 0)
__code uint16_t LangDescr[] = {
  ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(LangDescr), 0x0409 };  // US English

  // Manufacturer String Descriptor (Index 1)
  __code uint16_t ManufDescr[] = {
    ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ManufDescr), MANUFACTURER_STR };

    // Product String Descriptor (Index 2)
    __code uint16_t ProdDescr[] = {
      ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(ProdDescr), PRODUCT_STR };

      // Serial String Descriptor (Index 3)
      __code uint16_t SerDescr[] = {
        ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(SerDescr), SERIAL_STR };

        // Interface String Descriptor (Index 4)
        __code uint16_t InterfDescr[] = {
          ((uint16_t)USB_DESCR_TYP_STRING << 8) | sizeof(InterfDescr), INTERFACE_STR };

          // ===================================================================================
          // HID State
          // ===================================================================================

          volatile __bit HID_EP1_writeBusyFlag = 0;         // EP1 upload busy flag

          // ===================================================================================
          // HID Public API
          // ===================================================================================

          // Setup USB HID
          void HID_init(void) {
            USB_init();
            UEP1_T_LEN = 0;
          }

          // Send HID report
          void HID_sendReport(__xdata uint8_t* buf, uint8_t len) {
            uint8_t i;
            if (USB_MIS_ST & bUMS_SUSPEND) return; // NEW: don't block if suspended
            while(HID_EP1_writeBusyFlag);                   // wait for ready to write
            for(i=0; i<len; i++) EP1_buffer[i] = buf[i];    // copy report to EP1 buffer
            UEP1_T_LEN = len;                               // set length to upload
            HID_EP1_writeBusyFlag = 1;                      // set busy flag
            UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_ACK;  // upload data, respond ACK
          }

          // ===================================================================================
          // HID Endpoint Handlers (called from USB core via callback defines)
          // ===================================================================================

          // Setup HID endpoints
          void HID_setup(void) {
            UEP1_DMA   = EP1_ADDR;                          // EP1 data transfer address
            UEP1_CTRL  = bUEP_AUTO_TOG                      // EP1 auto flip sync flag
            | UEP_T_RES_NAK;                     // EP1 IN transaction returns NAK
            UEP4_1_MOD = bUEP1_TX_EN;                       // EP1 TX enable
          }

          // Reset HID parameters
          void HID_reset(void) {
            UEP1_CTRL = bUEP_AUTO_TOG | UEP_T_RES_NAK;
            HID_EP1_writeBusyFlag = 0;
          }

          // Endpoint 1 IN handler (HID report transfer complete)
          void HID_EP1_IN(void) {
            UEP1_T_LEN = 0;                                 // no data to send anymore
            UEP1_CTRL = UEP1_CTRL & ~MASK_UEP_T_RES | UEP_T_RES_NAK;  // default NAK
            HID_EP1_writeBusyFlag = 0;                      // clear busy flag
          }

          // ===================================================================================
          // USB Handler Variables
          // ===================================================================================

          uint16_t SetupLen;
          uint8_t  SetupReq, UsbConfig;
          __code uint8_t *pDescr;

          // ===================================================================================
          // Fast Descriptor Copy (EP0)
          // ===================================================================================
          // Copy descriptor *pDescr to Ep0 using double pointer
          // (Thanks to Ralph Doncaster)
          #pragma callee_saves USB_EP0_copyDescr
          void USB_EP0_copyDescr(uint8_t len) {
            len;                          // stop unreferenced argument warning
            __asm
            push ar7                    ; r7 -> stack
            mov  r7, dpl                ; r7 <- len
            inc  _XBUS_AUX              ; select dptr1
            mov  dptr, #_EP0_buffer     ; dptr1 <- EP0_buffer
            dec  _XBUS_AUX              ; select dptr0
            mov  dpl, _pDescr           ; dptr0 <- *pDescr
            mov  dph, (_pDescr + 1)
            01$:
            clr  a                      ; acc <- #0
            movc a, @a+dptr             ; acc <- *pDescr[dptr0]
            inc  dptr                   ; inc dptr0
            .DB  0xA5                   ; acc -> EP0_buffer[dptr1] & inc dptr1
            djnz r7, 01$                ; repeat len times
            pop  ar7                    ; r7 <- stack
            __endasm;
          }

          // ===================================================================================
          // EP0 Control Transfer Handlers
          // ===================================================================================

          void USB_EP0_SETUP(void) {
            uint8_t len = USB_RX_LEN;
            if(len == (sizeof(USB_SETUP_REQ))) {
              SetupLen = ((uint16_t)USB_setupBuf->wLengthH<<8) | (USB_setupBuf->wLengthL);
              len = 0;                                      // default is success and upload 0 length
              SetupReq = USB_setupBuf->bRequest;

              if( (USB_setupBuf->bRequestType & USB_REQ_TYP_MASK) != USB_REQ_TYP_STANDARD ) {
                len = 0xFF;                                 // non-standard requests not supported
              }
              else {
                switch(SetupReq) {
                  case USB_GET_DESCRIPTOR:
                    switch(USB_setupBuf->wValueH) {

                      case USB_DESCR_TYP_DEVICE:            // Device Descriptor
                        pDescr = (uint8_t*)&DevDescr;
                        len = sizeof(DevDescr);
                        break;

                      case USB_DESCR_TYP_CONFIG:            // Configuration Descriptor
                        pDescr = (uint8_t*)&CfgDescr;
                        len = sizeof(CfgDescr);
                        break;

                      case USB_DESCR_TYP_STRING:
                        switch(USB_setupBuf->wValueL) {      // String Descriptor Index
                          case 0:    pDescr = USB_STR_DESCR_i0; break;
                          case 1:    pDescr = USB_STR_DESCR_i1; break;
                          case 2:    pDescr = USB_STR_DESCR_i2; break;
                          case 3:    pDescr = USB_STR_DESCR_i3; break;
                          case 4:    pDescr = USB_STR_DESCR_i4; break;
                          default:   pDescr = USB_STR_DESCR_ix; break;
                        }
                        len = pDescr[0];
                        break;

                          case USB_DESCR_TYP_REPORT:
                            if(USB_setupBuf->wValueL == 0) {
                              pDescr = USB_REPORT_DESCR;
                              len = USB_REPORT_DESCR_LEN;
                            }
                            else len = 0xff;
                            break;

                          default:
                            len = 0xff;                         // unsupported descriptor
                            break;
                    }

                    if(len != 0xff) {
                      if(SetupLen > len) SetupLen = len;    // limit length
                      len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
                      USB_EP0_copyDescr(len);               // copy descriptor to Ep0
                      SetupLen -= len;
                      pDescr += len;
                    }
                    break;

                          case USB_SET_ADDRESS:
                            SetupLen = USB_setupBuf->wValueL;        // save the assigned address
                            break;

                          case USB_GET_CONFIGURATION:
                            EP0_buffer[0] = UsbConfig;
                            if (SetupLen >= 1) len = 1;
                            break;

                          case USB_SET_CONFIGURATION:
                            UsbConfig = USB_setupBuf->wValueL;
                            break;

                          case USB_GET_INTERFACE:
                            break;

                          case USB_SET_INTERFACE:
                            break;

                          case USB_CLEAR_FEATURE:
                            if( (USB_setupBuf->bRequestType & 0x1F) == USB_REQ_RECIP_DEVICE ) {
                              if( ( ( (uint16_t)USB_setupBuf->wValueH << 8 ) | USB_setupBuf->wValueL ) == 0x01 ) {
                                if( ((uint8_t*)&CfgDescr)[7] & 0x20) {
                                  // wake up
                                }
                                else len = 0xFF;
                              }
                              else len = 0xFF;
                            }
                            else if( (USB_setupBuf->bRequestType & USB_REQ_RECIP_MASK) == USB_REQ_RECIP_ENDP ) {
                              switch(USB_setupBuf->wIndexL) {
                                case 0x81:
                                  UEP1_CTRL = UEP1_CTRL & ~ ( bUEP_T_TOG | MASK_UEP_T_RES ) | UEP_T_RES_NAK;
                                  break;
                                default:
                                  len = 0xFF;                       // unsupported endpoint
                                  break;
                              }
                            }
                            else len = 0xFF;
                            break;

                                case USB_SET_FEATURE:
                                  if( (USB_setupBuf->bRequestType & 0x1F) == USB_REQ_RECIP_DEVICE ) {
                                    if( ( ( (uint16_t)USB_setupBuf->wValueH << 8 ) | USB_setupBuf->wValueL ) == 0x01 ) {
                                      if( !(((uint8_t*)&CfgDescr)[7] & 0x20) ) len = 0xFF;
                                    }
                                    else len = 0xFF;
                                  }
                                  else if( (USB_setupBuf->bRequestType & 0x1F) == USB_REQ_RECIP_ENDP ) {
                                    if( ( ( (uint16_t)USB_setupBuf->wValueH << 8 ) | USB_setupBuf->wValueL ) == 0x00 ) {
                                      switch( ( (uint16_t)USB_setupBuf->wIndexH << 8 ) | USB_setupBuf->wIndexL ) {
                                        case 0x81:
                                          UEP1_CTRL = UEP1_CTRL & (~bUEP_T_TOG) | UEP_T_RES_STALL;  // Set EP1 IN STALL
                                          break;
                                        default:
                                          len = 0xFF;
                                          break;
                                      }
                                    }
                                    else len = 0xFF;
                                  }
                                  else len = 0xFF;
                                  break;

                                        case USB_GET_STATUS:
                                          EP0_buffer[0] = 0x00;
                                          EP0_buffer[1] = 0x00;
                                          if(SetupLen >= 2) len = 2;
                                          else len = SetupLen;
                                          break;

                                        default:
                                          len = 0xff;
                                          break;
                }
              }
            }
            else len = 0xff;                                // wrong packet length

            if(len == 0xff) {
              SetupReq = 0xFF;
              UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_STALL | UEP_T_RES_STALL;  // STALL
            }
            else if(len <= EP0_SIZE) {
              UEP0_T_LEN = len;
              UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // Expect DATA1, ACK
            }
            else {
              UEP0_T_LEN = 0;
              UEP0_CTRL = bUEP_R_TOG | bUEP_T_TOG | UEP_R_RES_ACK | UEP_T_RES_ACK;  // Expect DATA1, ACK
            }
          }

          void USB_EP0_IN(void) {
            uint8_t len;
            switch(SetupReq) {

              case USB_GET_DESCRIPTOR:
                len = SetupLen >= EP0_SIZE ? EP0_SIZE : SetupLen;
                USB_EP0_copyDescr(len);                     // copy descriptor to Ep0
                SetupLen  -= len;
                pDescr    += len;
                UEP0_T_LEN = len;
                UEP0_CTRL ^= bUEP_T_TOG;                    // switch between DATA0 and DATA1
                break;

              case USB_SET_ADDRESS:
                USB_DEV_AD = USB_DEV_AD & bUDA_GP_BIT | SetupLen;
                UEP0_CTRL  = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;

              default:
                UEP0_T_LEN = 0;                             // end of transaction
                UEP0_CTRL  = UEP_R_RES_ACK | UEP_T_RES_NAK;
                break;
            }
          }

          void USB_EP0_OUT(void) {
            UEP0_T_LEN = 0;
            UEP0_CTRL |= UEP_R_RES_ACK | UEP_T_RES_NAK;    // respond NAK
          }

          // ===================================================================================
          // USB Interrupt Service Routine
          // ===================================================================================

          #pragma save
          #pragma nooverlay
          void USB_interrupt(void) {
            if(UIF_TRANSFER) {
              uint8_t callIndex = USB_INT_ST & MASK_UIS_ENDP;
              switch (USB_INT_ST & MASK_UIS_TOKEN) {
                case UIS_TOKEN_OUT:
                  switch (callIndex) {
                    case 0: EP0_OUT_callback(); break;
                    default: break;
                  }
                  break;
                    case UIS_TOKEN_IN:
                      switch (callIndex) {
                        case 0: EP0_IN_callback(); break;
                        case 1: EP1_IN_callback(); break;
                        default: break;
                      }
                      break;
                        case UIS_TOKEN_SETUP:
                          switch (callIndex) {
                            case 0: EP0_SETUP_callback(); break;
                            default: break;
                          }
                          break;
              }
              UIF_TRANSFER = 0;                             // clear interrupt flag
            }

            // Device mode USB bus reset
            if(UIF_BUS_RST) {
              UEP0_CTRL  = UEP_R_RES_ACK | UEP_T_RES_NAK;
              USB_RESET_handler();                          // HID_reset
              USB_DEV_AD   = 0x00;
              UIF_SUSPEND  = 0;
              UIF_TRANSFER = 0;
              UIF_BUS_RST  = 0;                             // clear interrupt flag
            }

            // USB bus suspend / wake up
            if (UIF_SUSPEND) {
              UIF_SUSPEND = 0;
              if ( !(USB_MIS_ST & bUMS_SUSPEND) ) USB_INT_FG = 0xFF;  // clear interrupt flag
            }
          }
          #pragma restore

          // ===================================================================================
          // USB Init
          // ===================================================================================

          void USB_init(void) {
            USB_CTRL  = bUC_DEV_PU_EN                       // USB internal pull-up enable
            | bUC_INT_BUSY                        // Return NAK if USB INT flag not clear
            | bUC_DMA_EN;                         // DMA enable
            UDEV_CTRL = bUD_PD_DIS                          // Disable UDP/UDM pulldown resistor
            | bUD_PORT_EN;                        // Enable port, full-speed

            #if F_CPU < 12000000                            // Set low-speed mode if SysFreq < 12 MHz
            USB_CTRL  |= bUC_LOW_SPEED;
            UDEV_CTRL |= bUD_LOW_SPEED;
            #endif

            UEP0_DMA  = EP0_ADDR;                           // EP0 data transfer address
            UEP0_CTRL = UEP_R_RES_ACK                       // EP0 OUT transaction returns ACK
            | UEP_T_RES_NAK;                      // EP0 IN transaction returns NAK

            USB_INIT_handler();                             // HID_setup: configure EP1

            USB_INT_EN |= bUIE_SUSPEND                      // Enable device hang interrupt
            | bUIE_TRANSFER                     // Enable USB transfer completion interrupt
            | bUIE_BUS_RST;                     // Enable device mode USB bus reset interrupt

            USB_INT_FG |= 0x1F;                             // Clear interrupt flag
            IE_USB      = 1;                                // Enable USB interrupt
            EA          = 1;                                // Enable global interrupts

            UEP0_T_LEN  = 0;                               // Must be zero at start
          }
