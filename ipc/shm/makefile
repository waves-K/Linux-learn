all:shm shm2
shm2:shm2.c
	gcc $^ -o $@
shm:shm.c
	gcc $^ -o $@

.PHONY:clean

clean:
	rm -rf *.o shm shm2
