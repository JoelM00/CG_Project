#pragma once
#ifndef __leitorXML__
#define __leitorXML__
#include <stdlib.h> 
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "Transformacoes.h"
#include "tinyxml2/tinyxml2.h"

#define _USE_MATH_DEFINES
#define DIR "../files/"
#include <math.h>

using namespace std;
using namespace tinyxml2;

class leitorXML {
	string ficheiro;
	Transformacao* transf;

public:
	leitorXML() {
		ficheiro = "";
		this->transf = NULL;
	}

	leitorXML(string fich, Transformacao* transf) {
		ficheiro = fich;
		this->transf = transf;
	}


	void parseModelo(XMLElement* models) {
	for (XMLElement* elem = models->FirstChildElement("model"); elem; elem = elem->NextSiblingElement("model")) {
		string file = elem->Attribute("file");
		(this->transf)->addModel(file);
	}
}

	void parseAnimatedTranslate(XMLElement* translate) {
		vector<float> trajetoria;
		int desenhaTrajetoria = 0;
		if (translate->FindAttribute("trace")) {
			desenhaTrajetoria = translate->Int64Attribute("trace");
		}
		for (XMLElement* ponto = translate->FirstChildElement("point"); ponto; 
			ponto = ponto->NextSiblingElement("point")) {
			trajetoria.push_back(ponto->FloatAttribute("x"));
			trajetoria.push_back(ponto->FloatAttribute("y"));
			trajetoria.push_back(ponto->FloatAttribute("z"));
		}
		transf->addTranslateAnimated(trajetoria, translate->FloatAttribute("time"),desenhaTrajetoria);
	}

	void parseGrupo(XMLElement* group) {
	transf->addPush();
	for (XMLElement* elem = group->FirstChildElement(); elem; elem = elem->NextSiblingElement()) { 
		string type = elem->Value();
		if (type.compare("translate") == 0) {
			if (elem->FindAttribute("time")) {
				parseAnimatedTranslate(elem);
			}else
			transf->addTranslate(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z"));
		}
		else if (type.compare("rotate") == 0) {
			if (elem->FindAttribute("time")) {
				transf->addRotateAnimated(elem->FloatAttribute("x"), elem->FloatAttribute("y"), 
					elem->FloatAttribute("z"),elem->FloatAttribute("time"));
			}else
			transf->addRotate(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z"), elem->FloatAttribute("angle"));
		}
		else if (type.compare("scale") == 0) {
			transf->addScale(elem->FloatAttribute("x"), elem->FloatAttribute("y"), elem->FloatAttribute("z"));
		}
		else if (type.compare("models") == 0) {
			parseModelo(elem);
		}
		else if (type.compare("group") == 0) {
			parseGrupo(elem);
		}
		else if (type.compare("colour") == 0) {
			float red = (elem->IntAttribute("r")) / 255.0f;
			float green = (elem->IntAttribute("g")) / 255.0f;
			float blue = (elem->IntAttribute("b")) / 255.0f;
			transf->addColor(red, green, blue);
		}
		else {
			cout << "Erro ao ler grupo!" << endl;
			cout << "Devido a: " << elem->Value() << endl;
		}
	}
	transf->addPop();
	}

	Transformacao* leXML() {
		XMLDocument doc;
		XMLNode* root;

	if (!(doc.LoadFile(this->ficheiro.c_str()))) {
		root = doc.FirstChild(); 
		for (XMLElement* group = root->FirstChildElement("group"); group; group = group->NextSiblingElement("group")) {
			parseGrupo(group);
		}
		cout << "Feito! A ler do ficheiro: " << this->ficheiro << endl;
	}
	else {
		cout << "Erro ao ler XML!" << endl;
	}
	return this->transf;
	}
};


#endif