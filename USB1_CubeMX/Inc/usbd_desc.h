 /*
00002  *  M-Stack USB CDC Device Class Structures
00003  *  Copyright (C) 2013 Alan Ott <alan@signal11.us>
00004  *  Copyright (C) 2013 Signal 11 Software
00005  *
00006  *  2013-09-27
00007  *
00008  *  M-Stack is free software: you can redistribute it and/or modify it under
00009  *  the terms of the GNU Lesser General Public License as published by the
00010  *  Free Software Foundation, version 3; or the Apache License, version 2.0
00011  *  as published by the Apache Software Foundation.  If you have purchased a
00012  *  commercial license for this software from Signal 11 Software, your
00013  *  commerical license superceeds the information in this header.
00014  *
00015  *  M-Stack is distributed in the hope that it will be useful, but WITHOUT
00016  *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
00017  *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public
00018  *  License for more details.
00019  *
00020  *  You should have received a copy of the GNU Lesser General Public License
00021  *  along with this software.  If not, see <http://www.gnu.org/licenses/>.
00022  *
00023  *  You should have received a copy of the Apache License, verion 2.0 along
00024  *  with this software.  If not, see <http://www.apache.org/licenses/>.
00025  */

 #ifndef USB_CDC_H__
 #define USB_CDC_H__

 #include <stdint.h>
 #include "usbd_conf.h"

 #if defined(__XC16__) || defined(__XC32__)
 #pragma pack(push, 1)
 #elif __XC8
 #else
#error "Compiler not supported"
 #endif

 /* CDC Specification 1.1 document sections are listed in the comments */
 #define CDC_DEVICE_CLASS 0x02 /* 4.1 */
 #define CDC_COMMUNICATION_INTERFACE_CLASS 0x02 /* 4.2 */
 #define CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS 0x02 /* 4.3 */
 /* Many of the subclass codes (section 4.3) are omitted here. Get in
  * contact with Signal 11 if you need something specific. */

 #define CDC_DATA_INTERFACE_CLASS 0x0a /* 4.5 */
 #define CDC_DATA_INTERFACE_CLASS_PROTOCOL_NONE 0x0 /* 4.7 */
 #define CDC_DATA_INTERFACE_CLASS_PROTOCOL_VENDOR 0xff /* 4.7 */
 /* Many of the protocol codes (section 4.7) are omitted here. Get in
  * contact with Signal 11 if you need something specific. */

 enum CDCDescriptorTypes {
         DESC_CS_INTERFACE = 0x24,
         DESC_CS_ENDPOINT  = 0x25,
 };

 /* Descriptor subtypes: 5.2.3 */
 enum CDCFunctionalDescriptorSubtypes {
         CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_HEADER = 0x0,
         CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_ACM = 0x2,
         CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_UNION = 0x6,
 };
 /* Many of the descriptor subtypes (section 5.2.3, table 25) are omitted
  * here.  Get in contact with Signal 11 if you need something specific. */

 enum CDCACMCapabilities {
         CDC_ACM_CAPABILITY_COMM_FEATURES = 0x1,
         CDC_ACM_CAPABILITY_LINE_CODINGS = 0x2,
         CDC_ACM_CAPABILITY_SEND_BREAK = 0x4,
         CDC_ACM_CAPABILITY_NETWORK_CONNECTION = 0x8,
 };

 enum CDCRequests {
         CDC_SEND_ENCAPSULATED_COMMAND = 0x0,
         CDC_GET_ENCAPSULATED_RESPONSE = 0x1,
         CDC_SET_COMM_FEATURE = 0x2,
         CDC_GET_COMM_FEATURE = 0x3,
        CDC_CLEAR_COMM_FEATURE = 0x4,
         CDC_SET_LINE_CODING = 0x20,
         CDC_GET_LINE_CODING = 0x21,
         CDC_SET_CONTROL_LINE_STATE = 0x22,
         CDC_SEND_BREAK = 0x23,
 };

 enum CDCCommFeatureSelector {
         CDC_FEATURE_ABSTRACT_STATE = 0x1,
         CDC_FEATURE_COUNTRY_SETTING = 0x2,
 };

 enum CDCCharFormat {
         CDC_CHAR_FORMAT_1_STOP_BIT = 0,
         CDC_CHAR_FORMAT_1_POINT_5_STOP_BITS = 1,
         CDC_CHAR_FORMAT_2_STOP_BITS = 2,
 };

 enum CDCParityType {
         CDC_PARITY_NONE  = 0,
         CDC_PARITY_ODD   = 1,
         CDC_PARITY_EVEN  = 2,
         CDC_PARITY_MARK  = 3,
         CDC_PARITY_SPACE = 4,
 };

 enum CDCNotifications {
         CDC_NETWORK_CONNECTION = 0x0,
         CDC_RESPONSE_AVAILABLE = 0x1,
         CDC_SERIAL_STATE = 0x20,
 };
 /* Many of the CDC Notifications are omitted here.  Get in contact with
  * Signal 11 if you need something specific.  */

 struct cdc_functional_descriptor_header {
         uint8_t bFunctionLength;
         uint8_t bDescriptorType;
         uint8_t bDescriptorSubtype;
         uint16_t bcdCDC;
 };

 struct cdc_acm_functional_descriptor {
         uint8_t bFunctionLength;
         uint8_t bDescriptorType;
         uint8_t bDescriptorSubtype;
         uint8_t bmCapabilities;
 };

 struct cdc_union_functional_descriptor {
         uint8_t bFunctionLength;
         uint8_t bDescriptorType;
         uint8_t bDescriptorSubtype;
         uint8_t bMasterInterface;
         uint8_t bSlaveInterface0;
         /* More bSlaveInterfaces cound go here, but you'll have to pack them
          * yourself into the configuration descriptor, and make sure
          * bFunctionLength covers them all. */
 };

 /* CDC Notification Header
  *
  * CDC Notifications all share this same header. It's very similar to a
  * @p setup_packet .
  */
 struct cdc_notification_header {
         union {
                 struct {
                         uint8_t destination : 5;
                         uint8_t type : 2;
                         uint8_t direction : 1;
                 };
                 uint8_t bmRequestType;
         } REQUEST;
         uint8_t bNotification;
         uint16_t wValue;
         uint16_t wIndex;
         uint16_t wLength;
 };


 /* CDC Serial State Notification
  *
  * See Section 6.3.5 of the CDC Specification, version 1.1.
  */
 struct cdc_serial_state_notification {
         struct cdc_notification_header header;
         union {
                 struct {
                         uint16_t bRxCarrier : 1;
                         uint16_t bTxCarrier : 1;
                         uint16_t bBreak : 1;
                         uint16_t bRingSignal : 1;
                         uint16_t bFraming : 1;
                         uint16_t bParity : 1;
                         uint16_t bOverrun : 1;
                         uint16_t : 1;
                         uint16_t : 8; /* XC8 can't handle a 9-bit bitfield */
                 } bits;
                 uint16_t serial_state;
         } data;
 };

 /* Many functional descriptors are omitted here. Get in contact with
  * Signal 11 if you need something specific. */

 /* Message Structures */

 struct cdc_line_coding {
         uint32_t dwDTERate;
         uint8_t bCharFormat;
         uint8_t bParityType;
         uint8_t bDataBits;
 };


 uint8_t process_cdc_setup_request(const struct setup_packet *setup);

 extern int8_t CDC_SEND_ENCAPSULATED_COMMAND_CALLBACK(uint8_t interface,
                                                      uint16_t length);

 #ifdef CDC_GET_ENCAPSULATED_RESPONSE_CALLBACK

 extern int16_t CDC_GET_ENCAPSULATED_RESPONSE_CALLBACK(uint8_t interface,
                                uint16_t length, const void **response,
                                usb_ep0_data_stage_callback *callback,
                                void **context);
 #endif

 #ifdef CDC_SET_COMM_FEATURE_CALLBACK

 extern int8_t CDC_SET_COMM_FEATURE_CALLBACK(uint8_t interface,
                                             bool idle_setting,
                                             bool data_multiplexed_state);
 #endif

 #ifdef CDC_CLEAR_COMM_FEATURE_CALLBACK

 extern int8_t CDC_CLEAR_COMM_FEATURE_CALLBACK(uint8_t interface,
                                               bool idle_setting,
                                               bool data_multiplexed_state);
 #endif

 #ifdef CDC_GET_COMM_FEATURE_CALLBACK

 extern int8_t CDC_GET_COMM_FEATURE_CALLBACK(
                                uint8_t interface,
                                bool *idle_setting,
                                bool *data_multiplexed_state);
 #endif

 #ifdef CDC_SET_LINE_CODING_CALLBACK

 extern int8_t CDC_SET_LINE_CODING_CALLBACK(uint8_t interface,
                                          const struct cdc_line_coding *coding);
 #endif

 #ifdef CDC_GET_LINE_CODING_CALLBACK

 extern int8_t CDC_GET_LINE_CODING_CALLBACK(uint8_t interface,
                                            struct cdc_line_coding *coding);
 #endif

 #ifdef CDC_SET_CONTROL_LINE_STATE_CALLBACK

 extern int8_t CDC_SET_CONTROL_LINE_STATE_CALLBACK(uint8_t interface,
                                                   bool dtr, bool dts);
 #endif

 #ifdef CDC_SEND_BREAK_CALLBACK

 extern int8_t CDC_SEND_BREAK_CALLBACK(uint8_t interface, uint16_t duration);
 #endif


 /* Doxygen end-of-group for cdc_items */
 #if defined(__XC16__) || defined(__XC32__)
 #pragma pack(pop)
 #elif __XC8
 #else
 #error "Compiler not supported"
 #endif

/* Doxygen end-of-group for public_api */
 #endif /* USB_CDC_H__ */
