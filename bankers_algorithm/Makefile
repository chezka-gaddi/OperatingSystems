CC = gcc
CPP = g++

OBJS = bankers.o customers.o

TARGET = bankers

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -pthread -o $(TARGET) $(OBJS)

bankers.o: bankers.c bank.h customers.h
	$(CC) -pthread -c bankers.c

customers.o: customers.c bank.h customers.h
	$(CC) -pthread -c customers.c

clean:
	$(RM) *.o *~ $(TARGET)
