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
protected:
    void initializeGL();
    void resizeGL(int width, int height);
    void exibeTexto();
    void DesenhaIgual(float x_init, float y_init);
    void DesenhaLosangulo(float x_init, float y_init);
    void DesenhaCubo(float x_init, float y_init);
    void DesenhaTriangulo(float x_init, float y_init);
    void DesenhaCarta(bool selecionado, float x_init, float y_init, carta carta);
    void paintGL();
    void desenhaBackground();
    
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