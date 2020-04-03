CC = gcc
CPP = g++

OBJS = bankers.o

TARGET = bankers

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) -pthread -o $(TARGET) $(OBJS)

bankers.o: bankers.c bank.h
	$(CC) -pthread -c bankers.c

clean:
	$(RM) *.o *~ $(TARGET)
