/*************************************************************************************************
*
 The goal of this program is to program the ECU for the following tasks:
1) Interface Light Sensor: Value to be sent on to the bus after every 200ms
2) Interface Door Sensors: Two switches SW3 and SW4 (For Left and Right Door). If value = 0 : Opened
																	  value = 1 : Closed
3) It Receives value from ECU3 about the Fuel Level and ECU4 about the Engine Coolant Temperature
4) If Fuel level is less than 0.1 of max. ==>
					Send Warning Signal (0) on the bus on Bit0  (0= 0N); (1: OFF) after 400ms
   If Coolant Temperature is greater than 0.9 of max. ==>
					Send Warning Signal (0) on the bus on Bit0 (0= ON); (1:OFF) after 400ms
5) Check if the ECU1 is Present or not : 
	If not, Continuously blink the LED7 and send the error signal both with an interval of 200ms

	
	//******* PENDING TO SEND ALIVE SIGNAL AND ERROR SIGNAL using BUFFER 4 and 5

	
Created By: BARUN PANDHWAL on 22/11/2018                       *
*****************************************************************************************************/

#include "init.h"
/********************************************************************
*                          global variables                         *
********************************************************************/
int light_sense;			// SENSED VALUE VARIABLE
int door_sense_right;		// SENSED VALUE VARIABLE
int door_sense_left;		// SENSED VALUE VARIABLE
int fuel_recv_1byte;		// RECEIVED VALUE VARIABLE
int fuel_recv_2byte;		// RECEIVED VALUE VARIABLE
int coolant_recv_1byte;		// RECEIVED VALUE VARIABLE
int coolant_recv_2byte;		// RECEIVED VALUE VARIABLE
int receive_id; 
/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
    /* turn off leds */
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1; 

	/* TWO TIMER CHANNELS ARE CONFIGURED */	
	PIT_ConfigureTimer(0,200); // Timer Configuration: Channel 0, 200 milisecond delay
    PIT_ConfigureTimer(1,400); // Timer Configuration: Channel 1, 400 milisecond delay
	PIT_StartTimer(0);
	PIT_StartTimer(1);
	/* forever */
    for(;;)
    {
     light_sense = ADC_0.CDR[2].B.CDATA; // Measure the light sensor value
	 door_sense_right = SW3;
	 door_sense_left = SW4;	
    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 ********************************************************************/  

void PITCHANNEL0(void)
{
    /* After 200 ms Perform the following */
    PIT.CH[0].TFLG.B.TIF = 1;
	/* CONFIGURE BUFFER 0 to Send Light Sensor Data */
	CAN_0.BUF[0].DATA.B[0] = light_sense; 
	CAN_0.BUF[0].DATA.B[1] = (light_sense >> 8) & 0x03; // To display the 9th and 10th bit
	CAN_0.BUF[0].CS.B.CODE = 8 ; 
	
	/* CONFIGURE BUFFER 1 to Send Door Sensor Data */
	// Assigning only 2 bits of CAN Buffer for message sending
	CAN_0.BUF[1].DATA.B[0] = ((door_sense_left & 0x01) | (door_sense_right & 0x02));
	CAN_0.BUF[1].DATA.B[1] = 0x00; // To display NULL
	CAN_0.BUF[1].CS.B.CODE = 8 ; 
	
	//******* PENDING TO SEND ALIVE SIGNAL AND ERROR SIGNAL using BUFFER 4 and 5
	
	
	/* CONFIGURE BUFFER 4 to Send ALIVE SIGNAL 
	CAN_0.BUF[4].DATA.B[0] = 1; // **********************CHECKKK***********
	CAN_0.BUF[4].DATA.B[1] = 0x00; // To display NULL
	CAN_0.BUF[4].CS.B.CODE = 8 ; 
	
	*/
	/* CONFIGURE BUFFER 5 to Send Error Signal
	CAN_0.BUF[5].DATA.B[0] = 1; // **********************CHECKKK***********
	CAN_0.BUF[5].DATA.B[1] = 0x00; // To display NULL
	CAN_0.BUF[5].CS.B.CODE = 8 ; 
	*/
	
	
	}

void PITCHANNEL1(void)
{
    /* After 400 ms Perform the following */
    PIT.CH[1].TFLG.B.TIF = 1;

	if ((fuel_recv_2byte = 0) & (fuel_recv_1byte < 0x66)) // if fuel is less than 102 i.e. 1/10th of max
	{
		CAN_0.BUF[2].DATA.B[0] = 0; //FUEL WARNING ON
		CAN_0.BUF[2].DATA.B[1] = 0x00; // To display NULL
	}
	else 
	{
		CAN_0.BUF[2].DATA.B[0] = 1; // FUEL WARNING OFF
		CAN_0.BUF[2].DATA.B[1] = 0x00; // To display NULL
	}
	/* CONFIGURE BUFFER 2 to Send FUEL LEVEL WARNING */
	CAN_0.BUF[2].CS.B.CODE = 8 ; 
	
	
	
	if ((coolant_recv_2byte > 1) & (coolant_recv_1byte > 0x99) // if temperature is greater than 922 i.e. 9/10th of max
	{
		CAN_0.BUF[3].DATA.B[0] = 0; //ENGINE WARNING ON
		CAN_0.BUF[3].DATA.B[1] = 0x00; // To display NULL
	}
	else 
	{
		CAN_0.BUF[3].DATA.B[0] = 1; // ENGINE WARNING OFF
		CAN_0.BUF[3].DATA.B[1] = 0x00; // To display NULL
	}
	
	/* CONFIGURE BUFFER 3 to ENGINE COOLING WARNING */
	    CAN_0.BUF[3].CS.B.CODE = 8 ; 
}
	
void CANMB0003(void)
{
/* No modifications needed here */
/* Receive interrupts are being cleared here */
    CAN_0.IFRL.B.BUF00I = 1;
    CAN_0.IFRL.B.BUF01I = 1;
    CAN_0.IFRL.B.BUF02I = 1;
    CAN_0.IFRL.B.BUF03I = 1;
}

void CANMB0407(void)
{
    /********************************************************************
    *                    _____  ___  ___   ___                          *
    *                   |_   _|/ _ \|   \ / _ \                         *
    *                     | | | (_) | |) | (_) |                        *
    *                     |_|  \___/|___/ \___/                         *
    *                                                                   *
    * CAN reception is handled here                                     *
    * The following buffers are important:                              *
    * CAN_0.RXFIFO.ID.B.STD_ID: ID of received message                  *
    * CAN_0.RXFIFO.DATA.B[i]: value of data byte 'i'                    *
    * IMPORTANT: check for the flag in CAN_0.IFRL.B.BUF05I first!       *
    ********************************************************************/  
   
    if (CAN_0.IFRL.B.BUF05I == 1) // Check for received frame 
	{
	receive_id =  CAN_0.RXFIFO.ID.B.STD_ID; // TO CHECK THE MESSAGE ID
	if (receive_id == 0x103)
	{
		fuel_recv_1byte = CAN_0.RXFIFO.DATA.B[0];
		fuel_recv_2byte = CAN_0.RXFIFO.DATA.B[1];	
	}
	else if (receive_id == 0x104)
	{
		coolant_recv_1byte = CAN_0.RXFIFO.DATA.B[0];
		coolant_recv_2byte = CAN_0.RXFIFO.DATA.B[1];	
	}
	
    /* clear flags as last step here! */
    /* don't change anything below! */
	
	
	
    CAN_0.IFRL.B.BUF04I = 1;
    CAN_0.IFRL.B.BUF05I = 1;
    CAN_0.IFRL.B.BUF06I = 1;
    CAN_0.IFRL.B.BUF07I = 1;
}

void CANMB0811(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */

    CAN_0.IFRL.B.BUF08I = 1;
    CAN_0.IFRL.B.BUF09I = 1;
    CAN_0.IFRL.B.BUF10I = 1;
    CAN_0.IFRL.B.BUF11I = 1;
}

void CANMB1215(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */
    CAN_0.IFRL.B.BUF12I = 1;
    CAN_0.IFRL.B.BUF13I = 1;
    CAN_0.IFRL.B.BUF14I = 1;
    CAN_0.IFRL.B.BUF15I = 1;
}



/********************************************************************
 *                   Interrupt Vector Table                         *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            PITCHANNEL0();
            break;
        case 60:
            PITCHANNEL1();
        case 68:
            CANMB0003();
            break;
        case 69:
            CANMB0407();
            break;
        case 70:
            CANMB0811();
            break;
        case 71:
            CANMB1215();
            break;        
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
