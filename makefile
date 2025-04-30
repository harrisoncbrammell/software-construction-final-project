TARGET = main
CODE = Project.cpp

make: clean
	g++ $(CODE) -o $(TARGET).elf -std=c++14
	g++ $(CODE) -o $(TARGET)_debug.elf -std=c++14 -g

clean:
	rm -f $(TARGET).elf
run: make
	./$(TARGET).elf
