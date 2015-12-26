#include <iostream>
#include <map>

//#define CMD_PRMPT_NAME_SIZE 15//2^4 - 1 because + 1 is added later.	cmd_prompt_name size is 10 characters excluding null terminator string..
#define CMD_ARG_MAX_SIZE 31	//2^4	sat as per the standard size as specified in the book..
#define TOTAL_NO_OF_ARGUMENTS 30

#define CMD_LINE_MAX_SIZE 127//2^7	SET AS PER THE STANDARD...
#define CMD_PRMPT_NAME_SIZE 15

#ifdef CMD_ARG_MAX_SIZE
	#ifdef TOTAL_NO_OF_ARGUMENTS
		#ifdef CMD_LINE_MAX_SIZE
			#undef CMD_LINE_MAX_SIZE
		#endif
		#define CMD_LINE_MAX_SIZE (TOTAL_NO_OF_ARGUMENTS*CMD_ARG_MAX_SIZE)
	#endif

	#ifdef CMD_PRMPT_NAME_SIZE
		#undef CMD_PRMPT_NAME_SIZE
	#endif
	#define CMD_PRMPT_NAME_SIZE CMD_ARG_MAX_SIZE
#endif


#ifdef CMD_LINE_MAX_SIZE

	//If CMD_LINE_MAX_SIZE exceeds 255, set its size count type to unsigned int
	//Assuming base machine as 32-bit machine, this will not work on 16 bit machines any-more..
	#if(CMD_LINE_MAX_SIZE > 255)
			typedef unsigned int TYPE_OF_CMD_LINE_SIZE_COUNT ;
	#else
			typedef unsigned short int TYPE_OF_CMD_LINE_SIZE_COUNT;
	#endif

#else
	
#endif


#ifdef CMD_ARG_MAX_SIZE

	//If CMD_ARG_MAX_SIZE exceeds 255, set its size count type to unsigned int
	//Assuming base machine as 32-bit machine, this will not work on 16 bit machines any-more..
	#if(CMD_ARG_MAX_SIZE > 255)
			typedef unsigned int TYPE_OF_CMD_ARG_MAX_SIZE ;
	#else
			typedef unsigned short int TYPE_OF_CMD_ARG_MAX_SIZE;
	#endif

#else
	
#endif

#ifdef TOTAL_NO_OF_ARGUMENTS

	//If TOTAL_NO_OF_ARGUMENTS exceeds 255, set its size count type to unsigned int
	//Assuming base machine as 32-bit machine, this will not work on 16 bit machines any-more..
	#if(TOTAL_NO_OF_ARGUMENTS > 255)
			typedef unsigned int TYPE_OF_NO_OF_ARGS;
	#else
			typedef unsigned short int TYPE_OF_NO_OF_ARGS;
	#endif

#else
	
#endif
	
enum SHELL_RET_TYPES
{
	//Should always be the first entry in the enum..
	SHELL_RET_INIT_ERROR = 0x00,//shell object not initialized..

	SHELL_RET_QUIT,//Quit from the shell..
	SHELL_RET_LEN_EXCEEDED,//Shell prompt new name length exceeded..
	SHELL_RET_NO_ARG,//No arguments are specified for the command which requires argument..
	SHELL_RET_ARG_INVALID,//Arguments are specified but are invalid..
	SHELL_RET_CMD_SUPPORTED,
	SHELL_RET_CMD_NOT_SUPPORTED,


	SHELL_RET_OK,
	SHELL_RET_UNKNOWN_ERROR,
};


enum SHELL_COMMANDS
{
	SHELL_CMD_EXIT = 0x00,

	SHELL_CMD_MKDIR,
	SHELL_CMD_RMDIR,
	SHELL_CMD_PWD,
	SHELL_CMD_CD,

	SHELL_CMD_HELP,
	SHELL_CMD_SYS_DATE,
	SHELL_CMD_SYS_TIME,

	SHELL_CMD_LIST_FILES,
	SHELL_CMD_FILE_COPY,
	SHELL_CMD_FILE_MOVE,
	SHELL_CMD_FILE_RENAME,
	SHELL_CMD_FILE_DEL,

	SHELL_CMD_CHANGE_CMD_PROMPT_NAME,

	SHELL_CMD_EMPTY_COMMAND,
	SHELL_CMD_UNKNOWN_CMD,
};


typedef struct cmd_line
{
	char* cmdLine;
	TYPE_OF_CMD_LINE_SIZE_COUNT cmdLength;
}STRUCT_CMD_LINE;

typedef struct cmd_structure
{
	//char* commandName;//This seems to be redundant code, not really required..
	SHELL_COMMANDS commandType;
	unsigned short argumentCount;
	char* argumentVector[TOTAL_NO_OF_ARGUMENTS]; 
}STRUCT_CMD_STRUCTURE;

typedef struct cmd_info
{
	const char* commandName;
	const char* cmd_description;
	const char* cmd_usage;
	//SHELL_COMMANDS commandType;//This also seems to be redundant... so commenting..
}CMD_INFO;

///The following are the list of supported commands...
//"exit"
//"help"
//"pwd"
//"mkdir"
//"rmdir"
//"date"
//"time"
//"ls"
//"cd"
//"copy_file"
//"move_file"
//"delete_file"
//"rename_file"
//"prompt_rename"

//Single Ton Class...
class My_Shell
{
public:

	//fn initializes only one instance of its own class..
	static My_Shell* initializeMyShellObject();
	SHELL_RET_TYPES startShell();

	//Destructor...
	~My_Shell();



private:

	static bool _isObjectCreated;
	static My_Shell *_myShellObj;
	static const char shell_cmd_prompt_default_name[CMD_PRMPT_NAME_SIZE+1];
	/*static std::map<char*,SHELL_COMMANDS> cmdName_cmdType_map;
	static std::map<SHELL_COMMANDS,char*> cmdType_description_map;*/
	static std::map<SHELL_COMMANDS,CMD_INFO> cmdType_cmdInfo_map;


	//Here even though we are using the pointers within the class,
	//we need not use copy constructor since we don't want multiple
	//copies of shell cmd prompt name since its a single-ton class
	char *shell_cmd_prompt_name;

	//This is single-ton class.. so constructor is private
	My_Shell();
	SHELL_RET_TYPES initializeCommandsList();
	void insertCmdIntoCmdMap(const char * const cmd_name,const char * const cmd_description,const char* const cmd_usage,const SHELL_COMMANDS shellCmdType);
	void obtainUserCmd(STRUCT_CMD_LINE&);
	SHELL_RET_TYPES parseInputCmd(const STRUCT_CMD_LINE&,STRUCT_CMD_STRUCTURE&);
	SHELL_RET_TYPES process_cmd(const STRUCT_CMD_STRUCTURE&);


	inline void releaseCmdStrMemory(STRUCT_CMD_STRUCTURE& cmdStructure);
	inline void releaseCmdLineStrMemory(STRUCT_CMD_LINE &StructCmdLine);
	SHELL_RET_TYPES isCommandSupported(const char* const);
	SHELL_RET_TYPES isCommandSupported(const SHELL_COMMANDS);
	CMD_INFO& getCommandInfo(const char* const command_name);
	CMD_INFO& getCommandInfo(const SHELL_COMMANDS);

	SHELL_RET_TYPES process_ChangeShellPromptName_cmd(const STRUCT_CMD_STRUCTURE & cmdStr);
	SHELL_RET_TYPES process_Help_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure);
	//SHELL_RET_TYPES process_file_rename_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure);
	//SHELL_RET_TYPES process_file_del_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure);
	SHELL_RET_TYPES process_cd_command(const STRUCT_CMD_STRUCTURE & cmdStructure);
	//SHELL_RET_TYPES process_file_move_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure);
	//SHELL_RET_TYPES process_file_copy_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure);
	SHELL_RET_TYPES process_list_command(const STRUCT_CMD_STRUCTURE & cmdStructure);
	SHELL_RET_TYPES process_date_time_command(const STRUCT_CMD_STRUCTURE & cmdStructure);
	SHELL_RET_TYPES process_pwd_command(const STRUCT_CMD_STRUCTURE & cmdStructure);
	SHELL_RET_TYPES process_dir_modification_command(const STRUCT_CMD_STRUCTURE & cmdStructure);
	SHELL_RET_TYPES process_file_operation_cmd(const STRUCT_CMD_STRUCTURE & cmdStructure);

	SHELL_RET_TYPES splitCmdNameAndArgs_AndProcessCmd(const STRUCT_CMD_STRUCTURE& cmdStructure);

};//end of my_shell class definition..


////This will result in multiple re-declararions if file included more than once..
////Initialize memory for the static variables of My_Shell class..
//bool My_Shell::_isObjectCreated = false;
//My_Shell* My_Shell::_myShellObj = NULL;
