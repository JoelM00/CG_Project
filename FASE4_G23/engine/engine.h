#pragma once
#include <stdlib.h> 
#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "tinyxml2/tinyxml2.h"
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glew.h>
#include <GL/glut.h>
#endif

#define _USE_MATH_DEFINES
#include <math.h>
#include "leitorXML.h"
#include "Transformacoes.h"

using namespace tinyxml2;
