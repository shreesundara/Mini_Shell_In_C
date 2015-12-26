#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "my_shell_kernel.h"
#include "my_shell_utility_fns.h"

#ifdef _WIN32

unsigned short int win_system_call(const char* const cmd_name, const char* const cmd_arguments)
{
	unsigned short int process_ret_value = 1;

	if(NULL != cmd_name && NULL != cmd_arguments)
	{
		char* tempCmd = NULL;
		tempCmd = (char *)(calloc(1,sizeof(char)*(CMD_LINE_MAX_SIZE+1)));
		if
			(
				0 == strcmp("cd",cmd_name)
				||
				0 == strcmp("pwd",cmd_name)
			)
		{
			//process_ret_value  = system(strcat_s("cd",CMD_LINE_MAX_SIZE,cmd_arguments));

			//First append the command name...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,"cd");
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';
		}


		else if
			(
				0 == strcmp("date",cmd_name)
				||
				0 == strcmp("time",cmd_name)
				||
				0 == strcmp("mkdir",cmd_name)
				||
				0 == strcmp("rmdir",cmd_name)
			)
		{
			//process_ret_value = system(strcat(cmd_name,cmd_arguments));

			//First append the command name...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,cmd_name);
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';
		}


		else if(0 == strcmp("ls",cmd_name))
		{
			//process_ret_value = system(strcat("dir",cmd_arguments));

			//First append the command name...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,"dir");
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';

		}


		else if(0 == strcmp("copy_file",cmd_name))
		{
			//process_ret_value = system(strcat("copy",cmd_arguments));
			
			//First append the command name...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,"copy");
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';
		}


		else if(0 == strcmp("move_file",cmd_name))
		{
			//process_ret_value = system(strcat("move",cmd_arguments));
			
			//First append the command name...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,"move");
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';
		}


		else if(0 == strcmp("delete_file",cmd_name))
		{
			//process_ret_value = system(strcat("del",cmd_arguments));

			//First append the command name...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,"del");
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';
		}

		else if(0 == strcmp("rename_file",cmd_name))
		{
			//process_ret_value = system(strcat("rename",cmd_arguments));

			//First append the command name...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,"rename");
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';

		}//End of if else ladder...


		//If the command provided is an invalid command...
		if(0 == strncmp(tempCmd,"\0",2))
		{
			assert("Unknown Command Encountered");
		}
		else
		{
			//Now append the command arguments...
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1," ");
			strcat_s(tempCmd,CMD_LINE_MAX_SIZE+1,cmd_arguments);
			tempCmd[CMD_LINE_MAX_SIZE] = '\0';

			process_ret_value = system(tempCmd);
		}//end of second if-else ladder..

		if(NULL != tempCmd)
			free(tempCmd);
		tempCmd = NULL;
	}//end of if(fn.. parameters != NULL)

	return process_ret_value;
}//end of process command function...

#else

#endif















//
////#ifdef _WIN32
////#include<windows.h>
//////#include<time.h>
////#else
//////#include<sys/time.h>
////#endif
//
//
////#ifdef _WIN32
////typedef SYSTEMTIME _mySystemTime;
////#else
////typedef SYSTEMTIME _mySystemTime;
////#endif
//
//void myGetLocalSystemTime()
//{
//	//_mySystemTime currentTime;
//	//GetLocalTime(&currentTime);
//	//cout<<"Day: "<<currentTime.wDay<<" . Month : "<<currentTime.wMonth<<" . Year :"<<currentTime.wYear<<endl;
//	//cout<<"Hours: "<<currentTime.wHour<<" . Mins : "<<currentTime.wMinute<<" . Secs :"<<currentTime.wSecond<<" . MilliSecs :"<<currentTime.wMilliseconds<<endl;
//	//cout<<"DayOfWeek : "<<currentTime.wDayOfWeek<<endl;
//
//	//cout<<currentTime.wDay<<"/"<<currentTime.wMonth<<"/"<<currentTime.wYear<<endl;
//	////tm onetime;
//	////time_t* onetime_t;
//	////*(onetime_t) = 45*12*30*24*60*60;//No.. of seconds since mid-night Jan 1, 1970..
//	//////onetime = localtime(0);
//	////localtime_s(&onetime,onetime_t);
//	////cout<<"Time is :"<<onetime.tm_hour<<":"<<onetime.tm_min<<":"<<onetime.tm_sec<<endl;
//	//int ret = system("time");
//}
//
////void mySetLocalSystemTime()
////{
////	_mySystemTime currentTime;
////	currentTime.wDay = 01;
////	currentTime.wMonth = 01;
////	currentTime.wYear = 2009;
////	currentTime.wHour = 19;
////	currentTime.wMinute = 00;
////	currentTime.wSecond = 50;
////	currentTime.wMilliseconds = 100;
////	currentTime.wDayOfWeek = 01;
////	BOOL retValue = SetLocalTime(&currentTime);
////	cout<<retValue<<" : is the ret value os set local time";
////}
//
//void myGetLocalSystemDate()
//{
//	//_mySystemTime currentTime;
//	//GetLocalTime(&currentTime);
//	////cout<<"Day: "<<currentTime.wDay<<" . Month : "<<currentTime.wMonth<<" . Year :"<<currentTime.wYear<<endl;
//	////cout<<"Hours: "<<currentTime.wHour<<" . Mins : "<<currentTime.wMinute<<" . Secs :"<<currentTime.wSecond<<" . MilliSecs :"<<currentTime.wMilliseconds<<endl;
//	////cout<<"DayOfWeek : "<<currentTime.wDayOfWeek<<endl;
//
//	//cout<<currentTime.wHour<<":"<<currentTime.wMinute<<endl;
//	//char* env;
//	//int ret = system("date");
//	
//}