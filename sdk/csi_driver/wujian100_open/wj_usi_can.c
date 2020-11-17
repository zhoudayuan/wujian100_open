/**
  ******************************************************************************
  * @file    wj_usi_can.c
  * @author  zhoudy
  * @version 
  * @date    2020/11/15 15:36:26
  * @brief   This file provides firmware functions to manage the following 
  *          functionalities of the Controller area network (CAN) peripheral and 
  *          applicable only for RYAM001 devices :
  *           + Initialization and Configuration 
  *           + CAN Frames Transmission 
  *           + CAN Frames Reception    
  *           + Operation modes switch  
  *           + Error management          
  *           + Interrupts and flags        
  *         
  @verbatim



   ===============================================================================      
                      ##### How to use this driver #####
 ===============================================================================                
    [..]
    (#) Enable the CAN controller interface clock using 
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN, ENABLE);      
    (#) CAN pins configuration:
        (++) Enable the clock for the CAN GPIOs using the following function:
             RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOx, ENABLE);   
        (++) Connect the involved CAN pins to AF0 using the following function 
             GPIO_PinAFConfig(GPIOx, GPIO_PinSourcex, GPIO_AF_CANx); 
        (++) Configure these CAN pins in alternate function mode by calling
             the function  GPIO_Init();
    (#) Initialise and configure the CAN using CAN_Init() and 
        CAN_FilterInit() functions.   
    (#) Transmit the desired CAN frame using CAN_Transmit() function.
    (#) Check the transmission of a CAN frame using CAN_TransmitStatus() function.
    (#) Cancel the transmission of a CAN frame using CAN_CancelTransmit() function.  
    (#) Receive a CAN frame using CAN_Recieve() function.
    (#) Release the receive FIFOs using CAN_FIFORelease() function.
    (#) Return the number of pending received frames using CAN_MessagePending() function.            
    (#) To control CAN events you can use one of the following two methods:
        (++) Check on CAN flags using the CAN_GetFlagStatus() function.  
        (++) Use CAN interrupts through the function CAN_ITConfig() at initialization 
             phase and CAN_GetITStatus() function into interrupt routines to check 
             if the event has occurred or not.
             After checking on a flag you should clear it using CAN_ClearFlag()
             function. And after checking on an interrupt event you should clear it 
             using CAN_ClearITPendingBit() function.            
                 
  @endverbatim
  *       
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */


void wj_CAN_irqhandler(int idx)
{
  
}



/**
  \brief       
  \param[in]   
  \param[in]   
  \return      
*/
can_handle_t drv_usi_can_initialize(int32_t idx, usart_event_cb_t cb_event)
{
    uint32_t  base;
    uint32_t  irq;
    void     *handler;
    int32_t ret = target_usi_can_init(idx, &base, &irq, &handler);
    ret = drv_usi_initialize(idx);
    drv_irq_register(iic_priv->irq, handler);
    drv_irq_enable(iic_priv->irq);
    return iic_priv;
}

