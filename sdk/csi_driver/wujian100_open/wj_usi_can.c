
#include <csi_config.h>
#include <string.h>
#include <drv_irq.h>
#include <drv_usi_can.h>
#include <wj_usi_can.h>
#include <drv_gpio.h>
#include <soc.h>

extern int32_t target_usi_can_init(int32_t idx, uint32_t *base, uint32_t *irq, void **handler);

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


can_handle_t drv_usi_can_initialize(int32_t idx, can_event_cb_t cb_event)
{
    //initialize instace
    uint32_t base;
    uint32_t irq;
    void *handler;

    int32_t ret  = target_usi_can_init(idx, &base, &irq, &handler);

    if (ret < 0 || ret >= CONFIG_USI_NUM) {
        return NULL;
    }

    ret = drv_usi_initialize(idx);

    if (ret < 0) {
        return NULL;
    }

    wj_usi_usart_priv_t *usart_priv = &usi_can_instance[idx];

    can_priv->base = base;
    can_priv->idx = idx;
    can_priv->irq = irq;
    can_priv->cb_event = cb_event;
    wj_usi_reg_t *addr = (wj_usi_reg_t *)(can_priv->base);

    addr->USI_EN = 0x0;
    addr->USI_INTR_UNMASK = WJ_UART_INT_ENABLE_DEFAUL;
    addr->USI_INTR_EN = WJ_UART_INT_ENABLE_DEFAUL;

    wj_usi_set_rxfifo_th(addr, USI_RX_MAX_FIFO);

    addr->USI_MODE_SEL = USI_MODE_UART;
    drv_irq_register(usart_priv->irq, handler);
    drv_irq_enable(usart_priv->irq);

    return usart_priv;
}
