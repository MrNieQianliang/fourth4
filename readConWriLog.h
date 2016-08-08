#ifndef _READ_CONFI_LOG_H_
#define _READ_CONFI_LOG_H_
#include<fstream>
#include<string>
#include<iostream>
#include<sys/socket.h>
#include<string>
#include<stdlib.h>
#include<cstring>
#include<errno.h>
#include<sstream>

#include<boost/archive/text_oarchive.hpp>
#include<boost/archive/text_iarchive.hpp>
#include<boost/filesystem.hpp>
class Log{
  public:
static int writeLog(std::ofstream &llog,std::string &log_last_name);
static int writeLog_circulation(int &index ,std::ofstream &llog ,std::string &log_last_name);
static int readConfig(char conf1[20],int &conf2);
static std::string Time();
//static int option(const int &new_fd,std::ofstream &log);
};
class Work{
  public: 
    static int option (const int &new_fd,std::ofstream &llog);
};


#endif
