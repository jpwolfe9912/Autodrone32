/** @file 		transition.c
 *  @brief
 *  	This file allows for the transition between flight modes.
 *
 *  @author 	Jeremy Wolfe
 *  @date 		16 MAR 2022
 */

/* Includes */
#include "board.h"

/*
 * PA8 - Worm 1 PWM
 * PA9 - Worm 2 PWM
 * PC8 - Worm 1 Dir
 * PC9 - Worm 2 Dir
 *
 * */

void
modeTransition(void)
{
	systemReady = false;
	motor_initialized = false;


	wormDrive();

	if(mode == TRANS_FLIGHT)
	{
		color(BLUE, YES);
		printf("\nTransitioning to FLIGHT MODE");
		colorDefault();
		motors3dOff();
	}
	else if(mode == TRANS_ROVER)
	{
		color(BLUE, YES);
		printf("\nTransitioning to ROVER MODE");
		colorDefault();
		motors3dOn();
	}

	printf("\nWaiting for arms...\n");
	while(!(arm1Hit & arm2Hit));	// wait for both arms to hit

	color(GREEN, YES);
	printf("\nArms have reached limits\n");
	colorDefault();

	if(mode == TRANS_FLIGHT)
	{
		mode = FLIGHT;
		printf("Transition to FLIGHT mode complete\n");
	}
	else if(mode == TRANS_ROVER)
	{
		mode = ROVER;
		printf("Transition to ROVER mode complete\n");
	}
	arm1Hit = false;
	arm2Hit = false;

	systemReady = true;
	motor_initialized = true;

}
