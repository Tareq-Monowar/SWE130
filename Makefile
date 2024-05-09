all:
	g++ -I src/include -L src/lib -o TASK102 TASK102.cpp -lmingw32 -lSDL2main -lSDL2