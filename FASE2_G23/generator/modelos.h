#include <cstring>
#include <math.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>

using namespace std;

void desenhaPlano(float tam, char *ficheiro);

void desenhaCaixa(float x, float y, float z, int d, char *ficheiro);

void desenhaEsfera(float r, int stacks, int slices, char * f);

void desenhaCone(float raio, float altura, int faces, int pilhas, char* ficheiro);

void desenhaTorus(float rInterno, float rExterno, int faces, int aneis, char *ficheiro);
