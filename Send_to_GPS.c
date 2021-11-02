//*** SEND COMMANDS TO THE GPS MODULE (GPS CONSOLE)*******
// Copyright (c) 2021 Trenser Technology Solutions (P) Ltd
// All Rights Reserved
//*************************************************************************************
// File				: Send_to_GPS.c
// Summary	: send the Message queue to receiver.
// Author			: Abhilash
// Date			: 01/11/21
//******************************* Include Files **************************************
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "Send_to_GPS.h"
//******************************* Global Types **************************************

//***************************** Global Constants ************************************

//**************************** Local variables *****************************************
static STMESSAGEBUFFER stMessage = {0};
static key_t ikey;
static uint16 uiMessageId = FALSE;
static uint8 ucConvertedData[CONVERTED_BUFF_MAX_SIZE] = {0};
static uint8 *ucHelpCommand[] = {"Start\n","Loc\n","Fix\n","SatCon\n","Reset\n","Stop\n","quit\n"};
static STPARSE command_array[] ={  {GPS_START, GPS_START_VALUE},
																	{GPS_LOC, GPS_LOC_VALUE},
																	{GPS_FIX, GPS_FIX_VALUE},
																	{GPS_SATCON, GPS_SATCON_VALUE},
																	{GPS_RESET, GPS_RESET_VALUE},
																	{GPS_HELP, GPS_HELP_VALUE} ,};

//*************************** Enum Declarations ***************************************

//****************************** Local Functions ****************************************
static void ParseTheCommand();
static void BuffClear();
static void GenerateMessageKey();
static void SendDataToGps(uint16 uiMId, uint8 *ReceiveBuff);
static void CreateMessageQueue();
static void ProcessCommand(uint8 *ucValue);

//******************************.FUNCTION_HEADER.********************************
//Purpose	: Send the message to the Receiver
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//********************************************************************************************
int main()
{
	/* To generate unique key */
	GenerateMessageKey();
	/* Creates a message queue and returns identifier */
	CreateMessageQueue();

	while(1)
	{
		printf("GPS>");
		fgets(stMessage.ucMessageText, MAX_SIZE, stdin);
		ParseTheCommand();

		if(strcmp(ucConvertedData, "\0") == FALSE)
		{
			while(1)
			{
				break;
			}
		}
		else
		{
			SendDataToGps(uiMessageId, ucConvertedData);
		}

		if(strcmp(stMessage.ucMessageText, QUIT_STRING) == FALSE )
		{
			break;
		}
	}

	printf("Message Terminated\n");

	return 0;
}

//******************************.FUNCTION_HEADER.****************************
//Purpose	: Identifying the user command 
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//***************************************************************************************
void ParseTheCommand()
{
	uint16 uiIteration;
	uint16 uiBufferlength = strlen(*ucHelpCommand);

	for(uiIteration = 0 ; uiIteration < 7 ; uiIteration ++)
	{
		if(strcmp(stMessage.ucMessageText, command_array[uiIteration].ucCommands) == FALSE )
		{
			ProcessCommand(command_array[uiIteration].ucValue);
			break;
		}
	}
}

//******************************.FUNCTION_HEADER.*************************
//Purpose	: Clearing the "ucConvertedData" buffer
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//************************************************************************************
void BuffClear()
{
	memset(ucConvertedData, '\0', sizeof(ucConvertedData));
}

//******************************.FUNCTION_HEADER.**************************
//Purpose	: Send the Data to the gps
//Inputs		: uint16 uiMId, uint8 *ReceiveBuff
//Outputs	: None
//Return		: None
//Notes		: None
//************************************************************************************
void SendDataToGps(uint16 uiMId, uint8 *ReceiveBuff)
{
	/* msgsnd() to send message */
	if( (msgsnd(uiMId, &ReceiveBuff, sizeof(ReceiveBuff), MESSAGE_SND_FLAG) == -1))
	{
		printf(" Error happened !! \n");
	}
	else
	{
		printf("Data send is :%s\n", ReceiveBuff);
		printf("Message Sent.. \n");
		BuffClear();
	}
}

//******************************.FUNCTION_HEADER.***************************
//Purpose	: 
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//***************************************************************************************
void ProcessCommand(uint8 *ucValue)
{
	uint16 uiIteration;
	uint16 uiBufferlength = strlen(*ucHelpCommand);

	if(strcmp(ucValue, GPS_HELP_VALUE) == FALSE )
	{
		for( uiIteration = 0; uiIteration < uiBufferlength; uiIteration++)
		{
			printf("%s", ucHelpCommand[uiIteration]);
		}
	}
	else
	{
		strcpy(ucConvertedData, ucValue);
	}

}

//******************************.FUNCTION_HEADER.****************************
//Purpose	: Generate Message key using ftok() and store it into iKey
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//***************************************************************************************
void GenerateMessageKey()
{
	ikey = ftok("progfile", 65);
}

//******************************.FUNCTION_HEADER.****************************
//Purpose	: Create a Message Queue using msgget() and return msg id
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//***************************************************************************************
void CreateMessageQueue()
{
	uiMessageId = msgget(ikey, MESSAGE_GET_FLAG);
	stMessage.ulMessageType = TRUE;
}

//EOF