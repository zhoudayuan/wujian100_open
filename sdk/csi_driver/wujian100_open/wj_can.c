
#include <csi_config.h>
#include <string.h>
#include <drv_irq.h>
#include <drv_can.h>
#include <wj_can.h>
#include <drv_gpio.h>
#include <soc.h>

extern int32_t target_can_init(int32_t idx, uint32_t *base, uint32_t *irq, void **handler);

typedef struct {
    uint32_t base;
    uint32_t irq;
    int32_t  ssel;
    spi_event_cb_t cb_event;
    uint32_t send_num;
    uint32_t recv_num;
    uint8_t *send_buf;
    uint8_t *recv_buf;
    uint8_t  enable_slave;
    uint32_t transfer_num;
    uint32_t clk_num;            //clock number with a process of communication
    uint8_t  state;               //Current SPI state
    uint32_t mode;               //Current SPI mode
    uint8_t  ss_mode;
    spi_status_t status;
    int32_t block_mode;
    int32_t idx;
#ifdef CONFIG_SPI_DMA
    int32_t dma_tx_id;
    int32_t dma_rx_id;
#endif
#define TRANSFER_STAT_IDLE      0
#define TRANSFER_STAT_SEND      1
#define TRANSFER_STAT_RCV       2
#define TRANSFER_STAT_TRAN      3
    uint8_t  transfer_stat;     //TRANSFER_STAT_* : 0 - idle, 1 - send , 2 -receive , 3 - transceive
    uint32_t tot_num;
} wj_can_priv_t;

static wj_can_priv_t can_instance[CONFIG_USI_NUM];


void wj_CAN_irqhandler(int idx)
{

}

void wj_usi_can_irqhandler(int32_t idx)
{
#if 0
    wj_usi_spi_priv_t *spi_priv = &spi_instance[idx];
    wj_usi_reg_t *addr = (wj_usi_reg_t *)(spi_priv->base);

    uint32_t intr_state = addr->USI_INTR_STA & 0x3ffff;

    /* deal with receive FIFO full interrupt */
    if (intr_state & USI_INT_SPI_STOP) {
        wj_spi_intr_rx_full(spi_priv);
        addr->USI_INTR_CLR = USI_INT_SPI_STOP;
    }

    /* deal with transmit FIFO empty interrupt */
    if (intr_state & USI_INT_TX_EMPTY) {
        wj_spi_intr_tx_empty(spi_priv);
        addr->USI_INTR_CLR = USI_INT_TX_EMPTY;
    }

    addr->USI_INTR_CLR = intr_state;
#endif
}

can_handle_t csi_can_initialize(int32_t idx, can_event_cb_t cb_event)
{
    return drv_can_initialize(idx, cb_event);
}

can_handle_t drv_can_initialize(int32_t idx, can_event_cb_t cb_event)
{
    //initialize instace
    uint32_t base;
    uint32_t irq;
    void    *handler;

    int32_t ret  = target_can_init(idx, &base, &irq, &handler);

    if (ret < 0 || ret >= CONFIG_USI_NUM) {
        return NULL;
    }

    wj_can_priv_t *can_priv = &can_instance[idx];
    can_priv->base = base;
    can_priv->idx  = idx;
    can_priv->irq  = irq;
    can_priv->cb_event = cb_event;

    drv_irq_register(can_priv->irq, handler);
    drv_irq_enable(can_priv->irq);
    return can_priv;
}

int32_t csi_can_config(can_handle_t handle, can_mode_e mode)
{
	drv_can_config_mode(handle, mode);
}


int32_t drv_can_config_mode(can_handle_t handle, can_mode_e mode)
{
    CAN_NULL_PARAM_CHK(handle);

    if ((int32_t)mode < 0) {
        return 0;
    }

    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);

    switch (mode) {
        case CAN_MODE_OPERATION:
            addr->CANMOD &= ~WJ_CAN_MODE_OPERATION;
            break;

        case CAN_MODE_RESET:
            addr->CANMOD |=  WJ_CAN_MODE_OPERATION;
            break;
        default:
            return ERR_IIC(DRV_ERROR_PARAMETER);
    }

    return 0;
}


void csi_can_send(pcsi_can, const void *data, uint32_t num)
{
    drv_can_config_mode(pcsi_can, CAN_MODE_OPERATION);
    return drv_spi_send(pcsi_can, data, num);
}

int32_t wj_can_set_mode(can_handle_t handle, can_mode_e mode)
{
    CAN_NULL_PARAM_CHK(handle);

    if ((int32_t)mode < 0) {
        return 0;
    }

    wj_can_priv_t *can_priv = (wj_can_priv_t *)handle;
    wj_can_reg_t *addr      = (wj_can_reg_t *)(can_priv->base);

    switch (mode) {
        case CAN_MODE_OPERATION:
            addr->CANMOD &= ~WJ_CAN_MODE_OPERATION;
            break;
        case CAN_MODE_RESET:
            addr->CANMOD |=  WJ_CAN_MODE_OPERATION;
            break;
        default:
            return ERR_CAN(DRV_ERROR_PARAMETER);
    }

    can_priv->mode = mode;
    return 0;
}

int32_t drv_spi_send(spi_handle_t handle, const void *data, uint32_t num)
{
	uint32_t b_flag = 0;
    if (handle == NULL || data == NULL || num == 0) {
        return ERR_CAN(DRV_ERROR_PARAMETER);
    }

    wj_can_priv_t *can_priv = handle;
    wj_can_set_mode(can_priv, CAN_MODE_OPERATION);
    wj_can_reg_t *addr = (wj_can_reg_t *)(can_priv->base);

	int timeout = 1000000;
	while (((addr->CANSR & CAN_SR_TRANSMIT_BUFFER_LOCKED) == 0) && (timeout--)); 	// wait transmit buffer free
	if (timeout < 0)
	{
		return ERR_CAN(DRV_ERROR_BUSY);
	}


}





