#include "engine.h"

//Camera
float camX = 00, camY = 30, camZ = 40;
int startX, startY, tracking = 0;
int alpha = 0, beta = 45, r = 50;
int vel = 1;

//Eixo e Trajetoria
bool exis = false;
bool traj = true;

Transformacao* transf;

//Mapa fps
int frame = 0;
int tempoFps = 0;

//Para Tempo de anima��es
float tempoAnterior = 0;
float tempoAtual;
float tempo = 0;

GLuint* figures, *normais, *texturas, *trajetorias;

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void atualizaTempo() {
    tempoAtual = glutGet(GLUT_ELAPSED_TIME);
    float delta = tempoAtual - tempoAnterior;
    tempoAnterior = tempoAtual;
    tempo += vel*delta;
}

void mostraFPS() {
    frame++;
    float fps;
    float tempoIntervaloFps;
    int tempoF = glutGet(GLUT_ELAPSED_TIME);
    char s[100];
    if (tempoF - tempoFps > 1000) {                 //Se passou 1 segundo
        tempoIntervaloFps = tempoF - tempoFps;
        fps = frame * 1000.0 / tempoIntervaloFps;
        tempoFps = tempoF;
        frame = 0;
        sprintf(s, "Engine FPS: %2.f", fps);  //2.f � para limitar a 2 algarismos decimais
        glutSetWindowTitle(s);
    }
}

void drawExis() {
    glBegin(GL_LINES);
    //eixo do X em Vermelho
    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(-1000.0f, 0.0f, 0.0f);
    glVertex3f(1000.0f, 0.0f, 0.0f);
    //eixo do Y em Verde
    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f, -1000.0f, 0.0f);
    glVertex3f(0.0f, 1000.0f, 0.0f);
    //eixo do Z em Azul
    glColor3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.0f, 0.0f, -1000.0f);
    glVertex3f(0.0f, 0.0f, 1000.0f);
    glEnd();
}


void renderScene() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(camX, camY, camZ,
        0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f);

    mostraFPS();
    atualizaTempo();
    transf->apllyTransformation(tempo,traj);
    if (exis) drawExis();
    // End of frame
    glutSwapBuffers();
}

void processMouseButtons(int button, int state, int xx, int yy) {
    if (state == GLUT_DOWN) {
        startX = xx;
        startY = yy;
        if (button == GLUT_LEFT_BUTTON)
            tracking = 1;
        else if (button == GLUT_RIGHT_BUTTON)
            tracking = 2;
        else
            tracking = 0;
    }
    else if (state == GLUT_UP) {
        if (tracking == 1) {
            alpha += (xx - startX);
            beta += (yy - startY);
        }
        else if (tracking == 2) {

            r -= yy - startY;
            if (r < 3)
                r = 3.0;
        }
        tracking = 0;
    }
}
void processMouseMotion(int xx, int yy) {
    int deltaX, deltaY;
    int alphaAux, betaAux;
    int rAux;

    if (!tracking)
        return;

    deltaX = xx - startX;
    deltaY = yy - startY;

    if (tracking == 1) {


        alphaAux = alpha + deltaX;
        betaAux = beta + deltaY;

        if (betaAux > 85.0)
            betaAux = 85.0;
        else if (betaAux < -85.0)
            betaAux = -85.0;

        rAux = r;
    }
    else if (tracking == 2) {

        alphaAux = alpha;
        betaAux = beta;
        rAux = r - deltaY;
        if (rAux < 3)
            rAux = 3;
    }
    camX = rAux * sin(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camZ = rAux * cos(alphaAux * 3.14 / 180.0) * cos(betaAux * 3.14 / 180.0);
    camY = rAux * sin(betaAux * 3.14 / 180.0);
}

void tecladoNormal(unsigned char tecla, int x, int y) {
    switch (tecla) {
    case 'e':
        exis = !exis;
        break;
    case 't':
        traj = !traj;
        break;
    case '+':
        vel += 1;
        break;
    case '-':
        vel -= 1;
        break;
    default:
        break;
    }
    //glutPostRedisplay();
}
void menu_frontal(int op) {
    switch (op) {
    case 1:
        glPolygonMode(GL_FRONT, GL_POINT);
        break;
    case 2:
        glPolygonMode(GL_FRONT, GL_LINE);
        break;
    case 3:
        glPolygonMode(GL_FRONT, GL_FILL);
        break;
    default:
        break;
    }
    //glutPostRedisplay();
}


void getTransf(string f) {
	transf = new Transformacao();
    leitorXML *a = new leitorXML(f, transf);
    transf = a->leXML();
    GLuint nFig = 0, nTranslatedAnimated = 0;
    nFig = transf->getSizeModelos();                       
    nTranslatedAnimated = transf->getNTranslatedAnimated();

    figures = new GLuint[nFig]();
    glGenBuffers(nFig, figures);
    normais = new GLuint[nFig]();
    glGenBuffers(nFig, normais);
    texturas = new GLuint[nFig]();
    glGenBuffers(nFig, texturas);
    trajetorias = new GLuint[nTranslatedAnimated]();
    glGenBuffers(nTranslatedAnimated, trajetorias);
     
    transf->prepare(figures,normais,texturas,trajetorias);

	cout << "Cenario: " << f << " carregado!" << endl << endl << endl;
    transf->toString();       
	cout << endl << endl;
}

int main(int argc, char **argv) {
    //Iniciar Janela
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Engine");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);
    glutIdleFunc(renderScene);

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(tecladoNormal);
    glutMouseFunc(processMouseButtons);
    glutMotionFunc(processMouseMotion);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glEnable(GL_NORMALIZE);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LIGHTING);

    //GlewInit
    glewInit();
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);

    //Carregar o ficheiro xml
    getTransf("../files/sistemaAnimado.xml");

    cout << endl << "Use E to turn Exis" << endl;
    cout << endl << "Use T to turn Trajectory" << endl;
   
    //Menus
        glutCreateMenu(menu_frontal);
        glutAddMenuEntry("GL POINT", 1);
        glutAddMenuEntry("GL LINE", 2);
        glutAddMenuEntry("GL FILL", 3);

        glutAttachMenu(1);
    
    // enter GLUT's main cycle
    glutMainLoop();
	return 1;
    
}
