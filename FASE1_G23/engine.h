
#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#pragma once
#include <stdlib.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include "tinyxml2/tinyxml2.h"

#include <math.h>

using namespace std;
using namespace tinyxml2;

struct Ponto { float x; float y; float z;};

#endif