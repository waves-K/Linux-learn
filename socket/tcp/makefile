all:tcp_serv tcp_cli tcp_serv_process
tcp_serv_process:tcp_serv_process.cpp
	g++ $^ -o $@ -pthread
tcp_serv:tcp_serv.cpp
	g++ $^ -o $@ -pthread
tcp_cli:tcp_cli.cpp 
	g++ $^ -o $@ 

.PHONY:clean
clean:
	rm -rf *.o tcp_cli tcp_serv tcp_serv_process
