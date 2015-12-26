//void myGetLocalSystemTime();
//void mySetLocalSystemTime();
//void myGetLocalSystemDate();

#ifdef _WIN32
unsigned short int win_system_call(const char* const cmd_name, const char* const cmd_arguments);
#else
#endif