CC := g++

# Compile flags
CFLAGS := -std=c++17 -Wall -no-pie

# Additional libs
CLIBS := -lpthread

all: main

main:
	clear
	@echo "Compiling main.cpp ... "
	$(CC) $(CFLAGS) $(CLIBS) I2CBusController.cpp BMP180Controller.cpp main.cpp -o main.out

clean:
	rm -rf main
