#ifndef _CSI_CAN_H_
#define _CSI_CAN_H_
#include <drv_common.h>
#ifdef __cplusplus
extern "C" {
#endif

#define Bool  int

//AM:CANMOD     PG:MODE REGISTER (MOD): ADDRESS 00h
//#define  WJ_CAN_MODE_OPERATION      (0x1UL<<0)
typedef enum {
    CAN_MODE_NONE = 0,
    CAN_MODE_OPERATION,
    CAN_MODE_RESET,
    CAN_MODE_ACCEPTANCE_SINGLE_FILTER,
    CAN_MODE_ACCEPTANCE_DUAL_FILTER,
} can_mode_e;


typedef enum {
    CAN_BUS_ERROR_INTERRUPT         = (0x1UL<<7),
    CAN_ARBITRATION_LOST_INTERRUPT  = (0x1UL<<6),
    CAN_ERROR_PASSIVE_INTERRUPT     = (0x1UL<<5),
    CAN_WAKE_UP_INTERRUPT           = (0x1UL<<4),
    CAN_DATA_OVERRUN_INTERRUPT      = (0x1UL<<3),
    CAN_ERROR_WARNING_INTERRUPT     = (0x1UL<<2),
    CAN_TRANSMIT_INTERRUPT          = (0x1UL<<1),
    CAN_RECEIVE_INTERRUPT           = (0x1UL<<0)
} can_IER_e;

typedef enum {
    CAN_BUS_STATUS                   = (0x1UL<<7),
    CAN_ERROR_STATUS                 = (0x1UL<<6),
    CAN_TRANSMIT_STATUS              = (0x1UL<<5),
    CAN_RECEIVE_STATUS               = (0x1UL<<4),
    CAN_TRANSMISSION_COMPLETE_STATUS = (0x1UL<<3),
    CAN_TRANSMIT_BUFFE_STATUS        = (0x1UL<<2),
    CAN_DATA_OVERRUN_STATUS          = (0x1UL<<1),
    CAN_RECEIVE_BUFFER_STATUS        = (0x1UL<<0)
} can_SR_e;


typedef enum {
    CAN_BUS_STATUS                   = (0x1UL<<7),
    CAN_ERROR_STATUS                 = (0x1UL<<6),
    CAN_TRANSMIT_STATUS              = (0x1UL<<5),
    CAN_RECEIVE_STATUS               = (0x1UL<<4),
    CAN_TRANSMISSION_COMPLETE_STATUS = (0x1UL<<3),
    CAN_TRANSMIT_BUFFE_STATUS        = (0x1UL<<2),
    CAN_DATA_OVERRUN_STATUS          = (0x1UL<<1),
    CAN_RECEIVE_BUFFER_STATUS        = (0x1UL<<0)
} can_transmit_e;







#define ERR_CAN(errno) (CSI_DRV_ERRNO_CAN_BASE | errno)
#define CAN_NULL_PARAM_CHK(para) HANDLE_PARAM_CHK(para, ERR_CAN(DRV_ERROR_PARAMETER))
#define ARRAY_SIZE(array) (sizeof(array)/sizeof(array[0]))

typedef void *can_handle_t;

typedef enum {
    USART_CAN_SEND_COMPLETE       = 0,  ///< Send completed; however USART may still transmit data
} can_event_e;







typedef void (*can_event_cb_t)(int32_t idx, can_event_e event);   ///< Pointer to \ref can_event_cb_t : USART Event call back.
can_handle_t drv_can_initialize(int32_t idx, can_event_cb_t cb_event);
int32_t drv_can_config_mode(can_handle_t handle, can_mode_e mode);
int32_t drv_can_send(can_handle_t handle, const void *data, uint32_t num);
Bool drv_can_is_reset_mode(can_handle_t handle);
int32_t drv_can_enable_interrupt(can_handle_t handle, uint32_t can_IER);
int32_t drv_can_disable_interrupt(can_handle_t handle, uint32_t can_IER);
int32_t drv_can_config_acceptance_filters(can_handle_t handle, uint32_t mode, uint32_t acr, uint32_t amr);
Bool drv_can_is_transmit_buffer_released(can_handle_t handle);



#ifdef __cplusplus
}
#endif

#endif /* _CSI_CAN_H_ */
