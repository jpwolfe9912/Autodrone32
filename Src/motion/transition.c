/** @file 		transition.c
 *  @brief
 *  	This file allows for the transition between flight modes.
 *
 *  @author 	Jeremy Wolfe
 *  @date 		16 MAR 2022
 */

/* Includes */
#include "board.h"

/** @brief This runs all the functions necessary to convert between modes.
 *
 *  @return Void.
 */
void
modeTransition(void)
{
	systemReady = false;
	motor_initialized = false;


	wormDrive();

//	if(mode == TRANS_FLIGHT)
//	{
//		color(BLUE, YES);
//		printf("\nTransitioning to FLIGHT MODE");
//		colorDefault();
//		motorsChangeMode(DSHOT_CMD_3D_MODE_OFF, MOTORS_24);
//
//	}
//	else if(mode == TRANS_ROVER)
//	{
//		color(BLUE, YES);
//		printf("\nTransitioning to ROVER MODE");
//		colorDefault();
//		motorsChangeMode(DSHOT_CMD_3D_MODE_ON, MOTORS_24);
//	}

	printf("\nWaiting for arms...\n");
//	while(!(arm1Hit & arm2Hit));	// wait for both arms to hit

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
