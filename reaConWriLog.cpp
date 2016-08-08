/*#include<iostream>
#include<string>
#include<sstream>
#include<fstream>
#include<boost/filesystem.hpp>
*/

#include"readConWriLog.h"
//打印系统时间..................
std::string Log::Time(){
  time_t t;
  time(&t);
  return ctime(&t);
}


//声明创建日志文件......................................
int Log::writeLog( std::ofstream &llog , std::string &log_last_name ){
  
   llog.open(log_last_name.c_str());

  if(!llog.is_open())
  {std::cout << "Error Open" << std::endl; return -1;}
return 0;
}


/*判断日志大小,做滚动日志,
 * 要求: 每个日志限制10MB,
 * 一共最多十个日志,若最后当十个日志满,则删除第一个日志
 */

int Log::writeLog_circulation(int &index,std::ofstream &llog,std::string &log_last_name)
{
  /*第一个参数是index,
   * 第二个参数是ofstream对象,
   * 第三个参数是last_log_name
   */
  if(boost::filesystem::file_size(log_last_name) > 10000000  &&  index < 9){
    llog.close();
    index += 1;

  std::stringstream os;
  //string log_name = "log";
  //string log_index_name="0";
  os << "log" << index << ".log";
  os >> log_last_name;
  llog.open(log_last_name.c_str());
  std::cout << "newLog  " << log_last_name << std::endl;
  }
  if(boost::filesystem::file_size(log_last_name) > 10000000 && index == 9 ){
    llog.close();
    index = 0;
    std::stringstream os;
    os << "log" << index << ".log";
    os >> log_last_name;
    llog.open(log_last_name.c_str());
    std::cout << "newLog  " << log_last_name << std::endl;
  }
  return 0;
}


//读取配置文档，文档中第一行为ip ，第二行为端口号............
int Log::readConfig(char conf1[20], int &conf2)
{
  std::ifstream config;
  char port[5];
  config.open("config.conf");
  if(!config.is_open())
  {std::cout << " configuration open error\n" << std::endl;}
  config.getline(conf1,20);
  config.getline(port,5);
  std::stringstream os;
  os << port ; os >> conf2;


}



