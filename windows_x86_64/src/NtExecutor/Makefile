CC = x86_64-w64-mingw32-g++
CFLAGS = -Wall -I"include" -mwindows -static
LDFLAGS = -lntdll -lpsapi
TARGET = bin/NtExecutor.exe
SRCS = src/Alloc.cpp src/jsoncpp.cpp src/main.cpp src/Output.cpp src/RunSeed.cpp src/SetupArg.cpp
OBJS = $(SRCS:src/%.cpp=bin/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS) $(LDFLAGS)

bin/%.o: src/%.cpp | bin
	$(CC) $(CFLAGS) -c $< -o $@

bin:
	mkdir -p bin

clean:
	rm -f bin/*.o $(TARGET)

.INTERMEDIATE: $(OBJS)

.PHONY: all clean
