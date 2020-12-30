
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
#define PRINT_REG_CAN_OFFSET(_reg_str, _reg_val)  do{printf("|%-25s|%#-7x|%#-7lx|\n", _reg_str, _reg_val, (uint32_t)(&(_reg_val))-WJ_CAN_BASE);\
                                                     printf("|-------------------------|-------|-------|\n");} while(0);




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
    drv_irq_register(can_priv->irq, handler);
    drv_irq_enable(can_priv->irq);
    return can_priv;
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
            printf("set [CAN_MODE_OPERATION]\n");
            addr->CANMOD &= ~CAN_BIT_RESET_MODE;
            break;
        case CAN_MODE_RESET:
            printf("set [CAN_MODE_RESET]\n");
            addr->CANMOD |=  CAN_BIT_RESET_MODE;
            break;
        case CAN_MODE_ACCEPTANCE_SINGLE_FILTER:
            printf("set [CAN_MODE_ACCEPTANCE_SINGLE_FILTER]\n");
            addr->CANMOD |=  CAN_BIT_ACCEPTANCE_FILTER_MODE;
            break;
        case CAN_MODE_ACCEPTANCE_DUAL_FILTER:
            printf("set [CAN_MODE_ACCEPTANCE_DUAL_FILTER]\n");
            addr->CANMOD &=  CAN_BIT_ACCEPTANCE_FILTER_MODE;
            break;
        default:
            return ERR_CAN(DRV_ERROR_PARAMETER);
    }
    return 0;
}

Bool drv_can_is_reset_mode(can_handle_t handle)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    return (addr->CANMOD & CAN_BIT_RESET_MODE == 0) ? TRUE : FALSE;
}


/*
    setp-1:Set CLKOUT frequency, derived from XTAL1 input
*/
int32_t drv_can_config_clock(can_handle_t handle, uint32_t clkout_enable, uint32_t fclk_osc)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->CANCDR  = ((addr->CANCDR & ~0x07)   | (fclk_osc & 0x07));
    addr->CANCDR  = ((addr->CANCDR & ~(1 << 3)) | (clkout_enable & 1 << 3));
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
#if 0
int32_t drv_can_config_IER_enable(can_handle_t handle, uint32_t interrupt_enable)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->CANIER |=  1 << interrupt_enable;
    return 0;
}
#endif
/*
    setp-4:Enable required interrupts
*/
#if 0
int32_t drv_can_config_IER_disable(can_handle_t handle, uint32_t interrupt_disable)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = (wj_can_reg_t *)(((wj_can_priv_t *)handle)->base);
    addr->CANIER &=  ~(1 << interrupt_disable);
    return 0;
}
#endif
/*
    setp-5-6:Set to select required range of identifiers
*/



int32_t drv_can_config_acceptance_filters(can_handle_t handle, uint32_t mode, uint32_t acr, uint32_t amr)
{
    CAN_NULL_PARAM_CHK(handle);
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
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




int32_t drv_can_enable_interrupt(can_handle_t handle, uint32_t can_IER)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANIER |= (can_IER&0XFF) ;
    return 0;
}

int32_t drv_can_disable_interrupt(can_handle_t handle, uint32_t can_IER)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANIER &= ~(can_IER&0XFF) ;
    return 0;
}








int32_t read_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANMOD;

    return 0;
}

void show_reg_CANMOD(can_handle_t handle)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    printf("CANMOD:[%#x]", addr->CANMOD);
}


int32_t show_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset)
{
    return 0;
}

void show_all_can_r_reg(can_handle_t handle)
{
    printf("show all readable reg\n");
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    printf("|-------------------------|-------|-------|\n");
    printf("|Name                     |Value  |offset |\n");
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG_CAN_OFFSET("CANMOD", addr->CANMOD);

    PRINT_REG_CAN_OFFSET("CANSR", addr->CANSR);

    PRINT_REG_CAN_OFFSET("CANIR",  addr->CANIR);

    PRINT_REG_CAN_OFFSET("CANIER", addr->CANIER);

    PRINT_REG_CAN_OFFSET("CANBTR0", addr->CANBTR0);

    PRINT_REG_CAN_OFFSET("CANBTR1", addr->CANBTR1);

    PRINT_REG_CAN_OFFSET("CANOCR", addr->CANOCR);

    PRINT_REG_CAN_OFFSET("CANALC", addr->CANALC);

    PRINT_REG_CAN_OFFSET("CANECC", addr->CANECC);

    PRINT_REG_CAN_OFFSET("CANTEWLR", addr->CANEWLR);

    PRINT_REG_CAN_OFFSET("CANRXERR", addr->CANRXERR);

    PRINT_REG_CAN_OFFSET("CANTXERR", addr->CANTXERR);

}


void show_all_r_reg_mode_reset(can_handle_t handle)
{
    printf("show all [read] reg, in [reset mode]\n");
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    printf("|-------------------------|-------|-------|\n");
    printf("|Name                     |Value  |offset |\n");
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG_CAN_OFFSET("CANMOD",                addr->CANMOD);
    PRINT_REG_CAN_OFFSET("CANIER",                addr->CANIER);
    PRINT_REG_CAN_OFFSET("CANBTR0",               addr->CANBTR0);
    PRINT_REG_CAN_OFFSET("CANBTR1",               addr->CANBTR1);
    PRINT_REG_CAN_OFFSET("CANOCR",                addr->CANOCR);
    PRINT_REG_CAN_OFFSET("CANALC",                addr->CANALC);
    PRINT_REG_CAN_OFFSET("CANECC",                addr->CANECC);
    PRINT_REG_CAN_OFFSET("CANEWLR",               addr->CANEWLR);
    PRINT_REG_CAN_OFFSET("CANRXERR",              addr->CANRXERR);
    PRINT_REG_CAN_OFFSET("CANTXERR",              addr->CANTXERR);
    PRINT_REG_CAN_OFFSET("CANACR[0]",             addr->U.A.CANACR[0]);
    PRINT_REG_CAN_OFFSET("CANACR[1]",             addr->U.A.CANACR[1]);
    PRINT_REG_CAN_OFFSET("CANACR[2]",             addr->U.A.CANACR[2]);
    PRINT_REG_CAN_OFFSET("CANACR[3]",             addr->U.A.CANACR[3]);
    PRINT_REG_CAN_OFFSET("CANAMR[0]",             addr->U.A.CANAMR[0]);
    PRINT_REG_CAN_OFFSET("CANAMR[1]",             addr->U.A.CANAMR[1]);
    PRINT_REG_CAN_OFFSET("CANAMR[2]",             addr->U.A.CANAMR[2]);
    PRINT_REG_CAN_OFFSET("CANAMR[3]",             addr->U.A.CANAMR[3]);
    PRINT_REG_CAN_OFFSET("CANRMC",                addr->CANRMC);
    PRINT_REG_CAN_OFFSET("CANRBSA",               addr->CANRBSA);
    PRINT_REG_CAN_OFFSET("CANCDR",                addr->CANCDR);
    PRINT_REG_CAN_OFFSET("CANReceive_FIFO[0]",    addr->CANReceive_FIFO[0]);
    PRINT_REG_CAN_OFFSET("CANTransmit_Buffer[0]", addr->CANTransmit_Buffer[0]);
}


void show_all_rw_reg_mode_reset(can_handle_t handle)
{
    printf("show all [read/write] reg, in [reset mode]\n");
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    printf("|-------------------------|-------|-------|\n");
    printf("|Name                     |Value  |offset |\n");
    printf("|-------------------------|-------|-------|\n");
    PRINT_REG_CAN_OFFSET("CANMOD",                addr->CANMOD);
    PRINT_REG_CAN_OFFSET("CANIER",                addr->CANIER);
    PRINT_REG_CAN_OFFSET("CANBTR0",               addr->CANBTR0);
    PRINT_REG_CAN_OFFSET("CANBTR1",               addr->CANBTR1);
    PRINT_REG_CAN_OFFSET("CANOCR",                addr->CANOCR);
    PRINT_REG_CAN_OFFSET("CANEWLR",               addr->CANEWLR);
    PRINT_REG_CAN_OFFSET("CANRXERR",              addr->CANRXERR);
    PRINT_REG_CAN_OFFSET("CANTXERR",              addr->CANTXERR);
//  PRINT_REG_CAN_OFFSET("CANTransmit",           addr->U.T.CANTransmit);
//  PRINT_REG_CAN_OFFSET("CANReceive",            addr->U.R.CANReceive);
    PRINT_REG_CAN_OFFSET("CANACR[0]",             addr->U.A.CANACR[0]);
    PRINT_REG_CAN_OFFSET("CANACR[1]",             addr->U.A.CANACR[1]);
    PRINT_REG_CAN_OFFSET("CANACR[2]",             addr->U.A.CANACR[2]);
    PRINT_REG_CAN_OFFSET("CANACR[3]",             addr->U.A.CANACR[3]);
    PRINT_REG_CAN_OFFSET("CANAMR[0]",             addr->U.A.CANAMR[0]);
    PRINT_REG_CAN_OFFSET("CANAMR[1]",             addr->U.A.CANAMR[1]);
    PRINT_REG_CAN_OFFSET("CANAMR[2]",             addr->U.A.CANAMR[2]);
    PRINT_REG_CAN_OFFSET("CANAMR[3]",             addr->U.A.CANAMR[3]);
//  PRINT_REG_CAN_OFFSET("CANRMC",                addr->CANRMC);
    PRINT_REG_CAN_OFFSET("CANRBSA",               addr->CANRBSA);
    PRINT_REG_CAN_OFFSET("CANCDR",                addr->CANCDR);
//  PRINT_REG_CAN_OFFSET("CANReceive_FIFO[0]",    addr->CANReceive_FIFO[0]);
    PRINT_REG_CAN_OFFSET("CANTransmit_Buffer[0]", addr->CANTransmit_Buffer[0]);
}


int32_t write_CANIER_reg(can_handle_t handle,  uint8_t reg_val)
{
    printf("write [%#x] to [CANIER]\n", reg_val);
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANIER = reg_val;
    return 0;
}

int32_t write_CANMOD_reg(can_handle_t handle, uint8_t reg_val)
{
    printf("write [%#x] to [CANMOD]\n", reg_val);
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANMOD = reg_val;
    return 0;
}

int32_t write_CANBTR0_reg(can_handle_t handle, uint8_t reg_val)
{
    printf("write [%#x] to [CANBTR0]\n", reg_val);
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANBTR0 = reg_val;
    return 0;
}

int32_t write_CANBTR1_reg(can_handle_t handle, uint8_t reg_val)
{
    printf("write [%#x] to [CANBTR1]\n", reg_val);
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANBTR1 = reg_val;
    return 0;
}

int32_t write_CANEWLR_reg(can_handle_t handle, uint8_t reg_val)
{
    printf("write [%#x] to [CANEWLR]\n", reg_val);
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->CANEWLR = reg_val;
    return 0;
}

int32_t write_CANTransmit_reg(can_handle_t handle, uint8_t reg_val)
{
    printf("write [%#x] to [CANEWLR]\n", reg_val);
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->U.T.CANTransmit = reg_val;
    return 0;
}


int32_t wj_can_transmission(uint8_t *p_send_buf, uint32_t send_buf_len)
{
#if 0

    wj_can_reg_t  *addr;

    if (drv_is_transmit_buffer_free(handle) == TRUE) {
        printf("[%s:%d] drv_is_transmit_buffer_free faill", __func__, __LINE__);
        return FALSE;
    }
    csi_write_transmit_buf(handle, data, ARRAY_SIZE(data));
    drv_can_config_mode(handle, CAN_MODE_OPERATION);
//    csi_can_send(handle, data, ARRAY_SIZE(data), CAN_FRAME_REMOTE);
#endif
    can_handle_t *handle = drv_can_initialize(0, NULL);
    if (drv_can_is_reset_mode(handle) != TRUE) {
        drv_can_config_mode(handle, CAN_MODE_RESET);
    }
    drv_can_enable_interrupt(handle, CAN_TRANSMIT_INTERRUPT|CAN_ERROR_PASSIVE_INTERRUPT);
    if (drv_can_is_transmit_buffer_released(handle) != TRUE)
    {
        can_log("drv_can_is_transmit_buffer_released != TRUE, fail");
        return FALSE;
    }
    drv_can_send(handle, p_send_buf, send_buf_len);




    return 0;
}


Bool drv_can_is_transmit_buffer_released(can_handle_t handle)
{
    int timeout = 1000000;
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    while (((addr->CANSR & CAN_TRANSMIT_BUFFE_STATUS) == 0) && (timeout--));  // SR.2=1:released, SR.2=0: lock
    if (timeout < 0) {
        return false;
    }
    return true;
}

#define     EFF     1
#define     SFF     0


int32_t drv_can_send(can_handle_t handle, const void *data, uint32_t num)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    uint32_t format = drv_can_get_frame_format(handle);
    if (format == SFF)
    {
        addr->U.T.CANTransmit_buf
    }
    else (format == EFF)
    {
        addr->U.T.CANTransmit_buf
    }
    
}

uint32_t drv_can_get_frame_format(can_handle_t handle)
{
    wj_can_reg_t *addr = get_can_reg_addr_base(handle);
    addr->U.T.CANTransmit = ;

    
}


