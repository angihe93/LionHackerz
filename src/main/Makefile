# Compiler
CC = g++

# Compiler flags
CFLAGS = -I../../include -Wall -g

# Linker flags for libcurl
LDFLAGS = -lcurl

# Source files
SRC = Main.cpp Matcher.cpp Database.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Output executable
TARGET = my_program

# Build the program
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) $(LDFLAGS)

# Rule to compile .cpp files to .o files
%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(TARGET)
