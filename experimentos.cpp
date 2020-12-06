#define GL_SILENCE_DEPRECATION

#include <bits/stdc++.h>
#include <GLUT/glut.h>
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>

#include "rangetree.h"
#include "macros.h"

using namespace std;

typedef pair<int, int> pii;
#define rep(i, a, b) for (int i = a; i <= b; ++i)
#define F first
#define S second
#define PB push_back
#define MP make_pair

// Prototyping OpenGL functions
GLvoid initGL();
GLvoid window_display();
GLvoid window_reshape(GLsizei width, GLsizei height);
GLvoid draw_points(int argc, char** argv);
GLvoid mouse_click(int button, int state, int x, int y);
GLvoid key_press(int key, int x, int y);
GLvoid key_pressed(unsigned char key, int x, int y);

void show_menu();

enum Color {
     ROJO = 31,
     VERDE = 32,
     AMARILLO = 33,
     AZUL = 34,
};

void print_with_color(Color color, const string& str);

int biggestX, biggestY;
int queryX1, queryX2;
int queryY1, queryY2;
pii points[1000];
vector<pii> query_result;
Node* root;

int main(int argc, char** argv) {
    srand(time(nullptr));

    // generar los 1000 puntos
    rep(i, 1, 1000) {
        int xval = rand() % (int)WINDOW_WIDTH;
        int yval = rand() % (int)WINDOW_HEIGHT;
        if (xval > biggestX) biggestX = xval;
        if (yval > biggestY) biggestY = yval;
        points[i-1] = MP(xval, yval);
    }

    // rep(i, 0, 999) {
    //     cout << points[i].F << ", " << points[i].S << '\n';
    // }
    // 

    sort(points, points+999);

    // crear el range tree
    root = create_range_tree(points, 0, 999, nullptr, true).F;

    /// Dibujar puntos

    // PARA 
    // draw_points(argc, argv);

    // descomentar para pruebas de stress
    int n = 10; // 100, 1000, 10000
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; ++i) {
        twoDimensionalQuery(root, rand()%500, rand()%500+499, rand()%500, rand()%500+499);
        // aveces da segfault porque X1 > X2 o Y1 > Y2 (osea el rango es vacío), ya que estamos usando numeros random
    }
    auto end = chrono::high_resolution_clock::now();
    auto executionTime = chrono::duration_cast<chrono::microseconds>(end - start);
    cout << "Query returned in : " << executionTime.count() << " us.\n";

    cout << "Programa terminando..." << endl;

    delete root;

    return 0;
}

void show_menu() {
    system("clear -x");
    print_with_color(ROJO, "Bienvenido al programa. Presionar 'q' para salir del programa.");
    print_with_color(AZUL, "La mayor coordenada en X es: " + to_string(biggestX));
    print_with_color(AZUL, "La mayor coordenada en Y es: " + to_string(biggestY));
    print_with_color(VERDE, "Ingresar rango.");
    print_with_color(VERDE, "Valor de X1: ");
    cin >> queryX1;
    print_with_color(VERDE, "Valor de X2: ");
    cin >> queryX2;
    print_with_color(VERDE, "Valor de Y1: ");
    cin >> queryY1;
    print_with_color(VERDE, "Valor de Y2: ");
    cin >> queryY2;
    query_result = twoDimensionalQuery(root, queryX1, queryX2, queryY1, queryY2);
    // query_result = oneDimensionalQuery(root->rootOfOwnRT->m_pSon[0]->m_pSon[0], 0, 999);
    cout << "Query result:\n";
    for (auto& par : query_result) {
        cout << par.F << ", " << par.S << "\n";
    }
}

GLvoid initGL() {
    glClearColor(RED, GREEN, BLUE, ALPHA);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
}

GLvoid window_display() {
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.0f, Z_DIST);

    /// Esto dibuja los puntos

    for (unsigned i = 0; i < 1000; ++i) {
        glPushMatrix();
            glTranslatef(points[i].F, points[i].S, 0);
            glColor3f(SPHERE_RED, SPHERE_GREEN, SPHERE_BLUE);
            glutWireSphere(NODE_RADIUS, SPHERE_SLICES, SPHERE_STACKS);
        glPopMatrix();
    }

    /// Esto dibuja las lineas que representan el query

    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex2f(queryX1, 0);
        glVertex2f(queryX1, WINDOW_HEIGHT);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(1.0f, 0.0f, 1.0f);
        glVertex2f(queryX2, 0);
        glVertex2f(queryX2, WINDOW_HEIGHT);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0, queryY1);
        glVertex2f(WINDOW_WIDTH, queryY1);
    glEnd();

    glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 1.0f);
        glVertex2f(0, queryY2);
        glVertex2f(WINDOW_WIDTH, queryY2);
    glEnd();

    // esto dibuja los puntos que el query retornó

    for (unsigned i = 0; i < query_result.size(); ++i) {
        glPushMatrix();
            glTranslatef(query_result[i].F, query_result[i].S, 0);
            glColor3f(1.0f, 0.0f, 0.0f);
            glutWireSphere(1.5f, SPHERE_SLICES, SPHERE_STACKS);
        glPopMatrix();
    }

    glutSwapBuffers();
    glFlush();
}

GLvoid window_reshape(GLsizei width, GLsizei height) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, width, 0.0f, height, 0.0f, Z_DIST);
    WINDOW_WIDTH = width;
    WINDOW_HEIGHT = height;
    glMatrixMode(GL_MODELVIEW);
}

GLvoid draw_points(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT); // define window size
    glutInitWindowPosition(WINDOW_X_POS, WINDOW_Y_POS); // donde se va a renderizar la pantalla
    glutCreateWindow("Range Tree Queries"); // titulo de la pantalla
    initGL(); // crear la ventana cargando la matriz identidad
    glEnable(GL_TEXTURE_2D); // habilitar funciones
    glutDisplayFunc(window_display);
    glutReshapeFunc(window_reshape);
    glutKeyboardFunc(key_pressed);
    glutMouseFunc(mouse_click);
    glutSpecialFunc(key_press);
    show_menu();
    glutMainLoop();
    // Unreachable code
}

void print_with_color(Color color, const string& str) {
    cout << "\033[1;" << color << "m"<< str << "\033[0m\n";
}

GLvoid mouse_click(int button, int state, int x, int y) {
    // cout << "x: " << x << " y: " << y << '\n';
}

GLvoid key_press(int key, int x, int y) {
    if (key == GLUT_KEY_LEFT) {
        cout << "Left arrow key pressed!\n";
    } else if (key == GLUT_KEY_RIGHT) {
        cout << "Right arrow key pressed!\n";
    }
}

GLvoid key_pressed(unsigned char key, int x, int y) {
    if (key == 'q') exit(0);
    cout << "key " << key << " pressed\n";
}