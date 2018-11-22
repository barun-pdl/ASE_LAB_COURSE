/* ********** DOCUMENTATION SECTION********************** 
This Code Performs the Following Functions
TASK1: Prepare the timer function and indicate the correct timer function through a toggling LED0
Hints: 1) Clear Interrupt Flag after processing the timer interrupt ==> (Done)
	   2) IRQ number => 59 for PIT0 
	   3) Ext_Isr() to implement a function call (IRQ Number) ==> (Done)
	   4) Use LAB1 interrupt handlers : 1) void PIT_ConfigureTimer( int timerChannel, unsigned int loadValue )
										2) PIT.CH[ int timerChannel ].TFLG.B.TIF 
Created by Barun Pandhwal on 04/11/2018 	
*/   
// ***************PENDING SIU.GPDO[9].R = ~SIU.GPDO[9].R ; // Toggle the LED0 POSITION CHECK******************
// ***************PENDING SIU.GPDO[9].R AS LED0***************************************************************


#include "init.h"

// Declare Global Functions

void PIT_ConfigureTimer( int timerChannel, unsigned int time );

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    
    Init();						/* board initialization */
	PIT_ConfigureTimer(0,1000); // Timer Configuration: Channel 0, 1 second delay
	LED0 = 1; // Initially Turn OFF LED0
    /* forever */
    for(;;)
    {
	PIT_StartTimer(0); //************** WHAT HAPPENS IF StartTimer triggers again before interrupt
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
			LED0 = ~LED0;		 // Toggle the LED0
            PIT_StopTimer(0);      // To avoid the weird behaviour of Timer
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
