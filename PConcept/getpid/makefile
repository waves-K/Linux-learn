zombie:zombie.c
	gcc zombie.c -o zombie 

ALL:main env main1
main:main.c
	gcc main.c -o main

env:env.c
	gcc env.c -o env

main1:main1.c
	gcc main1.c -o main1

.PHONY:clean env main1
clean:
	rm main env main1 

