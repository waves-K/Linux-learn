all:fifo2 fifo
fifo2:fifo2.c
	gcc $^ -o $@
fifo:fifo.c
	gcc $^ -o $@

.PHONY:clean

clean:
	rm -rf *.o fifo
