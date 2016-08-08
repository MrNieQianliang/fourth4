


#include"include.h"
#include<boost/thread/thread.hpp>

#define MAXEPOLLSIZE1 10000

  std::ofstream llog;
  struct epoll_event events[MAXEPOLLSIZE];
  int epfd,epfd1;
  std::string log_last_name = "log0.log"; 
  int sockfd;
   

int CreatSocket_Epoll() {


  int   new_fd;
  struct sockaddr_in my_addr,their_addr;
  int port;
  struct epoll_event ev;
  //struct epoll_event events[MAXEPOLLSIZE];
  //int epfd;
  char ip[10];
  if((sockfd = socket(AF_INET ,SOCK_STREAM,0)) == -1){
    perror ("socket");
    exit(1);
  }

  Log::readConfig(ip,port);//读取配置文档，被封装进Log类


  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons(port);
  my_addr.sin_addr.s_addr = inet_addr(ip);
  bzero(&(my_addr.sin_zero),8);
  std::cout << ip << "\t" << port << std::endl ;


  int opt = 1;//设置套接字属性
  setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  //closesocket 一般不会立即关闭，而需要一个TIME_WAIT的过程）
  //若后想继续用该则需要此设置

  // Log::writeLog(log,log_last_name);//写日志  


  //设置非阻塞...............................................
  int flag = fcntl(sockfd,F_GETFD,0);
  if(fcntl(sockfd,F_SETFL,flag|O_NONBLOCK) == -1)
  { perror ("fcntl");exit(1);}

  llog << Log::Time() << "将套接字设置为非阻塞" << std::endl <<std::endl;



  //绑定套接口.............................................
  if(bind(sockfd,(struct sockaddr*)(&my_addr),sizeof(struct sockaddr))
      == -1)
  {perror("bind"); exit(1);}
  llog << Log::Time() << "绑定套接口" << std::endl << std::endl;


  //监听套接口.............................................
  if(listen(sockfd,20) == -1)
  { perror("listen"); exit(1);}

  llog << Log::Time() << "设置监听套接口，服务器正常开始运行" << std::endl << std::endl;
  std::cout << Log::Time() << "server is run " << std::endl << std::endl;



  //创建一个epoll句柄且把监听到的sockeet放入epoll集中.......
//  epfd = epoll_create(MAXEPOLLSIZE);
  ev.events = EPOLLIN | EPOLLET ;
  ev.data.fd = sockfd;
  llog << Log::Time() <<  "创建一个epoll句柄" <<std::endl << std::endl;


  //添加事件............................
  if(epoll_ctl(epfd,EPOLL_CTL_ADD,sockfd,&ev) == -1)
  { perror("epoll_ctl");exit(1);}

  llog << Log::Time() << " 注册新事件到内核" << std::endl << std::endl;

  return 0; 
}





//监听活跃事件，以及对事件的处理......................................
//.......................................................................
int ctlEpoll(){


    int n;
    int index = 0;
    struct epoll_event ev;
    
    int addrlen;
    int new_fd;
    struct sockaddr_in their_addr;
   // struct epoll_event events[MAXEPOLLSIZE];



  while(1)
  {

    Log::writeLog_circulation(index,llog,log_last_name);

    //返回活跃事件.......................................................
    int nfds = epoll_wait (epfd,events,20,200);//Timeout(ms)
    if (nfds == -1)//返回-1则说明错误，成功则返回活跃事件个数
    { perror ("epoll_wait");break;}
    
    //llog << Log::Time() << "wait到 "<< nfds <<"  个活跃事件" << std::endl << std::endl;
    if(nfds == 0) continue;
    llog << Log::Time() << "wait到 "<< nfds <<"  个活跃事件" << std::endl << std::endl;
    
    
    //处理事件...............................
    for (n =0;n<nfds;n++){
      if(events[n].data.fd == sockfd)  
      {


        //接收链接..............................
        addrlen = sizeof(their_addr);
        while((new_fd = accept (sockfd, (struct sockaddr*)(&their_addr),(socklen_t*)&addrlen))>0){
        
          llog << Log::Time() <<  "成功accept到一个请求，并返回一个new_fd" << std::endl << std::endl;
          int flag = fcntl(sockfd,F_GETFD,0);
          if(fcntl(new_fd,F_SETFL,flag|O_NONBLOCK) == -1){
                 perror ("fcntlNew");}

         ev.events = EPOLLIN | EPOLLET;
        ev.data.fd = new_fd;  
        if( epoll_ctl(epfd1,EPOLL_CTL_ADD,new_fd,&ev) < 0)
          perror("listen_epoll_ctl");
        llog << Log::Time() <<  "epoll_ctl添加一个新事件" << std::endl << std::endl;
       
        }
        if(new_fd == -1){
          if(errno != EAGAIN && errno != ECONNABORTED
              && errno != EPROTO && errno != EINTR)
            perror("accept");
        }
      }
      //............此处为数据处理.................
    /*else{
       if( Work::option (events[n].data.fd,llog) == -1)
         epoll_ctl(epfd,EPOLL_CTL_DEL,new_fd ,&events[n]);   
         close(events[n].data.fd);
    }*/
    }
  } 
}

void listenThread(){
  
 // CreatSocket_Epoll();
  ctlEpoll();

}


void WorkThread(){

  int n;
  while(1){

  int nfds = epoll_wait (epfd1 ,events, 20 ,200);
 //llog << "work_epoll_wait 到" << nfds <<"  个事件" << std::endl << std::endl;

  if (nfds == -1)
  { perror("epoll_wait_work");break;}
  if(nfds == 0) continue;
 llog << "work_epoll_wait 到" << nfds <<"  个事件" << std::endl << std::endl;
  for( n =0 ; n < nfds ; n++){
    if(events[n].data.fd == sockfd)llog<<"这是一个监听新链接事件" << std::endl
      <<std::endl;
    if(events[n].data.fd != sockfd)
    {
      if(Work::option (events[n].data.fd,llog) == -1);//下面两句为压测时节约epoll资源
            epoll_ctl(epfd,EPOLL_CTL_DEL,events[n].data.fd,&events[n]);
      close(events[n].data.fd);
    }
  }
  }
}

void Thread_main(){

  
  
  //Log::writeLog();//写日志  
  Log::writeLog(llog,log_last_name);//写日志 
  epfd = epoll_create(MAXEPOLLSIZE);
  epfd1 = epoll_create(MAXEPOLLSIZE1);

  CreatSocket_Epoll();//创建套接字，epoll，并将监听套接字添加进内核


  boost::thread listen( &listenThread);
  boost::thread work(&WorkThread);
  //CreatSocket_Epoll(llog,epfd,sockfd);
 // ctlEpoll(llog,events,epfd,log_last_name,sockfd);
  //creatsocket_epoll(log,epfd,sockfd) ;//建立套接字，建立epoll事件，
// ctlepoll(log, events,epfd,log_last_name,sockfd);
//thrd.join();
//thrd1.join();

  listen.join();
  work.join();

  close(sockfd);
}
int main()
{

  


  boost::thread thr(&Thread_main);


  thr.join();
  return 0;
}









