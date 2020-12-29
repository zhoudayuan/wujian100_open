#ifndef _WJ_USI_CAN_H_
#define _WJ_USI_CAN_H_

#include "wj_usi.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE    1
#endif
#ifndef FALSE
#define FALSE   0
#endif


#define CAN_BIT_SLEEP_MODE                (1UL<<4)
#define CAN_BIT_ACCEPTANCE_FILTER_MODE    (1UL<<3)
#define CAN_BIT_SELF_TEST_MODE            (1UL<<2)
#define CAN_BIT_LISTEN_ONLY_MODE          (1UL<<1)
#define CAN_BIT_RESET_MODE                (1UL<<0)



//AM:CANSR
#define  CAN_SR_TRANSMIT_BUFFER_RELEASED        (0x1UL<<2)
#define  CAN_SR_TRANSMIT_BUFFER_LOCKED          (0x1UL<<2)


//CLOCK DIVIDER REGISTER (CDR):  ADDRESS 1Fh
#define  CAN_CLKOUT_DISABLE        (1UL<<3)  // Setting this bit allows the external CLKOUT signal to be disabled.
#define  CAN_CLKOUT_ENABLE         (0UL<<3)
#define  CAN_FCLK_OSC_DIVIDED_2    0x0
#define  CAN_FCLK_OSC_DIVIDED_4    0x1
#define  CAN_FCLK_OSC_DIVIDED_6    0x2
#define  CAN_FCLK_OSC_DIVIDED_8    0x3
#define  CAN_FCLK_OSC_DIVIDED_10   0x4
#define  CAN_FCLK_OSC_DIVIDED_12   0x5
#define  CAN_FCLK_OSC_DIVIDED_14   0x6
#define  CAN_FCLK_OSC_DIVIDED_0    0x7

//AM:CANOCR PG:OCR
#define CAN_NORMAL_OUTPUT_MODE                  0x02
#define CAN_CLOCK_OUTPUT_MODE                   0x03

//AM:CANIER
#define CAN_BUS_ERROR_INTERRUPT          (7)
#define CAN_ARBITRATION_LOST_INTERRUPT   (6)
#define CAN_ERROR_PASSIVE_INTERRUPT      (5)
#define CAN_WAKE_UP_INTERRUPT            (4)
#define CAN_DATA_OVERRUN_INTERRUPT       (3)
#define CAN_ERROR_WARNING_INTERRUPT      (2)
#define CAN_TRANSMIT_INTERRUPT           (1)
#define CAN_RECEIVE_INTERRUPT            (0)




typedef struct {
    __IOM uint8_t  CANMOD;                //Offset 0x00 MOD                    Read/Write  Read/Write
    __OM  uint8_t  CANCMR;                //Offset 0x01 CMR                    Write only  Write only
    __IM  uint8_t  CANSR;                 //Offset 0x02 SR                     Read only   Read only
    __IM  uint8_t  CANIR;                 //Offset 0x03 IR                     Read only   Read only
    __IOM uint8_t  CANIER;                //Offset 0x04 IER                    Read/Write  Read/Write
    __IOM uint8_t  Reserved0;             //Offset 0x05 Reserved0              N/A         N/A
    __IM  uint8_t  CANBTR0;               //Offset 0x06 BTR0                   Read only   Read/Write
    __IM  uint8_t  CANBTR1;               //Offset 0x07 BTR1                   Read only   Read/Write
    __IOM uint8_t  CANOCR;                //Offset 0x08 OCR                    Read only   Read/Write
    __IOM uint8_t  Reserved1;             //Offset 0x09 Reserved1              N/A         N/A
    __IOM uint8_t  Reserved2;             //Offset 0x0A Reserved2              N/A         N/A
    __IM  uint8_t  CANALC;                //Offset 0x0B ALC                    Read only   Read only
    __IM  uint8_t  CANECC;                //Offset 0x0C ECC                    Read only   Read only
    __IM  uint8_t  CANTEWLR;              //Offset 0x0D EWLR                   Read only   Read/Write
    __IM  uint8_t  CANRXERR;              //Offset 0x0E RXERR                  Read only   Read/Write
    __IM  uint8_t  CANTXERR;              //Offset 0x0F TXERR                  Read only   Read/Write
    
    union _U{
        struct _T {
            __OM  uint8_t  CANTransmit;              //Offset 0x10 Transmit               Write
            __OM  uint8_t  CANTransmit_buf[0x0C];    //Offset 0x11-1C Transmit_BUF        Write
        }T;
        struct _R {
            __IM  uint8_t  CANReceive;               //Offset 0x10                        Read
            __IM  uint8_t  CANReceive_buf[0x0C];     //Offset 0x11-0x1C Receive_buf       Read
            __IOM uint8_t  CANWindow[0x0C];          //Offset 0x11-0x1C
        }R;
        struct _A {        
            __IOM uint8_t  CANACR[4];                //Offset 0X10-0x13
            __IOM uint8_t  CANAMR[4];                //Offset 0x14-0x17
        } A;
    }U;
    __IM  uint8_t  CANRMC;                   //Offset 0x1D RMC                    Read only   Read only
    __IM  uint8_t  CANRBSA;                  //Offset 0x1E RBSA                   Read only  Read/Write
    __IOM uint8_t  CANCDR;                   //Offset 0x1F CDR                    Read/Write  Read/Write
    __IM  uint8_t  CANReceive_FIFO[0x40];    //Offset 0x20-5F CDR                 Read only  Read/Write
    __IM  uint8_t  CANTransmit_Buffer[0x0d]; //Offset 0x60-0x6C Transmit Buffer   Read only  Read only
    __IOM uint8_t  CANReserved3[0x13];       //Offset 0x6D-0x7F Transmit Buffer   N/A        N/A
} wj_can_reg_t;

#define get_can_reg_addr_base(_handle)  ((wj_can_reg_t *)(((wj_can_priv_t *)_handle)->base))


#if 0
typedef struct {
    __IOM uint32_t   CANMOD;                    // Offset 0x00 controller MODe setting register
    __IOM uint32_t   CANCMR;                    // Offset 0x04 CAN controller ComMand Register.
    __IOM uint32_t   CANSR;                     // Offset 0x08
    __IOM uint32_t   CANIR;                     // Offset 0x0C
    __IOM uint32_t   CANIER;                    // Offset 0x10
    __IOM uint32_t   CANBTR0;                   // Offset 0x14
    __IOM uint32_t   CANBTR1;                   // Offset 0x18
    __IOM uint32_t   CANOCR;                    // Offset 0x1C
    __IOM uint32_t   CANALC;                    // Offset 0x20
    __IOM uint32_t   CANECC;                    // Offset 0x24
    __IOM uint32_t   CANEWLR;                   // Offset 0x28
    __IOM uint32_t   CANRXERR;                  // Offset 0x2C
    __IOM uint32_t   CANTXERR;                  // Offset 0x30
    __IOM uint32_t   CANTRANSMIT_BUFFER_W0;     // Offset 0x40
    __IOM uint32_t   CANTRANSMIT_BUFFER_W1;     // Offset 0x44
    __IOM uint32_t   CANTRANSMIT_BUFFER_W2;     // Offset 0x48
    __IOM uint32_t   CANTRANSMIT_BUFFER_W3;     // Offset 0x4C
    __IOM uint32_t   CANTRANSMIT_BUFFER_R0;     // Offset 0x50
    __IOM uint32_t   CANTRANSMIT_BUFFER_R1;     // Offset 0x54
    __IOM uint32_t   CANTRANSMIT_BUFFER_R2;     // Offset 0x58
    __IOM uint32_t   CANTRANSMIT_BUFFER_R3;     // Offset 0x5C
    __IOM uint32_t   CANRECEIVE_BUFFER_R0;      // Offset 0x40  // RO
    __IOM uint32_t   CANRECEIVE_BUFFER_R1;      // Offset 0x44  // RO
    __IOM uint32_t   CANRECEIVE_BUFFER_R2;      // Offset 0x48  // RO
    __IOM uint32_t   CANRECEIVE_BUFFER_R3;      // Offset 0x4C  // RO
    //---------------//
    __IOM uint32_t   CANACR0;
    __IOM uint32_t   CANACR1;
    __IOM uint32_t   CANACR2;
    __IOM uint32_t   CANACR3;
    __IOM uint32_t   CANAMR0;
    __IOM uint32_t   CANAMR1;
    __IOM uint32_t   CANAMR2;
    __IOM uint32_t   CANAMR3;
    //---------------//
    __IOM uint32_t   CANRMC;                    // Offset 0x64
    __IOM uint32_t   CANRBSA;                   // Offset 0x68
    __IOM uint32_t   CANCDR;                    // Clock Divider Register (CDR): Address 1Fh
} wj_can_reg_t;

#endif


#define CANMOD_ID                    0         // MOD             Read/Write  Read/Write            
#define CANCMR_ID                    1         // CMR             Write only  Write only            
#define CANSR_ID                     2         // SR              Read only   Read only             
#define CANIR_ID                     3         // IR              Read only   Read only             
#define CANIER_ID                    4         // IER             Read/Write  Read/Write            
#define CANBTR0_ID                   5         // BTR0            Read only   Read/Write            
#define CANBTR1_ID                   6         // BTR1            Read only   Read/Write            
#define CANOCR_ID                    7         // OCR             Read only   Read/Write            
#define CANALC_ID                    8         // ALC             Read only   Read only             
#define CANECC_ID                    9          // ECC             Read only   Read only            
#define CANTEWLR_ID                  10         // EWLR            Read only   Read/Write           
#define CANRXERR_ID                  11         // RXERR           Read only   Read/Write           
#define CANTXERR_ID                  12         // TXERR           Read only   Read/Write           
#define CANTransmit_ID               13         // Transmit        Write                            
#define CANTransmit_buf_ID           14         // Transmit_BUF    Write  len_12  base  offset      
#define CANReceive_ID                15         //                 Read                             
#define CANReceive_buf_ID            16         //  Receive_buf    Read   len_12  base  offset      
#define CANWindow_ID                 17         //                                                  
#define CANACR_ID                    18         //                 len_12  base  offset             
#define CANAMR_ID                    19         //                 len_12  base  offset             
#define CANRMC_ID                    20         // RMC             Read only   Read only            
#define CANRBSA_ID                   21         // RBSA            Read only  Read/Write            
#define CANCDR_ID                    22         // CDR             Read/Write  Read/Write           
#define CANReceive_FIFO_ID           23         // CDR             Read only  Read/Write            
#define CANTransmit_Buffer_ID        24         // Transmit Buffer Read only  Read only             

typedef enum {
    CAN_READ,
    CAN_WRITE,
    CAN_SHOW
} can_op_e;


typedef int32_t  (*op_can_f)(can_handle_t handle, uint32_t base, uint32_t offset);

typedef struct _t_can_reg_op {
	uint32_t    id;
	op_can_f    can_fun[3];
} t_can_reg_op;


int32_t read_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset);
int32_t write_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset);
int32_t show_CANMOD_reg(can_handle_t handle, uint32_t base, uint32_t offset);





typedef void (*can_event_cb_t)(int32_t idx, can_event_e event);   ///< Pointer to \ref can_event_cb_t : USART Event call back.
can_handle_t csi_can_initialize(int32_t idx, can_event_cb_t cb_event);
int32_t drv_can_config_clock(can_handle_t handle, uint32_t fclk_osc_enable, uint32_t fclk_osc);
int32_t drv_can_config_OCR(can_handle_t handle, uint32_t output_cfg);
int32_t drv_can_config_IER_enable(can_handle_t handle, uint32_t interrupt_enable);
int32_t drv_can_config_IER_disable(can_handle_t handle, uint32_t interrupt_disable);
//int32_t drv_can_config_acceptance_filters(can_handle_t handle, uint32_t mode, uint32_t acr, uint32_t amr);
void show_all_r_reg_CANMOD(can_handle_t handle);



#ifdef __cplusplus
}
#endif

#endif /* _WJ_USI_USART_H_ */
