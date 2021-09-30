CC := g++

# Compile flags
CFLAGS := -std=c++17 -Wall -no-pie

# Additional libs
CLIBS := -lpthread

all: main

main:
	clear
	@echo "Compiling main.cpp ... "
	$(CC) $(CFLAGS) $(CLIBS) I2CBusController.cpp main.cpp MCP23017Controller.cpp BMP180Controller.cpp -o main.out

clean:
	rm -rf main
