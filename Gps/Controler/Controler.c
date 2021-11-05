//****RECEIVE COMMANDS FROM THE CONSOLE (GPS CONTROLER)******
// Copyright (c) 2021 Trenser Technology Solutions (P) Ltd
// All Rights Reserved
//*********************************************************************************************
// File				: _CONTROLER_H_.c
// Summary	: 
// Author			: Abhilash
// Date			: 01/11/21
//******************************* Include Files *********************************************
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "Controler.h"

//***************************** Global Constants *******************************************

//***************************** Local variables *********************************************
static uint8 ucConvertedData[CONVERTED_BUFF_MAX_SIZE] = {0};
static key_t ikey = 0;
static uint16 uiMessageId = FALSE;
static STMESSAGEBUFFER stMessage = {0};
static uint8 pucReceiveBuff[CONVERTED_BUFF_MAX_SIZE] = {0};

//**************************** Enum Declarations *****************************************

//****************************** Local Functions *******************************************
static void GenerateMessageKey();
static void CreateMessageQueue();
static void DeleteMessageQueue();
static void BuffClear();
static void ControlerReceiveData();
static void ControlerReceiveProcessData();

//******************************.FUNCTION_HEADER.********************************
//Purpose	: Receive Commands from the Message queue
//Inputs		: None
//Outputs	: None
//Return	: None
//Notes		: None
//********************************************************************************************

int main()
{
	GenerateMessageKey();
	CreateMessageQueue();
	//BuffClear();
	ControlerReceiveProcessData();

	printf("Message Terminated\n");
	/* to destroy the message queue */
	DeleteMessageQueue();

	return 0;
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Generate Message key using ftok() and store it into iKey
//Inputs		: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void GenerateMessageKey()
{
	ikey = ftok("progfile", 65);
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Create a Message Queue using msgget() and return msg id
//Inputs		: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void CreateMessageQueue()
{
	uiMessageId = msgget(ikey, MESSAGE_GET_FLAG);
	stMessage.uiMessageType = TRUE;
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Continuosly receiving the data and checking the quit command is occured or not.
//Inputs		: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void ControlerReceiveProcessData()
{
	while(1)
	{
		/* msgrcv() to receive message */
		ControlerReceiveData();

		if( (strcmp(stMessage.ucMessageText, QUIT_STRING)) == FALSE )
		{
			break;
		}
	}
	
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Receiving the data from console
//Inputs		: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void ControlerReceiveData()
{
	//printf("%d",strlen)
	if( msgrcv(uiMessageId, &stMessage, sizeof(stMessage),
					MESSAGE_TYPE, MESSAGE_RCV_FLAG) == MESSAGE_RCV_RETURN )
	{
		printf(" Error happened !! \n");
	}
	else
	{
		/* display the message */
		printf("Data Received is : %s \n",stMessage.ucMessageText);
	}
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Clearing the "ucConvertedData" buffer
//Inputs		: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void BuffClear()
{
	memset(stMessage.ucMessageText, '\0', strlen(stMessage.ucMessageText));
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Delete the Message queue created
//Inputs		: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void DeleteMessageQueue()
{
	msgctl(uiMessageId, IPC_RMID, NULL);
}