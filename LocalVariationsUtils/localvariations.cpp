//#include <QtGui>
//#include <iostream>
#include "localvariations.h" 
#include <algorithm>
using namespace std;



/*
 *  Renvoie un vecteur contenant les indices des minimas locaux:
 *  @param vectValues : l'ensemble des valeurs
 *  @param epsilon : valeur de précision pour considérer que deux valeurs sont égales
 *  @param isClosed: modifie le calcul dans le cas de courbes fermées.
*/
vector<int> LocalVariations::getLocalMinimaIndices (
                 vector<double> &vectValues, double epsilon, bool isClosed) {
  vector<int> vResult;
  vector<LocalVariations::LocalMinMaxArea> vectAreas
    = LocalVariations::getLocalMaxMinArea (vectValues, epsilon, isClosed);
  vector<LocalVariations::LocalMinMaxArea>::iterator it = vectAreas.begin ();
  int tailleTab = vectValues.size();
  
  while (it != vectAreas.end ()) {
    LocalVariations::LocalMinMaxArea area = *it;
    if (! area.isMaxArea) {      
      int deb = area.indiceDebut;
      int fin = area.indiceFin;
      int longueurInterval = (deb <= fin) ? (fin-deb) : ((fin+tailleTab)-deb);
      int milieu = (deb + (longueurInterval / 2)) % tailleTab;    
      vResult.push_back (milieu);        
    }
    ++it;
  }
  return vResult;
}



/*
 *  Renvoie un vecteur contenant les indices des minimas locaux:
 *  @param vectValues : l'ensemble des valeurs
 *  @param epsilon : valeur de précision pour considérer que deux valeurs sont égales
 *  @param isClosed: modifie le calcul dans le cas de courbes fermées.
*/
vector<int> LocalVariations::getLocalMaximaIndices (
               vector<double> &vectValues, double epsilon, bool isClosed) {
  vector<int> vResult;
  vector<LocalVariations::LocalMinMaxArea> vectAreas
    = LocalVariations::getLocalMaxMinArea (vectValues, epsilon, isClosed);
  vector<LocalVariations::LocalMinMaxArea>::iterator it = vectAreas.begin ();
  int tailleTab = vectValues.size ();
  while (it != vectAreas.end ()) {
    LocalVariations::LocalMinMaxArea area = *it;
    if(area.isMaxArea) {
      int deb = area.indiceDebut;
      int fin = area.indiceFin;
      int longueurInterval = (deb <= fin) ? (fin-deb) : ((fin+tailleTab)-deb);
      int milieu = (deb + (longueurInterval / 2)) % tailleTab;
      vResult.push_back (milieu);
    }
    ++it;
  }
  return vResult;
}



/*
 *  Renvoie un vecteur contenant les zones associées au minima/maxima locaux:
 *  Structures LocalMinMaxArea (debut, fin, bool estMax)
*/
vector<LocalVariations::LocalMinMaxArea> LocalVariations::getLocalMaxMinArea (
              vector<double> &vectValues, double epsilon, bool isClosed) {
  vector<LocalVariations::LocalMinMaxArea> vectResuMinMaxArea;
  vector<double>::iterator iter = vectValues.begin ();

  int tailleVValues = vectValues.size ();

  int decalage = 1;
  bool estMontant = true;

  for (int i = 0; i < tailleVValues; i++) {
    vectValues.at(i) = ((double) ((int)(vectValues.at(i) / epsilon))) * epsilon;
  }
  
  while (decalage < tailleVValues
         && vectValues.at ((tailleVValues + decalage) % tailleVValues)
            == vectValues.at (0)) {
    if (vectValues.at ((tailleVValues + decalage) % tailleVValues)
        -vectValues.at ((tailleVValues + decalage + 1) % tailleVValues) < 0) {
      estMontant = true;
      break;
    }
    if (vectValues.at ((tailleVValues + decalage) % tailleVValues)
        -vectValues.at ((tailleVValues + decalage + 1) % tailleVValues) > 0) {
      estMontant = false;
      break;
    }
    decalage++;
  }
  
  int borneSup = (isClosed) ? tailleVValues + decalage + 1 : tailleVValues - 1;
  
  for(int i = decalage; i < borneSup; i++) {
    if (estMontant){
      if ((vectValues.at ((i + 1) % tailleVValues)
           - vectValues.at (i % tailleVValues)) < 0) {
	estMontant = false;
	int k = i + tailleVValues;
	LocalVariations::LocalMinMaxArea lMinMax;
	lMinMax.indiceFin = k % tailleVValues;
       	while (vectValues.at (k % tailleVValues)
               == vectValues.at (i % tailleVValues)) {
	  k--;
	}
	lMinMax.indiceDebut = (k + 1) % tailleVValues;
	lMinMax.isMaxArea = true;
	vectResuMinMaxArea.push_back (lMinMax);
      }
    } else {
      if (vectValues.at ((i + 1) % tailleVValues)
          - vectValues.at ((i) % tailleVValues) > 0) {
	estMontant = true;
	int k = i + tailleVValues;
	LocalVariations::LocalMinMaxArea lMinMax;
	lMinMax.indiceFin = k % tailleVValues;
	while (vectValues.at (k % tailleVValues)
               == vectValues.at (i % tailleVValues)) {
	  k--;
	}
	lMinMax.indiceDebut = (k + 1) % tailleVValues;
	lMinMax.isMaxArea = false;
	vectResuMinMaxArea.push_back (lMinMax);
      }
    }
  }

  return vectResuMinMaxArea;
}



vector<int> LocalVariations::getVectGradientSection (vector<int> &vectSection) {
  vector<int> vectGradient;
  vector<int> vectFlou;
  int tailleSection = vectSection.size ();
  for (int i = 1; i < tailleSection - 1; i++) {
    int val = vectSection.at (i + 1)
              + vectSection.at (i - 1) + 2 * vectSection.at (i);
    if (i == 1) {
      vectFlou.push_back (val);
    }
    if (i == tailleSection - 2) {
      vectFlou.push_back (val);
    }
    vectFlou.push_back (val);
  }

  for (int i = 1; i < tailleSection - 1; i++) {
    int grad = vectFlou.at (i + 1) - vectFlou.at (i - 1);
    vectGradient.push_back (abs (grad));
  }
  return vectGradient;
}



vector<int> LocalVariations::getVectIndicesMaxLocalGradSect (
               vector<int> &vectSection) {
  vector<int> vectGradient = getVectGradientSection (vectSection);
  vector<double> vectGradientD;
  vector<int> vResult;
  vector<int>::iterator it = vectGradient.begin ();
  while (it != vectGradient.end ()) {
    vectGradientD.push_back ((double) *it);    
    ++it;
  }
  
  vector<int> vIndicesLoc = getLocalMaximaIndices (vectGradientD, 0.1, false);

  // Contruction d'un vecteur de gradient indices (+ rajout 1 dans les indices)
  vector<GradientIndices> vectGI;
  vector<int>::iterator itI = vIndicesLoc.begin ();
  while (itI != vIndicesLoc.end ()) {
    GradientIndices gi;
    gi.indice = (*itI) + 1;
    gi.gradient = vectGradient.at (gi.indice);
    vectGI.push_back (gi);
    ++ itI;
  }  

  // classement du vecteur
  sort (vectGI.begin (), vectGI.end (), compGradInd);
  // extraction des incides
  vector<GradientIndices>::iterator itGI = vectGI.begin ();
  while (itGI != vectGI.end ()) {
    GradientIndices gi = *itGI;
    vResult.push_back (gi.indice);
    ++ itGI;
  }

  return vResult;
}



bool LocalVariations::compGradInd (GradientIndices gi1, GradientIndices gi2) {
  int gradient1 = gi1.gradient;
  int gradient2 = gi2.gradient;
  return gradient1 > gradient2;  
}
