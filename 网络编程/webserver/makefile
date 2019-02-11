CC = g++
CXXFLAGS=-lpthread -g
target: main.cpp func.o ThreadPool.o
	$(CC) main.cpp func.o ThreadPool.o -o main $(CXXFLAGS)
func.o: func.cpp
	$(CC) -c func.cpp $(CXXFLAGS)
ThreadPool.o: ThreadPool.cpp
	$(CC) -c ThreadPool.cpp $(CXXFLAGS)
clean:
	rm *.o
