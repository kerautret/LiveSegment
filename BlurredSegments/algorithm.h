
#ifndef ALGORITHM_H
#define ALGORITHM_H

#include "antipodalh.h"
#include "antipodalv.h"

AntiPodalH determineAPH(Node* cvhull, AntiPodalH ah, Point2D M);
AntiPodalV determineAPV(Node* cvhull, AntiPodalV av, Point2D M);

#endif
