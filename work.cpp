

#include"readConWriLog.h"

//struct Algo {
  //int first;
  //int second ;
//};

int Work::option(const int &new_fd ,std::ofstream &llog)
{
  struct Algo{
    int first;
    int second;
  };
  struct Algo temp;
  int numbytes;//接收字节数
  int sum = 0;//两数之和
  char buff[512];
  
  int n = 0;
  memset(buff,0,sizeof(buff));
  while((numbytes = recv(new_fd , buff+n , sizeof(buff) , 0) ) >0)
         n += numbytes;
    if(numbytes < 0 && errno != EAGAIN && errno != EINTR ){
     // perror ("read error ");
         perror("recv");llog << Log::Time() << "recv数据失败" <<std::endl << std::endl ;
       }
    //客户端住的那个断开，关闭链接
    if(numbytes == 0){ return -1;}
    llog << numbytes << std::endl << std::endl;
    llog << "recv jieshoudaode shu ju " << buff << std::endl << std::endl;

  llog << Log::Time() << "work 中 recv函数成接收new_fd链接传输的数据" <<std:: endl << std::endl;
llog << Log::Time() << " 接收数据 " << std::endl << std::endl ;


//memset(&temp,0,sizeof(temp));
//memcpy(&temp,buff,sizeof(temp));//将数据转换成两个int型数据


//.........linshi...............
//std::istringstream is(buff);
/*boost::archive::text_iarchive ia(is);
ia >> temp.first >> temp.second;;
if(temp.first == 90909090 && temp.second == 9090909){
 llog << Log::Time() << " 收到关闭客户机指令，即将关闭客户机" << std::endl << std::endl ;
  exit(0);
}
*/
std::cout << " received " << n << "bytes" << std::endl;
llog << Log::Time() << "接收到客户端数据字节数 " << std::endl << std::endl;
llog << Log::Time () << "接收到客户端数据 " <<temp.first << temp.second << std::endl << std::endl;


llog << buff << std::endl << std::endl;

std::cout << "The message is : " << temp.first <<"\t" << temp.second << std::endl;
  sum = temp.first + temp.second ;
  std::cout << " The sum is : " << sum << std::endl << std::endl;
 

//...................temp..........
/*
  //std::string buf;
  std::ostringstream os;
  //os << sum ; os >> buf;
  boost::archive::text_oarchive ib(os);
  ib << sum;
 std::string pipe = os.str();
*/

  std::string http0 = "GET / HTTP/1.1 \r\n";

  std::string http1 = "Host: localhost:2323 \r\n";
  std::string http2 = "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0 \r\n";
  std::string http3 = "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n";
  std::string http4 = "Accept-Language: en-US,en;q=0.5 \r\n";
  std::string http5 = "Accept-Encoding: gzip, deflate \r\r";
  std::string http6 = "Connection: keep-alive\r\n ";
  
  std::string httpHeader = http0 + http1 + http2 + http3 + http4 + http5 +http6;


        
        

//非阻塞，epoll，et模式下写法，出错，原因不详
  /*int nwrite,data_size = pipe.size();
 int  m = data_size;
  while( m > 0){
    nwrite = send(new_fd,pipe.c_str() + pipe.size() - m,m,0);
    if(nwrite < 0){
      if ( nwrite == -1 && errno != EAGAIN ){
  perror("send");}
  break;
}
n -= nwrite;
}*/
 int nwrite = send(new_fd,httpHeader.c_str(),httpHeader.size(),0);
 //int nwrite = send(new_fd,&sum,sizeof(sum),0);
 if(nwrite < 0){
   if(nwrite == -1 && errno != EAGAIN){
     perror("send");}
   exit(1);
 }

//close(new_fd);
//epoll_ctl(epfd1,EPOLL_CTL_DEL,events[n].data.fd,&events[n]);
llog << Log::Time() << "正在发送数据给客户端" << std::endl << std::endl;
return 0;
}
