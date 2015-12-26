#include <iostream>
#include <conio.h>
#include <assert.h>

using namespace std;

//including user-defined functions...
#include "my_shell_kernel.h"
#include "my_shell_utility_fns.h"

//Initialize memory for the static variables of My_Shell class..
bool My_Shell::_isObjectCreated = false;
My_Shell* My_Shell::_myShellObj = NULL;
const char My_Shell::shell_cmd_prompt_default_name[CMD_PRMPT_NAME_SIZE+1] = "My Shell";
//std::map<char*,SHELL_COMMANDS> My_Shell::cmdName_cmdType_map;
//std::map<SHELL_COMMANDS,char*> My_Shell::cmdType_description_map;
std::map<SHELL_COMMANDS,CMD_INFO> My_Shell::cmdType_cmdInfo_map;

//fn initializes only one instance of its own class..
My_Shell* My_Shell::initializeMyShellObject()
{
	if(_isObjectCreated )
	{
		cout<<"\nShell Object is Initialized. . . .";
	}
	else
	{
		cout<<"\nInitializing Shell Object . . .";
		_myShellObj = new My_Shell();
		_isObjectCreated = true;
		cout<<"\nShell Object Initialization Complete. . .";
	}

	return _myShellObj;
}

void My_Shell::insertCmdIntoCmdMap(const char * const cmd_name,const char * const cmd_description,const char* const cmd_usage,const SHELL_COMMANDS shellCmdType)
{
	if(CMD_ARG_MAX_SIZE + 2 <= strnlen_s(cmd_name,(CMD_ARG_MAX_SIZE + 1) + 1))
	{
		cout<<"\n Command \""<<cmd_name \
			<<"\" was not added to command list since its size is : " \
			<<strnlen_s(cmd_name,(CMD_ARG_MAX_SIZE + 1) + 1)<<" which exceeds threshold length of " \
			<< CMD_ARG_MAX_SIZE +1<<endl;
		return;
	}
	else
	{
		//My_Shell::cmdName_cmdType_map.insert(
		//									std::pair<char*,SHELL_COMMANDS> (cmd_name,shellCmdType)
		//								);
		//My_Shell::cmdType_description_map.insert(
		//									std::pair<SHELL_COMMANDS,char*>(shellCmdType,cmd_description)
		//								);

		CMD_INFO info;
		info.commandName = cmd_name;
		info.cmd_description = cmd_description;
		info.cmd_usage = cmd_usage;
		//info.commandType = shellCmdType;

		My_Shell::cmdType_cmdInfo_map.insert
			(
				std::make_pair<SHELL_COMMANDS,CMD_INFO>(shellCmdType,info)
			);
	}
}

SHELL_RET_TYPES My_Shell::initializeCommandsList()
{
	/*My_Shell::cmdName_cmdType_map.clear();
	My_Shell::cmdType_description_map.clear();*/
	My_Shell::cmdType_cmdInfo_map.clear();

	try{
	
		//command to exit from the my_shell program..
		insertCmdIntoCmdMap("exit","Quits / Exits from shell",
			"exit (Enter-key)",SHELL_CMD_EXIT);
		
		//command to display list of supported commands in my_shell
		insertCmdIntoCmdMap("help","Displays list of supported commands",
			"help [cmd_name] (Enter-Key)",SHELL_CMD_HELP);
		
		//command to display the current working directory...
		insertCmdIntoCmdMap("pwd","Displays the current working directory",
			"pwd (Enter-key)",SHELL_CMD_PWD);

		//command to create a new directory within the current working directory...
		insertCmdIntoCmdMap("mkdir","Creates a new directory in specified location..",
			"mkdir directory (Enter-key)",SHELL_CMD_MKDIR);

		//command to remove the specified directory from the current working directory...
		insertCmdIntoCmdMap("rmdir","Deletes a specified directory",
			"rmdir directory (Enter-key)",SHELL_CMD_RMDIR);

		//command to display the current system date..
		insertCmdIntoCmdMap("date","Displays current system date",
			"date (Enter-key)",SHELL_CMD_SYS_DATE);

		//command to display the current system time..
		insertCmdIntoCmdMap("time","Displays current system time",
			"time (Enter-key)",SHELL_CMD_SYS_TIME);

		//command to list the files and folders within current working directory..
		insertCmdIntoCmdMap("ls","Displays files and folders contained within current working directory",
			"ls [directory] (Enter-key)",SHELL_CMD_LIST_FILES);

		//command to change current working directory..
		insertCmdIntoCmdMap("cd","Changes the current working directory",
			"cd directory (Enter-key)",SHELL_CMD_CD);

		//command to copy file..
		insertCmdIntoCmdMap("copy_file","Copies the file from one location to another",
			"copy_file src_file_path destination_file_path  (Enter-key)",SHELL_CMD_FILE_COPY);

		//command to move file..
		insertCmdIntoCmdMap("move_file","Moves the file to specified location",
			"move_file src_file_path destination_file_paht (Enter-key)",SHELL_CMD_FILE_MOVE);

		//command to delete a file..
		insertCmdIntoCmdMap("delete_file","Deletes the specified file",
			"delete_file src_file_path (Enter-key)",SHELL_CMD_FILE_DEL);

		insertCmdIntoCmdMap("rename_file","Renames the specified file",
			"rename_file src_file_name dest_file_name (Enter-key)",SHELL_CMD_FILE_RENAME);
		
		//command to change my_shell command prompt name..
		insertCmdIntoCmdMap("prompt_rename","Changes the shell prompt name",
			"prompt_rename new_name (Enter-key)",SHELL_CMD_CHANGE_CMD_PROMPT_NAME);
	}
	catch(std::exception except)
	{
		cout<<"\n Error while initializing the supported commands list";
		cout<<"\n"<<except.what();
		return SHELL_RET_INIT_ERROR;
	}
	return SHELL_RET_OK;
}

SHELL_RET_TYPES My_Shell::startShell()
{

	cout<<"\n\nStarting the Shell . . .\n";

	if(SHELL_RET_OK != initializeCommandsList())
		return SHELL_RET_INIT_ERROR;

	cout<<endl<<"The shell supports \""<<My_Shell::cmdType_cmdInfo_map.size()<<"\" commands";
	cout<<endl<<"The maximum no of arguments accepted by any command is limited to \""<<TOTAL_NO_OF_ARGUMENTS<<"\" characters";
	cout<<endl<<"The maximum length of any argument is limited to \""<<CMD_ARG_MAX_SIZE+1<<"\" characters";
	cout<<endl<<"The maximum size of command_name is limited to \""<<CMD_ARG_MAX_SIZE+1<<"\" characters";
	cout<<endl<<"The maximum no of characters accepted in command line is limited to \""<<CMD_LINE_MAX_SIZE+1<<"\" characters"<<endl;
	cout<<endl<<endl<<"Type \"help\" followed by any \"command_name\" (Enter-key) to know the command's usage"<<endl;
	cout<<endl<<"OR"<<endl;
	cout<<endl<<"Type \"help\" (Enter-key) to view list of supported commands"<<endl;


	bool quit = false;
	int i = 0;
	STRUCT_CMD_LINE StructCmdLine;
	StructCmdLine.cmdLength = 0;
	StructCmdLine.cmdLine = NULL;

	STRUCT_CMD_STRUCTURE StructCmdStructure;
	StructCmdStructure.argumentCount = 0;
	StructCmdStructure.commandType = SHELL_CMD_UNKNOWN_CMD;

	while(! quit)
	{
			cout<<endl<<this->shell_cmd_prompt_name<<" : ";
			obtainUserCmd(StructCmdLine);
			//if(StructCmdLine != NULL && StructCmdLine.cmdLength > 0 && StructCmdLine.cmdLine != NULL)
			if(0 < StructCmdLine.cmdLength && NULL != StructCmdLine.cmdLine)
			{
				//parse the input command..
				SHELL_RET_TYPES cmdProcessReturnCode = SHELL_RET_OK;
				SHELL_RET_TYPES cmdParseReturnCode = parseInputCmd(StructCmdLine,StructCmdStructure);
				releaseCmdLineStrMemory(StructCmdLine);

				//process the command...
				if(SHELL_RET_OK == cmdParseReturnCode)
					cmdProcessReturnCode  = process_cmd(StructCmdStructure);
				else//if error occurs while parsing the command line
				{
					cout<<endl<<"\tInvalid arguments provided for the command"<<endl;
					cout<<"\tUse \"help command_name\" to check the command usage"<<endl;
					cout<<"\tAlso check that the single argument length should not exceed "<<CMD_ARG_MAX_SIZE<<" characters"<<endl;
					cout<<"\tWords within single or double quotes are considered as single arguments"<<CMD_ARG_MAX_SIZE<<" characters"<<endl;
				}

				if(SHELL_RET_QUIT == cmdProcessReturnCode)
				{
					//this is set if command entered is exit or quit...
					cout<<"\nExiting the shell \n";
					quit = true;
				}
				else if(SHELL_RET_CMD_NOT_SUPPORTED == cmdProcessReturnCode)
				{
					if(SHELL_CMD_HELP == StructCmdStructure.commandType)
						cout<<endl<<"Command  \""<<StructCmdStructure.argumentVector[1]<<"\" is not supported"<<endl;
					else
						cout<<endl<<"Command  \""<<StructCmdStructure.argumentVector[0]<<"\" is not supported"<<endl;

					cout<<"\nUse \"help\" command to view the list of supported commands\n";
				}
				else if(SHELL_RET_UNKNOWN_ERROR == cmdProcessReturnCode)
				{
					cout<<"\nUnknown error encountered while processing command : "<<StructCmdStructure.argumentVector[0]<<endl;
					cout<<"\nThe arguments specified might not be correct.."<<endl;
				}
				else if
					(
						SHELL_RET_ARG_INVALID == cmdProcessReturnCode
						||
						SHELL_RET_NO_ARG == cmdProcessReturnCode
					)
				{
					cout<<endl<<"Command "<<StructCmdStructure.argumentVector[0]<<" usage not correct"<<endl;
					cout<<endl<<"Use \"help "<<StructCmdStructure.argumentVector[0]<<"\" to know its usage"<<endl;
				}
				else if(SHELL_RET_LEN_EXCEEDED == cmdProcessReturnCode )
				{
	   				cout<<"\nPrompt Name must not exceed \""<<CMD_PRMPT_NAME_SIZE<<"\" characters"<<endl;
				}
				else if(SHELL_RET_CMD_SUPPORTED == cmdProcessReturnCode)
				{
					if(SHELL_CMD_HELP == StructCmdStructure.commandType)
					{
						cout<<endl<<"Command  \""<<StructCmdStructure.argumentVector[1]
						<<"\" Usage : "<<endl<<"\t\t"<<getCommandInfo(StructCmdStructure.argumentVector[1]).cmd_usage<<endl;
					}
				}

				//Release the memory...
				releaseCmdStrMemory(StructCmdStructure);

			}
			else//if user has not entered any command...
			{
				//do nothing...
				//printerror and notufy user and continue..
			}

			//The following is to be done before going to next command processing...
			releaseCmdLineStrMemory(StructCmdLine);
			releaseCmdStrMemory(StructCmdStructure);

	}//end of infinite while loop...

	releaseCmdLineStrMemory(StructCmdLine);
	releaseCmdStrMemory(StructCmdStructure);

	return SHELL_RET_OK;
}


//This is single-ton class.. so constructor is private
My_Shell::My_Shell()
{
	shell_cmd_prompt_name = (char *)(calloc(1,sizeof(char)*(CMD_PRMPT_NAME_SIZE+1)));
	//strncpy(shell_cmd_prompt_name,"My_Shell : ",CMD_PRMPT_NAME_SIZE);
	//strncpy_s(shell_cmd_prompt_name,CMD_PRMPT_NAME_SIZE,"My_Shell : ",CMD_PRMPT_NAME_SIZE);
	strncpy_s(shell_cmd_prompt_name,CMD_PRMPT_NAME_SIZE,shell_cmd_prompt_default_name,CMD_PRMPT_NAME_SIZE);
	shell_cmd_prompt_name[CMD_PRMPT_NAME_SIZE] = '\0';
}

//Destructor...
My_Shell::~My_Shell()
{
	//My_Shell::cmdName_cmdType_map.clear();
	//My_Shell::cmdType_description_map.clear();
	My_Shell::cmdType_cmdInfo_map.clear();	

	if(NULL != shell_cmd_prompt_name)
		free(shell_cmd_prompt_name);

	shell_cmd_prompt_name = NULL;

	//to de-allocate my shell object..
	free(_myShellObj);
	_myShellObj = NULL;
}


//Function to start obtaining the user command input..
void My_Shell::obtainUserCmd(STRUCT_CMD_LINE & cmd_ln_params)
{
	cmd_ln_params.cmdLine = NULL;
	cmd_ln_params.cmdLength = 0;

	char ch = '\0';
	bool exceeded_cmd_length_limit = false;

	char* cmdLine = (char *)(calloc(1,sizeof(char)*(CMD_LINE_MAX_SIZE+1)));
	TYPE_OF_CMD_LINE_SIZE_COUNT cmdLineSize = CMD_LINE_MAX_SIZE;

	//To keep track of char count to check if the command line size exceeds the
	//assumed max length of the cmd line..
	TYPE_OF_CMD_LINE_SIZE_COUNT charCount = 0;
	TYPE_OF_CMD_LINE_SIZE_COUNT index = 0;
	//The above two variables type is dependent on the size of cmdLine i.e
	//It is dependent on the cmd_line_max_size,
	//If cmd_line_max_size is b/w 0 and 255, they are of type unsigned short int,
	//If cmd_line_max_size is > 255, they are of type unsigned int


	//While the imput is not Enter key or a new line character, continuing obtaining user input..
	while(13 != (ch = getchar()) && 10 != ch)
	//while((ch = _getch()) != 13)
	{
		cmdLine[index++] = ch;
		charCount++;

		if(cmdLineSize <= charCount)
		{
			////re-allocate the memory for extra-space
			////This is a part of enhancement, requires some extra work
			////to keep track of the type of charCount sice the size may exceed 255...
			//realloc(cmdLine,cmdLineSize+(CMD_LINE_MAX_SIZE/2));
			//cmdLineSize = cmdLineSize+(CMD_LINE_MAX_SIZE/2);

			exceeded_cmd_length_limit = true;
			break;
		}
	}//end of while function...
	cmdLine[index] = '\0';
	if(! exceeded_cmd_length_limit)
	{
		//STRUCT_CMD_LINE cmd_ln_params;
		
		//Instead of blindly copying the pointer values.. we have to do memcpy
		//nanually before returnin from fn because this will be freed after returning
		//from function
		//cmd_ln_params.cmdLine = cmdLine;
		
		//cmd_ln_params.cmdLine = (char *)(calloc(1,cmdLineSize+1));
		cmd_ln_params.cmdLine = (char *)(calloc(1,sizeof(char)*(CMD_LINE_MAX_SIZE+1)));
		memcpy(cmd_ln_params.cmdLine,cmdLine,CMD_LINE_MAX_SIZE);
		cmd_ln_params.cmdLine[CMD_LINE_MAX_SIZE] = '\0';

		cmd_ln_params.cmdLength = CMD_LINE_MAX_SIZE+1;//To hold the '\0' character...

		//cmd_ln_params.cmdLine[10000000] = '\0';
		
		//return cmd_ln_params;
	}
	else
	{
		//do nothing...
		//return NULL;
	}

	//Free the dynamically allocated memory...
	if(NULL != cmdLine)
		free(cmdLine);
	cmdLine = NULL;	

}

SHELL_RET_TYPES My_Shell::parseInputCmd(const STRUCT_CMD_LINE & cmd_line_params ,STRUCT_CMD_STRUCTURE & cmd_structure)
{
	cmd_structure.argumentCount = 0;
	//cmd_structure.commandName = NULL;
	cmd_structure.commandType = SHELL_CMD_UNKNOWN_CMD;

	if(NULL != cmd_line_params.cmdLine && 0 < cmd_line_params.cmdLength  && 0 != strncmp(cmd_line_params.cmdLine,"",cmd_line_params.cmdLength))
	{
		bool exceededArgLengthLimit = false;
		bool quoteCharEncountered = false;
		char prevCharacter = '\0';
		
		TYPE_OF_CMD_LINE_SIZE_COUNT char_read_index = 0;

		TYPE_OF_CMD_ARG_MAX_SIZE char_insert_index = 0;
		TYPE_OF_NO_OF_ARGS argumentVectorIndex = 0;

		cmd_structure.argumentVector[argumentVectorIndex] = (char *)(calloc(1,sizeof(char)*(CMD_ARG_MAX_SIZE+1)));
		cmd_structure.argumentCount++;

		//start parsing...
		while(char_read_index <= cmd_line_params.cmdLength && '\0' != cmd_line_params.cmdLine[char_read_index ] )
		{
			if(
				(	13 == cmd_line_params.cmdLine[char_read_index ]//if next char is enter-key...
					|| 
					'\n' == cmd_line_params.cmdLine[char_read_index ]//if next char is new line char..
					|| 
					' ' == cmd_line_params.cmdLine[char_read_index ]
				) 
				&& ' ' != prevCharacter  && '\0' != prevCharacter && !quoteCharEncountered )
			{
				cmd_structure.argumentVector[argumentVectorIndex][char_insert_index++] = '\0';
				argumentVectorIndex++;
				cmd_structure.argumentVector[argumentVectorIndex] = (char *)(calloc(1,sizeof(char)*(CMD_ARG_MAX_SIZE+1)));
				cmd_structure.argumentCount++;
				char_insert_index = 0;
			}
			//If its not a space character... then do the following...
			else
			{
				if(
					'\'' == cmd_line_params.cmdLine[char_read_index ]
					||
					'\"' == cmd_line_params.cmdLine[char_read_index ]
				  )
				{
				  quoteCharEncountered = !quoteCharEncountered;
				}
				else
				{
					cmd_structure.argumentVector[argumentVectorIndex][char_insert_index++] = cmd_line_params.cmdLine[char_read_index ];
					if(CMD_ARG_MAX_SIZE <= char_insert_index)
					{
					  exceededArgLengthLimit = true;
					  break;
					}
				}
			
			}//END OF IF ELSE
			
			prevCharacter = cmd_line_params.cmdLine[char_read_index ];
			char_read_index ++;
		}//end of while...

		cmd_structure.argumentVector[argumentVectorIndex][char_insert_index++] = '\0';
		argumentVectorIndex++;
		char_insert_index = 0;

		//If its the first argument, then set it as the command name itself...
		if(0 < cmd_structure.argumentCount && NULL != cmd_structure.argumentVector[0])
		{
			////To DO:- turncate the commandName...
			//cmd_structure.commandName = (char *)(calloc(1,sizeof(char)*(CMD_ARG_MAX_SIZE+1)));
			//strncpy_s(
			//		cmd_structure.commandName,
			//		CMD_ARG_MAX_SIZE + 1,
			//		cmd_structure.argumentVector[0],
			//		CMD_ARG_MAX_SIZE
			//		);
			////strncpy(cmd_structure.commandName,cmd_structure.argumentVector[0],CMD_ARG_MAX_SIZE);
			//cmd_structure.commandName[CMD_ARG_MAX_SIZE] = '\0';

			cmd_structure.commandType = SHELL_CMD_UNKNOWN_CMD;

			std::map<SHELL_COMMANDS,CMD_INFO>::iterator ite = My_Shell::cmdType_cmdInfo_map.begin();
			while(My_Shell::cmdType_cmdInfo_map.end() != ite)
			{
				if(0 == strncmp(ite->second.commandName,cmd_structure.argumentVector[0],CMD_ARG_MAX_SIZE + 1))
				{
					cmd_structure.commandType = ite->first;
					break;
				}
				ite++;
			}




			////Deprecated code-Old version... 
			//std::map<char *,SHELL_COMMANDS>::iterator ite = My_Shell::cmdName_cmdType_map.begin();
			//while(ite != My_Shell::cmdName_cmdType_map.end())
			//{
			//	//if(strncmp(ite->first,cmd_structure.commandName,CMD_ARG_MAX_SIZE + 1) == 0)
			//	if(strncmp(ite->first,cmd_structure.argumentVector[0],CMD_ARG_MAX_SIZE + 1) == 0)
			//	{
			//		cmd_structure.commandType = ite->second;
			//		break;
			//	}
			//	ite++;
			//}
		}

		if(exceededArgLengthLimit || quoteCharEncountered)
		{
			return SHELL_RET_ARG_INVALID;
			//do something..
		}

		//return cmd_structure;
	}
	else
	{
		cmd_structure.commandType = SHELL_CMD_EMPTY_COMMAND;
		//return NULL;
	}
	return SHELL_RET_OK;
}


SHELL_RET_TYPES My_Shell::process_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	SHELL_RET_TYPES process_exec_ret_type = SHELL_RET_OK;
	switch(cmdStructure.commandType)
	{
		case SHELL_CMD_EXIT :
			process_exec_ret_type = SHELL_RET_QUIT;
			break;

		case SHELL_CMD_UNKNOWN_CMD:
			process_exec_ret_type = SHELL_RET_CMD_NOT_SUPPORTED;
			break;

		case SHELL_CMD_MKDIR:
			//cout<<"\nCreating new directory\n";
			process_exec_ret_type  = process_dir_modification_command(cmdStructure);
			break;

		case SHELL_CMD_RMDIR:
			//cout<<"\nDeleting directory\n";
			process_exec_ret_type  = process_dir_modification_command(cmdStructure);
			break;

		case SHELL_CMD_PWD:
			//cout<<"\nCurrent working directory is: "<<endl;
			process_exec_ret_type  = process_pwd_command(cmdStructure);
			break;

		case SHELL_CMD_HELP:
			process_exec_ret_type  = process_Help_cmd(cmdStructure);
			break;

		case SHELL_CMD_SYS_DATE:
			//cout<<"\nCurrent system date \n";
			//myGetLocalSystemDate();
			process_exec_ret_type  = process_date_time_command(cmdStructure);
			break;

		case SHELL_CMD_SYS_TIME:
			//cout<<"\nCurrent system time \n";
			//myGetLocalSystemTime();
			process_exec_ret_type  = process_date_time_command(cmdStructure);
			break;	 

		case SHELL_CMD_LIST_FILES:
			//cout<<"\nlist of files in current working directory is: "<<endl;
			process_exec_ret_type  = process_list_command(cmdStructure);
			break;

		case SHELL_CMD_FILE_COPY:
			//cout<<"\nCopying files"<<endl;
			process_exec_ret_type  = process_file_operation_cmd(cmdStructure);
			break;

		case SHELL_CMD_FILE_MOVE:
			//cout<<"\nMoving files"<<endl;
			process_exec_ret_type  = process_file_operation_cmd(cmdStructure);
			break;

		case SHELL_CMD_CD:
			//cout<<"\nChanging directory"<<endl;
			process_exec_ret_type  = process_cd_command(cmdStructure);
			break;

		case SHELL_CMD_FILE_DEL:
			//cout<<"\nDeleting file"<<endl;
			process_exec_ret_type  = process_file_operation_cmd(cmdStructure);
			break;

		case SHELL_CMD_CHANGE_CMD_PROMPT_NAME:
			//cout<<"\nChanging command prompt name: "<<endl;
			process_exec_ret_type  = process_ChangeShellPromptName_cmd(cmdStructure);
			break;

		case SHELL_CMD_FILE_RENAME:
			//cout<<"\nChanging command prompt name: "<<endl;
			process_exec_ret_type  = process_file_operation_cmd(cmdStructure);
			break;			

		case SHELL_CMD_EMPTY_COMMAND:
			//cout<<"\nEmpty Command \n";
			//do nothing if command is empty..
			break;

		default:
			process_exec_ret_type = SHELL_RET_UNKNOWN_ERROR;
			break;
	}
	
	return process_exec_ret_type;
}


SHELL_RET_TYPES My_Shell::process_ChangeShellPromptName_cmd(const STRUCT_CMD_STRUCTURE & cmdStr)
{
	if(2 == cmdStr.argumentCount)
	{
		if(NULL != cmdStr.argumentVector[1])
		{
			if(CMD_PRMPT_NAME_SIZE+2 <= strnlen(cmdStr.argumentVector[1],(CMD_PRMPT_NAME_SIZE+1+1)))
			{
				return SHELL_RET_LEN_EXCEEDED;
			}
			else
			{
				strncpy_s(this->shell_cmd_prompt_name,CMD_PRMPT_NAME_SIZE+1,cmdStr.argumentVector[1],CMD_PRMPT_NAME_SIZE);
				this->shell_cmd_prompt_name[CMD_PRMPT_NAME_SIZE] = '\0';
				return SHELL_RET_OK;
			}
		}
		else
		{
			return SHELL_RET_UNKNOWN_ERROR;
		}
	}
	
	else
	{
		if(1 == cmdStr.argumentCount)
			return SHELL_RET_NO_ARG;
		else
			return SHELL_RET_ARG_INVALID;
	}
}

SHELL_RET_TYPES My_Shell::process_Help_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	//If help is entered with exactly one argument, display usage of specified command..
	if(2 == cmdStructure.argumentCount)
	{
		if(NULL != cmdStructure.argumentVector[1])
		{
			//SHELL_RET_TYPES shell_ret = isCommandSupported(cmdStructure.commandType);
			SHELL_RET_TYPES shell_ret = isCommandSupported(cmdStructure.argumentVector[1]);
			//if specified command is not supported.. then alert user that command is not supported
			return shell_ret;
		}
		else
			return SHELL_RET_UNKNOWN_ERROR;
	}
	
	//if help is entered without any arguments, display all supported commands...
	else if(1 == cmdStructure.argumentCount)
	{
		cout<<"\nList of supported commands"<<endl;
		cout<<"***************************************************"<<endl;
		std::map<SHELL_COMMANDS,CMD_INFO>::iterator cmd_info_iter;
		cmd_info_iter = My_Shell::cmdType_cmdInfo_map.begin();

		while(cmd_info_iter != My_Shell::cmdType_cmdInfo_map.end())
		{
			cout<<endl<<cmd_info_iter->second.commandName<<" :: "<<endl<<"\t"<<cmd_info_iter->second.cmd_description<<endl<<endl;
			cmd_info_iter ++;
		}
		cout<<"***************************************************"<<endl;
		return SHELL_RET_OK;
	}

	else
	{
		return SHELL_RET_ARG_INVALID;
	}
}

inline void My_Shell::releaseCmdLineStrMemory(STRUCT_CMD_LINE &StructCmdLine)
{
	//if(StructCmdLine != NULL)
	{
		if( NULL != StructCmdLine.cmdLine )
			free(StructCmdLine.cmdLine);
		StructCmdLine.cmdLine = NULL;
	}
}

inline void My_Shell::releaseCmdStrMemory(STRUCT_CMD_STRUCTURE& cmdStructure)
{
	if(0 < cmdStructure.argumentCount && NULL != cmdStructure.argumentVector)
	{
		for(int i = 0;i< cmdStructure.argumentCount;i++)
		{
			free(cmdStructure.argumentVector[i]);
			cmdStructure.argumentVector[i] = NULL;
		}
		cmdStructure.argumentCount = 0;
	}
}

SHELL_RET_TYPES My_Shell::isCommandSupported(const char* const command_name)
{
	std::map<SHELL_COMMANDS,CMD_INFO>::iterator cmd_info_iter;
	cmd_info_iter = My_Shell::cmdType_cmdInfo_map.begin();

	while(My_Shell::cmdType_cmdInfo_map.end() != cmd_info_iter)
	{
		if(0 == strncmp(cmd_info_iter->second.commandName,command_name,CMD_ARG_MAX_SIZE))
			return SHELL_RET_CMD_SUPPORTED;

		cmd_info_iter ++;
	}
	return SHELL_RET_CMD_NOT_SUPPORTED;
}

SHELL_RET_TYPES My_Shell::isCommandSupported(const SHELL_COMMANDS shellCmdType)
{
	std::map<SHELL_COMMANDS,CMD_INFO>::iterator cmd_info_iter;
	cmd_info_iter = My_Shell::cmdType_cmdInfo_map.begin();

	while(My_Shell::cmdType_cmdInfo_map.end() != cmd_info_iter)
	{
		if(shellCmdType == cmd_info_iter->first)
			return SHELL_RET_CMD_SUPPORTED;

		cmd_info_iter ++;
	}
	return SHELL_RET_CMD_NOT_SUPPORTED;
}

CMD_INFO& My_Shell::getCommandInfo(const char* const command_name)
{
	std::map<SHELL_COMMANDS,CMD_INFO>::iterator cmd_info_iter;
	cmd_info_iter = My_Shell::cmdType_cmdInfo_map.begin();

	while(My_Shell::cmdType_cmdInfo_map.end() != cmd_info_iter)
	{
		if(0 == strncmp(cmd_info_iter->second.commandName,command_name,CMD_ARG_MAX_SIZE))
			return cmd_info_iter->second;

		cmd_info_iter ++;
	}

	assert("Abnormal behaviour:- control should  not have come here without a valid argument to getCommandInfo(commandName) function..");

	CMD_INFO info;
	info.commandName = NULL;
	info.cmd_usage = NULL;
	info.cmd_description = NULL;
	return info;
}

CMD_INFO& My_Shell::getCommandInfo(SHELL_COMMANDS shellCmdType)
{
 	std::map<SHELL_COMMANDS,CMD_INFO>::iterator cmd_info_iter;
	cmd_info_iter = My_Shell::cmdType_cmdInfo_map.begin();

	while(My_Shell::cmdType_cmdInfo_map.end() != cmd_info_iter)
	{
		if(shellCmdType == cmd_info_iter->first)
			return cmd_info_iter->second;

		cmd_info_iter ++;
	}

	assert("Abnormal behaviour:- control should  not have come here without a valid argument to getCommandInfo(commandType) function..");

	CMD_INFO info;
	info.commandName = NULL;
	info.cmd_usage = NULL;
	info.cmd_description = NULL;
	return info;
}


SHELL_RET_TYPES My_Shell::process_file_operation_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	SHELL_RET_TYPES proc_ret_value = SHELL_RET_OK;

	//if file operation is to delete a file..
	if(SHELL_CMD_FILE_DEL == cmdStructure.commandType)
	{
		if(2 == cmdStructure.argumentCount)
		{
			proc_ret_value = splitCmdNameAndArgs_AndProcessCmd(cmdStructure);
		}
		else
		{
			proc_ret_value = SHELL_RET_ARG_INVALID;
		}
	}

	//if file operation is not delete...
	//and file operation is either to rename, or move or copy the file...
	else if
		(
			SHELL_CMD_FILE_COPY == cmdStructure.commandType
			||
			SHELL_CMD_FILE_MOVE == cmdStructure.commandType
			||
			SHELL_CMD_FILE_RENAME == cmdStructure.commandType
		)
	{
		if(3 == cmdStructure.argumentCount)
		{
			proc_ret_value = splitCmdNameAndArgs_AndProcessCmd(cmdStructure);
		}
		else
		{
			proc_ret_value = SHELL_RET_ARG_INVALID;
		}
	}//end of else if rename or move or copy..

	else//if invalid file operation requested...
	{
		proc_ret_value = SHELL_RET_UNKNOWN_ERROR;
	}

	return proc_ret_value;
}

SHELL_RET_TYPES My_Shell::process_cd_command(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	SHELL_RET_TYPES ret_type = SHELL_RET_OK;
	if(2 == cmdStructure.argumentCount)
	{
		splitCmdNameAndArgs_AndProcessCmd(cmdStructure);
	}
	else
	{
		ret_type = SHELL_RET_ARG_INVALID;
	}
	return ret_type ;

}

SHELL_RET_TYPES My_Shell::process_list_command(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	SHELL_RET_TYPES ret_type = SHELL_RET_OK;
	if(1 == cmdStructure.argumentCount || 2 == cmdStructure.argumentCount)
	{
		splitCmdNameAndArgs_AndProcessCmd(cmdStructure);
	}
	else
	{
		ret_type = SHELL_RET_ARG_INVALID;
	}
	return ret_type ;

}

SHELL_RET_TYPES My_Shell::process_date_time_command(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	SHELL_RET_TYPES ret_type = SHELL_RET_OK;
	if(1 == cmdStructure.argumentCount)
	{
		splitCmdNameAndArgs_AndProcessCmd(cmdStructure);
	}
	else
	{
		ret_type = SHELL_RET_ARG_INVALID;
	}
	return ret_type ;
}

SHELL_RET_TYPES My_Shell::process_pwd_command(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	SHELL_RET_TYPES ret_type = SHELL_RET_OK;
	if(1 == cmdStructure.argumentCount)
	{
		splitCmdNameAndArgs_AndProcessCmd(cmdStructure);
	}
	else
	{
		ret_type = SHELL_RET_ARG_INVALID;
	}
	return ret_type ;
}

SHELL_RET_TYPES My_Shell::process_dir_modification_command(const STRUCT_CMD_STRUCTURE & cmdStructure)
{
	SHELL_RET_TYPES ret_type = SHELL_RET_OK;
	if(
		SHELL_CMD_RMDIR == cmdStructure.commandType
		||
		SHELL_CMD_MKDIR == cmdStructure.commandType
	  )
	{
		if(2 == cmdStructure.argumentCount)
		{
			splitCmdNameAndArgs_AndProcessCmd(cmdStructure);
		}
		else
		{
			ret_type = SHELL_RET_ARG_INVALID;
		}
	}
	else//invalid dir modification command selected..
	{
		ret_type = SHELL_RET_ARG_INVALID;
	}
	return ret_type ;

}

SHELL_RET_TYPES My_Shell::splitCmdNameAndArgs_AndProcessCmd(const STRUCT_CMD_STRUCTURE& cmdStructure)
{
	SHELL_RET_TYPES proc_ret_value  = SHELL_RET_OK;
	//if commandArgumentVector != NULL
	if(NULL != cmdStructure.argumentVector[0])
	{
		char* tempCmdArgs = (char *)(calloc(1,sizeof(char)*(CMD_LINE_MAX_SIZE+1)));
		unsigned short int read_index = 1;//start from the second argument index...
		
		while(cmdStructure.argumentCount > read_index)
		{
			if(NULL != cmdStructure.argumentVector[read_index])
			{
				if(0 != strcat_s(tempCmdArgs,CMD_LINE_MAX_SIZE+1,cmdStructure.argumentVector[read_index]))
				{
					proc_ret_value = SHELL_RET_UNKNOWN_ERROR;
					break;
				}
				read_index++;
				//also copy a space character after copying the individual arguments...
				if(0 != strcat_s(tempCmdArgs,CMD_LINE_MAX_SIZE+1," "))
				{
					proc_ret_value = SHELL_RET_UNKNOWN_ERROR;
					break;
				}
			}
			else
			{
				proc_ret_value = SHELL_RET_UNKNOWN_ERROR;
				break;
			}
		}
		tempCmdArgs[CMD_LINE_MAX_SIZE] = '\0';
		if(SHELL_RET_OK == proc_ret_value)
		{
			short int sys_call_ret = win_system_call(cmdStructure.argumentVector[0],tempCmdArgs);
			if(0 == sys_call_ret)
				proc_ret_value = SHELL_RET_OK;
			else
				proc_ret_value = SHELL_RET_UNKNOWN_ERROR;
		}

		if(NULL != tempCmdArgs)
			free(tempCmdArgs);

		tempCmdArgs = NULL;
	}//end of if

	else//IF COMMAND argument Vector == NULL
	{
		proc_ret_value = SHELL_RET_UNKNOWN_ERROR;
	}
	return proc_ret_value;
}