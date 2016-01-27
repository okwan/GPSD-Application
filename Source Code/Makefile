CC=g++ -Wall -W -pedantic
CLIB=-lgps

dcgps: dcgps.o gps-utils.o gpsprint.o
	$(CC) -o dcgps dcgps.o gps-utils.o gpsprint.o $(CLIB) -lncurses -lpthread

clean:
	rm -f *.o core.* dcgps
dcgps.o: dcgps.cpp
	$(CC) -c -lncurses -lpthread dcgps.cpp 
gps-utils.o: gps-utils.cpp
	$(CC) -c -lncurses -lpthread gps-utils.cpp
gpsprint.o: gpsprint.cpp
	$(CC) -c -lncurses -lpthread gpsprint.cpp
