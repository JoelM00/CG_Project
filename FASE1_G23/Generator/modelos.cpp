#include "modelos.h"


void desenhaPlano(float tam, char *ficheiro) {
    FILE *fd;

    fd = fopen(ficheiro,"w");

    if (fd != NULL) {
        fprintf(fd,"%f %f %f\n", tam/2,0.0f,tam/2);
        fprintf(fd,"%f %f %f\n", tam/2,0.0f,-tam/2);
        fprintf(fd,"%f %f %f\n", -tam/2,0.0f,tam/2);

        fprintf(fd,"%f %f %f\n", -tam/2,0.0f,tam/2);
        fprintf(fd,"%f %f %f\n", tam/2,0.0f,-tam/2);
        fprintf(fd,"%f %f %f\n", -tam/2,0.0f,-tam/2);

        fprintf(fd,"%f %f %f\n", tam/2,0.0f,tam/2);
        fprintf(fd,"%f %f %f\n", -tam/2,0.0f,tam/2);
        fprintf(fd,"%f %f %f\n", -tam/2,0.0f,-tam/2);

        fprintf(fd,"%f %f %f\n", tam/2,0.0f,tam/2);
        fprintf(fd,"%f %f %f\n", -tam/2,0.0f,-tam/2);
        fprintf(fd,"%f %f %f\n", tam/2,0.0f,-tam/2);
    } else {
        cout << "Erro ao criar o ficheiro!" << endl;
    }
    fclose(fd);
}


void desenhaCaixa(float x, float y, float z, int divisoes, char *ficheiro) {
    FILE *fd;

    fd = fopen(ficheiro,"w");

    if (fd != NULL) {
        int div = divisoes + 1;
        float *coordX = new float[div + 1];
        float *coordY = new float[div + 1];
        float *coordZ = new float[div + 1];
        float distX = x / div;
        float distY = y / div;
        float distZ = z / div;
        x = x / 2;
        y = y / 2;
        z = z / 2;
        for (int i = 0; i < div + 1; i++) {
            coordX[i] = x - (distX * i);
            coordY[i] = y - (distY * i);
            coordZ[i] = z - (distZ * i);
        }
        for (int i = 0; i < div; i++) {
            for (int j = 0; j < div; j++) {

                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[0]  , coordZ[j]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[0]  , coordZ[j]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[0]  , coordZ[j + 1]);

                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[0]  , coordZ[j + 1]);
                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[0]  , coordZ[j + 1]);
                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[0]  , coordZ[j]);

                fprintf(fd, "%f %f %f\n", coordX[i+1]   , coordY[div], coordZ[j]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[div], coordZ[j + 1]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[div], coordZ[j] );

                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[div], coordZ[j]);
                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[div], coordZ[j + 1]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[div], coordZ[j+1]);

                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[j+1], coordZ[div]);
                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[j]  , coordZ[div]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[j+1], coordZ[div]);

                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[j]    , coordZ[div]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[j]    , coordZ[div]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[j + 1], coordZ[div]);

                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[j]    , coordZ[0]);
                fprintf(fd, "%f %f %f\n", coordX[i+1]   , coordY[j]    , coordZ[0]);
                fprintf(fd, "%f %f %f\n", coordX[i+1]   , coordY[j + 1], coordZ[0]);

                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[j]     , coordZ[0]);
                fprintf(fd, "%f %f %f\n", coordX[i + 1] , coordY[j+1]   , coordZ[0]);
                fprintf(fd, "%f %f %f\n", coordX[i]     , coordY[j + 1] , coordZ[0]);

                fprintf(fd, "%f %f %f\n", coordX[0] , coordY[i + 1] , coordZ[j]);
                fprintf(fd, "%f %f %f\n", coordX[0] , coordY[i + 1] , coordZ[j + 1]);
                fprintf(fd, "%f %f %f\n", coordX[0] , coordY[i]     , coordZ[j]);

                fprintf(fd, "%f %f %f\n", coordX[0] , coordY[i+1] , coordZ[j+1]);
                fprintf(fd, "%f %f %f\n", coordX[0] , coordY[i]   , coordZ[j+1]);
                fprintf(fd, "%f %f %f\n", coordX[0] , coordY[i]   , coordZ[j]);

                fprintf(fd, "%f %f %f\n", coordX[div] , coordY[i + 1] , coordZ[j+1]);
                fprintf(fd, "%f %f %f\n", coordX[div] , coordY[i+1]   , coordZ[j]);
                fprintf(fd, "%f %f %f\n", coordX[div] , coordY[i]     , coordZ[j]);

                fprintf(fd, "%f %f %f\n", coordX[div] , coordY[i + 1] , coordZ[j+1]);
                fprintf(fd, "%f %f %f\n", coordX[div] , coordY[i]     , coordZ[j]);
                fprintf(fd, "%f %f %f\n", coordX[div] , coordY[i]     , coordZ[j+1]);
            }
        }
    } else {
        cout << "Erro ao criar o ficheiro!" << endl;
    }
    fclose(fd);
}


void desenhaEsfera(float raio, int faces, int pilhas, char* ficheiro) {
    FILE *fd;

    fd = fopen(ficheiro,"w");

    if (fd != NULL) {

        float beta =  M_PI / pilhas;
        float alfa = (2 * M_PI) / faces;

        float alfaAnterior, alfaAtual = 0, betaAnterior, betaAtual = - M_PI_2;
        float xa, ya, za, xb, yb, zb, xc, yc, zc, xd, yd, zd;

        for (int i = 0; i < pilhas; i++) {
            betaAnterior = betaAtual;
            betaAtual += beta;

            for (int j = 0; j < faces; j++) {
                alfaAnterior = alfaAtual;
                alfaAtual += alfa;

                xa = raio * sin(alfaAnterior)* cos(betaAnterior) ;
                ya = raio * sin(betaAnterior);
                za = raio * cos(alfaAnterior) * cos(betaAnterior);

                xb = raio * sin(alfaAtual) * cos(betaAnterior) ;
                yb = raio * sin(betaAnterior);
                zb = raio * cos(alfaAtual) * cos(betaAnterior);

                xc = raio * sin(alfaAnterior) * cos(betaAtual) ;
                yc = raio * sin(betaAtual);
                zc = raio * cos(alfaAnterior) * cos(betaAtual);

                xd = raio * sin(alfaAtual) * cos(betaAtual) ;
                yd = raio * sin(betaAtual);
                zd = raio * cos(alfaAtual) * cos(betaAtual);

                fprintf(fd, "%f %f %f\n", xa,ya,za);
                fprintf(fd, "%f %f %f\n", xb,yb,zb);
                fprintf(fd, "%f %f %f\n", xc,yc,zc);

                fprintf(fd, "%f %f %f\n", xc,yc,zc);
                fprintf(fd, "%f %f %f\n", xb,yb,zb);
                fprintf(fd, "%f %f %f\n", xd,yd,zd);
            }
        }
    } else {
        cout << "Erro ao criar o ficheiro!" << endl;
    }
    fclose(fd);
}


void desenhaCone(float raio, float altura, int faces, int pilhas, char* ficheiro) {
    FILE *fd;

    fd = fopen(ficheiro,"w");

    if (fd != NULL) {
        float alturaCamada = altura/pilhas;
        float alfa = (2 * M_PI) / faces;

        float alfaAnterior, alfaAtual = 0;
        float alturaAtual = 0, alturaAnterior;
        float raioAtual = raio, raioAnterior;

        for (int j = 0; j < faces; j++) {
            alfaAnterior = alfaAtual;
            alfaAtual += alfa;

            fprintf(fd, "%f %f %f\n", raio * sin(alfaAnterior), 0.0f, raio * cos(alfaAnterior));
            fprintf(fd, "%f %f %f\n", 0.0f, 0.0f, 0.0f);
            fprintf(fd, "%f %f %f\n", raio * sin(alfaAtual), 0.0f, raio * cos(alfaAtual));
        }

        for (int i = 0; i < pilhas; i++) {

            alturaAnterior = alturaAtual;
            alturaAtual += alturaCamada;

            raioAnterior = raioAtual;
            raioAtual = raio * (altura - alturaAtual) / altura;

            for (int j = 0; j < faces; j++) {
                alfaAnterior = alfaAtual;
                alfaAtual += alfa;

                fprintf(fd, "%f %f %f\n", raioAnterior * sin(alfaAnterior), alturaAnterior, raioAnterior * cos(alfaAnterior));
                fprintf(fd, "%f %f %f\n", raioAnterior * sin(alfaAtual), alturaAnterior, raioAnterior * cos(alfaAtual));
                fprintf(fd, "%f %f %f\n", raioAtual * sin(alfaAnterior), alturaAtual, raioAtual * cos(alfaAnterior));
                fprintf(fd, "%f %f %f\n", raioAnterior * sin(alfaAtual), alturaAnterior, raioAnterior * cos(alfaAtual));
                fprintf(fd, "%f %f %f\n", raioAtual * sin(alfaAtual), alturaAtual, raioAtual * cos(alfaAtual));
                fprintf(fd, "%f %f %f\n", raioAtual * sin(alfaAnterior), alturaAtual, raioAtual * cos(alfaAnterior));
            }
        }
    } else {
        cout << "Erro ao criar o ficheiro!" << endl;
    }
    fclose(fd);
}





