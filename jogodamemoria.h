#ifndef JOGODAMEMORIA_H
#define JOGODAMEMORIA_H

#include <QGLWidget>
#include <QTime>
#include <QLabel>
#include <QPushButton>

class QTimer;
class QLabel;
class QPushButton;

struct
{
    bool escolhida = false;
    int figura = 1;
    int id = 0;
} typedef carta;

class JogoDaMemoria : public QGLWidget
{
    Q_OBJECT

public:
    JogoDaMemoria(QWidget *parent) : QGLWidget(parent) {};
    JogoDaMemoria();
    ~JogoDaMemoria();
    GLfloat x_carta = 0.33;
    GLfloat y_carta = -0.66;
    GLfloat win = 1;
    GLint view_w, view_h;
private:
    QTimer *timer;
    QTimer *timerReset;
    QTimer *timerJogar;
    bool jogavel = true;
    QLabel *label;
    QPushButton *button;
    GLuint _fundoTexture, _frenteTexture, _backgroundTexture;
};
#endif