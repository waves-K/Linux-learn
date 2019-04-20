all:udp_cli udp_serv
udp_cli:udp_cli.cpp
	g++ $^ -o $@ -g

udp_serv:udp_serv.c
	gcc $^ -o $@ -g

.PHONY:clean
clean:
	rm -rf *.o udp_cli udp_serv
