#example: clang++ src/main.cpp /usr/local/lib/libconfig++.a -o example

CC = clang++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall -std=c++17
  
  
# the name to use for both the target source file, and the output file:
TARGET = example

MAIN = target/main.o
CONFIG = target/config.o
SERVER = target/server.o

all: $(TARGET)
  
$(TARGET): src/main.cpp
	$(CC) $(CFLAGS) -o target/$(TARGET) src/main.cpp /usr/local/lib/libconfig++.a

torcd: $(MAIN) $(CONFIG) $(SERVER)
	$(CC) $(MAIN) $(CONFIG) $(SERVER) -o target/torcd /usr/local/lib/libconfig++.a -pthread

$(MAIN):
	$(CC) $(CFLAGS) -c -o $(MAIN) src/main.cpp 

$(CONFIG):
	$(CC) $(CFLAGS) -c src/config.cpp -Iinclude/config.hpp -o $(CONFIG)

$(SERVER):
	$(CC) $(CFLAGS) -c src/server.cpp -Iinclude/server.hpp -o $(SERVER)

clean:
	rm target/*.o target/torcd
	#rm include/*.gch
