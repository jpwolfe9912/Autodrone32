/** @file 		ibus.c
 *  @brief
 *  	This file initializes the ibus receiver protocol
 *
 *
 *  @author 	Jeremy Wolfe
 *  @date 		23 FEB 2022
 */

#include "board.h"

/* Static Function Prototypes */
static ibusStatus_e ibus_process_frame(void);
static void ibus_update(uint8_t* pData);
static bool ibus_frame_crc(uint8_t *pData);

/* Global Variables */
uint8_t ibusPayload[PAYLOAD_SIZE];	// payload buffer
uint8_t ibusCRC[CRC_SIZE];
uint16_t ibusChannels[RC_CHANNELS];

uint8_t state;
uint8_t frameLength, devID;

bool rcActive = false;

ibusStatus_e status;

/* Static Variables */
static uint8_t framePos = 0;
static uint8_t payloadPos = 0;
static uint8_t crcPos = 0;

static uint8_t init_error = 0;

/** @brief Initializes ibus ring buffer and low level usart registers.
 *
 *	@return bool True if initialization was successful.
 *				 False if failed.
 */
bool
ibusInit(void)
{
	bool ibus_initialized = false;

	printf("\nInitializing iBus Receiver\n");

	usart1Read(rx1Buf, RX1BUF_SIZE);


	status = IBUS_ERROR;

	for(uint8_t i = 0; i < 10; i++){
		ibusProcess();
		if(devID == 0x40){
			ibus_initialized = true;
			rcActive = true;
			break;
		}
		delay(5);
	}

	if(!ibus_initialized){
		color(RED, YES);
		printf("\niBus Initialization Failed. Try again?\n");
		if(init_error++ > 10)
		{
			printf("\nToo many failed attempts\n");
			color(WHITE, NO);
			return true;
		}
		color(WHITE, NO);
		return false;
#ifdef STLINK
		if(serialWaitFor('y'))
			return false;
		else
			return true;
#endif
	}
	else{
		color(GREEN, YES);
		printf("\niBus receiver recognized\n");
		color(WHITE, NO);
		ibusProcess();
		return true;
	}
	return true;
}

/** @brief Waits for the parser to not be busy
 *
 *  @return Void.
 */
void
ibusProcess(void)
{
	while(ibus_process_frame() == IBUS_BUSY);
}

/** @brief State machine that evaluates the raw data based on what section it is parsing through.
 *
 *  @return ibusStatus_e The status of the function.
 *  	READY, BUSY, ERROR
 */
static ibusStatus_e
ibus_process_frame(void)
{
	uint8_t b = rx1Buf[framePos++];

	status = IBUS_BUSY;
	switch(state){
	case IBUS_STATE_LENGTH: {				// length byte
		if(b == 0x20){

			frameLength = b;
			state++;
		}
		else
		{
			status = IBUS_ERROR;
		}
		break;
	}
	case IBUS_STATE_TYPE: {					// type byte
		if(b == 0x40){
			devID = b;
			state++;
		}
		else
			status = IBUS_ERROR;
		break;
	}
	case IBUS_STATE_PAYLOAD: {				// payload bytes
		ibusPayload[payloadPos++] = b;
		if(payloadPos == PAYLOAD_SIZE)
		{
			ibus_update(ibusPayload);
			state++;
			payloadPos = 0;
		}
		break;
	}
	case IBUS_STATE_CRC: {					// crc bytes
		ibusCRC[crcPos++] = b;
		if(crcPos == CRC_SIZE){
			if(ibus_frame_crc(ibusCRC))
			{
				status = IBUS_READY;
			}
			else{
				status = IBUS_ERROR;
			}
			crcPos = 0;
		}
		break;
	}
	}
	if(status == IBUS_ERROR)
	{
		framePos = 0;
		state = 0;
	}
	if(status == IBUS_READY)
	{
		framePos = 0;
		state = 0;
	}

	return status;
}

/** @brief Packs the raw data stored in two bytes in an array.
 * 	into one 16 bit integer.
 *
 *  @param *pData A pointer to the raw data array that we want.
 *  to pack.
 *  @return Void.
 */
static void
ibus_update(uint8_t *pData)
{
	for(uint8_t ch_index = 0, bf_index = 0; ch_index < RC_CHANNELS; ch_index++, bf_index += 2)
	{
		ibusChannels[ch_index] = pData[bf_index + 1] << 8 | pData[bf_index];
	}
}

/** @brief Checksum calculator for incoming data.
 *
 * 		This function calculates the checksum based on the incoming
 * 		data. It then compares that value with the checksum value
 * 		in the packet.
 *
 *  @return bool True if it matches.
 *  			 False if it does not match.
 */
static bool
ibus_frame_crc(uint8_t *pData)
{
	uint16_t checksum_cal = 0xFFFF;
	uint16_t checksum_ibus;

	for(uint8_t i = 0; i < 30; i++)
	{
		checksum_cal -= rx1Buf[i];
	}

	checksum_ibus = pData[1] << 8 | pData[0]; // checksum value from ibus

	return (checksum_ibus == checksum_cal);
}
