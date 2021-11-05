//*************** SEND COMMANDS TO THE GPS MODULE (GPS CONSOLE) ***************
// Copyright (c) 2021 Trenser Technology Solutions (P) Ltd
// All Rights Reserved
//**************************************************************************************************
// File			: Console.c
// Summary	: send the Message queue to receiver.
// Author		: Abhilash
// Date			: 01/11/21
//******************************************* Include Files ****************************************
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include "Console.h"
//****************************************** Global Types *****************************************

//****************************************** Global Constants *************************************

//*************************************** Local Functions ******************************************
static void BuffClear();
static void GenerateMessageKey();
static void CreateMessageQueue();
static void ConsoleParseCommand();
static void ConsoleParseSendData();
static void ConsoleStartCommand(uint8* pucValue);
static void ConsoleProcessCommand(uint8* pucValue);
static void ConsoleSendData(uint16 uiMId, uint8* pucReceiveBuff);

//****************************************** Local variables ****************************************
static STMESSAGEBUFFER stMessage = {0};
static key_t ikey = 0;
static uint16 uiMessageId = FALSE;
static uint8 ucConvertedData[CONVERTED_BUFF_MAX_SIZE] = {0};
static uint8 ucUserEnteredData[MAX_SIZE] = {0};
static uint8* pucHelpCommand[] = {	"Start\n",
														"Loc\n",
														"Fix\n",
														"SatCon\n",
														"Reset\n",
														"Stop\n",
														"quit\n"
													 };
static STPARSE CommandTable[] = {	{GPS_START,		GPS_START_VALUE		},
															{GPS_LOC,			GPS_LOC_VALUE			},
															{GPS_FIX,			GPS_FIX_VALUE				},
															{GPS_SATCON,	GPS_SATCON_VALUE		},
															{GPS_RESET,		GPS_RESET_VALUE		},
															{GPS_HELP,		GPS_HELP_VALUE			},
															{GPS_QUIT,			GPS_QUIT_VALUE			}
														};

//*************************************** Enum Declarations ***************************************


//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Send the message to the Receiver
//Inputs	: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
int main()
{
	/* Generate unique key */
	GenerateMessageKey();
	/* Creates a message queue */
	CreateMessageQueue();
	/* Parsing the command and send to Gps controler */
	ConsoleParseSendData();

	printf("Message Terminated\n");

	return 0;
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Generate Message key using ftok() and store it into iKey
//Inputs	: None
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
//Inputs	: None
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
//Purpose	: Parse the command given from user and send to the Gps controler 
//Inputs	: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void ConsoleParseSendData()
{
	while(1)
	{
		printf("GPS>");
		fgets(ucUserEnteredData, MAX_SIZE, stdin);
		ConsoleParseCommand();

		if(strcmp(stMessage.ucMessageText, "\0") == FALSE)
		{
			while(1)
			{
				break;
			}
		}
		else
		{
			ConsoleSendData(uiMessageId, ucConvertedData);
		}

		if(strcmp(ucUserEnteredData, GPS_QUIT) == FALSE )
		{
			break;
		}
	}
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Identifying the user command 
//Inputs	: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void ConsoleParseCommand()
{
	uint16 uiIteration;
	uint16 uiBufferlength = strlen(CommandTable);

	for(uiIteration = FALSE ; uiIteration <= uiBufferlength ; uiIteration ++)
	{
		if(strcmp(ucUserEnteredData, CommandTable[uiIteration].ucCommands) == FALSE )
		{
			ConsoleProcessCommand(CommandTable[uiIteration].ucValue);
			break;
		}
	}
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Clearing the "ucConvertedData" buffer
//Inputs	: None
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void BuffClear()
{
	memset(stMessage.ucMessageText, '\0', strlen(stMessage.ucMessageText));
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Send the Data to the gps
//Inputs	: uint16 uiMId : Message id
//				uint8* pucReceiveBuff : Storing the command to transmit to Gps 
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void ConsoleSendData(uint16 uiMId, uint8* pucReceiveBuff)
{
	/* msgsnd() to send message */
	if( (msgsnd(uiMId, &stMessage, sizeof(stMessage), 
					MESSAGE_SND_FLAG) == MESSAGE_SND_RETURN))
	{
		printf(" Error happened !! \n");
	}
	else
	{
		printf("Data send is :%s\n", stMessage.ucMessageText);
		printf("Message Sent.. \n");
		BuffClear();
	}
}

//*************************************.FUNCTION_HEADER.**************************************
//Purpose	: Storing appropriate command value to  buffer
//Inputs	: uint8* pucValue : Storing appropriate values for each command
//Outputs	: None
//Return	: None
//Notes		: None
//***************************************************************************************************
static void ConsoleProcessCommand(uint8* pucValue)
{
	uint16 uiIteration;
	uint16 uiBufferlength = strlen(*pucHelpCommand);

	if(strcmp(pucValue, GPS_HELP_VALUE) == FALSE )
	{
		for( uiIteration = 0; uiIteration < uiBufferlength; uiIteration++)
		{
			printf("%s", pucHelpCommand[uiIteration]);
		}
	}
	else if(strcmp(pucValue, GPS_QUIT_VALUE) == FALSE )
	{
		printf("quit \n");
		strcpy(stMessage.ucMessageText, GPS_QUIT);
	}
	else
	{
		strcpy(stMessage.ucMessageText, pucValue);
	}

}

//EOF