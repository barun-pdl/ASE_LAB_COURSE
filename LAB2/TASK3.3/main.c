/* ********** DOCUMENTATION SECTION********************** 
This Code Performs the Following Functions
TASK3: SUBTASK 3: Show the working functionality of CAN Filter by toggling LED3
Created by Barun Pandhwal on 11/11/2018 	
*/   

#include "init.h"
int receive_id;

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    
    Init();						/* board initialization; CANBUS ALSO*/ 
    LED3 = 1; 			// KEEP THE LED3 OFF initially
	for(;;)
    {
		
	}
	
}

// This function clears the timer interrupt flag
 void PITCHANNEL0() {
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1; 
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
			CAN_0.IFRL.B.BUF04I = 1;
			CAN_0.IFRL.B.BUF05I = 1;
			CAN_0.IFRL.B.BUF06I = 1;
			CAN_0.IFRL.B.BUF07I = 1;
			LED3 = ~LED3 ;    // TOGGLE LED3 When we have some data in message buffer (AFTER ID FILTER)

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
