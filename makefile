TARGET = main
CODE = Project.cpp

# Define LOUD macro to enable screen clearing throughout the program
# Define HIDE_PASSWORD macro to hide password input in the console
# Define DEBUG macro to enable debug mode with additional information

make: clean
	g++ $(CODE) -o $(TARGET).elf -DHIDE_PASSWORD -std=c++14
	g++ $(CODE) -o $(TARGET)_showpw.elf -std=c++14
	g++ $(CODE) -o $(TARGET)_debug.elf -DDEBUG -std=c++14 -g
clean:
	rm -f $(TARGET).elf
	rm -f $(TARGET)_debug.elf
	rm -f $(TARGET)_showpw.elf
run_prod: make
	./$(TARGET).elf
run_debug: make
	./$(TARGET)_debug.elf
run_showpw: make
	./$(TARGET)_showpw.elf