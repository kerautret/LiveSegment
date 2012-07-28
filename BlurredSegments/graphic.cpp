/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *                                                                  * 									   *
 *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/
#include "graphic.h"

Graphic::Graphic()
{
}

Graphic::Graphic(long startX, long startY, long ampleur)
{
	this->startX=startX;
	this->startY=startY;
	coeAmpleur=ampleur;	
}

Graphic::Graphic(long startX, long startY, long ampleur, char* file)
{
	this->startX=startX;
	this->startY=startY;
	coeAmpleur=ampleur;
	filename=file;
}

void Graphic::print()
{
	cout<<"StartX: "<<startX<<endl;
	cout<<"StartY: "<<startY<<endl;
	cout<<"Ampleur: "<<coeAmpleur<<endl;
	cout<<"File name: "<<filename<<endl;
}
long Graphic::getStartX()
{
	return startX;
}

long Graphic::getStartY()
{
	return startY;
}

int Graphic::getAmpleur()
{
	return coeAmpleur;
}

char* Graphic::getFilename()
{
	return filename;
}
void Graphic::setStartX(long startX)
{
	this->startX=startX;
}

void Graphic::setStartY(long startY)
{
	this->startY=startY;
}

void Graphic::setAmpleur(int ampleur)
{
	coeAmpleur=ampleur;
}
void Graphic::setFilename(char* filename)
{
	this->filename=filename;
}

Graphic::~Graphic()
{
}
