
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
    int32_t  idx;
    can_event_cb_t cb_event;
    uint32_t mode;
#if 0

    uint32_t send_num;
    uint32_t recv_num;
    uint8_t *send_buf;
    uint8_t *recv_buf;
    uint8_t  enable_slave;
    uint32_t transfer_num;
    uint32_t clk_num;            //clock number with a process of communication
    uint8_t  state;               //Current SPI state

    uint8_t  ss_mode;
    spi_status_t status;
    int32_t block_mode;

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
#endif
} wj_can_priv_t;


#define PRINT_REG_CAN(_reg_str, _reg_val)  do{printf("|%-25s|%#-7x|\n", _reg_str, _reg_val);} while(0);



static wj_can_priv_t can_instance[CONFIG_CAN_NUM];





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
//    drv_irq_register(can_priv->irq, handler);
//    drv_irq_enable(can_priv->irq);
    return can_priv;
}

int32_t csi_can_config(can_handle_t handle, can_mode_e mode)
{

    //(wj_can_priv_t *)

	return drv_can_config_mode(handle, mode);
}



#if 0
int32_t drv_can_config_mode_0(can_handle_t handle, can_mode_e mode)
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
            return ERR_CAN(DRV_ERROR_PARAMETER);
    }
    return 0;
}
#endif


int32_t drv_can_config_mode(can_handle_t handle, can_mode_e mode)
{
    CAN_NULL_PARAM_CHK(handle);
    if ((int32_t)mode < 0) {
        return 0;
    }
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    switch (mode) {
        case CAN_MODE_OPERATION:
            addr->CANMOD &= ~CAN_BIT_RESET_MODE;
            break;
        case CAN_MODE_RESET:
            addr->CANMOD |=  CAN_BIT_RESET_MODE;
            break;
        case CAN_MODE_ACCEPTANCE_SINGLE_FILTER:
            addr->CANMOD |=  CAN_BIT_ACCEPTANCE_FILTER_MODE;
            break;
        case CAN_MODE_ACCEPTANCE_DUAL_FILTER:
            addr->CANMOD &=  CAN_BIT_ACCEPTANCE_FILTER_MODE;
            break;
        default:
            return ERR_CAN(DRV_ERROR_PARAMETER);
    }
    return 0;
}

/*
    setp-1:Set CLKOUT frequency, derived from XTAL1 input
*/
int32_t drv_can_config_clock(can_handle_t handle, uint32_t clkout_enable, uint32_t fclk_osc)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->CANCDR  = ((addr->CANCDR & ~0x07)   | (fclk_osc & 0x07));
    addr->CANCDR  = ((addr->CANCDR & ~(1<<3)) | (clkout_enable & 1<<3));
    return 0;
}


/*
    setp-2:Configure the output driver for outputs TX0 and TX1
*/
int32_t drv_can_config_OCR(can_handle_t handle, uint32_t output_cfg)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->CANOCR = ((addr->CANOCR & ~0X3) | (output_cfg & 0X3));
    return 0;
}


/*
    setp-3:Enable required interrupts
*/
int32_t drv_can_config_IER_enable(can_handle_t handle, uint32_t interrupt_enable)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->CANIER |=  1 << interrupt_enable;
    return 0;
}

/*
    setp-4:Enable required interrupts
*/
int32_t drv_can_config_IER_disable(can_handle_t handle, uint32_t interrupt_disable)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->CANIER &=  ~(1 << interrupt_disable);
    return 0;
}

/*
    setp-5-6:Set to select required range of identifiers
*/
int32_t drv_can_config_acceptance_filters(can_handle_t handle, uint32_t mode, uint32_t acr, uint32_t amr)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->U.A.CANACR[0] = acr & 0xff;
    addr->U.A.CANACR[1] = (acr >> 8)  & 0xff;
    addr->U.A.CANACR[2] = (acr >> 16) & 0xff;
    addr->U.A.CANACR[3] = (acr >> 24) & 0xff;
    addr->U.A.CANAMR[0] = amr & 0xff;
    addr->U.A.CANAMR[1] = (amr >> 8)  & 0xff;
    addr->U.A.CANAMR[2] = (amr >> 16) & 0xff;
    addr->U.A.CANAMR[3] = (amr >> 24) & 0xff;
    return 0;
}


void csi_can_send(can_handle_t pcsi_can, const void *data, uint32_t num)
{


}



int32_t csi_can_set_mode(can_handle_t handle, can_mode_e mode)
{
	wj_can_priv_t *can_priv = handle;
    CAN_NULL_PARAM_CHK(handle);

    if ((int32_t)mode < 0) {
        return 0;
    }
    drv_can_config_mode(handle, mode);
    can_priv->mode = mode;
    return 0;
}


int32_t csi_write_transmit_buf(can_handle_t handle, const void *data, uint32_t num)
{
    return 0;
}




int32_t drv_can_send(can_handle_t handle, const void *data, uint32_t num)
{
    if (handle == NULL || data == NULL || num == 0) {
        return ERR_CAN(DRV_ERROR_PARAMETER);
    }

    wj_can_priv_t *can_priv = handle;
    wj_can_reg_t *addr = (wj_can_reg_t *)(can_priv->base);

	int timeout = 1000000;
	while (((addr->CANSR & CAN_SR_TRANSMIT_BUFFER_LOCKED) == 0) && (timeout--)); 	// wait transmit buffer free
	if (timeout < 0)
	{
		return ERR_CAN(DRV_ERROR_BUSY);
	}
	return 0;
}


int32_t drv_is_transmit_buffer_free(can_handle_t handle)
{
    int timeout = 1000000;
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    while (((addr->CANSR & CAN_SR_TRANSMIT_BUFFER_LOCKED) == 0) && (timeout--));
    if (timeout < 0)
    {
        return FALSE;
    }
    return TRUE;
}

int32_t read_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANMOD;

    return 0;
}

int32_t write_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset)
{
    return 0;
}

int32_t show_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset)
{
    return 0;
}

void show_reg_CANMOD(can_handle_t handle)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    printf("CANMOD:[%#x]", addr->CANMOD);
}

void show_all_r_reg_CANMOD(can_handle_t handle)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    printf("|-------------------------|-------|\n");
    printf("|Name                     |Value  |\n");
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("CANMOD", addr->CANMOD);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("CANSR",  addr->CANSR);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("CANSR",  addr->CANSR);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("CANSR",  addr->CANIR);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NIER",   addr->CANIER);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NBTR0",  addr->CANBTR0);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NBTR1",  addr->CANBTR1);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NOCR",   addr->CANOCR);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NALC",   addr->CANALC);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NECC",   addr->CANECC);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NTEWLR", addr->CANTEWLR);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NRXERR", addr->CANRXERR);
    printf("|-------------------------|-------|\n");
    PRINT_REG_CAN("NTXERR", addr->CANTXERR);
    printf("|-------------------------|-------|\n");
}



