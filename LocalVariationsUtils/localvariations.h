#ifndef LOCALVARIATIONS_H
#define LOCALVARIATIONS_H


#include <vector>
using namespace std;


class LocalVariations {

public:

  struct GradientIndices {
    int indice;
    int gradient;   
  };
    
    
  struct LocalMinMaxArea {
    int indiceDebut;
    int indiceFin;    
    bool isMaxArea;
  };


  static vector<LocalMinMaxArea> getLocalMaxMinArea (
             vector<double> &vectValues, double epsilon, bool isClosed);  
  static vector<int> getLocalMinimaIndices (
             vector<double> &vectValues, double epsilon, bool isClosed);
  static vector<int> getLocalMaximaIndices (
             vector<double> &vectValues, double epsilon, bool isClosed);

  static vector<int> getVectGradientSection (vector<int> &vectSection);
  static vector<int> getVectIndicesMaxLocalGradSect (vector<int> &vectSection);

  static bool compGradInd (GradientIndices gi1, GradientIndices gi2);
};
#endif
