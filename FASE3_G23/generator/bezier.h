#define CLASS5_BEZIER_H
#include <vector>
#include <cstdlib>
#include <cstring>
#include <cstdio>
#define _USE_MATH_DEFINES
#include <cmath> 
#include <fstream>

#include <string>

using namespace std;

struct Ponto {
    float x;
    float y;
    float z;
};

struct DadosPatch {
    int numPatch;
    int nrPontos;
    vector<vector<int>> indices;
    vector<Ponto> pontos;
};

class Bezier {
    DadosPatch dados;

public:
    void multMatrix(float a[1][4], float b[4][4], float res[1][4]) {
        res[0][0] = (float) ((a[0][0] * b[0][0]) + (a[0][1] * b[1][0]) + (a[0][2] * b[2][0]) + (a[0][3] * b[3][0]));
        res[0][1] = (float) ((a[0][0] * b[0][1]) + (a[0][1] * b[1][1]) + (a[0][2] * b[2][1]) + (a[0][3] * b[3][1]));
        res[0][2] = (float) ((a[0][0] * b[0][2]) + (a[0][1] * b[1][2]) + (a[0][2] * b[2][2]) + (a[0][3] * b[3][2]));
        res[0][3] = (float) ((a[0][0] * b[0][3]) + (a[0][1] * b[1][3]) + (a[0][2] * b[2][3]) + (a[0][3] * b[3][3]));
    }

    void multMatrix2(float t[1][4], float v[4][1], float* res) {
        *res = t[0][0] * v[0][0] + t[0][1] * v[1][0] + t[0][2] * v[2][0] + t[0][3] * v[3][0];
    }

    void calculaPontoBezier(float u, float v, int patch, float r[3]) {

        float vetorU[1][4] = { {powf(u,3),powf(u,2),u,1} };

        float vetorV[4][1] = { {powf(v,3)},{powf(v,2)},{v},{1} };

        float matBezier[4][4] = { {-1.0f,  3.0f, -3.0f,  1.0f},
                                  { 3.0f, -6.0f,  3.0f,  0.0f},
                                  {-3.0f,  3.0f,  0.0f,  0.0f},
                                  { 1.0f,  0.0f,  0.0f,  0.0f} };

        float matBezierTrans[4][4] = { {-1.0f,  3.0f, -3.0f, 1.0f},
                                       { 3.0f, -6.0f,  3.0f, 0.0f},
                                       {-3.0f,  3.0f,  0.0f, 0.0f},
                                       { 1.0f,  0.0f,  0.0f, 0.0f} };

        float pX[4][4];
        float pY[4][4];
        float pZ[4][4];

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                pX[i][j] = dados.pontos.at(dados.indices.at(patch).at(i * 4 + j)).x;
                pY[i][j] = dados.pontos.at(dados.indices.at(patch).at(i * 4 + j)).y;
                pZ[i][j] = dados.pontos.at(dados.indices.at(patch).at(i * 4 + j)).z;
            }
        }

        float res[1][4];

        float resX[1][4];
        float resY[1][4];
        float resZ[1][4];

        float tX[1][4];
        float tY[1][4];
        float tZ[1][4];

        float x, y, z;

        // (1x4)(4x4) = 1x4
        multMatrix(vetorU, matBezier, res);  

        // (1x4)(4x4) = 1x4
        multMatrix(res, pX, resX); 
        multMatrix(res, pY, resY);
        multMatrix(res, pZ, resZ);  

        // (1x4)(4x4) = 1x4
        multMatrix(resX, matBezierTrans, tX);
        multMatrix(resY, matBezierTrans, tY);
        multMatrix(resZ, matBezierTrans, tZ);  

        // (1x4)(4x1) = 1x1
        multMatrix2(tX, vetorV, &x);  
        multMatrix2(tY, vetorV, &y);
        multMatrix2(tZ, vetorV, &z);  

        r[0] = x;
        r[1] = y;
        r[2] = z;
    }



    void carregaPatch(string inputF, string outputF, int tess) {
        string linha;

        ifstream readFile(inputF);
        ofstream escreveFicheiro(outputF);
        if (readFile.is_open()) {

            getline(readFile, linha);
            dados.numPatch = stoi(linha);

            for (int i = 0; i < dados.numPatch; i++) {
                dados.indices.push_back({});
                getline(readFile, linha);
                char* arrayLinha = strdup(linha.c_str());
                char* point = strtok(arrayLinha, ", ");

                for (int j = 0; point != NULL; j++) {
                    dados.indices.at(i).push_back(stoi(point));
                    point = strtok(NULL, ", ");
                }
            }

            getline(readFile, linha);
            dados.nrPontos= stoi(linha);
            printf("\n\n");
            printf("Este é o numero de pontos: %i\n", dados.nrPontos);

            for (int i = 0; i < dados.nrPontos; i++) {
                Ponto p;
                getline(readFile, linha);
                char* arrayLinha = strdup(linha.c_str());
                char* point;
                point = strtok(arrayLinha, ", ");
                p.x = stof(point);
                point = strtok(NULL, ", ");
                p.y = stof(point);
                point = strtok(NULL, "\n ");
                p.z = stof(point);

                dados.pontos.push_back(p);

            }
        }

        else {
            printf("ERRO AO LER O FICHEIRO!\n");
        }
        readFile.close();

        float u1, v1, u2, v2;
        float p0[3], p1[3], p2[3], p3[3];
        if (escreveFicheiro.is_open()) {
            for (int a = 0; a < dados.numPatch; a++) {

                for (int u = 0; u < tess; u++) {
                    for (int v = 0; v < tess; v++) {
                        u1 = (float)u / tess;
                        v1 = (float)v / tess;
                        u2 = (float)(u + 1) / tess;
                        v2 = (float)(v + 1) / tess;

                        calculaPontoBezier(u1, v1, a, p0);
                        calculaPontoBezier(u1, v2, a, p1);
                        calculaPontoBezier(u2, v2, a, p2);
                        calculaPontoBezier(u2, v1, a, p3);

                        /*
                        0 . ---- . 1
                        |          |
                        |          |
                        3 . ---- . 2
                        */

                        //triangulo 012
                        escreveFicheiro << p0[0] << " " << p0[1] << " " << p0[2] << endl;
                        escreveFicheiro << p1[0] << " " << p1[1] << " " << p1[2] << endl;
                        escreveFicheiro << p2[0] << " " << p2[1] << " " << p2[2] << endl; 

                        //triangulo 023
                        escreveFicheiro << p0[0] << " " << p0[1] << " " << p0[2] << endl;
                        escreveFicheiro << p2[0] << " " << p2[1] << " " << p2[2] << endl;
                        escreveFicheiro << p3[0] << " " << p3[1] << " " << p3[2] << endl; 
                    }
                }
            }
            escreveFicheiro.close();
        }
        else {
            printf("ERRO NA ESCRITA DO FICHEIRO");
        }
    }

};

