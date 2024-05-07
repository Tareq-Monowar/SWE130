all:
	g++ -I src/include -L src/lib -o TASK101 TASK101.cpp -lmingw32 -lSDL2main -lSDL2