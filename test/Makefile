
build:
	make build-generator build-schedular build-clk build-process

build-clk : ../src/clk.c
	gcc -Wall ../src/clk.c -o clk.out
build-process : ../src/process.c
	gcc -Wall ../src/process.c -o process.out


#test generator
build-generator : ../src/read-input-file.c\
	../src/get-algorithm.c\
	../src/process-generator.c
	gcc -Wall ../src/read-input-file.c ../src/get-algorithm.c ../src/process-generator.c\
		../src/log-system.c\
			-o process-generator.out

#test pcb_queue
#test log system
build-log-system : test-log-system.c\
	../src/log-system.c\
	../src/get-algorithm.c\
	../src/read-input-file.c\
	../src/pcb-queue.c\
	../src/min-heap.c
	gcc -Wall test-log-system.c ../src/log-system.c\
		../src/get-algorithm.c ../src/read-input-file.c ../src/pcb-queue.c\
		../src/min-heap.c -o log-system.out
#test get algorithm
build-get-algorithm : test-get-algorithm.c\
	../src/get-algorithm.c
	gcc -Wall test-get-algorithm.c ../src/get-algorithm.c -o get-algorithm.out

#test read input file
build-read-input-file : test-read-input-file.c\
						../src/read-input-file.c
	gcc -Wall test-read-input-file.c ../src/read-input-file.c -o read-input.out

build-schedular :  
	gcc ../src/schedular.c ../src/log-system.c ../src/min-heap.c ../src/pcb-queue.c -o schedular.out
