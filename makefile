all: Client  Server

Server:
	g++ server.cpp work.cpp reaConWriLog.cpp -lboost_serialization -lboost_system -lboost_filesystem -lboost_thread -o Server

Client:
	g++ client.cpp -lboost_serialization -o Client

clean:
	rm Client Server 
