//-----------------------------------------------------------------------------
// main.c
//-----------------------------------------------------------------------------
// Copyright 2015 Silicon Laboratories, Inc.
// http://developer.silabs.com/legal/version/v11/Silicon_Labs_Software_License_Agreement.txt
//
// Program Description:
//
// This example demonstrates the available power modes on the EFM8BB3 STK using
// the LCD and push buttons.
//
//-----------------------------------------------------------------------------
// How To Test: EFM8BB3 STK
//-----------------------------------------------------------------------------
// 1) Place the SW104 switch in "AEM" mode.
// 2) Connect the EFM8BB3 STK board to a PC using a mini USB cable.
// 3) Compile and download code to the EFM8BB3 STK board.
//    In Simplicity Studio IDE, select Run -> Debug from the menu bar,
//    click the Debug button in the quick menu, or press F11.
// 4) Run the code.
//    In Simplicity Studio IDE, select Run -> Resume from the menu bar,
//    click the Resume button in the quick menu, or press F8.
// 5) The power modes example should start.
// 6) Press PB1 to cycle through the 6 different power modes shown on
//    the LCD.
// 7) Press PB0 to enter the selected power mode. The LED turns off.
//    For all modes except Shutdown, the display will show "*Active*" while
//    in the mode. For Shutdown mode the display is turned off.
// 8) Press PB0 (all modes except Stop and Shutdown) to return to the menu,
//    or press Reset (all modes) to restart the example. The LED turns on.
//
// Target:         EFM8BB3
// Tool chain:     Generic
//
// Release 0.1 (MD;AT)
//    - Initial Revision
//    - 1 APR 2016
//
//-----------------------------------------------------------------------------
// Resources:
//-----------------------------------------------------------------------------
// SYSCLK - 24.5 MHz HFOSC0 / 1
// SPI0   - 1 MHz
// Timer2 - 2 MHz (SPI CS delay)
// Timer3 - 1 kHz (1 ms tick)
// P2.6 - Display CS (Active High)
// P0.6 - SCK
// P1.0 - MOSI
// P1.4 - LED G
// P0.2 - Push Button 0
// P0.3 - Push Button 1
// P1.7 - Joystick (not used)
// P3.4 - Display enable
//
//-----------------------------------------------------------------------------
#include <SI_EFM8BB3_Register_Enums.h>
#include "bsp.h"
#include "pwr.h"

#include "draw.h"
#include "tick.h"
#include "InitDevice.h"
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

// Push button assignments
#define UI_BTN_MENU   (1 << 0)
#define UI_BTN_SELECT (1 << 1)


int toggle=0;
int toggle2=0;

// Read and return push button status
uint8_t UI_getButtons(void)
{
  uint8_t status = 0;
  if (BSP_PB0 == BSP_PB_PRESSED && toggle == 0) {
    status = 1;
    toggle=1;
  }
  return status;
}

int ms=0;
int sec=0;
int minute=0;
int hour=0;
// Port Match ISR - Triggered on leading edge of UI_BTN_SELECT.
SI_INTERRUPT (PMATCH_ISR, PMATCH_IRQn)
{
  // Turn on LED whenever UI_BTN_SELECT is pressed
	BSP_LED_G = BSP_LED_ON;
}

SI_INTERRUPT (TIMER3_ISR, TIMER3_IRQn)
{
    // Overflows every 1 ms
    TMR3CN0 &= ~TMR3CN0_TF3H__BMASK;
    if (ms<999){
    		  ms++;
    	  }
    	  else if(sec<59){
    		  ms=0;
    		  sec++;
    	  }
    	  else if(minute<59){
    		  sec=0;
    		  minute++;
    	  }
    	  else{
    		  minute=0;
    		  hour++;
    	  }
}


void main(void)
{

	int nxtline=8;
	int lap=0;

  // Initialize the device
  enter_DefaultMode_from_RESET();

  // Start with the LED on
  BSP_LED_G = BSP_LED_ON;

  // Enable all interrupts
  IE_EA = 1;

  if (BSP_PB1 == BSP_PB_PRESSED && toggle2 == 0) {
        toggle2=1;
  }

  // Mainloop
  while (toggle2)
  {
	  char strms[4];
	  char strsec[3];
	  char strmin[3];
	  char strhour[3];
	  char strtime[16];
	  sprintf(strhour, "%i", hour);
	  sprintf(strmin,"%i",minute);
	  sprintf(strsec, "%i", sec);
	  sprintf(strms,"%i",ms);
	  DrawScreenText("Swag Timer",0);
	  strcpy(strtime, strhour);
	  strcat(strtime,":");
	  strcat(strtime,strmin);
	  strcat(strtime,":");
	  strcat(strtime,strsec);
	  strcat(strtime,":");
	  strcat(strtime,strms);

	  DrawScreenText(strtime,8);
	  TMR3CN0 &= ~TMR3CN0_TF3H__BMASK;


	  if (UI_getButtons() == 1) {
		  char strlap[3]="";
		  char lapname[19]="";
		  nxtline=nxtline+8;
		  lap++;
		  sprintf(strlap,"%i",lap);
		  strcat(lapname, "Lap ");
		  strcat(lapname, strlap);
		  strcat(lapname,": "
				  "");
		  strcat(lapname,strtime);
		  DrawScreenText(lapname,nxtline);
		  if (nxtline==120){
			  nxtline=8;
		  }

	  }
	  if (BSP_PB0 == !BSP_PB_PRESSED && toggle == 1) {
	  			  toggle=0;
	  }
	  if (BSP_PB1 == BSP_PB_PRESSED && toggle2 == 1) {
	  	  		  toggle2=0;
	  }
  }
}
