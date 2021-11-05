//************************ SEND COMMANDS TO THE GPS MODULE (GPS CONSOLE) *************************
// Copyright (c) 2021 Trenser Technology Solutions (P) Ltd
// All Rights Reserved
//************************************************************************************************
// File		: Common.h
// Summary	: Common Header file for both C files.
// Author	: Abhilash
// Date		: 01/11/21
//*************************************************************************************************
#ifndef _COMMON_H_
#define _COMMON_H_
//******************************* Include Files ***************************************************


//******************************* Global Types ****************************************************
typedef unsigned char uint8;
typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char bool;

typedef void (*CommandOperation)  (uint8* pucValue );
//***************************** Global Constants **************************************************
#define MAX_SIZE							(100)
#define TRUE								(1)
#define FALSE								(0)
#define QUIT_BUFF_SIZE					(6)
#define MESSAGE_GET_FLAG				(0666 | IPC_CREAT)
#define MESSAGE_SND_FLAG				(0)
#define MESSAGE_SND_RETURN				(-1)
#define MESSAGE_RCV_FLAG				(0)
#define MESSAGE_RCV_RETURN				(-1)

#define CONVERTED_BUFF_MAX_SIZE		(10)

#define GPS_QUIT							("quit\n")
#define GPS_START							("start\n")
#define GPS_LOC							("loc\n")
#define GPS_FIX							("fix\n")
#define GPS_SATCON						("satcon\n")
#define GPS_RESET							("stop\n")
#define GPS_HELP							("help\n")

#define GPS_START_VALUE					("01")
#define GPS_LOC_VALUE					("02")
#define GPS_FIX_VALUE					("03")
#define GPS_SATCON_VALUE				("04")
#define GPS_RESET_VALUE					("05")
#define GPS_HELP_VALUE					("06")
#define GPS_QUIT_VALUE					("07")

//****************************** Structure ********************************************************
typedef struct _STMESSAGEBUFFER_
{
	uint16 usMessageType;
	uint8 pucMessageText[MAX_SIZE];
} STMESSAGEBUFFER;

typedef struct _STPARSE_
{
	uint8* pucCommands;
	uint8* pucValue;
}STPARSE;
//***************************** Global Variables *************************************************

//*************************** Global Functions ****************************************************

//*************************** Enum Declarations ***************************************************

#endif // _COMMON_H_
// EOF 