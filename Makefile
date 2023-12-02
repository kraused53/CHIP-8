TARGET = chip

# Compiler Flags
#	-Wvla   	-> Reports any instance of a variable length array declaration
#	-Werror 	-> Display warnings as errors
ERROR = -Wvla -Werror

# Compiler Warning Flags
#	-Wall       -> Compiler will list all warnings, no matter the severity
#	-Wshadow    -> Warns whenever a local variable shadows another variable
#	--pedantic	-> Forces compiler to follow strict ISO C guidelines
WARNING = -Wall -Wshadow --pedantic

# External Libraries
#
LIBS = -lSDL2

# Build the C Compiler Call
#	gcc			-> C Compiler
#	-g			-> Compile with debugging information (Allows GDB use)
#	$(WARNING)	-> Inserts the above WARNING variable
#	$(ERROR)	-> Inserts the above ERROR variable
GCC = gcc -g $(WARNING) $(ERROR)

# List of all c++ source files for this project
SRCS = main.c chip8.c

# Generates an object file for each given source file
OBJS = $(SRCS:%.c=%.o)

# Define how to compile program
$(TARGET): $(OBJS)
	$(GCC) $(OBJS) $(LIBS) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Make a rule for converting source files into object files
.c.o:
	$(GCC) -c $*.c

# Call "make clean" to remove all .o and compiled files
# Also, clear all output files
clean:
	rm -f $(OBJS) $(TARGET)