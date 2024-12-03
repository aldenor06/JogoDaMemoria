#include "jogodamemoria.h"
#include <QKeyEvent>
#include <GL/gl.h>
#include <GL/glu.h>
#include <QLabel>
#include <QTimer>
#include <QPushButton>
#include <QApplication>
#include <QGuiApplication>
#include <QProcess>
#include <QDebug>


GLint cartaSelecionada = 6;
void comparaCarta();
void inicializarCartas();
carta cartas[8];
int contAcertos = 0;
int cartaA = -1;
int cartaB = -1;
int cartaE = -1;
GLfloat aspecto, up = 0, escala = 1;
GLint largura, altura, ang = 0;
bool girar = false;

JogoDaMemoria::JogoDaMemoria()
{
    // Chama a função para fixar o tamanho da janela
    fixWindowSize();

    // Faz com que a janela nao possa ser fixada
    setWindowFlags(windowFlags() & ~Qt::WindowMaximizeButtonHint);

    // Define o título da janela
    setWindowTitle("Jogo da Memória");

    // Inicializa o QLabel e o QPushButton
    label = new QLabel();
    button = new QPushButton("& Fechar", label);

    // Conecta o botão à ação de fechar o label
    connect(button, SIGNAL(clicked()), label, SLOT(close()));

    // Inicializa os timers
    timerReset = new QTimer(this);
    timerJogar = new QTimer(this);
    timer = new QTimer(this);

    // Configura o timer principal como de disparo único
    timer->setSingleShot(true);

    // Conecta o timer principal ao método updateGL para atualizar a tela
    connect(timer, SIGNAL(timeout()), this, SLOT(updateGL()));

    // Define o estado inicial do jogo como jogável
    jogavel = true;

    // Inicializa as cartas do jogo
    inicializarCartas();
}
void JogoDaMemoria::fixWindowSize() {
    setFixedSize(800, 600); // Define o tamanho fixo da janela
}



JogoDaMemoria::~JogoDaMemoria() {}

void JogoDaMemoria::initializeGL()
{
    glShadeModel(GL_SMOOTH);
    qglClearColor(Qt::white);
    glClearDepth(1.0f);
    glClearStencil(0);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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

void JogoDaMemoria::resizeGL(int width, int height)
{
    if (height == 0)
        height = 1;
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

void JogoDaMemoria::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    desenhaBackground();
    for (int i = 0; i < 8; i++)
    {
        DesenhaCarta(i == cartaSelecionada, -0.7 + 0.38 * (i % 4), i < 4 ? 0.8 : -0.14, cartas[i]);
    }
}

void JogoDaMemoria::DesenhaCarta(bool selecionado, float x_init, float y_init, carta carta)
{
    if (girar && selecionado)
    {
        glPushMatrix();
        ang += 1;
        up += 0.94 / 180;
        escala += ang <= 90 ? (float)1 / 90 : (float)-1 / 90;
        float y_up = y_init > 0 ? -up : up;
        glRotatef(y_init > 0 ? ang : -ang, 1, 0, 0);
        glTranslatef(0, y_up, 0);
        glScalef(escala, escala, 1);
        timer->start(5);
    }
    else if (carta.escolhida || carta.id == cartaE)
    {
        glRotatef(-180, 1, 0, 0);
        y_init += y_init > 0 ? -0.94 : 0.94;
    }
    glColor3f(0.7, 0.7, 0.7);
    glBindTexture(GL_TEXTURE_2D, _fundoTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x_init, y_init, 0.004);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x_carta + x_init, y_init, 0.004);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x_carta + x_init, y_carta + y_init, 0.004);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x_init, y_carta + y_init, 0.004);
    glEnd();
    glColor3f(0.7, 0.7, 0.7);
    glBindTexture(GL_TEXTURE_2D, _frenteTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x_init, y_init, 0);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x_carta + x_init, y_init, 0);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x_carta + x_init, y_carta + y_init, 0);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x_init, y_carta + y_init, 0);
    glEnd();
    if (carta.figura == 0)
    {
        DesenhaCubo(x_init, y_init);
    }
    else if (carta.figura == 1)
    {
        DesenhaTriangulo(x_init, y_init);
    }
    else if (carta.figura == 2)
    {
        DesenhaIgual(x_init, y_init);
    }
    else if (carta.figura == 3)
    {
        DesenhaLosangulo(x_init, y_init);
    }

    if (carta.escolhida)
    {
        glColor3f(0, 1, 0);
    }
    else if (carta.id == cartaE || (cartaE > -1 && girar && selecionado))
    {
        glColor3f(1, 0, 0);
    }
    else
    {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    if (selecionado)
    {
        glColor3f(0.0f, 0.0f, 0.0f);
    }
    glLineWidth(3.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_init, y_init, 0.004);
    glVertex3f(x_carta + x_init, y_init, 0.004);
    glVertex3f(x_carta + x_init, y_carta + y_init, 0.004);
    glVertex3f(x_init, y_carta + y_init, 0.004);
    glEnd();
    glBegin(GL_LINE_LOOP);
    glVertex3f(x_init, y_init, 0);
    glVertex3f(x_carta + x_init, y_init, 0);
    glVertex3f(x_carta + x_init, y_carta + y_init, 0);
    glVertex3f(x_init, y_carta + y_init, 0);
    glEnd();
    if (girar && selecionado)
    {
        glPopMatrix();
        if (ang == 180)
        {
            up = 0;
            ang = 0;
            timer->stop();
            girar = false;
        }
    }
    else if (carta.escolhida || carta.id == cartaE)
    {
        glRotatef(180, 1, 0, 0);
    }
}

void JogoDaMemoria::exibeTexto()
{
    // Configuração do label
    label->setFrameStyle((QFrame::Panel | QFrame::Sunken));
    label->setAutoFillBackground(true);
    label->setAlignment((Qt::AlignCenter));
    label->move(view_w / 4, view_h / 2);
    label->resize(800, 200);
    label->setStyleSheet("QLabel { background-color :#37374e; color : blue; font:50px }");
    label->setText("Parabéns, você ganhou!!");
    label->show();

    // Configuração do botão "Fechar"
    button->setText("Fechar");
    button->setStyleSheet("QPushButton {background-color:#d91a27;font:bold;font-size:13px;}");
    button->move(260, 150);
    button->resize(80, 50);
    button->show();
    connect(button, &QPushButton::clicked, this, &QWidget::close);

    // Configuração do botão "Jogar novamente"
    QPushButton *playAgainButton = new QPushButton("Jogar novamente", this);
    playAgainButton->setStyleSheet("QPushButton {background-color:#1a73e8;font:bold;font-size:13px;}");
    playAgainButton->move(360, 150);
    playAgainButton->resize(120, 50);
    playAgainButton->show();

    // Conexão para reiniciar o jogo
    connect(playAgainButton, &QPushButton::clicked, this, [this]() {
        // Esconde a janela atual antes de sair
        this->hide();

        // Caminho para o executável
        QString command = "./JogoDaMemoria";

        // Reinicia o jogo
        if (!QProcess::startDetached(command)) {
            qDebug() << "Erro ao reiniciar o jogo.";
        }

        // Sai da aplicação atual
        QGuiApplication::quit();
    });
}


void JogoDaMemoria::DesenhaCubo(float x_init, float y_init)
{
    x_init = x_init + x_carta / 5;
    y_init = y_init + y_carta / 4;
    float x_end = x_init + 3 * x_carta / 5;
    float y_end = y_init + 2 * y_carta / 4;

    glColor3f(0, 0, 0);
    glBegin(GL_QUADS);
    glVertex3f(x_init, y_init, -0.01);
    glVertex3f(x_end, y_init, -0.01);
    glVertex3f(x_end, y_end, -0.01);
    glVertex3f(x_init, y_end, -0.01);
    glEnd();
}

void JogoDaMemoria::DesenhaTriangulo(float x_init, float y_init)
{
    x_init = x_init + x_carta / 5;
    y_init = y_init + y_carta / 4;
    float x_end = x_init + 3 * x_carta / 5;
    float y_end = y_init + 2 * y_carta / 4;

    glColor3f(1, 0, 0);
    glBegin(GL_POLYGON);
    glVertex3f(x_end / 2 + x_init / 2, y_end, -0.01); 
    glVertex3f(x_end, y_init, -0.01);                
    glVertex3f(x_init, y_init, -0.01);               
    glEnd();
}

void JogoDaMemoria::DesenhaIgual(float x_init, float y_init)
{
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

void JogoDaMemoria::DesenhaLosangulo(float x_init, float y_init)
{

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

void JogoDaMemoria::desenhaBackground()
{
    glColor3f(0.7, 0.7, 0.7);
    glBindTexture(GL_TEXTURE_2D, _backgroundTexture);
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-2, -2, -0.5);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-2, 2, -0.5);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(2, 2, -0.5);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(2, -2, -0.5);
    glEnd();
}

void resetarCarta(int indiceCarta)
{

    cartas[indiceCarta].escolhida = false;
    cartas[indiceCarta].id = false;
}

void comparaCarta()
{
    if (cartaA >= 0 && cartaB >= 0)
    {

        if (cartas[cartaA].figura != cartas[cartaB].figura)
        {

            resetarCarta(cartaA);
            resetarCarta(cartaB);
        }
        else
        {

            contAcertos++;
        }
        cartaE = cartaA;
        cartaA = -1;
        cartaB = -1;
    }
}

int JogoDaMemoria::getCartaIndex(int x, int y)
{
    float normalizedX = (2.0f * x) / width() - 1.0f;
    float normalizedY = 1.0f - (2.0f * y) / height();
    float cartaWidth = 0.38f;
    float cartaHeight = 0.94f;
    float startX = -0.7f;
    float startY = 0.8f;
    int linha = -1;
    if (normalizedY >= startY - cartaHeight && normalizedY <= startY)
    {
        linha = 0;
    }
    else if (normalizedY >= startY - 2 * cartaHeight && normalizedY <= startY - cartaHeight)
    {
        linha = 1;
    }
    int coluna = -1;
    float adjustedX = normalizedX;
    for (int i = 0; i < 4; i++)
    {
        float cardLeft = startX + (i * cartaWidth);
        float cardRight = cardLeft + x_carta;

        if (adjustedX >= cardLeft && adjustedX <= cardRight)
        {
            coluna = i;
            break;
        }
    }

    if (linha == -1 || coluna == -1)
    {
        return -1;
    }

    return linha * 4 + coluna;
}

void JogoDaMemoria::mousePressEvent(QMouseEvent *event)
{
    if (!jogavel)
        return;

    if (contAcertos == (sizeof(cartas) / sizeof(cartas[0])) / 2)
    {
        exibeTexto();
    }

    if (event->button() == Qt::LeftButton && !girar)
    {
        int cartaIndex = getCartaIndex(event->x(), event->y());

        if (cartaIndex >= 0 && cartaIndex < 8 && !cartas[cartaIndex].escolhida)
        {
            cartas[cartaIndex].escolhida = true;
            girar = true;
            cartaSelecionada = cartaIndex;

            if (cartaA == -1)
            {
                cartaA = cartaIndex;
            }
            else
            {
                cartaB = cartaIndex;

                jogavel = false;

                timerReset->singleShot(2000, this, [=]()
                                       {
                    if (cartas[cartaA].figura != cartas[cartaB].figura)
                    {
                        resetarCarta(cartaA);
                        resetarCarta(cartaB);
                    }
                    else
                    {
                        contAcertos++;
                    }
                    cartaA = -1;
                    cartaB = -1;
                    jogavel = true; 
                    updateGL(); });
            }

            updateGL();
        }
    }
}

void JogoDaMemoria::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Escape:
        close();
        break;
    default:
        QGLWidget::keyPressEvent(event);
    }
}

void JogoDaMemoria::changeEvent(QEvent *event)
{
    switch (event->type())
    {
    case QEvent::WindowStateChange:
        if (windowState() == Qt::WindowFullScreen)
            setCursor(Qt::BlankCursor);
        else if (windowState() == Qt::WindowMaximized)
        {
            update();
        }
        else
            unsetCursor();
        break;
    default:
        break;
    }
}

void inicializarCartas()
{
    srand(time(NULL));
    int QtdFig0 = 0;
    int QtdFig1 = 0;
    int QtdFig2 = 0;
    int QtdFig3 = 0;
    bool pares;
    for (int i = 0; i < 8; i++)
    {
        pares = false;
        int fig = rand() % 4;
        while (!pares)
        {
            pares = true;
            if (QtdFig0 == 2 && fig == 0)
            {
                fig = 1;
                pares = false;
            }
            if (QtdFig1 == 2 && fig == 1)
            {
                fig = 2;
                pares = false;
            }
            if (QtdFig2 == 2 && fig == 2)
            {
                fig = 3;
                pares = false;
            }
            if (QtdFig3 == 2 && fig == 3)
            {
                fig = 0;
                pares = false;
            }
        }
        QtdFig0 += fig == 0 ? 1 : 0;
        QtdFig1 += fig == 1 ? 1 : 0;
        QtdFig2 += fig == 2 ? 1 : 0;
        QtdFig3 += fig == 3 ? 1 : 0;
        cartas[i].figura = fig;
        cartas[i].id = i;
    }
}