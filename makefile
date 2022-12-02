CFLAGS  = -Wall -Wno-unused-result -O3

all: odd-even-open-mpi odd-even-open-mp odd-even-open-mp-alt driver

odd-even-open-mpi:
	mpicc lib/util.c lib/quicksort.c src/odd-even-open-mpi.c -o odd-even-open-mpi $(CFLAGS) 

odd-even-open-mp:
	gcc lib/util.c lib/quicksort.c src/odd-even-open-mp.c -o odd-even-open-mp $(CFLAGS) -fopenmp

odd-even-open-mp-alt:
	gcc lib/util.c lib/quicksort.c src/odd-even-open-mp-alt.c -o odd-even-open-mp-alt $(CFLAGS) -fopenmp

driver:
	gcc lib/util.c src/driver.c -o driver $(CFLAGS)

clean: 
	-rm odd-even-open-mpi
	-rm odd-even-open-mp
	-rm odd-even-open-mp-alt
	-rm driver
