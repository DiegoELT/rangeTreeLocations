all:
	g++ -std=c++17 -o main -framework OpenGL -framework GLUT main.cpp -Wall

clean:
	rm main