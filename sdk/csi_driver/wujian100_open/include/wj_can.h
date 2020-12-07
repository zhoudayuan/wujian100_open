#ifndef _WJ_USI_CAN_H_
#define _WJ_USI_CAN_H_

#include "wj_usi.h"

#ifdef __cplusplus
extern "C" {
#endif


//AM:CANMOD     PG:MODE REGISTER (MOD): ADDRESS 00h
//#define  WJ_CAN_MODE_OPERATION      (0x1UL<<0)
typedef enum {
    CAN_MODE_OPERATION  = 0,
    CAN_MODE_RESET,
    CAN_MODE_ACCEPTANCE_SINGLE_FILTER,
    CAN_MODE_ACCEPTANCE_DUAL_FILTER
} can_mode_e;


#define CAN_BIT_SLEEP_MODE                (1UL<<4)
#define CAN_BIT_ACCEPTANCE_FILTER_MODE    (1UL<<3)
#define CAN_BIT_SELF_TEST_MODE            (1UL<<2)
#define CAN_BIT_LISTEN_ONLY_MODE          (1UL<<1)
#define CAN_BIT_RESET_MODE                (1UL<<0)



//AM:CANSR
#define  CAN_SR_TRANSMIT_BUFFER_RELEASED        (0x1UL<<2)
#define  CAN_SR_TRANSMIT_BUFFER_LOCKED          (0x1UL<<2)

//AM:CANCDR PG:CDR
#define  CAN_CLKOUT_ENABLE                      (1UL<<3)
#define  CAN_FCLK_OSC_DIVIDED_2                 0x0
#define  CAN_FCLK_OSC_DIVIDED_4                 0x1
#define  CAN_FCLK_OSC_DIVIDED_6                 0x2
#define  CAN_FCLK_OSC_DIVIDED_8                 0x3
#define  CAN_FCLK_OSC_DIVIDED_10                0x4
#define  CAN_FCLK_OSC_DIVIDED_12                0x5
#define  CAN_FCLK_OSC_DIVIDED_14                0x6
#define  CAN_FCLK_OSC_DIVIDED_0                 0x7

//AM:CANOCR PG:OCR
#define CAN_NORMAL_OUTPUT_MODE                  0x02
#define CAN_CLOCK_OUTPUT_MODE                   0x03

//AM:CANIER
#define CAN_BUS_ERROR_INTERRUPT          (7)
#define CAN_ARBITRATION_LOST_INTERRUPT   (6)
#define CAN_ERROR_PASSIVE_INTERRUPT      (5)
#define CAN_WAKE_UP_INTERRUPT            (4)
#define CAN_DATA_OVERRUN_INTERRUPT       (3)
#define CAN_ERROR_WARNING_INTERRUPT      (2)
#define CAN_TRANSMIT_INTERRUPT           (1)
#define CAN_RECEIVE_INTERRUPT            (0)


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
    //---------------//
    __IOM uint32_t   CANACR0;
    __IOM uint32_t   CANACR1;
    __IOM uint32_t   CANACR2;
    __IOM uint32_t   CANACR3;
    __IOM uint32_t   CANAMR0;
    __IOM uint32_t   CANAMR1;
    __IOM uint32_t   CANAMR2;
    __IOM uint32_t   CANAMR3;
    //---------------//
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
