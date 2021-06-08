#pragma once
#ifndef __MY__TRANSF__
#define __MY__TRANSF__

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
using namespace std;

#include "leitorXML.h"

#include <IL/il.h>

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
	string nome;
	vector<float> pontos;
	vector<float> normais;
	vector<float> text;
	string textura;
	GLuint vboID;
	float *ambient, *diffuse, *emissive, *specular;
	float size = 0;
	int idTextura;

public:
	Modelo(string nome, GLuint g,vector<float> d,vector<float> e,vector<float> a,vector<float> s, string textura) {
		this->nome = nome;
		leModelo(nome);
		this->textura = textura;
		this->vboID = g;
		this->diffuse = new float [4];
		this->emissive = new float[4];
		this->ambient = new float[4];
		this->specular = new float[4];
		for (int i = 0; i < 4; i++) {
			diffuse[i] = d.at(i);
			ambient[i] = a.at(i);
			emissive[i] = e.at(i);
			specular[i] = s.at(i);
		}
	}

	string getNome() { return this->nome;}

	int loadTexture() {
		unsigned int t, tw, th;
		unsigned char* texData;
		unsigned int texID;

		ilInit();
		ilEnable(IL_ORIGIN_SET);
		ilOriginFunc(IL_ORIGIN_LOWER_LEFT);

		ilGenImages(1, &t);
		ilBindImage(t);
		string texfile = DIR + this->textura;
		ilLoadImage((ILstring)(texfile.c_str()));
		tw = ilGetInteger(IL_IMAGE_WIDTH);
		th = ilGetInteger(IL_IMAGE_HEIGHT);
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		texData = ilGetData();

		glGenTextures(1, &texID);

		glBindTexture(GL_TEXTURE_2D,texID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tw, th, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);	
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);
		return texID;
	}

	void formaVbo(GLuint* figuras,GLuint* normais,GLuint* texturas) {
		size = this->pontos.size() / 3;

		float* p = new float[int(size) * 3];
		float* n = new float[int(size)  * 3];
		float* t = new float[int(size)  * 3];

		int i = 0,k = 0;
		for (int j = 0; j < size; j++) {
			p[i] = this->pontos.at(i);
			p[i + 1] = this->pontos.at(i+1);
			p[i + 2] = this->pontos.at(i+2);
			n[i] = this->normais.at(i);
			n[i + 1] = this->normais.at(i+1);
			n[i + 2] = this->normais.at(i+2);
			t[k] = this->text.at(k);
			t[k + 1] = this->text.at(k+1);
			i += 3;
			k += 2;
		}

		glBindBuffer(GL_ARRAY_BUFFER, figuras[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, p, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, normais[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 3, n, GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, texturas[vboID]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * size * 2, t, GL_STATIC_DRAW);

		if (this->textura.compare("nada") != 0) {
			int txId=loadTexture();
			this->idTextura = txId;
		}else {
			this->idTextura = -1;
		}
	}

	void leModelo(string f) {
		float posX, posY, posZ;
		float normX, normY, normZ;
		float textX, textY;
		ifstream file(DIR + f);

		if (file.is_open()) {
			while (file.good()) {	
				file >> posX >> posY >> posZ >> normX >> normY >> normZ >> textX >> textY;
				if (file.eof()) break;
				this->pontos.push_back(posX);
				this->pontos.push_back(posY);
				this->pontos.push_back(posZ);
				this->normais.push_back(normX);
				this->normais.push_back(normY);
				this->normais.push_back(normZ);
				this->text.push_back(textY);
				this->text.push_back(textX);
			}
		}else {
			cout << "ERROR READING FILE " << f << " (Does not exist?)" << endl;
		}
		file.close();
	}

	void resetColor() {
		float* diffuse, * ambiente, * specular, * emissive;

		diffuse = new float[4];
		ambiente = new float[4];
		specular = new float[4];
		emissive = new float[4];

		diffuse[0] = 0.8f; diffuse[1] = 0.8f; diffuse[2] = 0.8; diffuse[3] = 1.0f;
		ambiente[0] = 0.2f; ambiente[1] = 0.2f; ambiente[2] = 0.2f; ambiente[3] = 1.0f;
		specular[0] = 0.0f; specular[1] = 0.0f; specular[2] = 0.0f; specular[3] = 1.0f;
		emissive[0] = 0.0f; emissive[1] = 0.0f; emissive[2] = 0.0f; emissive[3] = 1.0f;

		glMaterialfv(GL_FRONT, GL_AMBIENT, ambiente);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, emissive);
	}
	
	void desenhaPontos(GLuint* figuras,GLuint* normais,GLuint* texturas) {
		glBindBuffer(GL_ARRAY_BUFFER, figuras[vboID]);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glBindBuffer(GL_ARRAY_BUFFER, normais[vboID]);
		glNormalPointer(GL_FLOAT, 0, 0);

		glMaterialfv(GL_FRONT, GL_AMBIENT, this->ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, this->diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, this->specular);
		glMaterialfv(GL_FRONT, GL_EMISSION, this->emissive);

		if (this->textura=="nada") {
			glDrawArrays(GL_TRIANGLES, 0,size);
		}
		else {
			glBindBuffer(GL_ARRAY_BUFFER, texturas[vboID]);
			glTexCoordPointer(2, GL_FLOAT, 0, 0);
			glBindTexture(GL_TEXTURE_2D, idTextura);
			glDrawArrays(GL_TRIANGLES, 0, size);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
		resetColor();
	}


	void toString() {
		cout << "Model:" << nome <<"  Tamanho Pontos:" << this->size << endl;
		cout << "     Diff:" << diffuse[0] <<" "<<diffuse[1]<<" "<< diffuse[2]<<" " << diffuse[3] << endl;
		cout << "     Emiss:" << emissive[0] << " " << emissive[1] << " " << emissive[2] << " " << emissive[3] << endl;
		cout << "     Spec:" << specular[0] << " " << specular[1] << " " << specular[2] << " " << specular[3]<<endl;
		cout << "     Amb:" << ambient[0] << " " << ambient[1] << " " << ambient[2]<<" " << ambient[3] << endl;
		cout << "     Texture:" << this->textura<<endl;
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
	bool rotate;

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
	TranslateAnimated(float time, vector<float> trajetoria, int desenhaTrajetoria,int idTrajetoria,bool rotate) {
		this->timeAnimacao = time * 1000;
		this->desenhaTrajetoria = desenhaTrajetoria;
		this->rotate = rotate;
		this->indice = idTrajetoria;
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
		cout << "TranslateAnimated" << "  Time:" << this->timeAnimacao <<"  trace:"<< this->desenhaTrajetoria<< "  Trajetoria:" << endl;
		for (int i = 0; i < this->trajetoria.size(); i++) {
			cout << "        (" << trajetoria.at(i).x << "," << trajetoria.at(i).y << "," << trajetoria.at(i).z << ")" << endl;
		}
	}

	void prepare(GLuint* trajetorias) {
		if (desenhaTrajetoria != 0) {
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
		glBindBuffer(GL_ARRAY_BUFFER, trajetorias[this->indice]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * desenhaTrajetoria * 3, arrayPontos, GL_STATIC_DRAW);
	}
}

	void traceCurve(GLuint* trajetorias) {
		if (desenhaTrajetoria != 0) {
			glEnable(GL_COLOR_MATERIAL);
			glColor3f(1.0f, 1.0f, 1.0f);
			glBindBuffer(GL_ARRAY_BUFFER, trajetorias[this->indice]);
			glVertexPointer(3, GL_FLOAT, 0, 0);
			glDrawArrays(GL_LINE_LOOP, 0, desenhaTrajetoria);
			glDisable(GL_COLOR_MATERIAL);
		}
	}

	void apply(float timeAtual, bool traj, GLuint* trajetorias) {
		float matrizRotacao[4][4];
		float tempoRelativo;
		tempoRelativo = timeAtual / this->timeAnimacao;
		tempoRelativo -= floor(tempoRelativo);

		if (traj) traceCurve(trajetorias);

		getGlobalCatmullRomPoint(tempoRelativo, pos, deriv);
		glTranslatef(pos[0], pos[1], pos[2]); // por o modelo no sitio

		if (this->rotate) {
			//X(i)=P'(T)=deriv
			normalize(deriv);
				
			//Z(i)=X(i) x Y(i-1)
			cross(deriv, up, z);
			normalize(z);
	
			//Y(i)=Z(i) x X(i)
			cross(z, deriv, up);
			normalize(up);
	
			buildRotMatrix(deriv, up, z, *matrizRotacao);		// matriz para rodar o objeto
			glMultMatrixf(*matrizRotacao);						// aplicar matriz de rota��o
		}

	}
};

class Luz {
	string tipo;
	GLenum id;
	float* pos;
	float* dir;
	float atenuacao,cutoff, exp;
	float* diff, *spec, *amb;

public:
	Luz(string tipo, GLenum i, float* p, float* d, float a, float cut,float exp, float* diff, float* amb, float* spec) {
		this->tipo = tipo;
		this->id = i;
		this->pos = p;
		this->dir = d;
		this->atenuacao = a;
		this->cutoff = cut;
		this->exp = exp;
		this->diff = diff;
		this->amb = amb;
		this->spec = spec;
	}
	void prepare() {
		glEnable(GL_LIGHT0 + this->id);
	}
	void apply() {
		glLightfv(GL_LIGHT0 + this->id, GL_AMBIENT, amb);
		glLightfv(GL_LIGHT0 + this->id, GL_DIFFUSE, diff);
		glLightfv(GL_LIGHT0 + this->id, GL_SPECULAR, spec);
		glLightfv(GL_LIGHT0 + this->id, GL_POSITION, pos);
		if (tipo == "POINT")
			glLightfv(GL_LIGHT0 + this->id, GL_QUADRATIC_ATTENUATION, &(this->atenuacao));
		else if (tipo == "SPOT" ) {
			glLightfv(GL_LIGHT0 + this->id, GL_SPOT_DIRECTION, this->dir);
			glLightfv(GL_LIGHT0 + this->id, GL_SPOT_EXPONENT, &(this->exp));
			glLightfv(GL_LIGHT0 + this->id, GL_SPOT_CUTOFF, &(this->cutoff));
		}
	}

	void toString() {
		cout << "Light: " << tipo << endl;
	}
};

class Instrucao {
	string nome;
	float x=0;
	float y=0;
	float z=0;
	float angle=0;
	float time=0;                   
	TranslateAnimated* tAnimated;
	Modelo* model;
	Luz* luz;

public:
	Instrucao(string nome) {
		this->nome = nome;
		this->tAnimated = NULL;
		this->model = NULL;
		this->luz = NULL;
	}
	Instrucao() {}

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

	void translateAnimated(vector<float> trajetoria, float time,int desenhaTrajetoria,int idTrajetorias,bool rotate) {
		this->nome = "TranslateAnimated" ;
		this->tAnimated = new TranslateAnimated(time,trajetoria,desenhaTrajetoria,idTrajetorias,rotate);
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
	void luzIns(Luz* luz) {
		this->nome = "Luz";
		this->luz = luz;
	}
	void popMatrix() {
		this->nome = "PopMatrix";
	}
	void pushMatrix() {
		this->nome = "PushMatrix";
	}
	void prepare(GLuint* figuras,GLuint* normais,GLuint* texturas,GLuint* trajetorias) {
		if (this->nome == "TranslateAnimated")
			tAnimated->prepare(trajetorias);
		else if (this->nome == "Model")
			model->formaVbo(figuras, normais, texturas);
		else if (this->nome == "Luz")
			luz->prepare();
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
			this->model->toString();
		else if (nome == "PopMatrix")
			cout << nome << endl;
		else if (nome == "PushMatrix")
			cout << nome << endl;
		else if (nome == "Luz")
			this->luz->toString();
		else cout << "Invalido:" << nome << endl;
	}

	void apply(GLuint* figuras, GLuint* normais, GLuint* texturas, GLuint* trajetorias,float tempo,bool traj) {	
		if (nome == "Scale")
			glScalef(this->x, this->y, this->z);
		else if (nome == "Rotate")
			glRotatef(this->angle, this->x, this->y, this->z);
		else if (nome == "RotateAnimated")
			glRotatef(((tempo / this->time) * 360), this->x, this->y, this->z);
		else if (nome == "Translate")
			glTranslatef(this->x, this->y, this->z);
		else if (nome == "TranslateAnimated")
			this->tAnimated->apply(tempo, traj, trajetorias);
		else if (nome == "Color")
			glColor3f(this->x, this->y, this->z);
		else if (nome == "Model")
			model->desenhaPontos(figuras, normais,texturas);
		else if (nome == "PopMatrix")
			glPopMatrix();
		else if (nome == "PushMatrix")
			glPushMatrix();
		else if (nome == "Luz")
			this->luz->apply();
		else cout << "Invalido:" << nome << endl;
	}
};

class Transformacao {
	vector<Instrucao*> instrucoes;
	GLuint* figuras;
	GLuint* normais;
	GLuint* texturas;
	GLuint* trajetorias;
	int nTrajetorias=0;
	int nModelos = 0;
	int nLuzes = 0;

public:
	Transformacao() {
		this->instrucoes = vector<Instrucao*>();
		figuras = NULL;
		normais = NULL;
		texturas = NULL;
		trajetorias = NULL;
	}

	int getNTranslatedAnimated() {
		return this->nTrajetorias;
	}
	
	GLuint getSizeModelos() {
		return this->nModelos;
	}

	void prepare(GLuint* figures,GLuint* normais,GLuint* texturas,GLuint* trajetorias) {
		this->figuras = figures;
		this->normais = normais;
		this->texturas = texturas;
		this->trajetorias = trajetorias;
		for (Instrucao* i : this->instrucoes) {
				i->prepare(this->figuras,this->normais,this->texturas,this->trajetorias);
		}
	}

	void addModel(string nomeModel, vector<float> d, vector<float> e,vector<float> a,vector<float> s, string textura) {
		Instrucao* i = new Instrucao();
		Modelo* newModel = new Modelo(nomeModel,nModelos,d,e,a,s,textura);
		i->modelo(newModel);
		instrucoes.push_back(i);
		nModelos++;
	}

	void addLight(string tipo, float* posicao, float* direcao, float at, float cut, float expoente,float* diff, float* amb, float* spec) {
		Instrucao* i = new Instrucao();
		Luz* luz = new Luz(tipo,nLuzes,posicao,direcao,at,cut,expoente,diff,amb,spec);
		i->luzIns(luz);
		instrucoes.push_back(i);
		nLuzes++;
	}

	void addTranslate(float x, float y, float z) {
		Instrucao* i =new Instrucao();
		i->translate(x, y, z);
		instrucoes.push_back(i);
	}

	void addTranslateAnimated(vector<float> trajetoria, float time,int desenhaTrajetoria,bool rotate) {
		Instrucao* i = new Instrucao();
		i->translateAnimated(trajetoria,time,desenhaTrajetoria,this->nTrajetorias,rotate);
		instrucoes.push_back(i);
		this->nTrajetorias++;
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
			m->apply(figuras,normais,texturas,trajetorias,time,traj);
		}
	}

	void toString() {
		for (Instrucao* m : this->instrucoes) {
			m->toString();
		}
	}
};

#endif