#ifndef _WJ_USI_CAN_H_
#define _WJ_USI_CAN_H_

#include "wj_usi.h"

#ifdef __cplusplus
extern "C" {
#endif

#define  WJ_CAN_MODE_OPERATION      (0x1UL<<0)

typedef struct {
    __IOM uint32_t   CANMOD;                    // Offset 0x00 controller MODe setting register
    __IOM uint32_t   CANCMR;                    // Offset 0x04 CAN controller ComMand Register.
    __IOM uint32_t   CANSR;                     // Offset 0x08
    __IOM uint32_t   CANIR;                     // Offset 0x0C
    __IOM uint32_t   CANIER;                    // Offset 0x10
    __IOM uint32_t   CANBTR0;                   // Offset 0x14
    __IOM uint32_t   CANBTR1;                   // Offset 0x18
    __IOM uint32_t   CANOCR;                    // Offset 0x1C
    __IOM uint32_t   CANALC;                    // Offset 0x20
    __IOM uint32_t   CANECC;                    // Offset 0x24
    __IOM uint32_t   CANEWLR;                   // Offset 0x28
    __IOM uint32_t   CANRXERR;                  // Offset 0x2C
    __IOM uint32_t   CANTXERR;                  // Offset 0x30
    __IOM uint32_t   CANTRANSMIT_BUFFER_W0;     // Offset 0x40
    __IOM uint32_t   CANTRANSMIT_BUFFER_W1;     // Offset 0x44
    __IOM uint32_t   CANTRANSMIT_BUFFER_W2;     // Offset 0x48
    __IOM uint32_t   CANTRANSMIT_BUFFER_W3;     // Offset 0x4C
    __IOM uint32_t   CANTRANSMIT_BUFFER_R0;     // Offset 0x50
    __IOM uint32_t   CANTRANSMIT_BUFFER_R1;     // Offset 0x54
    __IOM uint32_t   CANTRANSMIT_BUFFER_R2;     // Offset 0x58
    __IOM uint32_t   CANTRANSMIT_BUFFER_R3;     // Offset 0x5C
    __IOM uint32_t   CANRECEIVE_BUFFER_R0;      // Offset 0x40  // RO
    __IOM uint32_t   CANRECEIVE_BUFFER_R1;      // Offset 0x44  // RO
    __IOM uint32_t   CANRECEIVE_BUFFER_R2;      // Offset 0x48  // RO
    __IOM uint32_t   CANRECEIVE_BUFFER_R3;      // Offset 0x4C  // RO
    __IOM uint32_t   CANACR;                    // Offset 0x60
    __IOM uint32_t   CANRMC;                    // Offset 0x64
    __IOM uint32_t   CANRBSA;                   // Offset 0x68
    __IOM uint32_t   CANCDR;                    // Offset 0x6C
} wj_can_reg_t;

typedef void (*can_event_cb_t)(int32_t idx, can_event_e event);   ///< Pointer to \ref can_event_cb_t : USART Event call back.
can_handle_t csi_can_initialize(int32_t idx, can_event_cb_t cb_event);

#ifdef __cplusplus
}
#endif

#endif /* _WJ_USI_USART_H_ */
