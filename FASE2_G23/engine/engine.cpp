#include "engine.h"

float camAlpha = 0, camBeta = 0;
float px=-64.0f, py=34.0f, pz=-258.0f;
Transformacao* transf;

void changeSize(int w, int h) {
    if (h == 0) h = 1;
    float ratio = w * 1.0 / h;
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluPerspective(45.0f, ratio, 1.0f, 1000.0f);
    glMatrixMode(GL_MODELVIEW);
}

void renderScene() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    gluLookAt(px, py, pz,
        px + sin(camAlpha) * cos(camBeta), 
        py + sin(camBeta), 
        pz + cos(camAlpha) * cos(camBeta),
        0.0f, 1.0f, 0.0f);

    transf->apllyTransformation();

    // End of frame
    glutSwapBuffers();
}

void processSpecialKeys(int tecla, int x, int y) {
    switch (tecla) {
    case GLUT_KEY_UP:
        if (camBeta + 0.05 < M_PI_2)  
            camBeta += 0.05;
        break;
    case GLUT_KEY_DOWN:
        if (camBeta - 0.05 > -M_PI_2)
            camBeta -= 0.05;
        break;

    case GLUT_KEY_LEFT:
        camAlpha += 0.05;
        break;
    case GLUT_KEY_RIGHT:
        camAlpha -= 0.05;
        break;
    }
    glutPostRedisplay();
}
void tecladoNormal(unsigned char tecla, int x, int y) {
    switch (tecla) {
    case 'd':
        px += 2.0f * sin(camAlpha - M_PI_2);
        pz += 2.0f * cos(camAlpha - M_PI_2);
        break;
    case 'a':
        px += 2.0f * sin(camAlpha + M_PI_2);
        pz += 2.0f * cos(camAlpha + M_PI_2);
        break;
    case 'w':
        px += 2.0f * sin(camAlpha);
        pz += 2.0f * cos(camAlpha);
        break;
    case 's':
        px -= 2.0f * sin(camAlpha);
        pz -= 2.0f * cos(camAlpha);
        break;
    case '+':
        py += 2.0f;
        break;
    case '-':
        py -= 2.0f;
        break;
    }
    glutPostRedisplay();
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
    glutPostRedisplay();
}


void getTransf(string f) {
	transf = new Transformacao();
    leitorXML *a = new leitorXML(f, transf);
    transf = a->leXML();

	cout << "Cenario: " << f << " carregado!" << endl << endl << endl;
    //transf->toString();
	cout << endl << endl;
    //transf->toStringModels();
}

int main(int argc, char **argv) {
    //Carregar o ficheiro xml
	getTransf("../files/sistema_disperso.xml");

    //Iniciar Janela
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(800, 800);
    glutCreateWindow("Engine");

    // Required callback registry
    glutDisplayFunc(renderScene);
    glutReshapeFunc(changeSize);

    // put here the registration of the keyboard callbacks
    glutKeyboardFunc(tecladoNormal);
    glutSpecialFunc(processSpecialKeys);
    cout << endl << "Use W S A D to Move" << endl;
    cout << "UP_Key,DOWN_Key,LEFT_Key,Right_Key to See Around" << endl;

    //Menus
        glutCreateMenu(menu_frontal);
        glutAddMenuEntry("GL POINT", 1);
        glutAddMenuEntry("GL LINE", 2);
        glutAddMenuEntry("GL FILL", 3);

        glutAttachMenu(2);
    //  OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    // enter GLUT's main cycle
    glutMainLoop();
	return 1;
}
