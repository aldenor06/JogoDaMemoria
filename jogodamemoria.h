#ifndef JOGODAMEMORIA_H
#define JOGODAMEMORIA_H

#include <QGLWidget>
#include <QTime>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QSize>
#define NUM_CARTAS 6  // Ou o número desejado de cartas


// Estrutura para representar uma carta do jogo
struct carta
{
    bool escolhida = false;
    int figura = 1;
    int id = 0;
};

// Classe principal do jogo da memória
class JogoDaMemoria : public QGLWidget
{
    Q_OBJECT

public:
    // Construtor e destrutor
    explicit JogoDaMemoria(QWidget *parent = nullptr);
    ~JogoDaMemoria();

    // Atributos para coordenadas e proporções
    GLfloat x_carta = 0.33;
    GLfloat y_carta = -0.66;
    GLfloat win = 1;
    GLint view_w, view_h;

protected:
    // Métodos de inicialização e renderização
    void initializeGL() override;
    void resizeGL(int width, int height) override;
    void paintGL() override;

    // Eventos
    void keyPressEvent(QKeyEvent *event) override;
    void changeEvent(QEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;

    // Métodos auxiliares
    void screenToOpenGL(int screenX, int screenY, float &glX, float &glY);
    void DesenhaCarta(bool selecionado, float x_init, float y_init, carta carta);
    void reiniciarJogo();
    void DesenhaCubo(float x_init, float y_init);
    void DesenhaTriangulo(float x_init, float y_init);
    void DesenhaIgual(float x_init, float y_init);
    void DesenhaLosangulo(float x_init, float y_init);
    void areaDesenho(int x_init, int y_init);
    void desenhaBackground();
    void exibeTexto();
    int getCartaIndex(int x, int y);

private:
    QTimer *timer;       // Timer para controle de animações
    QTimer *timerReset;  // Timer para resetar estado
    QTimer *timerJogar;  // Timer para intervalos de jogadas
    bool jogavel = true; // Controle do estado de jogabilidade

    QLabel *label;       // Label para exibir texto
    QPushButton *button; // Botão para ações no jogo

    // Texturas para renderização
    GLuint _fundoTexture, _frenteTexture, _backgroundTexture;

    // Variáveis para controle de reinicializações
    int contadorReinicializacoes = 0;  // Contador de reinicializações
    QLabel *contadorLabel;              // Label para exibir o contador de reinicializações

    // Método para fixar o tamanho da janela
    void fixWindowSize();
};

#endif // JOGODAMEMORIA_H
