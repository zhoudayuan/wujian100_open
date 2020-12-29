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

t_can_reg_op can_reg_op[] = {
	{CANMOD_ID             , {read_CANMOD_reg             , write_CANMOD_reg             , show_CANMOD_reg             }},   // MOD             Read/Write  Read/Write
#if  0
	{CANCMR_ID             , {read_CANCMR_reg             , write_CANCMR_reg             , show_CANCMR_reg             }},   // CMR             Write only  Write only
	{CANSR_ID              , {read_CANSR_reg              , NULL			             , show_CANSR_reg              }},   // SR              Read only   Read only
	{CANIR_ID              , {read_CANIR_reg              , NULL              			 , show_CANIR_reg              }},   // IR              Read only   Read only
	{CANIER_ID             , {read_CANIER_reg             , write_CANIER_reg             , show_CANIER_reg             }},   // IER             Read/Write  Read/Write
	{CANBTR0_ID            , {read_CANBTR0_reg            , write_CANBTR0_reg            , show_CANBTR0_reg            }},   // BTR0            Read only   Read/Write
	{CANBTR1_ID            , {read_CANBTR1_reg            , write_CANBTR1_reg            , show_CANBTR1_reg            }},   // BTR1            Read only   Read/Write
	{CANOCR_ID             , {read_CANOCR_reg             , write_CANOCR_reg             , show_CANOCR_reg             }},   // OCR             Read only   Read/Write
	{CANALC_ID             , {read_CANALC_reg             , NULL             			 , show_CANECC_reg             }},   // ALC             Read only   Read only
	{CANECC_ID             , {read_CANECC_reg             , NULL             			 , show_CANECC_reg             }},   // ECC             Read only   Read only
	{CANTEWLR_ID           , {read_CANTEWLR_reg           , write_CANTEWLR_reg           , show_CANTEWLR_reg           }},   // EWLR            Read only   Read/Write
	{CANRXERR_ID           , {read_CANRXERR_reg           , write_CANRXERR_reg           , show_CANRXERR_reg           }},   // RXERR           Read only   Read/Write
	{CANTXERR_ID           , {read_CANTXERR_reg           , write_CANTXERR_reg           , show_CANTXERR_reg           }},   // TXERR           Read only   Read/Write
	{CANTransmit_ID        , {read_CANTransmit_reg        , write_CANTransmit_reg        , show_CANTransmit_reg        }},   // Transmit        Write
	{CANTransmit_buf_ID    , {read_CANTransmit_buf_reg    , write_CANTransmit_buf_reg    , show_CANTransmit_buf_reg    }},   // Transmit_BUF    Write  len_12  base  offset
	{CANReceive_ID         , {read_CANReceive_reg         , write_CANReceive_reg         , show_CANReceive_reg         }},   //                 Read
	{CANReceive_buf_ID     , {read_CANReceive_buf_reg     , NULL     					 , show_CANReceive_buf_reg     }},   //  Receive_buf    Read   len_12  base  offset
	{CANWindow_ID          , {read_CANWindow_reg          , NULL          				 , show_CANWindow_reg          }},   //
	{CANACR_ID             , {read_CANACR_reg             , write_CANACR_reg             , show_CANACR_reg             }},   //                 len_12  base  offset
	{CANAMR_ID             , {read_CANAMR_reg             , write_CANAMR_reg             , show_CANAMR_reg             }},   //                 len_12  base  offset
	{CANRMC_ID             , {read_CANRMC_reg             , write_CANRMC_reg             , show_CANRMC_reg             }},   // RMC             Read only   Read only
	{CANRBSA_ID            , {read_CANRBSA_reg            , write_CANRBSA_reg            , show_CANRBSA_reg            }},   // RBSA            Read only  Read/Write
	{CANCDR_ID             , {read_CANCDR_reg             , write_CANCDR_reg             , show_CANCDR_reg             }},   // CDR             Read/Write  Read/Write
	{CANReceive_FIFO_ID    , {read_CANReceive_FIFO_reg    , write_CANReceive_FIFO_reg    , show_CANReceive_FIFO_reg    }},   // CDR             Read only  Read/Write
	{CANTransmit_Buffer_ID , {read_CANTransmit_Buffer_reg , write_CANTransmit_Buffer_reg , show_CANTransmit_Buffer_reg }},   // Transmit Buffer Read only  Read only
#endif
};




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
    can_handle_t *pcsi_can = csi_can_initialize(0 , NULL);
    drv_can_config_mode(pcsi_can, CAN_MODE_RESET);
    pcsi_can = csi_can_initialize(0 , NULL);
//    show_reg_CANMOD(pcsi_can);
    //show_all_reg_CANMOD(pcsi_can);
	show_all_r_reg_CANMOD(pcsi_can);
}




