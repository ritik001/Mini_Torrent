CC = g++
CFLAGS = -Wall -std=c++1z
FLAGS= -I. -L.
DEPS = Server.h
OBJ = Server.o

%.o: %.cpp $(DEPS)
	g++ -c -std=c++11 Server.h
	$(CC) $(CFLAGS) $(FLAGS) -c -o $@ $< 

Final_Result: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

clean :
	rm $(OBJ)

