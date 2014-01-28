#ifndef IMAGEDATA_H
#define IMAGEDATA_H

#include <cstdlib>
#include <vector>
#include "directionalscan.h"
#include "pixel.h"
using namespace std;


/** 
 * @class ImageData imagedata.h
 * \brief Image data processing tools.
 * \author {B. Kerautret}
 * Revisited by P. Even
 */
class ImageData
{

public:

  /** 
   * \brief  Creates a structure to store and process image data.
   * \author {P. Even}
   */
  ImageData (int largeur, int hauteur);

  /** 
   * \brief  Deletes the image data structure.
   * \author {P. Even}
   */
  ~ImageData ();

  /** 
   * \brief  OVNI.
   * \author {B. Kerautret}
   */
  Pixel **getVectorsGradientRepereNornal (int **tabImage);

  /** 
   * \brief  Builds the gradient map.
   * \author {P. Even}
   */
  void buildGradientMap (int **data);

  /** 
   * \brief  Returns the image width.
   * \author {P. Even}
   */
  int getWidth ();

  /** 
   * \brief  Returns the image height.
   * \author {P. Even}
   */
  int getHeight ();

  /**
   * \brief  Checks if the given pixel lies within the image bounds.
   * \author {P. Even}
   * @param p : tested pixel.
   */
  inline bool contains (int i, int j)
  {
    return (i >= 0 && i && width && j >= 0 && j < height);
  }

  /** 
   * \brief  Extracts points with maximal gradient along the directional scan.
   * \author {B. Kerautret}
   */
  vector<Pixel> maxGradient (DirectionalScan &dirScan,
                           int direction, int threshold);


  /** 
   * \brief  Extracts ridges with maximal gradient along the directional scan.
   * \author {B. Kerautret}
   */
  vector<vector<Pixel> > gradientRidges (DirectionalScan &dirScan,
                           int direction, Pixel vectDir, double cosDirLimits);

  /**
   * Renvoie un vecteur contenant les indices des maximas locaux:
   * @param vectValues : l'ensemble des valeurs
   * 2 erreurs :
   *  - utilisation d'une methode generique alors que nos contours sont
   *     toujours ouverts
   *  - plus genant : erreur de parametrage qui fait que cette methode ne
   *     retourne que des max et non des plateaux
   */
  vector<int> getLocalMaximaIndices (vector<long> &vectValues);

  /**
   * Renvoie un vecteur contenant les indices des minimas locaux:
   * @param vectValues : l'ensemble des valeurs
   * 2 erreurs :
   *  - utilisation d'une methode generique alors que nos contours sont
   *     toujours ouverts
   *  - plus genant : erreur de parametrage qui fait que cette methode ne
   *     retourne que des max et non des plateaux
   */
  vector<int> getLocalMinimaIndices (vector<long> &vectValues);



private:

  /** Gradient vector. */
  struct Gradient
  {
    int x;
    int y;    
  };

  /** Combination of point and gradient magnitude information. */
  struct PointAndGrad
  {
    Pixel pt;
    long grad;    
  };

  /** Digital signal extrema area. */
  struct LocalMinMaxArea {
    int indiceDebut;
    int indiceFin;
    bool isMaxArea;
  };


  /** Image width. */
  int width;
  /** Image height. */
  int height;

  /** Flag set to true if a image map is stored. */
  bool imageMapDefined;
  /** Image map. */
  int *imageMap;

  /** Flag set to true if a gradient map is stored. */
  bool gradientMapDefined;
  /** Gradient map. */
  Gradient *gradientMap;



  /** 
   * \brief  Compares point-and-gradient structures.
   * \author {B. Kerautret}
   */
  static bool comparePtAndGrad (PointAndGrad pg1, PointAndGrad pg2);

  /**
   * Returns a vector of extrema areas of a gradient magnitude signal.
   * @param vectValues : gradient magnitude signal.
   */
  vector<LocalMinMaxArea> getLocalMaxMinArea (vector<long> &vectValues);
};

#endif
