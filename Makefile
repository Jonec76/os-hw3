CFLAGS = -B -g -Wall -std=c++11
CFLAGS += -O2
CC = g++

.PHONY = clean
OBJS := main.o compare.o
TARGET := main compare
DEPS := main.cpp compare.cpp 
.PHONY: clean
main: main.cpp
	$(CC) -o $@ $< $(CFLAGS)

compare: compare.cpp
	$(CC) -o $@ $< $(CFLAGS)

# %.o: %.cpp $(DEPS) 
# 	$(CC) -c -o $@ $< $(OBJS) $(CFALGS)
clean:
	rm *.o main compare


