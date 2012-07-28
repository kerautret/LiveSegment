#include <cstdlib>
#include <iostream>
#include <algorithm>
#include "util.h"
#include <vector>
#include "linedetection.h"
#include "directionalscan.h"
#include "directionalutil.h"
#include "localvariations.h"


double  LineDetection::seuil = 0.0;

static QPoint pRefComp;


LineDetection::LineDetection ()
{
  isDetected = false;
}


/**
  * Line detection: 
  * -detect 1: Detection en fonction simplement de la morme du gradient non directionel
  * -detect 2: Prem Max local directionnel qui fait augmenter la longueur du seg max.
  * -detect 3: Prem Max local directionnel qui fait augmenter la longueur du seg max.
  */


vector<QPoint> LineDetection::detectLineProfils (QPoint p1, QPoint p2,
                    int **tabImage, int largeur, int hauteur,
                    double epaisseurDetection, int nbPixelInteruption,
                    int numberSolution, int numStep)
{
  vector<QPoint> vectResu;

  // Premiere detection max des gradients sans direction
  //----------------------------------------------------
  DirectionalScan dirScan = DirectionalScan (p1, p2);
  dirScan.computeAllScans (0, 0, largeur, hauteur);

  vector<int> sectionImg = DirectionalUtil::getImageSection (dirScan,
                                       0, RIGHT, tabImage, largeur, hauteur);
  vector<int> centerGradient
         = LocalVariations::getVectIndicesMaxLocalGradSect (sectionImg);

  int sizeSolutionGradientSection0 = centerGradient.size ();
  int solutionSelected = numberSolution%sizeSolutionGradientSection0;

  LineDetection lined (dirScan, tabImage, largeur, hauteur,
		       3, 0, centerGradient.at (solutionSelected),
                       epaisseurDetection, nbPixelInteruption);

  if (! lined.isDetected)
  {
    cerr << "return vect VIDE" << endl;
    return vectResu;
  }

  if (numStep == 1)
  {
    vector<QPoint> pSeg= lined.segmentFlou->getPrintedPoints ();
    return pSeg;
  }

  // Detection 2
  //------------
  QPoint pCenter = lined.segmentFlou->getCenter ();
  double direction = lined.segmentFlou->getDirection ();
  QPoint *pointsAjusted = DirectionalUtil::getAjustedPoints (pCenter,
                            direction + M_PI / 2.0, 2 * epaisseurDetection);
  DirectionalScan dirScan2 = DirectionalScan (pointsAjusted[0],
                                              pointsAjusted[1]);
  dirScan2.computeAllScans (0, 0, largeur, hauteur);
  sectionImg = DirectionalUtil::getImageSection (dirScan2,
                                      0, RIGHT, tabImage, largeur, hauteur);
  centerGradient = LocalVariations::getVectIndicesMaxLocalGradSect (sectionImg);
  sizeSolutionGradientSection0 = centerGradient.size ();
  solutionSelected = numberSolution%sizeSolutionGradientSection0;

  LineDetection lined2 (dirScan2, tabImage, largeur, hauteur,
                        4, 2, centerGradient.at (solutionSelected),
                        epaisseurDetection, nbPixelInteruption);

  if (! lined2.isDetected)
  {
    cerr << "return vect VIDE" << endl;
    return vectResu;
  }

  if (numStep == 2)
  {
    vector<QPoint> pSeg = lined2.segmentFlou->getPrintedPoints ();
    return  pSeg;
  }

  // Detection 3
  //------------
  QPoint pCenter3 = lined2.segmentFlou->getCenter ();
  double direction3 = lined2.segmentFlou->getDirection ();
  QPoint *pointsAjusted3 = DirectionalUtil::getAjustedPoints (pCenter3,
                             direction3 + M_PI / 2.0, 2 * epaisseurDetection);
  DirectionalScan dirScan3 = DirectionalScan (pointsAjusted3[0],
                                              pointsAjusted3[1]);
  dirScan3.computeAllScans (0, 0, largeur, hauteur);
  sectionImg = DirectionalUtil::getImageSection (dirScan3,
                                   0, RIGHT, tabImage, largeur, hauteur);
  centerGradient = LocalVariations::getVectIndicesMaxLocalGradSect (sectionImg);
  sizeSolutionGradientSection0 = centerGradient.size ();
  solutionSelected = numberSolution % sizeSolutionGradientSection0;

  LineDetection lined3 (dirScan3, tabImage, largeur, hauteur,
	                4, 2, centerGradient.at (solutionSelected),
                        epaisseurDetection, nbPixelInteruption);
  
  if (! lined3.isDetected)
  {
    cerr << "return vect VIDE" << endl;
    return vectResu;
  }

  vector<QPoint> pSeg = lined3.segmentFlou->getPrintedPoints ();
  // vector<QPoint> pGradMaxLoc = getPointsFromIndices(dirScan,
  //                                           0, RIGHT, centerGradient);
  // vector<QPoint>::iterator it = pGradMaxLoc.begin ();
  // while (it != pGradMaxLoc.end ())
  // {
  //   pSeg.push_back (*it);
  //   ++it;
  // }
  return pSeg;
  // return vectResu;
}


/**
  * Line detection: 
  * -detect 1: Detection en fonction simplement de la morme du gradient non directionel
  *            + TROIS VECTEURS DE POINTS SOLUTIONS
  * -detect 2: Prem Max local directionnel qui fait augmenter la longueur du seg max.
  * -detect 3: Prem Max local directionnel qui fait augmenter la longueur du seg max.
  */
vector<QPoint> LineDetection::detectLine3 (QPoint p1, QPoint p2,
                     int largeur, int hauteur,
                     QPoint **tabGradient, bool **tabGradientUsed,
                     double epaisseurDetection, int nbPixelInteruption,
                     int numSolution, int numStep)
{
  vector<QPoint> vectResu;

  // Premiere detection max des gradients sans direction
  //----------------------------------------------------
  DirectionalScan dirScan = DirectionalScan (p1, p2);
  dirScan.computeAllScans (0, 0, largeur, hauteur);

  vector < vector<QPoint> > pointRightCandidatesV
              = DirectionalUtil::getPointsGradMaxLocalFromScans (dirScan,
                        tabGradient, largeur, hauteur,
                        tabGradientUsed, RIGHT,
                        QPoint (p1.x () - p2.x (), p1.y () - p2.y ()), 0.0);

  vector < vector<QPoint> > pointLeftCandidatesV
              = DirectionalUtil::getPointsGradMaxLocalFromScans (dirScan,
                        tabGradient, largeur, hauteur,
                        tabGradientUsed, LEFT,
                        QPoint(p1.x()-p2.x(), p1.y () - p2.y ()), 0.0);

  vector<QPoint> vectCandSini = pointLeftCandidatesV.at (0);

  if (pointLeftCandidatesV.size () == 0 || pointRightCandidatesV.size () == 0)
  {
    cerr << "aucune droite detectable (pas de candidats a gauche ou a droite)"
         << endl;
    return vectResu;
  }
  LineDetection *lined = new LineDetection (
                    pointLeftCandidatesV, pointRightCandidatesV,
                    epaisseurDetection, nbPixelInteruption, numSolution);
  if (! lined->isDetected)
  {
    cerr << "aucune droite detectable (pas assez de candidats initiaux)"
         << endl;
    return vectResu;
  }

  QPoint p0 = lined->pCentral0;
  QPoint pR = lined->pRight;
  QPoint pL = lined->pLeft;

  if (numStep == 1)
  {
    vector<QPoint> vectToAff = lined->segmentFlou->getPrintedPoints ();
    vector<QPoint>::iterator it = vectCandSini.begin ();
    int nb=0;
    while (it != vectCandSini.end ())
    {
      vectToAff.push_back (*it);
      ++it;
      nb++;
      if(nb == 5) break;
    }
    return vectToAff;
  }

  // Deuxieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction
  //---------------------------------------------------------------
  QPoint pCenter = lined->segmentFlou->getCenter ();
  double distanceP1P2 = sqrt ((p1.x () - p2.x ()) * (p1.x () - p2.x ())
                              + (p1.y () - p2.y ()) * (p1.y () - p2.y ()));
  double direction = lined->segmentFlou->getDirection ();
  QPoint *pointsAjusted2 = DirectionalUtil::getAjustedPoints (pCenter,
                             direction + M_PI / 2.0, 2 * epaisseurDetection);
  DirectionalScan dirScan2 = DirectionalScan (pointsAjusted2[0],
                                              pointsAjusted2[1], direction);
  dirScan2.computeAllScans (0, 0, largeur, hauteur);
  
  QPoint p11 = pointsAjusted2[0];
  QPoint p12 = pointsAjusted2[1];

  pointRightCandidatesV = DirectionalUtil::getPointsGradMaxLocalFromScans (
               dirScan2, tabGradient, largeur, hauteur, tabGradientUsed,
               RIGHT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);

  pointLeftCandidatesV = DirectionalUtil::getPointsGradMaxLocalFromScans (
               dirScan2, tabGradient, largeur, hauteur, tabGradientUsed,
               LEFT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);

  if (pointLeftCandidatesV.size () == 0 || pointRightCandidatesV.size () == 0)
  {
    cerr << "aucune droite detectable." << endl;
    return vectResu;
  }

  lined = new LineDetection (pointLeftCandidatesV, pointRightCandidatesV,
                             epaisseurDetection, nbPixelInteruption);
  if (! lined->isDetected)
  {
    return vectResu;
  }

  if (numStep == 2)
  {
    vector<QPoint> vectToAff = lined->segmentFlou->getPrintedPoints ();
    vector<QPoint>::iterator it = vectCandSini.begin ();
    int nb = 0;
    while (it != vectCandSini.end ())
    {
      vectToAff.push_back (*it);
      ++it;
      nb++;
      if (nb == 5) break;
    }
    return vectToAff;
  }


  // Troisieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction 
  //---------------------------------------------------------------
  distanceP1P2 = sqrt ((p11.x () - p12.x ()) * (p11.x () - p12.x ())
                       + (p11.y () - p12.y ()) * (p11.y () - p12.y ()));
  direction = lined->segmentFlou->getDirection ();
 
  pointsAjusted2 = DirectionalUtil::getAjustedPoints (pCenter,
                      direction + M_PI / 2.0, 2 * epaisseurDetection);
  dirScan2 = DirectionalScan (pointsAjusted2[0], pointsAjusted2[1], direction);
  p11 = pointsAjusted2[0];
  p12 = pointsAjusted2[1];
  dirScan2.computeAllScans (0, 0, largeur, hauteur);

  pointRightCandidatesV = DirectionalUtil::getPointsGradMaxLocalFromScans (
               dirScan2, tabGradient, largeur, hauteur, tabGradientUsed,
               RIGHT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.98);

  pointLeftCandidatesV = DirectionalUtil::getPointsGradMaxLocalFromScans (
               dirScan2, tabGradient, largeur, hauteur, tabGradientUsed,
               LEFT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.98);

  if (pointLeftCandidatesV.size () == 0 || pointRightCandidatesV.size () == 0)
  {
    cerr << "aucune droite detectable." << endl;
    return vectResu;
  }

  lined = new LineDetection (pointLeftCandidatesV, pointRightCandidatesV,
                             epaisseurDetection, nbPixelInteruption);

  if (! lined->isDetected || lined->nbPointAdded <= 4)
    return vectResu;

  vector<QPoint> vectToAff = lined->segmentFlou->getPrintedPoints ();
  vector<QPoint>::iterator it = vectCandSini.begin ();
  int nb = 0;
  while (it != vectCandSini.end ())
  {
    vectToAff.push_back (*it);
    ++it;
    nb++;
    if (nb == 5) break;
  }

  if (numSolution == 1) cerr << pointLeftCandidatesV.at(0).size ()
                             << " solutions" << endl;
  return vectToAff;
}


/**
  * Line detection: 
  * -detect 1: Detection en fonction simplement de la morme du gradient non directionel
  * -detect 2: Prem Max local directionnel qui fait augmenter la longueur du seg max.
  * -detect 3: Prem Max local directionnel qui fait augmenter la longueur du seg max.
  */
vector<QPoint> LineDetection::detectLine2 (QPoint p1, QPoint p2,
                int **tabImage, int largeur, int hauteur,
                QPoint **tabGradient, bool **tabGradientUsed,
                double epaisseurDetection, int nbPixelInteruption, int numStep)
{
  vector<QPoint> vectResu;

  // Premiere detection max des gradients sans direction
  //----------------------------------------------------
  DirectionalScan dirScan = DirectionalScan (p1, p2);
  dirScan.computeAllScans (0, 0, largeur, hauteur);
  vector<QPoint> pointLeftCandidates
            = DirectionalUtil::getGradientMaxFromScans (dirScan,
                       tabImage, largeur, hauteur,
                       tabGradient, tabGradientUsed, LEFT, seuil);
  vector<QPoint> pointRightCandidates
            = DirectionalUtil::getGradientMaxFromScans (dirScan,
                       tabImage, largeur, hauteur,
                       tabGradient, tabGradientUsed, RIGHT, seuil);
  if (pointLeftCandidates.size () == 0 || pointRightCandidates.size () == 0)
  {
    cerr << "aucune droite detectable (pas de candidats a gauche ou a droite)"
         << endl;
    return vectResu;
  }
  LineDetection *lined = new LineDetection (
                               pointLeftCandidates, pointRightCandidates,
                               epaisseurDetection, nbPixelInteruption);
  if (! lined->isDetected)
  {
    cerr << "aucune droite detectable (pas assez de candidats initiaux)"
         << endl;
    return vectResu;
  }

  if (numStep == 1)
    return lined->segmentFlou->getPrintedPoints ();

  // Deuxieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction
  //---------------------------------------------------------------
  QPoint pCenter = lined->segmentFlou->getCenter ();
  double distanceP1P2 = sqrt ((p1.x () - p2.x ()) * (p1.x () - p2.x ())
                              + (p1.y () - p2.y ()) * (p1.y () - p2.y ()));
  double direction = lined->segmentFlou->getDirection ();
  QPoint *pointsAjusted2 = DirectionalUtil::getAjustedPoints (pCenter,
                              direction + M_PI / 2.0, 2 * epaisseurDetection);
  DirectionalScan dirScan2 = DirectionalScan (pointsAjusted2[0],
                                              pointsAjusted2[1], direction);
  dirScan2.computeAllScans(0, 0, largeur, hauteur);

  QPoint p11 = pointsAjusted2[0];
  QPoint p12 = pointsAjusted2[1];

  // vector < vector<QPoint> > pointRightCandidatesV
  //    = DirectionalUtil::getPointsCorrelMaxLocalFromScans (dirScan2,
  //                          tabImage, largeur, hauteur, RIGHT, 2, 2,
  //                          dirScan2.getScan (0, RIGHT).size () / 2, 0);
  // vector < vector<QPoint> > pointLeftCandidatesV
  //    =  DirectionalUtil::getPointsCorrelMaxLocalFromScans (dirScan2,
  //                          tabImage, largeur, hauteur, LEFT, 2, 2,
  //                          dirScan2.getScan (0, RIGHT).size () / 2, 0);

  vector < vector<QPoint> > pointRightCandidatesV
              = DirectionalUtil::getPointsGradMaxLocalFromScans (dirScan2,
                      tabGradient, largeur, hauteur, tabGradientUsed, RIGHT,
                      QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);
  vector < vector<QPoint> > pointLeftCandidatesV
              = DirectionalUtil::getPointsGradMaxLocalFromScans (dirScan2,
                      tabGradient, largeur, hauteur, tabGradientUsed, LEFT,
                      QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);

  if (pointLeftCandidates.size () == 0 || pointRightCandidates.size () == 0)
  {
    cerr << "aucune droite detectable." << endl;
    return vectResu;
  }

  lined = new LineDetection (pointLeftCandidatesV, pointRightCandidatesV,
                             epaisseurDetection, nbPixelInteruption);
  if (! lined->isDetected)
    return vectResu;

  if (numStep == 2)
    return lined->segmentFlou->getPrintedPoints ();

  // Troisieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction 
  //---------------------------------------------------------------
  distanceP1P2 = sqrt ((p11.x () - p12.x ()) * (p11.x () - p12.x ())
                       + (p11.y () - p12.y ()) * (p11.y () - p12.y ()));
  direction = lined->segmentFlou->getDirection ();
 
  pointsAjusted2 = DirectionalUtil::getAjustedPoints (pCenter,
                             direction + M_PI / 2.0, 2 * epaisseurDetection);
  dirScan2 = DirectionalScan (pointsAjusted2[0], pointsAjusted2[1], direction);
  p11 = pointsAjusted2[0];
  p12 = pointsAjusted2[1];
  dirScan2.computeAllScans (0, 0, largeur, hauteur);

  pointRightCandidatesV = DirectionalUtil::getPointsGradMaxLocalFromScans (
               dirScan2, tabGradient, largeur, hauteur, tabGradientUsed,
               RIGHT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);
  pointLeftCandidatesV = DirectionalUtil::getPointsGradMaxLocalFromScans (
               dirScan2, tabGradient, largeur, hauteur, tabGradientUsed,
               LEFT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);

  // pointRightCandidatesV = DirectionalUtil::getPointsCorrelMaxLocalFromScans (
  //           dirScan2, tabImage, largeur, hauteur,
  //           RIGHT, 2, 2, dirScan2.getScan (0, RIGHT).size () /2, 0);
  // pointLeftCandidatesV =  DirectionalUtil::getPointsCorrelMaxLocalFromScans (
  //           dirScan2, tabImage, largeur, hauteur,
  //           LEFT, 2, 2, dirScan2.getScan (0, RIGHT).size () / 2, 0);

  if (pointLeftCandidates.size () == 0 || pointRightCandidates.size () == 0)
  {
    cerr << "aucune droite detectable." << endl;
    return vectResu;
  }

  lined = new LineDetection (pointLeftCandidatesV, pointRightCandidatesV,
                             epaisseurDetection, nbPixelInteruption);
  if (! lined->isDetected)
    return vectResu;
  
  // vector<QPoint>::iterator it = (lined->pointAdded).begin ();
  // while (it != (lined->pointAdded).end ())
  // {
  //   QPoint p = *it;
  //   tabGradientUsed[p.y()][p.x()] = true;
  //   it ++;
  // }

  return lined->segmentFlou->getPrintedPoints ();
}


/**
  * Line detection:
  * -detect 1: Detection en fonction simplement de la morme du gradient non directionel
  * -detect 2: Detection a partir du Gradient Max en fonction de la direction.
  * -detect 3: Prem Max local directionnel qui fait augmenter la longueur du seg max.
  */
vector<QPoint> LineDetection::detectLine (QPoint p1, QPoint p2,
                     int **tabImage, int largeur, int hauteur,
                     QPoint **tabGradient, bool **tabGradientUsed,
                     double epaisseurDetection, int nbPixelInteruption,
                     int numStep)
{
  vector<QPoint> vectResu;

  // Premiere detection max des gradients sans direction
  //----------------------------------------------------
  DirectionalScan dirScan = DirectionalScan (p1, p2);
  dirScan.computeAllScans (0, 0, largeur, hauteur);
  vector<QPoint> pointLeftCandidates
                      = DirectionalUtil::getGradientMaxFromScans (dirScan,
                            tabImage, largeur, hauteur,
                            tabGradient, tabGradientUsed, LEFT, seuil);
  vector<QPoint> pointRightCandidates
                      = DirectionalUtil::getGradientMaxFromScans (dirScan,
                            tabImage, largeur, hauteur,
                            tabGradient, tabGradientUsed, RIGHT, seuil);
  if (pointLeftCandidates.size () == 0 || pointRightCandidates.size () == 0)
  {
    cerr << "aucune droite detectable (pas de candidats a gauche ou a droite)"
         << endl;
    return vectResu;
  }
  LineDetection *lined = new LineDetection (
                               pointLeftCandidates, pointRightCandidates,
                               epaisseurDetection, nbPixelInteruption);
  if (! lined->isDetected)
  {
    cerr << "aucune droite detectable (pas assez de candidats initiaux)"
         << endl;
    return vectResu;
  }

  if (numStep == 1)
    return lined->segmentFlou->getPrintedPoints ();

  // Deuxieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction
  //---------------------------------------------------------------
  QPoint pCenter = lined->segmentFlou->getCenter ();
  double distanceP1P2 = sqrt ((p1.x () - p2.x ()) * (p1.x () - p2.x ())
                              + (p1.y () - p2.y ()) * (p1.y () - p2.y ()));
  double direction = lined->segmentFlou->getDirection ();
  QPoint *pointsAjusted = DirectionalUtil::getAjustedPoints (pCenter,
                             direction + M_PI / 2.0, distanceP1P2 / 2.0);
  QPoint p11 = pointsAjusted[0];
  QPoint p12 = pointsAjusted[1];
     
  dirScan = DirectionalScan (pointsAjusted[0], pointsAjusted[1], direction);
  dirScan.computeAllScans (0, 0, largeur, hauteur);
  pointLeftCandidates = DirectionalUtil::getGradientMaxFromScans (dirScan,
                   tabImage, largeur, hauteur,
                   tabGradient, tabGradientUsed, LEFT,
                   QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), seuil);
  pointRightCandidates = DirectionalUtil::getGradientMaxFromScans (dirScan,
                   tabImage, largeur, hauteur,
                   tabGradient, tabGradientUsed, RIGHT,
                   QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), seuil);
  if (pointLeftCandidates.size () == 0 || pointRightCandidates.size () == 0)
  {
    cerr << "aucune droite detectable." << endl;
    return vectResu;
  }

  lined = new LineDetection (pointLeftCandidates, pointRightCandidates,
                             epaisseurDetection, nbPixelInteruption);
  if (! lined->isDetected)
    return vectResu;

  if (numStep == 2)
    return lined->segmentFlou->getPrintedPoints ();

  // Troisieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction 
  //---------------------------------------------------------------
  distanceP1P2 = sqrt ((p11.x () - p12.x ()) * (p11.x () - p12.x ())
                       + (p11.y () - p12.y ()) * (p11.y () - p12.y ()));
  direction = lined->segmentFlou->getDirection ();

  QPoint *pointsAjusted2 = DirectionalUtil::getAjustedPoints (pCenter,
                             direction + M_PI / 2.0, 2 * epaisseurDetection);
  DirectionalScan dirScan2 = DirectionalScan (pointsAjusted2[0],
                                              pointsAjusted2[1], direction);
  dirScan2.computeAllScans (0, 0, largeur, hauteur);
  p11 = pointsAjusted2[0];
  p12 = pointsAjusted2[1];
  dirScan2.computeAllScans (0, 0, largeur, hauteur);

  // vector < vector<QPoint> > pointRightCandidatesV
  //             = DirectionalUtil::getGradientVectorsFromScans (dirScan2,
  //      tabImage, largeur, hauteur, tabGradient, tabGradientUsed,
  //      RIGHT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), seuil);
  // vector < vector<QPoint> > pointLeftCandidatesV
  //             = DirectionalUtil::getGradientVectorsFromScans (dirScan2,
  //      tabImage, largeur, hauteur, tabGradient, tabGradientUsed,
  //      LEFT, QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), seuil);

  vector < vector<QPoint> > pointRightCandidatesV
              = DirectionalUtil::getPointsGradMaxLocalFromScans (dirScan2,
                    tabGradient, largeur, hauteur, tabGradientUsed, RIGHT,
                    QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);
  vector < vector<QPoint> > pointLeftCandidatesV
              = DirectionalUtil::getPointsGradMaxLocalFromScans (dirScan2,
                    tabGradient, largeur, hauteur, tabGradientUsed, LEFT,
                    QPoint (p11.x () - p12.x (), p11.y () - p12.y ()), 0.9);

  if (pointLeftCandidates.size () == 0 || pointRightCandidates.size () == 0)
  {
    cerr << "aucune droite detectable." << endl;
    return vectResu;
  }

  lined = new LineDetection (pointLeftCandidatesV, pointRightCandidatesV,
                             epaisseurDetection, nbPixelInteruption);
  if (! lined->isDetected)
    return vectResu;

  // vector<QPoint>::iterator it = (lined->pointAdded).begin ();
  // while (it != (lined->pointAdded).end ())
  // {
  //   QPoint p = *it;
  //   tabGradientUsed[p.y()][p.x()] = true;
  //   it ++;
  // }

  return lined->segmentFlou->getPrintedPoints ();
}


/**
  * Detection en fonction de l'ensemble des candidats deja selectionnes
  */
LineDetection::LineDetection (vector<QPoint> vLeft, vector<QPoint> vRight,
                              double epaisseur, int interuptSizeMax)
{
  isDetected = false;
  if (vLeft.size () < 5 || vRight.size () < 5)
  {
    // cerr << "detection de segments flous pas possible (simple)" << endl;
    cerr << "Bande de recherche trop courte" << endl;
      return;
  }

  Point2D pCentral (vLeft.at(0).x (), vLeft.at(0).y ());
  Point2D pLeft (vLeft.at(1).x (), vLeft.at(1).y ());
  Point2D pRight (vRight.at(0).x (), vRight.at(0).y ());
  int pos = 2;

  while (isLeft (pCentral, pLeft, pRight) == 0
         && pos < (int) vLeft.size () && pos < (int) vRight.size ())
  {
    pLeft = Point2D (vLeft.at(pos).x (), vLeft.at(pos).y ());
    pRight = Point2D (vRight.at(pos).x (), vRight.at(pos).y ());
    pos ++;
  }
    
  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    // cerr << "detection de segments flous pas possible: trop peu de points (align)" << endl;
    cerr <<  "Trop peu de points (align)" << endl;
    return;
  }

  segmentFlou = new BlurredSegment (pCentral, pLeft, pRight, epaisseur);
  isDetected = true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  vector<QPoint>::iterator iterRight = vRight.begin ();
  vector<QPoint>::iterator iterLeft = vLeft.begin ();

  bool scanRightStopped = false;
  bool scanLeftStopped = false;

  while(! scanRightStopped || ! scanLeftStopped)
  {
    if (! scanRightStopped)
    {
      QPoint pr = *iterRight;
      bool added = false;
      if (! (pr.x () == -1 && pr.y () == -1))
        added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), RIGHT);
      if (added) nbVideRight = 0;
      else nbVideRight ++;
    }
    if(! scanLeftStopped)
    {
      QPoint pr = *iterLeft;
      bool added = false;
      if (! (pr.x () == -1 && pr.y () == -1))
        added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), LEFT);
      if (added) nbVideLeft = 0;
      else nbVideLeft ++;
    }

    iterRight ++;
    iterLeft ++;
    if (iterRight == vRight.end () || nbVideRight > interuptSizeMax)
      scanRightStopped = true;
    if (iterLeft == vLeft.end () || nbVideLeft > interuptSizeMax)
      scanLeftStopped = true;
  }
  isDetected = true;
}
      

/** DETECTION GRADIENT MAX MULTIPLES CANDIDATS
  * ET CLASSEMENT PAR RAPPORT A LA DISTANCE AU CENTRE
  * Detection en fonction d'un ensemble des candidats potentiels 
  */
LineDetection::LineDetection (vector< vector<QPoint> > vLeft,
                              vector< vector<QPoint> > vRight,
                              double epaisseur, int interuptSizeMax,
                              bool centered)
{
  // centered inutilise, mais necessaire sinon redefinition...

  isDetected = false;
  int nbSolutionsCentral = vLeft.at(0).size ();
  int numSolution = 0;
  Point2D pCentral (vLeft.at(0).at (numSolution % nbSolutionsCentral).x (),
                    vLeft.at(0).at (numSolution % nbSolutionsCentral).y());

  // Calcul du voisin le plus proche dans la liste des voisins Gauche
  int decalL = 1;
  while (vLeft.at(decalL).size () == 0 && decalL < vLeft.size () -1)
    decalL ++;
  int decalR = 1;
  while (vRight.at(decalR).size () == 0 && decalR < vRight.size () -1)
    decalR ++;
  
  if (decalR >= vRight.size () || decalL >= vLeft.size ()
      || vRight.at (decalR).size () == 0 || vLeft.at (decalL).size () == 0)
  {
    //cerr << "detection de segments flous pas possible: pas assez de points pour l'initialisation" << endl;
    cerr << "Pas assez de points pour l'initialisation" << endl;
    return;
  }

  vector<QPoint> vectCandLeft = vLeft.at (decalL);
  vector<QPoint> vectCandRight = vRight.at (decalR);
  pRefComp = QPoint (pCentral.getX (), pCentral.getY ());
  sort (vectCandLeft.begin (), vectCandLeft.end (), compNearest);
  sort (vectCandRight.begin (), vectCandRight.end (), compNearest);

  Point2D pLeft (vectCandLeft.at(0).x (), vectCandLeft.at(0).y ());
  Point2D pRight (vectCandRight.at(0).x (), vectCandRight.at(0).y ());

  int pos = 0;

  while (isLeft (pCentral, pLeft, pRight) == 0
         && pos + decalL < (int) vLeft.size ()
         && pos + decalR < (int) vRight.size ())
  {
    if (vLeft.at (pos + decalL).size () != 0
        && vRight.at (pos + decalR).size () != 0)
    {
      vectCandLeft = vLeft.at (decalL + pos);
      vectCandRight = vRight.at (decalR + pos);
      // pRefComp = QPoint (pCentral.x (), pCentral.x ());
      sort (vectCandLeft.begin (), vectCandLeft.end (), compNearest);
      sort (vectCandRight.begin (), vectCandRight.end (), compNearest);
      pLeft = Point2D(vectCandLeft.at(0).x (), vectCandLeft.at(0).y ());
      pRight = Point2D(vectCandRight.at(0).x (), vectCandRight.at(0).y ());
    }
    pos ++;
  }
  
  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    //cerr << "detection de segments flous pas possible: trop peu de points" << endl;
    cerr << "Pas assez de points" << endl;
    return;
  }

  segmentFlou = new BlurredSegment (pCentral, pLeft, pRight, epaisseur);
  isDetected= true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  vector< vector<QPoint> >::iterator iterRight = vRight.begin () + decalR + pos;
  vector< vector<QPoint> >::iterator iterLeft = vLeft.begin () +decalL + pos;
  
  bool scanRightStopped = false;
  bool scanLeftStopped = false;

  while (! scanRightStopped || ! scanLeftStopped)
  {
    if (! scanRightStopped)
    {
      vector<QPoint> vectCandidats = *iterRight;
      sort (vectCandidats.begin (), vectCandidats.end (), compNearest);
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
	while (! added && iterCandidats != vectCandidats.end ())
        {
	  QPoint pr = *iterCandidats;
	  added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), RIGHT);
	  if (added)
          {
            nbVideRight = 0;
            pointAdded.push_back (pr);
	  }
          else nbVideRight ++;
	  iterCandidats ++;
	}
      }
      else nbVideRight ++;
    }

    if (! scanLeftStopped)
    {
      vector<QPoint> vectCandidats = *iterLeft;
      sort (vectCandidats.begin (), vectCandidats.end (), compNearest);
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
	while (! added && iterCandidats != vectCandidats.end ())
        {
	  QPoint pr = *iterCandidats;
	  added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), LEFT);
	  if (added)
          {
            nbVideLeft = 0;
            pointAdded.push_back (pr);
	  }
          else nbVideLeft ++;
	  iterCandidats ++;
	}
      }
      else nbVideLeft ++;
    }
    
    iterRight ++;
    iterLeft ++;
    if (iterRight == vRight.end () || nbVideRight > interuptSizeMax)
      scanRightStopped = true;
    if (iterLeft == vLeft.end () || nbVideLeft > interuptSizeMax)
      scanLeftStopped = true;
  }
  isDetected = true;
}
   

/** DETECTION GRADIENT MAX MULTIPLES CANDIDATS
  * Detection en fonction d'un ensemble des candidats potentiels
  * // Reconnaissance Phase 2 et 3
  */
LineDetection::LineDetection (
            vector< vector<QPoint> > vLeft, vector< vector<QPoint> > vRight,
            double epaisseur, int interuptSizeMax)
{
  nbPointAdded = 0;
  isDetected = false;
  if (vLeft.size () < 2 || vRight.size () < 2 || vLeft.at(0).size () == 0)
  {
    // cerr << "detection de segments flous pas possible (second) vleft size = "
    //      << vLeft.at(0).size () << endl;
    cerr << "Second vleft size = " << vLeft.at(0).size () << endl;
    return;
  }

  int decalC = 0;
  Point2D pCentral (vLeft.at(decalC).at(0).x (), vLeft.at(decalC).at(0).y ());
  int decalL = 1;
  while (vLeft.at(decalL).size () == 0 && decalL < vLeft.size () - 1)
    decalL ++;

  int decalR = 1;
  while (vRight.at(decalR).size () == 0 && decalR < vRight.size () - 1)
    decalR ++;    

  if (decalR >= vRight.size () || decalL >= vLeft.size ()
      || vRight.at(decalR).size () == 0 || vLeft.at(decalL).size () == 0)
  {
    // cerr << "detection de segments flous pas possible: pas assez de points pour l'initialisation"  << endl;
    cerr << "Pas assez de points pour l'initialisation" << endl;
    return;
  }

  Point2D pLeft (vLeft.at(decalL).at(0).x (), vLeft.at(decalL).at(0).y ());
  Point2D pRight (vRight.at(decalR).at(0).x (), vRight.at(decalR).at(0).y ());

  int pos = 0;
  while (isLeft (pCentral, pLeft, pRight) == 0
         && pos + decalL < (int) vLeft.size ()
         && pos + decalR < (int) vRight.size ())
  {
    if (vLeft.at (pos + decalL).size () != 0 &&
        vRight.at (pos + decalR).size () != 0)
    {
      pLeft = Point2D (vLeft.at (decalL + pos).at(0).x (),
                       vLeft.at (decalL + pos).at(0).y ());
      pRight = Point2D (vRight.at (decalR + pos).at(0).x (),
                        vRight.at (decalR + pos).at(0).y ());
    }
    pos ++;
  }

  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    // cerr << "detection de segments flous pas possible: trop peu de points"
    //      << endl;
    cerr << "Trop peu de points" << endl;
    return;
  }

  segmentFlou = new BlurredSegment (pCentral, pLeft, pRight, epaisseur);
  isDetected = true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  vector< vector<QPoint> >::iterator iterRight = vRight.begin () + decalR + pos;
  vector< vector<QPoint> >::iterator iterLeft = vLeft.begin () + decalL + pos;
    
  bool scanRightStopped = false;
  bool scanLeftStopped = false;

  while (! scanRightStopped || ! scanLeftStopped)
  {
    if (! scanRightStopped)
    {
      vector<QPoint> vectCandidats = *iterRight;
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;      
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), RIGHT);
          if (added)
          {
            nbVideRight = 0;
            pointAdded.push_back (pr);
            nbPointAdded ++;
          }
          else nbVideRight ++;
          iterCandidats ++;	  
        }
      }
      else nbVideRight ++;
    }
    if (! scanLeftStopped)
    {
      vector<QPoint> vectCandidats = *iterLeft;
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), LEFT);
          if (added)
          {
            nbVideLeft = 0;
            nbPointAdded ++;
            pointAdded.push_back (pr);
          }
          else nbVideLeft ++;
          iterCandidats ++;
        }      
      }
      else nbVideLeft ++;
    }
    iterRight ++;
    iterLeft ++;
    if (iterRight == vRight.end () || nbVideRight > interuptSizeMax)
      scanRightStopped = true;
    if (iterLeft == vLeft.end () || nbVideLeft > interuptSizeMax)
      scanLeftStopped = true;
  }
  isDetected = true;
}


/** DETECTION GRADIENT MAX MULTIPLES CANDIDATS****
 * Detection en fonction d'un ensemble des candidats potentiels
 * //### Reconnaissance Phase 2 et 3  AVEC INITIALISATION DE PHASE 1
 */
LineDetection::LineDetection (
        vector< vector<QPoint> > vLeft, vector< vector<QPoint> > vRight,
        double epaisseur, int interuptSizeMax, QPoint p0, QPoint pL, QPoint pR)
{
  nbPointAdded = 0;
  isDetected = false;
  if (vLeft.size () < 2 || vRight.size () < 2 || vLeft.at(0).size () == 0)
  {
    // cerr << "detection de segments flous pas possible (second) vleft size = "
    //      << vLeft.at(0).size () << endl;
    cerr << "Second vleft size = " << vLeft.at(0).size () << endl;
    return;
  }

  int decalC =0;
  vector<QPoint> vCentral = vLeft.at (0);
  pRefComp = p0;
  sort (vCentral.begin (), vCentral.end (), compNearest);
  Point2D pCentral (p0.x (), p0.y ());

  int decalL = 1;
  // while (vLeft.at (decalL).size () == 0 && decalL < vLeft.size () - 1)
  //   decalL ++;
  int decalR = 1;
  // while (vRight.at (decalR).size () == 0 && decalR < vRight.size () - 1)
  //   decalR ++;
  
  // if (decalR >= vRight.size () || decalL >= vLeft.size ()
  //     || vRight.at (decalR).size () == 0 || vLeft.at (decalL).size () == 0)
  // {
  //   cerr << "detection de segments flous pas possible: pas assez de points pour l'initialisation"  << endl;
  //   return;
  // }
  
  // vector<QPoint> vl = vLeft.at (decalL);
  // vector<QPoint> vr = vRight.at (decalR);
  // pRefComp = QPoint (pCentral.getX (), pCentral.getY ());
  // sort (vr.begin (), vr.end (), compNearest);
  // Point2D pRight (vr.at (0).x (), vr.at (0).y ());
  Point2D pRight (pR.x (), pR.y ());
  // pRefComp = pL;
  // sort (vl.begin (), vl.end (), compNearest);
  Point2D pLeft (pL.x (), pL.y ());

  int pos = 0;
  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    // cerr << "detection de segments flous pas possible: trop peu de points"
    //      << endl;
    cerr << "Trop peu de points" << endl;
    return;
  }

  segmentFlou = new BlurredSegment (pCentral, pLeft, pRight, epaisseur);
  isDetected = true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  vector< vector<QPoint> >::iterator iterRight = vRight.begin () + decalR + pos;
  vector< vector<QPoint> >::iterator iterLeft = vLeft.begin () + decalL + pos;
 
  bool scanRightStopped = false;
  bool scanLeftStopped = false;

  while (! scanRightStopped || ! scanLeftStopped)
  {
    if (! scanRightStopped)
    {
      vector<QPoint> vectCandidats = *iterRight;
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), RIGHT);
          if (added)
          {
            nbVideRight = 0;
            pointAdded.push_back (pr);
            nbPointAdded ++;
          }
          else nbVideRight ++;
          iterCandidats ++;
        }
      }
      else nbVideRight ++;
    }
    if (! scanLeftStopped)
    {
      vector<QPoint> vectCandidats = *iterLeft;
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), LEFT);
          if (added)
          {
            nbVideLeft = 0;
            nbPointAdded ++;
            pointAdded.push_back (pr);
          }
          else nbVideLeft ++;
          iterCandidats ++;
        }
      }
      else nbVideLeft ++;
    }
 
    iterRight ++;
    iterLeft ++;
    if (iterRight == vRight.end () || nbVideRight > interuptSizeMax)
      scanRightStopped = true;
    if (iterLeft == vLeft.end () || nbVideLeft > interuptSizeMax)
      scanLeftStopped = true;
  }
  isDetected = true;
}


/** DETECTION GRADIENT MAX MULTIPLES CANDIDATS ET SELECTION NB SOLUTION*******
 * Detection en fonction d'un ensemble des candidats potentiels
 * //### Reconnaissance Phase 1
 */
LineDetection::LineDetection (
         vector< vector<QPoint> > vLeft, vector< vector<QPoint> > vRight,
         double epaisseur, int interuptSizeMax, int numSolution)
{
  nbPointAdded = 0;
  isDetected = false;
  if (vLeft.size () < 2 || vRight.size () < 2  || vLeft.at(0).size () == 0)
  {
    // cerr << "detection de segments flous pas possible (second) vleft size = "
    //      << vLeft.at(0).size() << endl;
    cerr << "Second vleft size = " << vLeft.at(0).size () << endl;
    return;
  }

  int nbSolutionsCentral = vLeft.at(0).size ();
  Point2D pCentral (vLeft.at(0).at (numSolution % nbSolutionsCentral).x (),
                    vLeft.at(0).at (numSolution % nbSolutionsCentral).y ());

  // Calcul du voisin le plus proche dans la liste des voisins Gauche
  int decalL = 1;
  while (vLeft.at(decalL).size () == 0 && decalL <vLeft.size () - 1)
    decalL ++;
  int decalR = 1;
  while (vRight.at(decalR).size () == 0 && decalR <vRight.size () - 1)
    decalR ++;

  if (decalR >= vRight.size () || decalL >= vLeft.size ()
      || vRight.at(decalR).size () == 0 || vLeft.at(decalL).size () == 0)
  {
    // cerr << "detection de segments flous pas possible: pas assez de points pour l'initialisation"  << endl;
    cerr << "Pas assez de points pour l'initialisation" << endl;
    return;
  }
  
  vector<QPoint> vectCandLeft = vLeft.at (decalL);
  vector<QPoint> vectCandRight = vRight.at (decalR);
  pRefComp = QPoint (pCentral.getX (), pCentral.getY ());
  sort (vectCandLeft.begin (), vectCandLeft.end (), compNearest);
  sort (vectCandRight.begin (), vectCandRight.end (), compNearest);

  Point2D pLeft (vectCandLeft.at(0).x (), vectCandLeft.at(0).y ());
  Point2D pRight (vectCandRight.at(0).x (), vectCandRight.at(0).y ());
  
  int pos=0;
  while (isLeft (pCentral, pLeft, pRight) == 0
         && pos + decalL < (int) vLeft.size ()
         && pos + decalR < (int) vRight.size ())
  {
    if (vLeft.at (pos + decalL).size() != 0
        && vRight.at (pos + decalR).size () != 0)
    {
      vectCandLeft = vLeft.at (decalL + pos);
      vectCandRight = vRight.at (decalR + pos);
      // pRefComp = QPoint (pCentral.x (), pCentral.x ());
      sort (vectCandLeft.begin (), vectCandLeft.end (), compNearest);
      sort (vectCandRight.begin (), vectCandRight.end (), compNearest);      
      pLeft = Point2D (vectCandLeft.at(0).x (), vectCandLeft.at(0).y ());
      pRight = Point2D (vectCandRight.at(0).x (), vectCandRight.at(0).y ());
    }
    pos ++;
  }
  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    // cerr << "detection de segments flous pas possible: trop peu de points"
    //      << endl;
    cerr << "Trop peu de points" << endl;
    return;
  }

  this->pCentral0 = QPoint (pCentral.getX (), pCentral.getY ());
  this->pRight = QPoint (pRight.getX (),pRight.getY ());
  this->pLeft = QPoint (pLeft.getX (), pLeft.getY ());

  segmentFlou = new BlurredSegment (pCentral, pLeft, pRight, epaisseur);
  isDetected = true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  vector< vector<QPoint> >::iterator iterRight = vRight.begin () + decalR + pos;
  vector< vector<QPoint> >::iterator iterLeft = vLeft.begin () + decalL + pos;

  bool scanRightStopped = false;
  bool scanLeftStopped = false;

  while (! scanRightStopped || ! scanLeftStopped)
  {
    if (! scanRightStopped)
    {
      vector<QPoint> vectCandidats = *iterRight;
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), RIGHT);
          if (added)
          {
            nbVideRight = 0;
            nbPointAdded ++;
            pointAdded.push_back (pr);
          }
          else nbVideRight ++;
          iterCandidats ++;
        }
      }
      else nbVideRight ++;
    }
    if (! scanLeftStopped)
    {
      vector<QPoint> vectCandidats = *iterLeft;
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), LEFT);
          if (added)
          {
            nbVideLeft = 0;
            nbPointAdded ++;
            pointAdded.push_back (pr);
          }
          else nbVideLeft ++;
          iterCandidats ++;
        }
      }
      else nbVideLeft ++;
    }
    iterRight ++;
    iterLeft ++;
    if (iterRight == vRight.end () || nbVideRight > interuptSizeMax)
      scanRightStopped = true;
    if (iterLeft == vLeft.end () || nbVideLeft > interuptSizeMax)
      scanLeftStopped = true;
  }
  isDetected = true;
}


/** DETECTION CORRELATION **********************
  * Detection en fonction d'un ensemble des candidats potentiels
  * Constructeur pour la detection avec les profils
  */
LineDetection::LineDetection (DirectionalScan &dirScan,
          int **tabImage, int largeur, int hauteur,
          int correlWidth, int correlThick, int indiceCenterSection0,
          double epaisseur, int interuptSizeMax)
{
  cerr << " deb LineDEtection" << endl;
  DirectionalUtil dirUtile (tabImage, largeur, hauteur,
                            correlWidth, correlThick, indiceCenterSection0);
  isDetected = false;
  if (! dirUtile.isScanImgDefined (dirScan, 0, RIGHT))
  {
    // cerr << "detection de segments flous pas possible (second) vleft size = "
    //      <<  endl;
    cerr << "Second vleft size = " <<  endl;
    return;
  }

  int decalL = 1;
  int decalR = 1;
  if (indiceCenterSection0 + correlWidth >= dirScan.getScan (0, RIGHT).size ()
      || indiceCenterSection0 - correlWidth < 0)
  {
    cerr << "detection pas possible centre trop exentre " <<  endl;
    return;
  }

  QPoint p = dirScan.getScan (0, RIGHT).at (indiceCenterSection0);
  Point2D pCentral (p.x (), p.y ());
  Point2D pLeft;
  Point2D pRight;
  int pos = 1;
  bool foundPointNotAligned = false;
  
  while (! foundPointNotAligned
         && dirUtile.isScanImgDefined (dirScan, pos, LEFT)
         && dirUtile.isScanImgDefined (dirScan, pos, RIGHT))
  {
    if (dirUtile.getScanVectCorrel (dirScan, pos, LEFT).size () != 0
        && dirUtile.getScanVectCorrel (dirScan, pos, RIGHT).size () != 0)
    {
      pLeft = Point2D (
                dirUtile.getScanVectCorrel (dirScan, pos, LEFT).at(0).x (),
                dirUtile.getScanVectCorrel (dirScan, pos, LEFT).at(0).y ());
      pRight = Point2D (
                dirUtile.getScanVectCorrel (dirScan, pos, RIGHT).at(0).x (),
                dirUtile.getScanVectCorrel (dirScan, pos, RIGHT).at(0).y ());
      foundPointNotAligned = (! isLeft (pCentral, pLeft, pRight) == 0);
    }
    pos ++;
  }

  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    // cerr << "detection de segments flous pas possible: trop peu de points"
    //      << endl;
    cerr << "Trop peu de points" << endl;
    return;
  }

  segmentFlou = new BlurredSegment (pCentral, pLeft, pRight, epaisseur);
  isDetected = true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  int posR = decalR + pos;
  int posL = decalL + pos;
    
  bool scanRightStopped = false;
  bool scanLeftStopped = false;
  cerr << "after creation du seg flou" << endl;
  
  while (! scanRightStopped || ! scanLeftStopped)
  {
    if (! (dirScan.isScanDefined (posR,RIGHT)) || nbVideRight > interuptSizeMax)
      scanRightStopped = true;

    if (! (dirScan.isScanDefined (posL, LEFT)) || nbVideLeft > interuptSizeMax)
      scanLeftStopped = true;
    if(! scanRightStopped)
    {
      vector<QPoint> vectCandidats
              = dirUtile.getScanVectCorrel (dirScan, posR, RIGHT);
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), RIGHT);
          if (added)
          {
            nbVideRight = 0;
            pointAdded.push_back (pr);
          }
          else nbVideRight ++;
          iterCandidats ++;
        }
      }
      else nbVideRight ++;
    }
    if (! scanLeftStopped)
    {
      vector<QPoint> vectCandidats
              = dirUtile.getScanVectCorrel (dirScan, posL, LEFT);
      vector<QPoint>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          QPoint pr = *iterCandidats;
          added = segmentFlou->addPoint (Point2D (pr.x (), pr.y ()), LEFT);
          if (added)
          {
            nbVideLeft = 0;
            pointAdded.push_back (pr);
          }
            else nbVideLeft ++;
          iterCandidats ++;
        }
      }
      else nbVideLeft ++;
    }
    posL ++;
    posR ++;
    if (! (dirScan.isScanDefined (posR,RIGHT)) || nbVideRight > interuptSizeMax)
      scanRightStopped = true;
    if (! (dirScan.isScanDefined (posL, LEFT)) || nbVideLeft > interuptSizeMax)
      scanLeftStopped = true;
  }
  isDetected = true;
}


vector<QPoint> LineDetection::getPointsFromIndices (DirectionalScan &dirScan,
                        int numSection, int direction, vector<int> vIndice)
{
  vector<QPoint> vResult;
  vector<int>::iterator it = vIndice.begin ();
  vector<QPoint> vPoints = dirScan.getScan (numSection, direction);
  while (it != vIndice.end ())
  {
    int pos = *it;
    vResult.push_back (vPoints.at (pos));
    ++it;
  }
  return vResult;
}


LineDetection::~LineDetection ()
{
  delete segmentFlou;
}


bool LineDetection::compNearest (QPoint p1, QPoint p2)
{
  double d1 = (p1.x () - pRefComp.x ()) * (p1.x () - pRefComp.x ())
               + (p1.y () - pRefComp.y ()) * (p1.y () - pRefComp.y ());
  double d2 = (p2.x () - pRefComp.x ()) * (p2.x () - pRefComp.x ())
              + (p2.y () - pRefComp.y ()) * (p2.y () - pRefComp.y ());
  return d1 < d2;
}
