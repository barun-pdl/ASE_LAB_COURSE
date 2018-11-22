/* ********** DOCUMENTATION SECTION********************** 
This Code Performs the Following Functions
TASK3: SUBTASK 2: When we receive a message with ID 1, 
LED2 should turn ON and start sending the message from Task2.3
Created by Barun Pandhwal on 11/11/2018 	
*/   

#include "init.h"
// Declare Global Variable
int speed = 0;
int exit_loop = 0; // 
int state = 0; // DETERMINES INCREMENT (0) OR DECREMENT (1) SPEED
int receive_id;
/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    
    Init();						/* board initialization; CANBUS ALSO*/ 
    PIT_ConfigureTimer(0,200); // Timer Configuration: Channel 0, 200 milisecond delay
	LED0 = 1; 			// KEEP THE LED0 OFF initially
	LED1 = 1; 			// KEEP THE LED1 OFF initially
	LED2 = 1; 			// KEEP THE LED2 OFF initially
  //  PIT_StartTimer(0);
	for(;;)
    {
		
	}
	
}

// This function clears the timer interrupt flag

void PITCHANNEL0() {
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1; 
	//LED0 = ~LED0;
	CAN_0.BUF[0].DATA.B[0] = speed; 
	CAN_0.BUF[0].DATA.B[1] = (speed >> 8) & 0x01;  // To Display the 9th Bit
    CAN_0.BUF[0].CS.B.CODE = 0xC ;

	if (speed >= 0)
	{
		if (speed == 300)
		{
			state = 1; // DECREMENT STATE
		}
		if (state == 0) // INCREMENT STATE
		{
			speed += 5;
            LED1 = ~LED1;
		}
		else   // IF STATE = 1 ==> DECREMENT
		{
			if (speed > 0)
            {		
                speed -= 5;
                LED2 = ~LED2;
            }
            else if (speed == 0)
			state = 0;   
		}
	}	
	//PIT_StopTimer(0);
}

#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
	// Execute this when TImer 0 is used to interrupt
    //59 is the IRQ number for PIT0
	//68 is the IRQ number for BUF 00 to 03
	//69 is the IRQ number for BUF 04 to 07
	//70 is the IRQ number for BUF 08 to 11
	//71 is the IRQ number for BUF 12 to 15
	case 59:					 
            /* Timer Interrupt */
            PITCHANNEL0();
            break;
    case 68: // CLEARING THE INTERRUPTS FLAGS FOR MESSAGE BUFFER
			CAN_0.IFRL.B.BUF00I = 1;
			CAN_0.IFRL.B.BUF01I = 1;
			CAN_0.IFRL.B.BUF02I = 1;
			CAN_0.IFRL.B.BUF03I = 1;
			break;
	case 69:// CLEARING THE INTERRUPTS FLAGS FOR MESSAGE BUFFER
			receive_id = CAN_0.RXFIFO.ID.B.STD_ID ;
			if (receive_id == 0x1) 
			{
				LED2 = 0 ; // TURN ON THE LED2
				PIT_StartTimer(0); 
			}
			CAN_0.IFRL.B.BUF04I = 1;
			CAN_0.IFRL.B.BUF05I = 1;
			CAN_0.IFRL.B.BUF06I = 1;
			CAN_0.IFRL.B.BUF07I = 1;
			break;
	case 70:// CLEARING THE INTERRUPTS FLAGS FOR MESSAGE BUFFER
			CAN_0.IFRL.B.BUF08I = 1;
			CAN_0.IFRL.B.BUF09I = 1;
			CAN_0.IFRL.B.BUF10I = 1;
			CAN_0.IFRL.B.BUF11I = 1;
			break;
	case 71:// CLEARING THE INTERRUPTS FLAGS FOR MESSAGE BUFFER
			CAN_0.IFRL.B.BUF12I = 1;
			CAN_0.IFRL.B.BUF13I = 1;
			CAN_0.IFRL.B.BUF14I = 1;
			CAN_0.IFRL.B.BUF15I = 1;
			break;
		default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
