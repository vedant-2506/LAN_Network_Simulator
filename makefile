CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -std=c11
CXXFLAGS = -Wall -Wextra -std=c++17

TARGET = MS2503

all: $(TARGET)

$(TARGET): MS2503_main.o MS2503_fun.o
	$(CXX) $^ -o $@

MS2503_main.o: MS2503_main.cpp MS2503_fun.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

MS2503_fun.o: MS2503_fun.c MS2503_fun.h MS2503_param.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) MS2503_main.o MS2503_fun.o
