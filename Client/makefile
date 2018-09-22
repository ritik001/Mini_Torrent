CC = g++
CFLAGS = -Wall -std=c++1z
FLAGS= -I. -L.
DEPS = Client.h
OBJ = Chunk_Split_Merger.o  Client.o

%.o: %.cpp $(DEPS)
	g++ -c -std=c++11 Client.h
	$(CC) $(CFLAGS) $(FLAGS) -c -o $@ $< 

Final_Result: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^ 

clean :
	rm $(OBJ)

