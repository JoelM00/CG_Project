#include "engine.h"

float camDistCentro = 5.0f;
float camAlpha = 0.5f, camBeta = 0.5f;
float camX, camY, camZ;

vector<Ponto> pontos;

void posicionaCam() {
    camX = camDistCentro * cos(camBeta) * sin(camAlpha);
    camY = camDistCentro * sin(camBeta);
    camZ = camDistCentro * cos(camBeta) * cos(camAlpha);
}

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}


void desenhaPontos() {
    Ponto p1, p2, p3;
    int size = pontos.size();

    glBegin(GL_TRIANGLES);
    for (int i = 0; i < size; i += 6) {
        p1 = pontos.at(i);
        p2 = pontos.at(i + 1);
        p3 = pontos.at(i + 2);

        glColor3f(1, 0, 0);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);

        p1 = pontos.at(i + 3);
        p2 = pontos.at(i + 4);
        p3 = pontos.at(i + 5);

        glColor3f(0,0,1);
        glVertex3f(p1.x, p1.y, p1.z);
        glVertex3f(p2.x, p2.y, p2.z);
        glVertex3f(p3.x, p3.y, p3.z);
    }
    glEnd();
}


void renderScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glLoadIdentity();
    gluLookAt(camX, camY, camZ,
              0.0, 0.0, 0.0,
              0.0f, 1.0f, 0.0f);

    desenhaPontos();

    glutSwapBuffers();
}


void leModelo(string fich) {
    string linha;
    string novo;
    string delimiter = " ";
    int pos;
    float xx, yy, zz;
    Ponto p;

    ifstream file(fich);

    if (file.is_open()) {
        while (getline(file, linha)) {

            pos = linha.find(delimiter);
            novo = linha.substr(0, pos);
            xx = atof(novo.c_str());
            linha.erase(0, pos + delimiter.length());
            p.x = xx;

            pos = linha.find(delimiter);
            novo = linha.substr(0, pos);
            yy = atof(novo.c_str());
            linha.erase(0, pos + delimiter.length());
            p.y = yy;

            pos = linha.find(delimiter);
            novo = linha.substr(0, pos);
            zz = atof(novo.c_str());
            linha.erase(0, pos + delimiter.length());
            p.z = zz;

            pontos.push_back(p);
        }
        file.close();
    }
    else {
        cout << "ERRO AO LER FICHEIRO" << endl;
    }
}

void leXML(string f) {
    XMLDocument doc;
    XMLElement* root;

    if (!(doc.LoadFile(f.c_str()))) {
        root = doc.FirstChildElement();
        for (XMLElement* elem = root->FirstChildElement(); elem != NULL; elem = elem->NextSiblingElement()) {
            string modelo = elem->Attribute("file");
            cout << "Iniciando leitura do modelo: " << modelo << endl;
            leModelo(modelo);
        }
    } else {
        cout << "Erro ao ler XML!" << endl;
    }
}

void menu_frontal(int op) {
    switch (op) {
        case 1:
            glPolygonMode(GL_FRONT,GL_POINT);
            break;
        case 2:
            glPolygonMode(GL_FRONT,GL_LINE);
            break;
        case 3:
            glPolygonMode(GL_FRONT,GL_FILL);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void menu_traseiro(int op) {
    switch (op) {
        case 1:
            glPolygonMode(GL_BACK,GL_POINT);
            break;
        case 2:
            glPolygonMode(GL_BACK,GL_LINE);
            break;
        case 3:
            glPolygonMode(GL_BACK,GL_FILL);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void menu_cull(int op) {
    switch (op) {
        case 1:
            glEnable(GL_CULL_FACE);
            break;
        case 2:
            glDisable(GL_CULL_FACE);
            break;
        case 3:
            glCullFace(GL_FRONT);
            break;
        case 4:
            glCullFace(GL_BACK);
            break;
        default:
            break;
    }
    glutPostRedisplay();
}

void menu(int op) {}

void tecladoNormal(unsigned char c, int xx, int yy) {
    switch (c) {
        case 'w':
            camBeta += 0.1f;
            if (camBeta > 1.5f)
                camBeta = 1.5f;
            break;
        case 's':
            camBeta -= 0.1f;
            if (camBeta < -1.5f)
                camBeta = -1.5f;
            break;
        case 'a':
            camAlpha += 0.1;
            break;
        case '+':
            camDistCentro+= 0.1f;
            break;
        case '-':
            camDistCentro -= 0.1f;
            break;
        case 'd':
            camAlpha -= 0.1;
            break;
    }
    posicionaCam();
    glutPostRedisplay();
}


int main(int argc, char **argv) {

    string dir = "../Modelos/";
    string xml = "primitivas.xml";

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(1000,1000);
    glutCreateWindow("Engine");

    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    glutKeyboardFunc(tecladoNormal);

    leXML(dir + xml);
    posicionaCam();

    int menuFrontal = glutCreateMenu(menu_frontal);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);

    int menuTraseiro = glutCreateMenu(menu_traseiro);
    glutAddMenuEntry("GL POINT",1);
    glutAddMenuEntry("GL LINE",2);
    glutAddMenuEntry("GL FILL",3);

    int menuCull = glutCreateMenu(menu_cull);
    glutAddMenuEntry("ATIVAR",1);
    glutAddMenuEntry("DESATIVAR",2);
    glutAddMenuEntry("FRONT",3);
    glutAddMenuEntry("BACK",4);

    glutCreateMenu(menu);
    glutAddSubMenu("GL FRONT",menuFrontal);
    glutAddSubMenu("GL BACK",menuTraseiro);
    glutAddSubMenu("GL CULL FACE",menuCull);

    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    glutMainLoop();

    return 1;
}
