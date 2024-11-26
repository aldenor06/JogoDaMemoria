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
