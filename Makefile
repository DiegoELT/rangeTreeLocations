all:
	g++ -std=c++17 -framework OpenGL -framework GLUT experimentos.cpp -Wall

clean:
	rm a.out
