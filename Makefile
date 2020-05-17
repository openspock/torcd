#example: clang++ src/main.cpp /usr/local/lib/libconfig++.a -o example

CC = clang++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS  = -g -Wall
  
  
# the name to use for both the target source file, and the output file:
TARGET = example
  
all: $(TARGET)
  
$(TARGET): src/main.cpp
	$(CC) $(CFLAGS) -o target/$(TARGET) src/main.cpp /usr/local/lib/libconfig++.a 
