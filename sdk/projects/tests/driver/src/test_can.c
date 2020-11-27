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

static void test_can_interfaces(void)
{
    can_handle_t pcsi_usart;
	test_pin_can_init();
    pcsi_usart = csi_can_initialize(0 , NULL);
    ASSERT_TRUE(pcsi_usart != NULL);
}


int test_can(void)
{
    dtest_suite_info_t test_suite_info = {
        "test_can", NULL, NULL, NULL, NULL
    };
    dtest_suite_t *test_suite = dtest_add_suite(&test_suite_info);
    dtest_case_info_t test_case_info_array[] = {
//		{"test_can_rw_reg",      test_can_rw_reg,     CAN_TEST_REG_EN},
//        { "test_can_fun",        test_can_fun,        CAN_TEST_FUN_EN },
        { "test_can_interfaces", test_can_interfaces, CAN_TEST_INTERFACE_EN },
        { NULL, NULL }
    };
    dtest_add_cases(test_suite, test_case_info_array);
    return 0;
}

