#ifndef LINEDETECTION_H
#define LINEDETECTION_H

#include <QPoint>
#include <vector>
#include "BlurredSegment.h"
#include "directionalscan.h"
using namespace std;


/** 
 * @class LineDetection linedetection.h
 * \brief Class to create a scan in the main direction defined by two points.
 * \author {B. Kerautret}
 */
class LineDetection
{

public:
  
  LineDetection ();

  static double  seuil;  

  static vector<QPoint> detectLine (QPoint p1, QPoint p2,
                   int **tabImage, int largeur, int hauteur,
                   QPoint **tabGradient, bool **tabGradientUsed,
                   double epaisseurDetection, int nbPixelInteruption,
                   int numberStep);

  static vector<QPoint> detectLine2 (QPoint p1, QPoint p2,
                   int** tabImage, int largeur, int hauteur,
                   QPoint **tabGradient, bool **tabGradientUsed,
                   double epaisseurDetection, int nbPixelInteruption,
                   int numberStep);

  static vector<QPoint> detectLine3 (QPoint p1, QPoint p2,
                   int largeur, int hauteur,
                   QPoint **tabGradient, bool **tabGradientUsed,
                   double epaisseurDetection, int nbPixelInteruption,
                   int numSolution, int numStep);

  static vector<QPoint> detectLineProfils (QPoint p1, QPoint p2,
                   int **tabImage, int largeur, int hauteur,
                   double epaisseurDetection, int nbPixelInteruption,
                   int numberSolution, int numberStep);

  LineDetection (vector<QPoint> vLeft, vector<QPoint> vRight,
                 double epaisseur, int interuptSizeMax);
  
  LineDetection (vector<vector<QPoint> > vLeft, vector<vector<QPoint> > vRight,
                 double epaisseur, int interuptSizeMax);

  LineDetection (vector<vector<QPoint> > vLeft, vector<vector<QPoint> > vRight,
                 double epaisseur, int interuptSizeMax,
                 QPoint p0, QPoint pL, QPoint pR);

  LineDetection (vector< vector<QPoint> > vLeft,
                 vector< vector<QPoint> > vRight,
                 double epaisseur, int interuptSizeMax, int numSolution);

  LineDetection (DirectionalScan &dirScan, int **tabImage,
            int largeur, int  hauteur, int correlWidth, int correlThick,
            int indiceCenterSection0, double epaisseur, int interuptSizeMax);

  LineDetection (vector< vector<QPoint> > vLeft,
                 vector< vector<QPoint> > vRight,
                 double epaisseur, int interuptSizeMax, bool centered);

  static vector<QPoint> getPointsFromIndices (DirectionalScan &dirScan,
                 int numSection, int direction, vector<int> vIndice);

  ~LineDetection ();

  static bool compNearest (QPoint p1, QPoint p2);

  BlurredSegment *segmentFlou;
  bool isDetected;
  vector<QPoint> pointAdded;
  int nbPointAdded;

  QPoint pCentral0;
  QPoint pRight;
  QPoint pLeft;

};
#endif
