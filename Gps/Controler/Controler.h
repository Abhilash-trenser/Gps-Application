//****RECEIVE COMMANDS FROM THE CONSOLE (GPS CONTROLER)******
// Copyright (c) 2021 Trenser Technology Solutions (P) Ltd
// All Rights Reserved
//*********************************************************************************************
// File				: _CONTROLER_H_.h
// Summary	: 
// Author			: Abhilash
// Date			: 01/11/21
//**************************************************************************************
#ifndef _CONTROLER_H_
#define _CONTROLER_H_
//******************************* Include Files ***************************************


//******************************* Global Types **************************************
typedef unsigned char uint8;
typedef unsigned long uint32;
typedef unsigned short uint16;
typedef unsigned char bool;

//***************************** Global Constants ***********************************
#define MAX_SIZE							(100)
#define QUIT_BUFF_SIZE					(6)
#define TRUE									(1)
#define FALSE									(0)
#define MESSAGE_TYPE					(0)
#define MESSAGE_RCV_FLAG			(0)
#define MESSAGE_RCV_RETURN	(-1)
#define MESSAGE_GET_FLAG			(0666 | IPC_CREAT)
#define CONVERTED_BUFF_MAX_SIZE		(10)

#define QUIT_STRING						("quit\n")

//****************************** Structure *********************************************
typedef struct _STMESSAGEBUFFER_
{
	uint16 uiMessageType;
	uint8 ucMessageText[MAX_SIZE];
} STMESSAGEBUFFER;

//***************************** Global Variables *************************************

//*************************** Global Functions ***************************************

//*************************** Enum Declarations *************************************

#endif // _CONTROLER_H_ 
// EOF 