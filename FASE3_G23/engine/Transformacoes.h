#pragma once
#ifndef __MY__TRANSF__
#define __MY__TRANSF__

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>

#include "leitorXML.h"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif
#define _USE_MATH_DEFINES
#define DIR "../files/"
#include <math.h>


using namespace std;

struct Ponto { float x; float y; float z; };

class Modelo {
	vector<Ponto> pontos;
	string nome;
	GLuint vboID;

public:
	Modelo(string nome,GLuint g) {
		this->nome = nome;
		this->pontos = leModelo(nome);
		this->vboID = g;
	}

	void formaVbo(GLuint* buffer) {
		int size = this->pontos.size();
		float* p = new float[size * 3];
		int i = 0;
		for (int j = 0; j < size; j++) {
			p[i] = this->pontos.at(j).x;
			p[i + 1] = this->pontos.at(j).y;
			p[i + 2] = this->pontos.at(j).z;
			i += 3;
		}
		glBindBuffer(GL_ARRAY_BUFFER, buffer[this->vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, p, GL_STATIC_DRAW);
	}

	string getNome() {
		return this->nome;
	}

	vector<Ponto> leModelo(string f) {
		string linha;
		string novo;
		string delimiter = " ";
		int pos;
		float xx, yy, zz;
		vector<Ponto> pontos;
		Ponto p;

		ifstream file(DIR + f);

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
		return pontos;
	}

	void desenhaPontos(GLuint* buffer) {
		glBindBuffer(GL_ARRAY_BUFFER, buffer[vboID]);
		glVertexPointer(3, GL_FLOAT, 0, 0);
		glDrawArrays(GL_TRIANGLES, 0, pontos.size());
	}


	void toString() {
		cout << "Model:" << nome <<"  Tamanho Pontos:" << pontos.size() << endl;
	}
};

class TranslateAnimated {
	float timeAnimacao;

	vector<Ponto> trajetoria;
	int nPontos;

	float pos[3];
	float deriv[3];
	float z[3];
	float up[3] = { 0,1,0 };	//Inicial = Eixo Dos Y
	int desenhaTrajetoria = 0;

	GLuint* buffer;
	GLuint indice;

private:
	void buildRotMatrix(float* x, float* y, float* z, float* m) {

		m[0] = x[0]; m[1] = x[1]; m[2] = x[2]; m[3] = 0;
		m[4] = y[0]; m[5] = y[1]; m[6] = y[2]; m[7] = 0;
		m[8] = z[0]; m[9] = z[1]; m[10] = z[2]; m[11] = 0;
		m[12] = 0; m[13] = 0; m[14] = 0; m[15] = 1;
	}

	void transpose(float* a, float* b) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				b[4 * i + j] = a[4 * j + i];
			}
		}
	}

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

	float length(float* v) {

		float res = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		return res;

	}

	void multMatrixVector(float* m, float* v, float* res) {
		for (int j = 0; j < 4; ++j) {
			res[j] = 0;
			for (int k = 0; k < 4; ++k) {
				res[j] += v[k] * m[j * 4 + k];
			}
		}
	}

	float multVectors(float* v1, float* v2) {
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2] + v1[3] * v2[3];
	}

	void getCatmullRomPoint(float t, Ponto p0, Ponto p1, Ponto p2, Ponto p3, float* pos, float* deriv) {

		// catmull-rom matrix
		float m[4][4] = { {-0.5f,  1.5f, -1.5f,  0.5f},
				  { 1.0f, -2.5f,  2.0f, -0.5f},
				  {-0.5f,  0.0f,  0.5f,  0.0f},
				  { 0.0f,  1.0f,  0.0f,  0.0f} };

		// Compute A = M * P
		float px[4] = { p0.x,p1.x,p2.x,p3.x };
		float py[4] = { p0.y,p1.y,p2.y,p3.y };
		float pz[4] = { p0.z,p1.z,p2.z,p3.z };
		float vetorX[4]; float vetorY[4]; float vetorZ[4];
		multMatrixVector(*m, px, vetorX);
		multMatrixVector(*m, py, vetorY);
		multMatrixVector(*m, pz, vetorZ);

		// Compute pos = T * A
		float tVector[4] = { powf(t,3) , powf(t,2) , t , 1 };
		pos[0] = multVectors(tVector, vetorX); //pos[0]=[t^3,t^2,t,1] * [VectorX[0],VectorX[1],VectorX[2]]
		pos[1] = multVectors(tVector, vetorY); //...
		pos[2] = multVectors(tVector, vetorZ); //...
		// compute deriv = T' * A
		float tVectorD[4] = { 3 * powf(t,2) , 2 * t , 1 , 0 };
		deriv[0] = multVectors(tVectorD, vetorX);
		deriv[1] = multVectors(tVectorD, vetorY);
		deriv[2] = multVectors(tVectorD, vetorZ);
		// ...
	}

	void getGlobalCatmullRomPoint(float tempoRelativo, float* pos, float* deriv) {


		float t = tempoRelativo * this->nPontos;	
		int index = floor(t);						
		t = t - index;								

		int indices[4];
		indices[0] = (index + this->nPontos - 1) % this->nPontos; 
		indices[1] = (indices[0] + 1) % this->nPontos;			 
		indices[2] = (indices[1] + 1) % this->nPontos;			 
		indices[3] = (indices[2] + 1) % this->nPontos;			

		getCatmullRomPoint(t, this->trajetoria.at(indices[0]), this->trajetoria.at(indices[1]), this->trajetoria.at(indices[2]), this->trajetoria.at(indices[3]), pos, deriv);
	}

public:
	TranslateAnimated(float time, vector<float> trajetoria, int desenhaTrajetoria) {
		this->timeAnimacao = time * 1000;
		this->desenhaTrajetoria = desenhaTrajetoria;
		vector<Ponto> pontos;
		Ponto controlPonto;
		for (int i = 0; i < trajetoria.size(); i += 3) {
			controlPonto.x = trajetoria.at(i);
			controlPonto.y = trajetoria.at(i + 1);
			controlPonto.z = trajetoria.at(i + 2);
			pontos.push_back(controlPonto);
		}
		this->trajetoria = pontos;
		this->nPontos = trajetoria.size() / 3;
	}

	void toString() {
		cout << "AnimatedTranslate" << "  Time:" << this->timeAnimacao <<"  trace:"<< this->desenhaTrajetoria<< "  Trajetoria:" << endl;
		for (int i = 0; i < this->trajetoria.size(); i++) {
			cout << "        (" << trajetoria.at(i).x << "," << trajetoria.at(i).y << "," << trajetoria.at(i).z << ")" << endl;
		}
	}

	bool eParaDesenharTrajetoria() {
		if (desenhaTrajetoria != 0) return true;
		else return false;
	}

	void prepare(GLuint* buffer, GLuint tbuffer) {
		if (desenhaTrajetoria != 0) {
			this->buffer = buffer;
			this->indice = tbuffer;
			float* arrayPontos = new float[desenhaTrajetoria * 3];
			float gt;
			int p = 0;
			for (int i = 0; i < this->desenhaTrajetoria; i++) {
				gt = i / (float)desenhaTrajetoria;			
				getGlobalCatmullRomPoint(gt, pos, deriv);	
				arrayPontos[p] = pos[0];		
				arrayPontos[p + 1] = pos[1];
				arrayPontos[p + 2] = pos[2];
				p += 3;
			}
		glBindBuffer(GL_ARRAY_BUFFER, buffer[this->indice]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * desenhaTrajetoria * 3, arrayPontos, GL_STATIC_DRAW);
	}
}

	void traceCurve() {
		if (desenhaTrajetoria != 0) {
			glBindBuffer(GL_ARRAY_BUFFER, buffer[this->indice]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_LINE_LOOP, 0, desenhaTrajetoria);
		}
	}

	void apply(float timeAtual,bool traj) {
		float matrizRotacao[4][4];
		float tempoRelativo;
		tempoRelativo = timeAtual / this->timeAnimacao;	
		tempoRelativo -= floor(tempoRelativo);			

		if (traj) traceCurve();

		getGlobalCatmullRomPoint(tempoRelativo, pos, deriv);
		
		//X(i)=P'(T)=deriv
		normalize(deriv);

		//Z(i)=X(i) x Y(i-1)
		cross(deriv, up, z);
		normalize(z);
		
		//Y(i)=Z(i) x X(i)
		cross(z, deriv, up);
		normalize(up);
		
		glTranslatef(pos[0], pos[1], pos[2]);				// por o modelo no sitio
		buildRotMatrix(deriv, up, z, *matrizRotacao);		// matriz para rodar o objeto
		glMultMatrixf(*matrizRotacao);						// aplicar matriz de rota��o

	}
};

class Instrucao {
	string nome;
	float x;
	float y;
	float z;
	float angle;
	float time;                   
	TranslateAnimated* tAnimated; 
	Modelo* model;

public:
	Instrucao(string nome) {
		this->nome = nome;
	}
	Instrucao() {}

	bool eParaDesenharTrajetoria() {	
		if (this->nome == "TranslateAnimated") {
			return (this->tAnimated->eParaDesenharTrajetoria());
		}
		else return false;
	}

	void scale(float x, float y, float z) {
		this->nome = "Scale";
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void color(float x, float y, float z) {
		this->nome = "Color";
		this->x = x;
		this->y = y;
		this->z = z;
	}

	void translate(float x, float y, float z) {
		this->nome = "Translate";
		this->x=x;
		this->y = y;
		this->z = z;
	}

	void translateAnimated(vector<float> trajetoria, float time,int desenhaTrajetoria) {
		this->nome = "TranslateAnimated" ;
		this->tAnimated = new TranslateAnimated(time,trajetoria,desenhaTrajetoria);
	}

	void rotate(float x, float y, float z,float angle) {
		this->nome = "Rotate";
		this->x = x;
		this->y = y;
		this->z = z;
		this->angle = angle;
	}
	void rotateAnimated(float x, float y, float z,float time) {
		this->nome = "RotateAnimated";
		this->x = x;
		this->y = y;
		this->z = z;
		this->time = time*1000;
	}
	void modelo(Modelo* model) {
		this->nome = "Model";
		this->model = model;
	}
	void popMatrix() {
		this->nome = "PopMatrix";
	}
	void pushMatrix() {
		this->nome = "PushMatrix";
	}
	void prepare(GLuint* buffer,int tbuffer) {
		if (this->nome == "TranslateAnimated")
			tAnimated->prepare(buffer,tbuffer);
	}
	void toString() {
		if (nome == "Scale")
			cout << nome << " (" << x << "," << y << "," << z << ")" << endl;
		else if (nome == "Rotate")
			cout << nome << " (" << x << "," << y << "," << z << ") Angle:" << angle << endl;
		else if (nome == "RotateAnimated")
			cout << nome << " (" << x << "," << y << "," << z << ") Time:" << time << endl;
		else if (nome == "Translate")
			cout << nome << " (" << x << "," << y << "," << z << ")" << endl;
		else if (nome == "TranslateAnimated")
			this->tAnimated->toString();
		else if (nome == "Color")
			cout << nome << " (" << x << "," << y << "," << z << ")" << endl;
		else if (nome == "Model")
			cout << nome << "= " << (model->getNome()) << endl;
		else if (nome == "PopMatrix")
			cout << nome << endl;
		else if (nome == "PushMatrix")
			cout << nome << endl;
		else cout << "Invalido:" << nome << endl;
	}

	void apply(GLuint* buffer,float tempo,bool traj) {	//traj � tecla t do utilizador, desenhar ou n�o trajetorias.
		if (nome == "Scale")
			glScalef(this->x, this->y, this->z);
		else if (nome == "Rotate")
			glRotatef(this->angle, this->x, this->y, this->z);
		else if (nome == "RotateAnimated")
			glRotatef(((tempo / this->time) * 360), this->x, this->y, this->z);
		else if (nome == "Translate")
			glTranslatef(this->x, this->y, this->z);
		else if (nome == "TranslateAnimated")
			this->tAnimated->apply(tempo,traj);
		else if (nome == "Color")
			glColor3f(this->x, this->y,this->z);
		else if (nome == "Model")
			model->desenhaPontos(buffer);
		else if (nome == "PopMatrix")
			glPopMatrix();
		else if (nome == "PushMatrix")
			glPushMatrix();
		else cout << "Invalido:" << nome << endl;
	}
};

class Transformacao {
	vector<Instrucao*> instrucoes;
	vector<Modelo*> models;
	GLuint* buffer;
	int nTranslatedAnimated=0;

public:
	Transformacao() {
		this->instrucoes = vector<Instrucao*>();
		this->models = vector<Modelo*>();
		buffer = NULL;
	}

	int getNTranslatedAnimated() {
		return this->nTranslatedAnimated;
	}
	
	GLuint getSizeModelos() {
		return this->models.size();
	}

	void prepare(GLuint* figures) {
		this->buffer = figures;
		for (Modelo* m : this->models) {
			m->formaVbo(buffer);
		}
		int tbuffer = this->models.size();
		
		for (Instrucao* i : this->instrucoes) {
			if (i->eParaDesenharTrajetoria()) {	
				i->prepare(buffer, tbuffer);
				tbuffer++;
			}
		}
	}

	void addModel(string nomeModel) {
		Instrucao* i = new Instrucao();
		bool encontrado = false;
		for (Modelo* m : this->models) {	
			if (m->getNome() == nomeModel) {
				i->modelo(m);
				instrucoes.push_back(i);
				encontrado = true;
			}
		}
		if (!encontrado) {	
			Modelo* newModel = new Modelo(nomeModel,this->models.size());
			i->modelo(newModel);
			instrucoes.push_back(i);
			models.push_back(newModel);
		}
	}

	void addTranslate(float x, float y, float z) {
		Instrucao* i =new Instrucao();
		i->translate(x, y, z);
		instrucoes.push_back(i);
	}

	void addTranslateAnimated(vector<float> trajetoria, float time,int desenhaTrajetoria) {
		Instrucao* i = new Instrucao();
		i->translateAnimated(trajetoria,time,desenhaTrajetoria);
		instrucoes.push_back(i);
		if (desenhaTrajetoria!=0) this->nTranslatedAnimated++;
	}
	
	void addScale(float x,float y,float z) {
		Instrucao* i = new Instrucao();
		i->scale(x, y, z);
		instrucoes.push_back(i);
	}

	void addRotate(float x,float y,float z,float angle) {
		Instrucao* i = new Instrucao();
		i->rotate(x, y, z,angle);
		instrucoes.push_back(i);
	}

	void addRotateAnimated(float x, float y, float z,float time) {
		Instrucao* i = new Instrucao();
		i->rotateAnimated(x, y, z,time);
		instrucoes.push_back(i);
	}

	void addColor(float r, float g, float b) {
		Instrucao* i = new Instrucao();
		i->color(r, g, b);
		instrucoes.push_back(i);
	}

	void addPop() {
		Instrucao* i = new Instrucao();
		i->popMatrix();
		instrucoes.push_back(i);
	}
	void addPush() {
		Instrucao* i = new Instrucao();
		i->pushMatrix();
		instrucoes.push_back(i);
	}

	void apllyTransformation(float time,bool traj) {
		for (Instrucao* m : this->instrucoes) {
			m->apply(buffer,time,traj);
		}
	}

	void toString() {
		for (Instrucao* m : this->instrucoes) {
			m->toString();
		}
	}

	void toStringModels() {
		for (Modelo* m : this->models) {
			m->toString();
		}
	}
};

#endif