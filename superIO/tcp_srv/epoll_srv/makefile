all:epoll_srv epoll_srv1
epoll_srv1:epoll_srv1.cpp
	g++ $^ -o $@ -std=c++0x
epoll_srv:epoll_srv.cpp
	g++ $^ -o $@ -std=c++0x

.PHONY:clean
clean:
	rm -rf *.o epoll_srv epoll_srv1
