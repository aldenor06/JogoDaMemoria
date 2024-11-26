#include "jogodamemoria.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include <QLabel>
#include <QTimer>
#include <QPushButton>

JogoDaMemoria::JogoDaMemoria()
{
    setWindowTitle("jogo da memória");
    label = new QLabel();
    button = new QPushButton("& Fechar", label);
    connect(button, SIGNAL(clicked()), label, SLOT(close()));
    timerReset = new QTimer(this);
    timerJogar = new QTimer(this);
    timer = new QTimer(this);
    timer->setSingleShot(true);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));
    jogavel = true;
}

JogoDaMemoria::~JogoDaMemoria() {}

void JogoDaMemoria::initializeGL() {
    view_w = 800;
    view_h = 600;
    glShadeModel(GL_SMOOTH); // Enable smooth shading
    qglClearColor(Qt::white);
    glClearDepth(1.0f);                                // Depth Buffer Setup
    glClearStencil(0);                                 // Stencil Buffer Setup
    glEnable(GL_DEPTH_TEST);                           // Enables Depth Testing
    glDepthFunc(GL_LEQUAL);                            // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Really Nice Perspective Calculations
                                                       // Habilita o uso de texturas
    glEnable(GL_TEXTURE_2D);
    QImage imgFundo = convertToGLFormat(QImage("fundo2.jpg"));
    QImage imgFrente = convertToGLFormat(QImage("frente2.jpg"));
    QImage imgbackground = convertToGLFormat(QImage("background1.jpg"));
    glGenTextures(1, &_fundoTexture);
    glBindTexture(GL_TEXTURE_2D, _fundoTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgFundo.width(), imgFundo.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgFundo.bits());
    glGenTextures(1, &_frenteTexture);
    glBindTexture(GL_TEXTURE_2D, _frenteTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgFrente.width(), imgFrente.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgFrente.bits());
    glGenTextures(1, &_backgroundTexture);
    glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imgbackground.width(), imgbackground.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, imgbackground.bits());
    GLfloat especularidade[4] = {1.0, 1.0, 1.0, 1.0};
    GLint especMaterial = 16;
    GLfloat ambLight[] = {1.f, 1.f, 1.f, 1.0f};
    GLfloat diffLight[] = {1.0f, 1.0f, 1.0f, 1.0f};
    GLfloat especLight[] = {1.0, 1.0, 1.0, 1.0};
    GLfloat lightPos[] = {0.0f, 0.0f, 2.0f, 1.0f};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, especularidade);
    glMateriali(GL_FRONT, GL_SHININESS, especMaterial);
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambLight);
    glLightfv(GL_LIGHT1, GL_AMBIENT, ambLight);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffLight);
    glLightfv(GL_LIGHT1, GL_SPECULAR, especLight);
    glLightfv(GL_LIGHT1, GL_POSITION, lightPos);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glEnable(GL_DEPTH_TEST);
}

void JogoDaMemoria::resizeGL(int width, int height){
    if (height == 0) height = 1;
    view_w = width;
    view_h = height;
    glViewport(0, 0, view_w, view_h); 
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); 
    gluPerspective(8.f, static_cast<GLfloat>(width) / height, 5.f, 250.0f);
    gluLookAt(0.f, -0.5f, 15.f, 0, 0, 0, 0, 1, 0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void JogoDaMemoria::DesenhaIgual(float x_init, float y_init){
    x_init = x_init + x_carta / 5;
    y_init = y_init + y_carta / 4;
    float x_end = x_init + 3 * x_carta / 5;
    float y_end = y_init + 2 * y_carta / 4;

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(x_init, y_init, -0.01);
    glVertex3f(x_end, y_init, -0.01);
    glVertex3f(x_end, y_init + 1.5 * y_carta / 8, -0.01);
    glVertex3f(x_init, y_init + 1.5 * y_carta / 8, -0.01);

    glVertex3f(x_init, y_init + 2.5 * y_carta / 8, -0.01);
    glVertex3f(x_end, y_init + 2.5 * y_carta / 8, -0.01);
    glVertex3f(x_end, y_end, -0.01);
    glVertex3f(x_init, y_end, -0.01);
    glEnd();
}

void JogoDaMemoria::DesenhaLosangulo(float x_init, float y_init){

    x_init = x_init + x_carta / 5;
    y_init = y_init + y_carta / 4;
    float x_end = x_init + 3 * x_carta / 5;
    float y_end = y_init + 2 * y_carta / 4;

    glColor3f(212, 37, 37);
    glBegin(GL_QUADS);
    glVertex3f(x_init, (y_init + y_end) / 2, -0.01);
    glVertex3f((x_init + x_end) / 2, y_init, -0.01);
    glVertex3f(x_end, (y_init + y_end) / 2, -0.01);
    glVertex3f((x_init + x_end) / 2, y_end, -0.01);
    glEnd();
}