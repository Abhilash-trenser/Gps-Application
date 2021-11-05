//******************* RECEIVE COMMANDS FROM THE CONSOLE (GPS CONTROLLER) **************************
// Copyright (c) 2021 Trenser Technology Solutions (P) Ltd
// All Rights Reserved
//*************************************************************************************************
// File			: Controller.c
// Summary		: Receives the message from console application and process it.
// Author		: Abhilash
// Date			: 01/11/21
//******************************* Include Files ***************************************************
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "Common.h"

//***************************** Global Constants **************************************************

//***************************** Local variables ***************************************************
static key_t ikey = 0;
static uint16 usMessageId = FALSE;
static STMESSAGEBUFFER stMessage = {0};
static uint8 pucReceiveBuff[CONVERTED_BUFF_MAX_SIZE] = {0};
static uint8 pucConvertedData[CONVERTED_BUFF_MAX_SIZE] = {0};

//**************************** Enum Declarations **************************************************

//****************************** Local Functions **************************************************
static void BuffClear();
static void GenerateMessageKey();
static void CreateMessageQueue();
static void DeleteMessageQueue();
static void ControlerReceiveData();
static void ControlerReceiveProcessData();

//******************************.FUNCTION_HEADER.**************************************************
//Purpose	: Receive Commands from the Message queue
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
	/* Receive and process data from console application */
	ControlerReceiveProcessData();
	/* Destroy the message queue */
	DeleteMessageQueue();

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

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Continuosly receiving the data and checking the quit command is occured or not.
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void ControlerReceiveProcessData()
{
	while(1)
	{
		/* Receiving the data from console */
		ControlerReceiveData();
		/* If Quit command is occured, stop the transmission */
		if( (strcmp(stMessage.pucMessageText, QUIT_STRING)) == FALSE )
		{
			break;
		}
	}
	printf("Message Terminated\n");
}

//*************************************.FUNCTION_HEADER.*******************************************
//Purpose	: Receiving the data from console
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void ControlerReceiveData()
{
	uint16 usMessageReceiveReturn = 0;
	/* msgrcv() to Receive message from console application */
	usMessageReceiveReturn = msgrcv(usMessageId, &stMessage, sizeof(stMessage),MESSAGE_TYPE, MESSAGE_RCV_FLAG)
	/* Checking whether message is Received or not */
	if(usMessageReceiveReturn == MESSAGE_RCV_RETURN )
	{
		printf(" Error happened !! \n");
	}
	else
	{
		printf("Data Received is : %s \n",stMessage.pucMessageText);
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
//Purpose	: Delete the Message queue created
//Inputs		: None
//Outputs	: None
//Return		: None
//Notes		: None
//*************************************************************************************************
static void DeleteMessageQueue()
{
	msgctl(usMessageId, IPC_RMID, NULL);
}