CC = gcc # Compiler
CFLAGS = -ansi -Wall -pedantic -g # Compiler flags

# Source files
SOURCES = SourceFiles/driver.c SourceFiles/preprocessor.c SourceFiles/utils.c SourceFiles/cleanfile.c SourceFiles/first_second_pass.c SourceFiles/line.c SourceFiles/symbol.c SourceFiles/macro.c SourceFiles/binary_line.c SourceFiles/error_handle.c

OBJECTS = $(SOURCES:.c=.o) # Object files
TARGET = driver.exe # Target executable

# create the executable
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

# compile .c files to .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# clean the object files and the executable
clean:
	rm -f $(OBJECTS) $(TARGET)

