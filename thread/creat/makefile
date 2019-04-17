creat:creat.c
	# 因为我们使用的是库函数，会链接库，所以要加上 -pthread
	gcc $^ -o $@ -pthread

.PHONY:clean
clean:
	rm -rf *.o creat
