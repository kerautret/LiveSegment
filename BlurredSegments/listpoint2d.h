/***************************************************************************
 *   Copyright (C) 2007 by Thanh-Phuong Nguyen                             *
 *   nguyentp@loria.fr                                              	   *                                                                       *
 *   This program is in the framework of project GEODIB, supported by ANR  *
 *   Its object aim at curvature and its applications for noisy discrete   *
 *   object   	                                                           *
 *   									   *
 *   No part of this program is provided in any method without author's    *
 *   permission                                                            * 
 ***************************************************************************/

#ifndef _LISTPOINT2D_H
#define _LISTPOINT2D_H


#include "point2d.h"
#include <vector>

using namespace std;

typedef vector <Point2D> Points2D;

class ListePoint2D{
protected:
	Points2D points;
	int size;
public:

    ListePoint2D();
    
    //Ajout d'un point 2D à la liste
    void addPoint2D(Point2D point);
    
    //return le point 2D d'indice i dans la liste
    Point2D getPointI(int i) const;
    
    // return la taille de la courbe
    int getSize() const;
    
    friend ostream& operator<<(ostream &os, const ListePoint2D &list);

    
    // afichage de la courbe polygonale formé par la séquence de la liste points
//     void affiche(char *nomFichierSvg);
    
    //afichage de la courbe polygonale en pixel

    void affichePixel(Graphic graphic, Color color);
    
    void affichePixel(Graphic graphic, Color color, int position);
    
    void affichePixel(Graphic graphic, Color color, int begin, int end);
    
    //afichage de la courbe en couleur
//     void affichePixelColor(char *nomFichierSvg, int Dxy, int color);
    
    //afichage de la courbe avec couleur en chaque pixel
//     void afficheWithPixelColor(char *nomFichierSvg, int Dxy, int color);
    
    
    ~ListePoint2D();

};

#endif
