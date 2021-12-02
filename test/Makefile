
#test log system
build-log-system : test-log-system.c\
	../src/log-system.c\
	../src/get-algorithm.c\
	../src/read-input-file.c
	gcc -Wall test-log-system.c ../src/log-system.c\
		../src/get-algorithm.c ../src/read-input-file.c -o log-system.out
#test get algorithm
build-get-algorithm : test-get-algorithm.c\
	../src/get-algorithm.c
	gcc -Wall test-get-algorithm.c ../src/get-algorithm.c -o get-algorithm.out

#test read input file
build-read-input-file : test-read-input-file.c\
						../src/read-input-file.c
	gcc -Wall test-read-input-file.c ../src/read-input-file.c -o read-input.out


