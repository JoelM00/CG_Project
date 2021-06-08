#define _USE_MATH_DEFINES
#include <cmath> 
#include <cstring>
//#include <math.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void desenhaPlano(int eixo, float comp, float larg, char *ficheiro);

void desenhaCaixa(float x, float y, float z, int divisoes, char *ficheiro);

void desenhaEsfera(float raio, int faces, int pilhas, char* ficheiro);

void desenhaCone(float raio, float altura, int faces, int pilhas, char* ficheiro);

void desenhaTorus(float rInterno, float rExterno, int faces, int aneis, char *ficheiro);
