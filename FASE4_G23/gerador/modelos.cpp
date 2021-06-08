#include "modelos.h"

void cross(float* a, float* b, float* res) {

    res[0] = a[1] * b[2] - a[2] * b[1];
    res[1] = a[2] * b[0] - a[0] * b[2];
    res[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(float* a) {

    float l = sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    a[0] = a[0] / l;
    a[1] = a[1] / l;
    a[2] = a[2] / l;
}


void desenhaPlano(int eixo, float comp, float larg, char *ficheiro) {
    FILE *fd;
    float x = larg / 2.0f;
    float y = comp / 2.0f;
    fd = fopen(ficheiro,"w");

    if (fd != NULL) {
        switch(eixo){
        case 0: //Eixo xOz

            //triangulo 1
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", -x, 0.0, -y, 0.0, 1.0, 0.0, 0.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", x, 0.0, y, 0.0, 1.0, 0.0, 1.0, 1.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", x, 0.0, -y, 0.0, 1.0, 0.0, 0.0, 1.0);

            //triangulo 2
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", -x, 0.0, -y, 0.0, 1.0, 0.0, 0.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", -x, 0.0, y, 0.0, 1.0, 0.0, 1.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", x, 0.0, y, 0.0, 1.0, 0.0, 1.0, 1.0);
            break;

        case 1: //Eixo xOy

            //triangulo 1
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", x, y, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", -x, y, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", x, -y, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0);

            //triangulo 2
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", -x, y, 0.0, 0.0, 0.0, 1.0, 1.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", -x, -y, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", x, -y, 0.0, 0.0, 0.0, 1.0, 0.0, 1.0);
            break;

        case 2: //Eixo yOz
    
            //triangulo 1
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", 0.0, x, y, 1.0, 0.0, 0.0, 1.0, 1.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", 0.0, -x, y, 1.0, 0.0, 0.0, 1.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", 0.0, -x, -y, 1.0, 0.0, 0.0, 0.0, 0.0);

            //triangulo 2
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", 0.0, x, y, 1.0, 0.0, 0.0, 1.0, 1.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", 0.0, -x, -y, 1.0, 0.0, 0.0, 0.0, 0.0);
            fprintf(fd, "%f %f %f %f %f %f %f %f\n", 0.0, x, -y, 1.0, 0.0, 0.0, 0.0, 1.0);
            break;

        default:
            break;
        }
        
    } 
    else {
        cout << "Erro ao criar o ficheiro do plano!" << endl;
       
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

    float altura = 1.0 / 3;
	float lado = 1.0 / 4;
	float cx = altura / div;
	float cy = lado / div;
	float ladoAtual, alturaAtual;

        for (int i = 0; i < div; i++) {
            for (int j = 0; j < div; j++) {

                //FACE DE CIMA: Y=0
                ladoAtual = lado * 2;
				alturaAtual = altura * 2;
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[0]  , coordZ[j],0.0,1.0,0.0, (alturaAtual + cx*j), (ladoAtual - cy * (i+1)));     	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[0]  , coordZ[j],0.0,1.0,0.0, (alturaAtual + cx * j), (ladoAtual - cy * (i)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[0]  , coordZ[j + 1], 0.0,1.0,0.0, (alturaAtual + cx * (j+1)), (ladoAtual - cy * i ));   
											  
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[0]  , coordZ[j + 1],0.0,1.0,0.0, (alturaAtual + cx * (j+1)), (ladoAtual - cy * i));  
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[0]  , coordZ[j + 1],0.0,1.0,0.0, (alturaAtual + cx * (j + 1)),(ladoAtual - cy * (i + 1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[0]  , coordZ[j],0.0,1.0,0.0,(alturaAtual + cx * j), (ladoAtual - cy * (i+1)));

	        
				//FACE DE BAIXO: 
				ladoAtual = lado*2;
				alturaAtual = altura;
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i+1]   , coordY[div], coordZ[j],0.0,-1.0,0.0, (alturaAtual - cx * j ),(ladoAtual - cy * (i+1)));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[div], coordZ[j + 1],0.0,-1.0,0.0,(alturaAtual - cx * (j+1)), (ladoAtual - cy * (i))) ;
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[div], coordZ[j],0.0,-1.0,0.0, (alturaAtual - cx * j),(ladoAtual - cy * i));
											
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[div], coordZ[j],0.0,-1.0,0.0,(alturaAtual - cx * (j)) ,(ladoAtual - cy * (i+1)));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[div], coordZ[j + 1],0.0,-1.0,0.0,(alturaAtual - cx * (j + 1)), (ladoAtual - cy * (i + 1)));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[div], coordZ[j+1],0.0,-1.0,0.0, (alturaAtual - cx * (j+1)) , (ladoAtual - cy * (i)));


                //FACE com z minimo: (face contraria ao normal) Trás
                ladoAtual = lado * 3;
				alturaAtual = altura * 2;
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[j+1], coordZ[div],0.0,0.0,-1.0,(alturaAtual - cx * (j + 1)), (ladoAtual + cy * (i + 1)));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[j]  , coordZ[div],0.0,0.0,-1.0,(alturaAtual - cx * (j)) ,(ladoAtual + cy *(i+1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[j+1], coordZ[div],0.0,0.0,-1.0,(alturaAtual - cx * (j+1)) , (ladoAtual + cy * (i)));	
											
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[j]    , coordZ[div],0.0,0.0,-1.0,(alturaAtual - cx * (j)), (ladoAtual + cy * (i+1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[j]    , coordZ[div],0.0,0.0,-1.0, (alturaAtual - cx * j) , (ladoAtual + cy * i));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[j + 1], coordZ[div],0.0,0.0,-1.0,(alturaAtual - cx * (j+1)), (ladoAtual + cy * (i)));


                //FACE COM Z maximo: (face frontal) 
                ladoAtual = lado * 2;
				alturaAtual = altura * 2;
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[j]    , coordZ[0],0.0,0.0,1.0, (alturaAtual - cx * (j)), (ladoAtual - cy * (i)));   	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i+1]   , coordY[j]    , coordZ[0],0.0,0.0,1.0, (alturaAtual - cx * (j)), (ladoAtual - cy * (i + 1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i+1]   , coordY[j + 1], coordZ[0],0.0,0.0,1.0, (alturaAtual - cx * (j + 1)), (ladoAtual - cy * (i + 1)));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[j]     , coordZ[0],0.0,0.0,1.0, (alturaAtual - cx * (j)), (ladoAtual - cy * (i)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i + 1] , coordY[j+1]   , coordZ[0],0.0,0.0,1.0, (alturaAtual - cx * (j + 1)), (ladoAtual - cy * (i + 1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[i]     , coordY[j + 1] , coordZ[0],0.0,0.0,1.0, (alturaAtual - cx * (j + 1)), (ladoAtual - cy * (i)));

                //FACE com X max (face lateral direita)
                ladoAtual = lado * 2;
				alturaAtual = altura*2;
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[0] , coordY[i + 1] , coordZ[j],1.0,0.0,0.0, (alturaAtual - cx * (i+1)), (ladoAtual + cy * (j)));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[0] , coordY[i + 1] , coordZ[j + 1],1.0,0.0,0.0, (alturaAtual - cx * (i+1)), (ladoAtual + cy * (j+1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[0] , coordY[i]     , coordZ[j],1.0,0.0,0.0, (alturaAtual - cx * (i)), (ladoAtual + cy * (j)));	
											
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[0] , coordY[i+1] , coordZ[j+1],1.0,0.0,0.0, (alturaAtual - cx * (i+1)), (ladoAtual + cy * (j+1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[0] , coordY[i]   , coordZ[j+1],1.0,0.0,0.0, (alturaAtual - cx * (i)), (ladoAtual + cy * (j+1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[0] , coordY[i]   , coordZ[j],1.0,0.0,0.0, (alturaAtual - cx * (i)), (ladoAtual + cy * (j)));

                //FACE com X min (face lateral esquerda)
                ladoAtual = lado;
				alturaAtual = altura *2;
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[div] , coordY[i + 1] , coordZ[j+1],-1.0,0.0,0.0, (alturaAtual - cx * (i+1)), (ladoAtual - cy * (j+1)));	
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[div] , coordY[i+1]   , coordZ[j],-1.0,0.0,0.0, (alturaAtual - cx * (i+1)), (ladoAtual - cy * (j)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[div] , coordY[i]     , coordZ[j],-1.0,0.0,0.0, (alturaAtual - cx * (i)), (ladoAtual - cy * (j)));	
											
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[div] , coordY[i + 1] , coordZ[j+1],-1.0,0.0,0.0, (alturaAtual - cx * (i+1)), (ladoAtual - cy * (j+1)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[div] , coordY[i]     , coordZ[j],-1.0,0.0,0.0, (alturaAtual - cx * (i)), (ladoAtual - cy * (j)));
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", coordX[div] , coordY[i]     , coordZ[j+1],-1.0,0.0,0.0, (alturaAtual - cx * (i)), (ladoAtual - cy * (j+1)));
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

        float ladoIncrementa = 1.0 / faces;
        float cimaIncrementa = 1.0 / pilhas;
        float cima1, cima2, lado1, lado2;

        for (int i = 0; i < pilhas; i++) { //stacks
            betaAnterior = betaAtual;
            betaAtual += beta;

            //Texturas:
            cima1 = cimaIncrementa * i;
            cima2 = cimaIncrementa * (i + 1.0f);

            for (int j = 0; j < faces; j++) { //fatias, slices
                alfaAnterior = alfaAtual;
                alfaAtual += alfa;

                //Texturas:
                lado1 = ladoIncrementa * j;
                lado2 = ladoIncrementa * (j + 1.0f);

            
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

                //(x,y,z, normalX, normalY, normalZ, texturaPilhas, texturaFaces)
                //abc
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", xa,ya,za, xa/raio, ya/raio, za/raio, cima1, lado1);
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", xb,yb,zb, xb/raio, yb/raio, zb/raio, cima1, lado2);
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", xc,yc,zc, xc/raio, yc/raio, zc/raio, cima2, lado1);

                //cbd
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", xc, yc, zc, xc / raio, yc / raio, zc / raio, cima2, lado1);
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", xb, yb, zb, xb / raio, yb / raio, zb / raio, cima1, lado2);
                fprintf(fd, "%f %f %f %f %f %f %f %f\n", xd,yd,zd, xd/raio, yd/raio, zd/raio, cima2, lado2);
            }
        }
    } else {
        cout << "Erro ao criar o ficheiro da esfera!" << endl;
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


void desenhaTorus(float rInterno, float rExterno, int faces, int aneis,char *ficheiro) {
     FILE *fd;
     fd = fopen(ficheiro,"w");

    if (fd != NULL) {
        float aflaInterno = (2 * M_PI) / faces;
        float alfaExterno = (2 * M_PI) / aneis;
        float alfaIntAnt, alfaIntAtual = 0, alfaExtAnt, alfaExtAtual = 0;
        float x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, z4;


        for (int i = 0; i < aneis; i++) {
            alfaExtAnt = alfaExtAtual;
            alfaExtAtual += alfaExterno;

            for (int j = 0; j < faces; j++) {
                alfaIntAnt = alfaIntAtual;
                alfaIntAtual += aflaInterno;

                x1 = (rInterno + rExterno * cos(alfaIntAnt)) * sin(alfaExtAnt);     	//x1
                y1 = rExterno * sin(alfaIntAnt);				    	//y1				   
                z1 = (rInterno + rExterno * cos(alfaIntAnt)) * cos(alfaExtAnt);     	//z1   

                x2 = (rInterno + rExterno * cos(alfaIntAtual)) * sin(alfaExtAnt);   	//x1
                y2 = rExterno * sin(alfaIntAtual);				   	//y2
                z2 = (rInterno + rExterno * cos(alfaIntAtual)) * cos(alfaExtAnt);   	//z2

                x3 = (rInterno + rExterno * cos(alfaIntAnt)) * sin(alfaExtAtual);   	//x2
                y3 = rExterno * sin(alfaIntAnt);				    	//y1
                z3 = (rInterno + rExterno * cos(alfaIntAnt)) * cos(alfaExtAtual);   	//z3 

                x4 = (rInterno + rExterno * cos(alfaIntAtual)) * sin(alfaExtAtual); 	//x2
                y4 = rExterno * sin(alfaIntAtual);				    	//y2
                z4 = (rInterno + rExterno * cos(alfaIntAtual)) * cos(alfaExtAtual); 	//z4
                
                float v1[3] = { x3 - x1, y3 - y1, z3 - z1 };
                float v2[3] = { x4 - x1, y4 - y1, z4 - z1 };
                float* n1 = new float[3];
                cross(v1, v2, n1);
                normalize(n1);

                float v3[3] = { x2 - x4, y2 - y4, z2 - z4 };
                float v4[3] = { x1 - x4, y1 - y4, z1 - z4 };
                float* n2 = new float[3];
                cross(v3, v4, n2);
                normalize(n2);

                printf("%f %f %f %f %f %f %f %f\n", x1, y1, z1, n1[0], n1[1], n1[2], 0.0f, 0.0f);
                printf("%f %f %f %f %f %f %f %f\n", x3, y3, z3, n1[0], n1[1], n1[2], 0.0f, 0.0f);
                printf("%f %f %f %f %f %f %f %f\n", x4, y4, z4, n1[0], n1[1], n1[2], 0.0f, 0.0f);

                printf("%f %f %f %f %f %f %f %f\n", x4, y4, z4, n2[0], n2[1], n2[2], 0.0f, 0.0f);
                printf("%f %f %f %f %f %f %f %f\n", x2, y2, z2, n2[0], n2[1], n2[2], 0.0f, 0.0f);
                printf("%f %f %f %f %f %f %f %f\n", x1, y1, z1, n2[0], n2[1], n2[2], 0.0f, 0.0f);
            }
        }
    } else {
        cout << "Erro ao criar o ficheiro torus!" << endl;
    }
    fclose(fd);
}

