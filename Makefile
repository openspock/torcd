LIB_CONFIG = libconfig-1.7.2

CC = clang++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
ifeq ($(DEBUG), true)
	CFLAGS  = -g -Wall -std=c++17
else
	CFLAGS = -std=c++17
endif
  
  
# the name to use for both the target source file, and the output file:
TARGET = torcd

MAIN = target/main.o
CONFIG = target/config.o
SERVER = target/server.o

all: $(TARGET)

torcd: $(MAIN) $(CONFIG) $(SERVER)
	$(CC) $(MAIN) $(CONFIG) $(SERVER) -o target/torcd target/$(LIB_CONFIG)/lib/libconfig++.a -pthread

$(MAIN):
	$(CC) $(CFLAGS) -c -o $(MAIN) src/main.cpp 

$(CONFIG):
	$(CC) $(CFLAGS) -c src/config.cpp -Iinclude -o $(CONFIG)

$(SERVER):
	$(CC) $(CFLAGS) -c src/server.cpp -Iinclude -o $(SERVER)

clean:
	rm target/*.o target/torcd