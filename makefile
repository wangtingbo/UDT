ser:udpserver.o Recv.o Msg.o Send.o thread.o
	g++ -o ser udpserver.o  Recv.o Msg.o Send.o thread.o -lpthread
Send.o:Send.cpp 
	g++ -c Send.cpp
Recv.o:Recv.cpp
	g++ -c Recv.cpp 	
udpserver.o:udpserver.cpp 
	g++ -c udpserver.cpp
thread.o:thread.cpp
	g++ -c thread.cpp
.PHONY:clean
clean:
	rm -f *.o	
