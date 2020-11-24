#ifndef _CSI_CAN_H_
#define _CSI_CAN_H_


#include <drv_common.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void *can_handle_t;

typedef enum {
    USART_CAN_SEND_COMPLETE       = 0,  ///< Send completed; however USART may still transmit data
} can_event_e;


typedef void (*can_event_cb_t)(int32_t idx, can_event_e event);   ///< Pointer to \ref usart_event_cb_t : USART Event call back.

can_handle_t csi_can_initialize(int32_t idx, can_event_cb_t cb_event);


#ifdef __cplusplus
}
#endif

#endif /* _CSI_CAN_H_ */
