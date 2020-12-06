all:
	g++ -std=c++17 -framework OpenGL -framework GLUT main.cpp -Wall

clean:
	rm a.out