#ifndef _CSI_CAN_H_
#define _CSI_CAN_H_


#include <drv_common.h>

#ifdef __cplusplus
extern "C" {
#endif


#define ERR_CAN(errno) (CSI_DRV_ERRNO_CAN_BASE | errno)
#define CAN_NULL_PARAM_CHK(para) HANDLE_PARAM_CHK(para, ERR_CAN(DRV_ERROR_PARAMETER))


#define  ARRAY_SIZE(array) (sizeof(array) / sizeof(array[0]))


typedef void *can_handle_t;

typedef enum {
    USART_CAN_SEND_COMPLETE       = 0,  ///< Send completed; however USART may still transmit data
} can_event_e;

typedef void (*can_event_cb_t)(int32_t idx, can_event_e event);   ///< Pointer to \ref can_event_cb_t : USART Event call back.
can_handle_t drv_can_initialize(int32_t idx, can_event_cb_t cb_event);
int32_t drv_can_config_mode(can_handle_t handle, can_mode_e mode);
int32_t drv_can_send(can_handle_t handle, const void *data, uint32_t num);
#ifdef __cplusplus
}
#endif

#endif /* _CSI_CAN_H_ */
