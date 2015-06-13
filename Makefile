CC = g++
FLAGS = -O3
INCUDE = -I./src/
FB = -ly -ll
all: shell 
shell: main ast table converter tester power
	$(CC) $(FLAGS) $(FB) main.o ast.o table.o converter.o tester.o power.o -o shell
main: src/main.cpp
	$(CC) $(FLAGS) -c src/main.cpp
power: src/power.cpp
	$(CC) $(FLAGS) -c src/power.cpp
ast: src/ast.cpp 
	$(CC) $(FLAGS) -c src/ast.cpp
table: src/table.cpp
	$(CC) $(FLAGS) -c src/table.cpp
tester: src/tester.cpp
	$(CC) $(FLAGS) -c src/tester.cpp
converter: src/converter.cpp	
	$(CC) $(FLAGS) -c src/converter.cpp
clean:
	rm -rf *.o cda


