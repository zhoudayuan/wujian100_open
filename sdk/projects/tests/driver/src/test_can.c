/*
 * Copyright (C) 2017-2019 Alibaba Group Holding Limited
 */

//#include "drv_usart.h"
#include "drv_can.h"
#include "stdio.h"
#include "soc.h"
#include "dtest.h"
#include "pin.h"
#include "test_driver_config.h"
#include <csi_config.h>
#include "wj_can.h"


#define can_log(format, ...)    printf("[%s:%d] "format"\n", __func__, __LINE__, ##__VA_ARGS__)

static void test_can_rw_reg(void);

void test_pin_can_init(void)
{
    //drv_pinmux_config(TEST_PIN_USART_TX, TEST_PIN_USART_TX_FUNC);
    //drv_pinmux_config(TEST_PIN_USART_RX, TEST_PIN_USART_RX_FUNC);

}



#if  0
static void test_can_fun(void)
{


    int32_t baud[]         = { 4800, 9600, 14400, 19200, 38400, 56000, 57600, 115200};
    int32_t parity[]       = { USART_PARITY_NONE, USART_PARITY_EVEN, USART_PARITY_ODD};
    int32_t stopbits[]     = { USART_STOP_BITS_1, USART_STOP_BITS_2};
    int32_t databits[]     = { USART_DATA_BITS_5, USART_DATA_BITS_6, USART_DATA_BITS_7, USART_DATA_BITS_8};

    uint8_t i, j, k, l;

    for (i = 0; i < sizeof(baud) / sizeof(baud[0]); i++)
        for (j = 0; j < sizeof(parity) / sizeof(parity[0]); j++)
            for (k = 0; k < sizeof(stopbits) / sizeof(stopbits[0]); k++)
                for (l = 0; l < sizeof(databits) / sizeof(databits[0]); l++) {
                    if (test_function(baud[i], parity[j], stopbits[k], databits[l]) < 0) {
                        printf("test_function error\n");
                        return;
                    }
                }

}
#endif



static void test_can_fun(void)
{
}



static void test_can_interfaces(void)
{
	uint8_t data[] = {1,2,3,4};
    can_handle_t pcsi_can;
	test_pin_can_init();
    pcsi_can = csi_can_initialize(0 , NULL);
    drv_can_config_mode(pcsi_can,  CAN_MODE_RESET);
    drv_can_config_clock(pcsi_can, CAN_CLKOUT_ENABLE, CAN_FCLK_OSC_DIVIDED_4);
    drv_can_config_OCR(pcsi_can, CAN_NORMAL_OUTPUT_MODE);
    drv_can_config_IER_disable(pcsi_can, CAN_BUS_ERROR_INTERRUPT);
    drv_can_config_IER_disable(pcsi_can, CAN_ARBITRATION_LOST_INTERRUPT);
    drv_can_config_IER_disable(pcsi_can, CAN_ERROR_PASSIVE_INTERRUPT);
    drv_can_config_IER_disable(pcsi_can, CAN_WAKE_UP_INTERRUPT);
    drv_can_config_IER_disable(pcsi_can, CAN_DATA_OVERRUN_INTERRUPT);
    drv_can_config_IER_disable(pcsi_can, CAN_ERROR_WARNING_INTERRUPT);
    drv_can_config_IER_disable(pcsi_can, CAN_TRANSMIT_INTERRUPT);
    drv_can_config_IER_disable(pcsi_can, CAN_RECEIVE_INTERRUPT);
    drv_can_config_mode(pcsi_can, CAN_MODE_ACCEPTANCE_SINGLE_FILTER);
    drv_can_config_acceptance_filters(pcsi_can, CAN_MODE_ACCEPTANCE_SINGLE_FILTER);
    drv_can_config_clock(pcsi_can, CAN_CLKOUT_ENABLE, CAN_FCLK_OSC_DIVIDED_4);

	// 配置时序
	// 接受过滤器将应用于收到的邮件
	// 传输位流的副本或TX1上的传输时钟
	// 中断配置
	ASSERT_TRUE(pcsi_can != NULL);
    csi_can_send(pcsi_can, data, ARRAY_SIZE(data));
    //csi_can_config(pcsi_can, CAN_MODE_OPERATION);

}







static int32_t test_can_transmission(void)
{
    can_handle_t *pcsi_can;
    wj_can_reg_t  *addr;
    uint8_t data[] = {1,2,3,4};
    pcsi_can = (can_handle_t *)csi_can_initialize(0 , NULL);
    csi_can_set_mode(pcsi_can, CAN_MODE_OPERATION);
    if (drv_is_transmit_buffer_free(pcsi_can) == TRUE)
    {
        printf("[%s:%d] drv_is_transmit_buffer_free faill", __func__, __LINE__);
        return FALSE;
    }

    csi_write_transmit_buf(pcsi_can, data, ARRAY_SIZE(data));


    
    drv_can_config_mode(pcsi_can, CAN_MODE_OPERATION);  // 配置为OP模式
//    csi_can_send(pcsi_can, data, ARRAY_SIZE(data), CAN_FRAME_REMOTE);
}



int test_can(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_can", NULL, NULL, NULL, NULL
    };
    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);
    dtest_case_info_t test_case_info_array[] = {
        {"test_can_rw_reg",        test_can_rw_reg,       CAN_TEST_REG_EN},
//        { "test_can_fun",          test_can_fun,          CAN_TEST_FUN_EN},
//        { "test_can_interfaces",   test_can_interfaces,   CAN_TEST_INTERFACE_EN},
//        { "test_can_transmission", test_can_transmission, CAN_TEST_TRANSMISSION_EN},
          { NULL, NULL }
    };
    dtest_add_cases(test_suite, test_case_info_array);
    return 0;
}





static void test_can_rw_reg(void)
{
    can_handle_t *pcsi_can = csi_can_initialize(0 , NULL)
    drv_can_config_mode(pcsi_can, CAN_MODE_RESET);
    show_can_reg();
}


static void show_can_reg(can_handle_t handle, )
{
}



#if 0
#define CANMOD_ADDR                        //Offset 0x00 MOD                    Read/Write  Read/Write
#define CANCMR_ADDR                        //Offset 0x01 CMR                    Write only  Write only
#define CANSR_ADDR                         //Offset 0x02 SR                     Read only   Read only
#define CANIR_ADDR                         //Offset 0x03 IR                     Read only   Read only
#define CANIER_ADDR                        //Offset 0x04 IER                    Read/Write  Read/Write
#define Reserved0_ADDR                     //Offset 0x05 Reserved0              N/A         N/A
#define CANBTR0_ADDR                       //Offset 0x06 BTR0                   Read only   Read/Write
#define CANBTR1_ADDR                       //Offset 0x07 BTR1                   Read only   Read/Write
#define CANOCR_ADDR                        //Offset 0x08 OCR                    Read only   Read/Write
#define Reserved1_ADDR                     //Offset 0x09 Reserved1              N/A         N/A
#define Reserved2_ADDR                     //Offset 0x0A Reserved2              N/A         N/A
#define CANALC_ADDR                        //Offset 0x0B ALC                    Read only   Read only
#define CANECC_ADDR                        //Offset 0x0C ECC                    Read only   Read only
#define CANTEWLR_ADDR                      //Offset 0x0D EWLR                   Read only   Read/Write
#define CANRXERR_ADDR                      //Offset 0x0E RXERR                  Read only   Read/Write
#define CANTXERR_ADDR                      //Offset 0x0F TXERR                  Read only   Read/Write
#define CANTransmit_ADDR                   //Offset 0x10 Transmit               Write
#define CANTransmit_buf[0x0C]_ADDR         //Offset 0x11-1C Transmit_BUF        Write
#define CANReceive_ADDR                    //Offset 0x10                        Read
#define CANReceive_buf[0x0C]_ADDR          //Offset 0x11-0x1C Receive_buf       Read
#define CANWindow[0x0C]_ADDR               //Offset 0x11-0x1C
#define CANACR[4]_ADDR                     //Offset 0X10-0x13
#define CANAMR[4]_ADDR                     //Offset 0x14-0x17
#define CANRMC_ADDR                        //Offset 0x1D RMC                    Read only   Read only
#define CANRBSA_ADDR                       //Offset 0x1E RBSA                   Read only  Read/Write
#define CANCDR_ADDR                        //Offset 0x1F CDR                    Read/Write  Read/Write
#define CANReceive_FIFO[0x40]_ADDR         //Offset 0x20-5F CDR                 Read only  Read/Write
#define CANTransmit_Buffer[0x0d]_ADDR      //Offset 0x60-0x6C Transmit Buffer   Read only  Read only
#define CANReserved3[0x13]_ADDR            //Offset 0x6D-0x7F Transmit Buffer   N/A        N/A

#endif

