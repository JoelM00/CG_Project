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

	void parseLights(XMLElement* lights) {
		string tipo;
		float* pos,*dir,*amb,*diff,*spec;
		float atenuacao=0.0f, cutoff=0.0f, expoente=0.0f;

		for (XMLElement* elem = lights->FirstChildElement("light"); elem; elem = elem->NextSiblingElement("light")) {
			pos = new float[4];
			pos[0] = 0.0f; pos[1] = 0.0f; pos[2] = 0.0f; pos[3] = 0.0f;
			dir = new float[3];
			dir[0]=0.0f; dir[1]=0.0f; dir[2]=0.0f;
			amb = new float[4];
			amb[0] = 1.0f; amb[1] = 1.0f; amb[2] = 1.0f; amb[3] = 1.0f;
			diff = new float[4];
			diff[0] = 0.8f; diff[1] = 0.8f; diff[2] = 0.8f; diff[3] = 1.0f;
			spec = new float[4];
			spec[0] = 1.0f; spec[1] = 1.0f; spec[2] = 1.0f; spec[3] = 1.0f;

			elem->QueryFloatAttribute("diffR", &diff[0]);
			elem->QueryFloatAttribute("diffG", &diff[1]);
			elem->QueryFloatAttribute("diffB", &diff[2]);

			elem->QueryFloatAttribute("ambR", &amb[0]);
			elem->QueryFloatAttribute("ambG", &amb[1]);
			elem->QueryFloatAttribute("ambB", &amb[2]);

			elem->QueryFloatAttribute("specR", &spec[0]);
			elem->QueryFloatAttribute("specG", &spec[1]);
			elem->QueryFloatAttribute("specB", &spec[2]);
			
			elem->QueryFloatAttribute("posX", &pos[0]);
			elem->QueryFloatAttribute("posY", &pos[1]);
			elem->QueryFloatAttribute("posZ", &pos[2]); 

			tipo = elem->Attribute("type");
			if (tipo.compare("DIRECTIONAL") == 0) {
				(this->transf)->addLight(tipo,pos,dir,atenuacao, cutoff, expoente,diff,amb,spec);
			}
			else if (tipo.compare("POINT") == 0) {
				pos[3]= 1.0f;
				elem->QueryFloatAttribute("atenuacao", &atenuacao);
				(this->transf)->addLight(tipo, pos, dir, atenuacao, cutoff, expoente,diff, amb, spec);
			}
			else if (tipo.compare("SPOT") == 0) {
				pos[3] = 1.0f;
				elem->QueryFloatAttribute("cutoff", &cutoff);
				elem->QueryFloatAttribute("expoente", &expoente);
				elem->QueryFloatAttribute("dirX", &dir[0]);
				elem->QueryFloatAttribute("dirY", &dir[1]);
				elem->QueryFloatAttribute("dirZ", &dir[2]);
				(this->transf)->addLight(tipo, pos, dir, atenuacao, cutoff, expoente, diff, amb, spec);
			}		
		}
	}

	void parseModelo(XMLElement* models) {
		
		for (XMLElement* elem = models->FirstChildElement("model"); elem; elem = elem->NextSiblingElement("model")) {
		string file = elem->Attribute("file");
		
		vector<float> diffuse;
		vector<float> ambiente;
		vector<float> specular;
		vector<float> emissive;

		float controlPoint;

		if (elem->FindAttribute("diffR")) {
			diffuse.push_back(elem->FloatAttribute("diffR"));
			diffuse.push_back(elem->FloatAttribute("diffG"));
			diffuse.push_back(elem->FloatAttribute("diffB"));
			diffuse.push_back(1.0f);
		}else diffuse = { 0.8f,0.8f,0.8f,1.0f };

		if (elem->FindAttribute("ambR")) {
			ambiente.push_back(elem->FloatAttribute("ambR"));
			ambiente.push_back(elem->FloatAttribute("ambG"));
			ambiente.push_back(elem->FloatAttribute("ambB"));
			ambiente.push_back(1.0f);
		}else ambiente = { 0.2,0.2,0.2,1.0f };

		if (elem->FindAttribute("specR")) {
			specular.push_back(elem->FloatAttribute("specR"));
			specular.push_back(elem->FloatAttribute("specG"));
			specular.push_back(elem->FloatAttribute("specB"));
			specular.push_back(1.0f);
		}else specular = {0.0f,0.0f,0.0f,1.0f};

		if (elem->FindAttribute("emissR")) {
			emissive.push_back(elem->FloatAttribute("emissR"));
			emissive.push_back(elem->FloatAttribute("emissG"));
			emissive.push_back(elem->FloatAttribute("emissB"));
			emissive.push_back(1.0f);
		}else emissive = { 0.0f,0.0f,0.0f,1.0f };
		
		const char* textura = elem->Attribute("texture");

		if (textura)
			(this->transf)->addModel(file,diffuse,emissive,ambiente,specular,textura);
		else 
			(this->transf)->addModel(file, diffuse, emissive, ambiente, specular, "nada");
	}
}

	void parseAnimatedTranslate(XMLElement* translate) {
		vector<float> trajetoria;
		int desenhaTrajetoria = 0;
		bool rotate =true;
		if (translate->FindAttribute("trace")) {
			desenhaTrajetoria = translate->Int64Attribute("trace");
		}
		if (translate->FindAttribute("rotate")) {
			string rot = translate->Attribute("rotate");
			if (rot == "true") rotate = true; else rotate = false;
		}
		for (XMLElement* ponto = translate->FirstChildElement("point"); ponto; 
			ponto = ponto->NextSiblingElement("point")) {
			trajetoria.push_back(ponto->FloatAttribute("x"));
			trajetoria.push_back(ponto->FloatAttribute("y"));
			trajetoria.push_back(ponto->FloatAttribute("z"));
		}
		transf->addTranslateAnimated(trajetoria, translate->FloatAttribute("time"),desenhaTrajetoria,rotate);
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
		else if (type.compare("lights") == 0) {
			parseLights(elem);
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