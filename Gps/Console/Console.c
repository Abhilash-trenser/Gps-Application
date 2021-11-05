//*********************** SEND COMMANDS TO THE GPS MODULE (GPS CONSOLE) ****************************
// Copyright (c) 2021 Trenser Technology Solutions (P) Ltd
// All Rights Reserved
//**************************************************************************************************
// File			: Console.c
// Summary		: send the Message queue to receiver.
// Author		: Abhilash
// Date			: 01/11/21
//******************************************* Include Files ****************************************
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "Common.h"
//****************************************** Global Types ******************************************

//****************************************** Global Constants **************************************

//****************************************** Local Functions ***************************************
static void BuffClear();
static void GenerateMessageKey();
static void CreateMessageQueue();
static void ConsoleParseCommand();
static void ConsoleParseSendData();
static void ConsoleStartCommand(uint8* pucValue);
static void ConsoleProcessCommand(uint8* pucValue);
static void ConsoleSendData(uint16 uiMId, uint8* pucReceiveBuff);

//****************************************** Local variables ***************************************
static STMESSAGEBUFFER stMessage = {0};
static key_t ikey = 0;
static uint16 usMessageId = FALSE;
static uint8 pucConvertedData[CONVERTED_BUFF_MAX_SIZE] = {0};
static uint8 pucUserEnteredData[MAX_SIZE] = {0};
static STPARSE CommandTable[] = {	{GPS_START,		GPS_START_VALUE	},
												{GPS_LOC,		GPS_LOC_VALUE		},
												{GPS_FIX,		GPS_FIX_VALUE		},
												{GPS_SATCON,	GPS_SATCON_VALUE	},
												{GPS_RESET,		GPS_RESET_VALUE	},
												{GPS_HELP,		GPS_HELP_VALUE		},
												{GPS_QUIT,		GPS_QUIT_VALUE		}
											};

//*************************************** Enum Declarations ***************************************


//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Send the message to the Receiver
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
int main()
{
	/* Generate unique key */
	GenerateMessageKey();
	/* Creates a message queue */
	CreateMessageQueue();
	/* Parsing the command and send to Gps controler */
	ConsoleParseSendData();

	return 0;
}

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Generate Message key using ftok() and store it into iKey
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void GenerateMessageKey()
{
	ikey = ftok("progfile", 65);
}

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Create a Message Queue using msgget() and return msg id
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void CreateMessageQueue()
{
	usMessageId = msgget(ikey, MESSAGE_GET_FLAG);
	stMessage.usMessageType = TRUE;
}

//*************************************.FUNCTION_HEADER.******************************************
//Purpose	: Parse the command given from user and send to the Gps controler 
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void ConsoleParseSendData()
{
	while(1)
	{
		printf("GPS>");
		fgets(pucUserEnteredData, MAX_SIZE, stdin);
		ConsoleParseCommand();

		/* If message Buffer is null continue on the loop */
		if(strcmp(stMessage.pucMessageText, "\0") == FALSE)
		{
			continue;
		}
		/* If Buffer have the data message will send to Recever section */
		else
		{
			ConsoleSendData(usMessageId, pucConvertedData);
		}
		/* If Quit command is occured, stop the transmission */
		if(strcmp(pucUserEnteredData, GPS_QUIT) == FALSE )
		{
			break;
		}
	}
	printf("Message Terminated\n");
}

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Identifying the user command 
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void ConsoleParseCommand()
{
	uint16 usIteration = 0;
	uint16 usBufferlength = strlen(CommandTable);

	for(usIteration = FALSE; usIteration <= usBufferlength; usIteration++)
	{
		if(strcmp(pucUserEnteredData, CommandTable[usIteration].pucCommands) == FALSE )
		{
			ConsoleProcessCommand(CommandTable[usIteration].pucValue);
			break;
		}
	}
}

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Clearing the Data buffer
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void BuffClear()
{
	memset(stMessage.pucMessageText, '\0', strlen(stMessage.pucMessageText));
}

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Send the Data to the gps
//Inputs		: uiMId : Message id
//				  pucReceiveBuff : Storing the command to transmit to Gps 
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void ConsoleSendData(uint16 uiMId, uint8* pucReceiveBuff)
{

	uint16 usMessageSendReturn = 0;
	/* msgsnd() to send message */
	usMessageSendReturn = msgsnd(uiMId, &stMessage, sizeof(stMessage), MESSAGE_SND_FLAG);

	/* Checking whether message is send or not */
	if(usMessageSendReturn == MESSAGE_SND_RETURN)
	{
		printf(" Error happened !! \n");
	}
	else
	{
		printf("Data send is :%s\n", stMessage.pucMessageText);
		printf("Message Sent.. \n");
		BuffClear();
	}
}

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Storing appropriate command value to buffer
//Inputs		: pucValue : Storing appropriate values for each command
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void ConsoleProcessCommand(uint8* pucValue)
{
	uint16 usIteration = 0;
	uint16 usBufferlength = strlen(CommandTable);

	if(strcmp(pucValue, GPS_HELP_VALUE) == FALSE)
	{
		for(usIteration = 0; usIteration < usBufferlength; usIteration++)
		{
			printf("%s",CommandTable[usIteration].pucCommands);
		}
	}
	else if(strcmp(pucValue, GPS_QUIT_VALUE) == FALSE)
	{
		strcpy(stMessage.pucMessageText, GPS_QUIT);
	}
	else
	{
		strcpy(stMessage.pucMessageText, pucValue);
	}

}

//EOF