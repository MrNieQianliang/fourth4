#include<unistd.h>
#include<stdlib.h>
#include<arpa/inet.h>
#include<cstring>
#include<iostream>
#include<sys/types.h>
#include<sys/socket.h>
#include<string>
#include<stdio.h>
#include<fstream>
#include<sstream>

#include<boost/archive/text_iarchive.hpp>
#include<boost/archive/text_oarchive.hpp>
//定义一个结构体进行数据封装
struct Algo {
  int first;
  int second;
   
};
std::ofstream llog;
void open_write_log (){
  llog.open("client.log");
}
     int main()
{

    int sockfd,numbytes;
    struct Algo msg;
    struct sockaddr_in their_addr;
    //char data[50];
  // char inde[50];
    int sum;
    open_write_log();
    std:: cout << "my ip is : 127.0.0.1" << std::endl << "my port is : 2323 " << std::endl << std::endl;
        their_addr.sin_family = AF_INET;
        their_addr.sin_port = htons(2323);
        their_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
        bzero(&(their_addr.sin_zero),8);
    while(1){
      
      std:: cin >> msg.first >> msg.second ;
      //  memset(data,0,sizeof(data));
       // memcpy(data,&msg,sizeof(msg));
 
      std::ostringstream st;
      boost::archive::text_oarchive archive(st);
      archive << msg.first << msg.second;
      std::string pipe = st.str();

        if((sockfd = socket(AF_INET,SOCK_STREAM,0)) ==-1){
          perror("socket");
          exit(1);
        }

        llog << "新建一个套接字" << std::endl << std::endl;
        
        if(connect(sockfd,(struct sockaddr *)&their_addr,sizeof(struct sockaddr)) == 0){
          llog << "请求对方链接chenggong" << std::endl << std::endl;
        //    perror("connect");
          //  exit(1);
        }
 
       
        if(send(sockfd,pipe.c_str(),pipe.size(),0) == -1){
          perror("send");
          exit(1);
        }
        //std::cout << data << std::endl;
        llog << "send successfully" << std::endl << std::endl;
        char inde[50];
           memset(inde,0,sizeof(inde));
        if((numbytes = recv (sockfd,inde,sizeof(inde),0)) == -1){
          perror("recv");
          exit(1);
        }
        if(numbytes == 0) break; 
       std::istringstream os(inde);
        boost::archive::text_iarchive is(os);
           is >> sum;
        std:: cout << sum << std::endl;
         llog << "recv successfully  " <<  inde << std::endl << std::endl;
        
    }
        close(sockfd);
        return 0;

}
        
