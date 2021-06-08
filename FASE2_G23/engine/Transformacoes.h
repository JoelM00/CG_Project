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

public:
	Modelo(string nome, vector<Ponto> p) {
		this->pontos = p;
		this->nome = nome;
	}
	Modelo(string nome) {

		this->nome = nome;
		this->pontos = leModelo(nome);
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

	void desenhaPontos() {
		Ponto p1, p2, p3;
		int size = pontos.size();

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < size; i += 6) {
			p1 = pontos.at(i);
			p2 = pontos.at(i + 1);
			p3 = pontos.at(i + 2);

			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);

			p1 = pontos.at(i + 3);
			p2 = pontos.at(i + 4);
			p3 = pontos.at(i + 5);

			glVertex3f(p1.x, p1.y, p1.z);
			glVertex3f(p2.x, p2.y, p2.z);
			glVertex3f(p3.x, p3.y, p3.z);
		}
		glEnd();
	}


	void toString() {
		cout << "Model:" << nome <<"  Tamanho Pontos:" << pontos.size() << endl;
	}
};

class Instrucao {
	string nome;
	float x;
	float y;
	float z;
	float angle;
	Modelo* model;

public:
	Instrucao(string nome) {
		this->nome = nome;
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
	void rotate(float x, float y, float z,float angle) {
		this->nome = "Rotate";
		this->x = x;
		this->y = y;
		this->z = z;
		this->angle = angle;
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
	void toString() {
		if (nome == "Scale")
			cout << nome << " (" << x << "," << y << "," << z << ")" << endl;
		else if (nome == "Rotate")
			cout << nome << " (" << x << "," << y << "," << z << ") Angle:" << angle << endl;
		else if (nome == "Translate")
			cout << nome << " (" << x << "," << y << "," << z << ")" << endl;
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

	void apply() {
		if (nome == "Scale")
			glScalef(this->x, this->y, this->z);
		else if (nome == "Rotate")
			glRotatef(this->angle, this->x, this->y, this->z);
		else if (nome == "Translate")
			glTranslatef(this->x, this->y, this->z);
		else if (nome == "Color")
			glColor3f(this->x, this->y,this->z);
		else if (nome == "Model")
			model->desenhaPontos();
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

public:
	Transformacao() {
		this->instrucoes = vector<Instrucao*>();
		this->models = vector<Modelo*>();
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
			Modelo* newModel = new Modelo(nomeModel);
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

	void apllyTransformation() {
		for (Instrucao* m : this->instrucoes) {
			m->apply();
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

