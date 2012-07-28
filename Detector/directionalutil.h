#ifndef DIRECTIONALUTIL_H
#define DIRECTIONALUTIL_H

#include <QPoint>
#include <cstdlib>
#include <vector>
#include "directionalscan.h"
#include <QImage>
using namespace std;


/** 
 * @class DirectionalGradient directionalGradient.h
 * \brief Class to create compute gradient from scan obtained by DirectionalScans.
 * \author {B. Kerautret}
 */
class DirectionalUtil
{

public:

  struct PointAndGrad
  {
    QPoint pt;
    double grad;    
  };

  struct PointAndCorrel
  {
    QPoint pt;
    double correl;    
  };

  
  /**
   * \brief  Checks if @param{pg1} correlation value is less than @param{pg2} one.
   * \author {B. Kerautret}
   */  
  static bool compPtAndCorrel (PointAndCorrel pg1, PointAndCorrel pg2);

  /**
   * \brief  Checks if @param{pg1} gradient norm is greater than @param{pg2} one.
   * \author {B. Kerautret}
   */  
  static bool compPtAndGrad (PointAndGrad pg1, PointAndGrad pg2);

  /**
   * \brief  Checks if norm(@param{p}[1]) is greater than norm (@param{q}[1]).
   * \author {B. Kerautret}
   */  
  static bool compGradVectors (QPoint *p , QPoint *q);

  /**
   * \brief  Returns the norm of the difference between @param{vReference} and @param{v}.
   * \author {B. Kerautret}
   */  
  static int getCorrelationValue (vector<int> &vReference, vector<int> &v);

  /**
  * A partir d'un vecteur de correlation donne, calcule pour chaque point
  * du profil @numscan la valeur de la correlation,
  * correlation definie par la moyenne dans un voisinage pour chaque point
  * en fonction des deux parametres @correlWidth et @correlThick
  * @correlWidth = Demi-largeur de la correlation = N/2
  *   ou N est le nombre de points dans la section d'une correlation
  * @correlThick =  Demi-epaisseur de la correlation = K/2
  *   ou K est l'epaisseur totale de la correlation (ou le nb de scans)
  */
  static vector<PointAndCorrel> getPointsCorrelFromScans (
                  vector<int> &correlRef, DirectionalScan &dirScan,
                  int numScan, int **tabImage, int largeur, int hauteur,
                  int direction, int correlWidth, int correlThick);

  static vector< vector<QPoint> > getPointsCorrelMaxLocalFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur, int direction,
                  int correlWidth, int correlThick, // int indiceSection0,
                  int indicePcenterSection0);

  static vector<int>  getMeanValueCorrelScan (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur, int numScan, int direction,
                  int posDansScan, int correlWidth, int correlThick);

  static vector< vector<QPoint> > getPointsGradMaxLocalFromScans (
                  DirectionalScan &dirScan, QPoint **tabGradient,
                  int largeur, int hauteur, bool ** tabGradientUsed,
                  int direction, QPoint vectDir, double cosDirLimits);

  static vector<QPoint> getGradientMaxFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur,
                  QPoint **tabGradient, bool **tabGradientUsed,
                  int direction, QPoint vectDir, double seuil);

  static vector<QPoint> getGradientMaxFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur,
                  QPoint **tabGradient, bool **tabGradientUsed,
                  int direction, int seuil);

  static vector<QPoint> getGradientDirMaxFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur, int direction);
 
  static vector<QPoint> getGradientDirMaxFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur, int direction, QPoint vectDir);
 
  static QPoint **retVectorsGradientRepereNornal (
                  int **tabImage, int largeur, int hauteur);
  
  static QPoint **retVectorsGradientRepereNornal (
                  int **tabImage, int largeur, int hauteur, int nbIteration);

  static vector< vector<QPoint> > getGradientVectorsFromScans(
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur,
                  QPoint **tabGradient, bool **tabGradientUsed,
                  int direction, QPoint vectDir, double seuil);
 
  static vector<QPoint> getPointsCorrelMaxLocalFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur, int direction,
                  int correlWidth, int correlThick, int indiceSection0,
		  int indicePcenterSection0, int position);

  // static vector<QPoint>* getGradientVectorsMaxFromScans (
  //                DirectionalScan &dirScan, int **tabImage,
  //                int largeur, int hauteur, int direction);


  /** 
   * 
   * \brief  Returns the vector of points with maximal gradient value in the direction @param{Direction}
   * \author {B. Kerautret}
   */  
  static vector<QPoint>* getGradientDirVectorsFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur, int direction, int numScan); 
 

  /** 
   * 
   * \brief  Returns the vector of points associated to the border lines on one of the sides of the scan area.
   * \author {B. Kerautret}
   */
  static vector<QPoint> getScansBorder (DirectionalScan &dirScan,
                                        QImage &image, int direction);

  /** 
   * 
   * \brief  Returns all the "num"-th elements from each scan.
   * \author {B. Kerautret}
   */
  static vector<QPoint> getElementsI (DirectionalScan &dirScan,
                                      QImage &image, int direction, int num);

  /** 
   * 
   * \brief  Returns the image of duplicated points in vect.
   * \author {B. Kerautret}
   */
  static vector<QPoint> getDuplicatedPoints (vector<QPoint> &vect,
                                             int largeur, int hauteur);
 
  /** 
   * 
   * \brief  Returns two points at a given distance from the given point in the given direction.
   * \author {B. Kerautret}
   */
  static QPoint *getAjustedPoints (QPoint pCenter,
                                   double direction, double distance);


  /** 
   * 
   * \brief  Returns the gray-value of the pixels of the scan area within the image bounds.
   * \author {B. Kerautret}
   */
  static vector<int> getImageSection (DirectionalScan &dirScan,
                                      int numSection, int direction,
                                      int **tabImage, int largeur, int hauteur);


  DirectionalUtil (int **tabImage, int largeur, int hauteur,
		   int correlWidth, int correlThick, int indicePcenterSection0);

  vector<QPoint> getScanVectCorrel (DirectionalScan &dirScan,
                                    int pos, int direction);

  static bool isScanImgDefined (DirectionalScan &dirScan,
                                int pos, int direction);
 
  ~DirectionalUtil ();


private:

  int **tabImage;
  int largeur;
  int hauteur;
  int correlThick;
  int correlWidth;
  int indicePcenterSection0;
};

#endif
