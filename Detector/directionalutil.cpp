#include <cstdlib>
#include <iostream>
#include <vector>
#include <QColor>
#include <limits>
#include <algorithm>
#include <math.h>
#include "directionalutil.h"
#include "localvariations.h"
#include "directionalscan.h"
#include "util.h"

using namespace std;


DirectionalUtil::DirectionalUtil (int **tabImage, int largeur, int hauteur,
                                  int correlWidth, int correlThick,
                                  int indicePcenterSection0)
{
  this->tabImage = tabImage;
  this->largeur = largeur;
  this->hauteur = hauteur;
  this->correlWidth = correlWidth;
  this->correlThick = correlThick;
  this->indicePcenterSection0 = indicePcenterSection0;
}


DirectionalUtil::~DirectionalUtil ()
{
}


vector<QPoint> DirectionalUtil::getScanVectCorrel (
                   DirectionalScan &dirScan, int pos, int direction)
{
  vector<QPoint> vResult = getPointsCorrelMaxLocalFromScans (
                              dirScan, tabImage, largeur, hauteur,
                              direction, correlWidth, correlThick,
                              indicePcenterSection0, 0, pos);
  return vResult;
}


bool DirectionalUtil::isScanImgDefined (
                   DirectionalScan &dirScan, int pos, int direction)
{
  bool isScanDefined = dirScan.isScanDefined (pos, direction);
  if (! isScanDefined) return false;
  QPoint p = dirScan.getScan (pos, direction).at (0);
  return ((p.x () < dirScan.xmax) && (p.x () >= dirScan.xmin)
          && (p.y () < dirScan.ymax) && (p.y () >= dirScan.ymin));
}


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
vector<DirectionalUtil::PointAndCorrel>
DirectionalUtil::getPointsCorrelFromScans (
                    vector<int> &correlRef, DirectionalScan &dirScan,
                    int numScan, int **tabImage, int largeur, int hauteur,
                    int direction, int correlWidth, int correlThick)
{
  vector<PointAndCorrel> vectResult;
  vector<QPoint> scanCenter = dirScan.getScan (numScan, direction);
  int longueurScanCenter = scanCenter.size ();
  
  // parcour de la section du scan en tenant compte de la largeur du scan.
  for (int i = correlWidth; i < longueurScanCenter - correlWidth; i++)
  {
    // calcul du vecteur moyen 
    if (dirScan.isScanDefined (numScan, direction)
        && dirScan.isInImageBounds (scanCenter.at (i)))
    {
      vector<int> vMoyen = getMeanValueCorrelScan (dirScan, tabImage,
                             largeur, hauteur, numScan, direction, i,
                             correlWidth, correlThick);

      // calcul de la correlation entre le vecteur moyen
      // et le vecteur de reference.
      int correlationVal = getCorrelationValue (correlRef, vMoyen);
      PointAndCorrel pc;
      pc.pt = dirScan.getScan (numScan, direction).at (i);
      pc.correl = correlationVal;
      vectResult.push_back (pc);
    }
  }
  return vectResult;
} 


vector<int> DirectionalUtil::getMeanValueCorrelScan (
                 DirectionalScan &dirScan, int **tabImage,
                 int largeur, int hauteur, int numScan, int direction,
                 int posDansScan, int correlWidth, int correlThick)
{
  // cerr << "getMeanVal num " << numScan << "pos dans scan " << posDansScan
  //      << " correlWidth " <<  correlWidth << " correlThick "
  //      << correlThick << endl;
  vector<int> vMoyen;
  for (int k = -correlWidth; k <= correlWidth; k++)
  {
    int valSomme = 0;
    int nbVal = 0;
    for (int l = -correlThick; l <= correlThick; l++)
    {
      if (dirScan.isScanDefined (numScan - l, direction)
          && posDansScan + k
             < dirScan.getScanOrientedIndex (numScan - l, direction).size ()
          && posDansScan + k >= 0)
      {
        QPoint p = dirScan.getScanOrientedIndex (numScan - l, direction).at (
                              posDansScan + k);
        if (p.x () < largeur && p.x () >= 0 && p.y () <hauteur && p.y () >= 0)
        {
          valSomme += tabImage[p.y()][p.x()];
          nbVal ++;
        }
      }
    }
    int valMoyenne = valSomme / nbVal;
    vMoyen.push_back (valMoyenne);
  }
  return vMoyen;
}


int DirectionalUtil::getCorrelationValue (vector<int> &vReference,
                                          vector<int> &v)
{
  if (vReference.size () != v.size ())
  {
    cerr << "Taille des vecteurs a comparer differentes: (v1, v2): ("
         << vReference.size() << "," << v.size() << ")"  << endl;
    exit (0);
  }
  vector<int>::iterator itR = vReference.begin ();
  vector<int>::iterator itV = v.begin ();
  int resu = 0;
  while (itR != vReference.end ())
  {
    int valR = *itR;
    int valV = *itV;
    resu += (valR - valV) * (valR - valV);
    ++itR;
    ++itV;
  }
  return resu;
}


vector< vector<QPoint> > DirectionalUtil::getPointsCorrelMaxLocalFromScans (
                   DirectionalScan &dirScan, int **tabImage,
                   int largeur, int hauteur, int direction,
                   int correlWidth, int correlThick, 
                   // int indiceSection0, // servira a priori plus tard
                   int indicePcenterSection0)
{
  vector < vector<QPoint> > vResult;
  vector<int> correlRef = getMeanValueCorrelScan (
                  dirScan, tabImage, largeur, hauteur, 0, direction,
                  indicePcenterSection0, correlWidth, correlThick);

  int nbScan = dirScan.getNbScan (direction);
  for (int i = 1; i < nbScan; i++)
  {
    vector<PointAndCorrel> vectPC = getPointsCorrelFromScans (
                  correlRef, dirScan, i, tabImage, largeur, hauteur,
                  direction, correlWidth, correlThick);
    //recup de la liste des valeurs de correlation
    vector<double> vectListValues;
    vector<PointAndCorrel>::iterator it = vectPC.begin ();
    while (it != vectPC.end ())
    {
      PointAndCorrel pc = *it;
      vectListValues.push_back ((double) pc.correl);
      it ++;
    }
    vector<int> vIndiceMinLoc = LocalVariations::getLocalMinimaIndices (
                  vectListValues, 0.0001, false);

    vector<PointAndCorrel> vectPtsEtCorrel;
    vector<int>::iterator itIndice = vIndiceMinLoc.begin ();
    while (itIndice != vIndiceMinLoc.end ())
    {
      int indice = *itIndice;
      vectPtsEtCorrel.push_back (vectPC.at (indice));
      ++itIndice;
    }

    vector<QPoint> vCandidats;
    sort (vectPtsEtCorrel.begin (), vectPtsEtCorrel.end (), compPtAndCorrel);
    vector<PointAndCorrel>::iterator itPC = vectPtsEtCorrel.begin ();
    while (itPC != vectPtsEtCorrel.end ())
    {
      PointAndCorrel pc = *itPC;
      vCandidats.push_back (pc.pt);
      // break;
      ++itPC;
    }
    vResult.push_back (vCandidats);
  }
  return vResult;
}


vector<QPoint> DirectionalUtil::getPointsCorrelMaxLocalFromScans (
                  DirectionalScan &dirScan, int **tabImage,
                  int largeur, int hauteur, int direction,
                  int correlWidth, int correlThick,
                  int indiceSection0, // servira a priori plus tard
                  int indicePcenterSection0, int position)
{
  vector<QPoint> vResult;
  if (indicePcenterSection0 + correlWidth >= dirScan.getScan (0, RIGHT).size ()
      || indicePcenterSection0 - correlWidth < 0)
  {
    cerr << "Vecteur de reference moyen non Calculable" << endl;
    return vResult;
  }

  vector<int> correlRef = getMeanValueCorrelScan (
                 dirScan, tabImage, largeur, hauteur, 0, direction,
                 indicePcenterSection0, correlWidth, correlThick);
  // Unused // int nbScan = dirScan.getNbScan (direction);
  vector<PointAndCorrel> vectPC = getPointsCorrelFromScans (
                 correlRef, dirScan, position, tabImage,
                 largeur, hauteur, direction, correlWidth, correlThick);
  
  // recup de la liste des valeurs de correlation
  vector<double> vectListValues;
  vector<PointAndCorrel>::iterator it = vectPC.begin ();
  while (it != vectPC.end ())
  {
    PointAndCorrel pc = *it;
    vectListValues.push_back ((double) pc.correl);
    it ++;
  }
  
  vector<int> vIndiceMinLoc = LocalVariations::getLocalMinimaIndices (
                 vectListValues, 0.0001, false);
  vector<PointAndCorrel> vectPtsEtCorrel;
  vector<int>::iterator itIndice = vIndiceMinLoc.begin ();
  while (itIndice != vIndiceMinLoc.end ())
  {
    int indice = *itIndice;
    vectPtsEtCorrel.push_back (vectPC.at (indice));
    ++itIndice;
  }

  vector<QPoint> vCandidats;
  sort (vectPtsEtCorrel.begin (), vectPtsEtCorrel.end (), compPtAndCorrel);
  vector<PointAndCorrel>::iterator itPC = vectPtsEtCorrel.begin ();
  while (itPC != vectPtsEtCorrel.end ())
  {
    PointAndCorrel pc = *itPC;
    vCandidats.push_back (pc.pt);
    break;
    ++itPC;
  }
  return vCandidats;
}


/** Return the list of the gradient vectors associated to each point of the k ieme scan.
 * First: QPoint in the image
 * Second: gradient vector
 */
vector<QPoint>* DirectionalUtil::getGradientDirVectorsFromScans (
                   DirectionalScan &dirScan, int **tabImage,
                   int largeur, int hauteur, int direction, int numScan)
{
  vector<QPoint> *vectResult = new vector<QPoint>[2];
  vector<QPoint> vectPix;
  vector<QPoint> vectGrad;
  vector<QPoint> vectScan = dirScan.getScan (numScan, direction);
  vector<QPoint> vectScanPrec = dirScan.getScan (numScan - 1, direction);
  vector<QPoint> vectScanSuiv = dirScan.getScan (numScan + 1, direction);

  vector<QPoint>::iterator iter = vectScan.begin ();
  // on ne traite pas les bords.
  QPoint p = *iter;
  iter ++;
  int positionCourante = 1;

  vector<double> vectValue;
  while (iter != (vectScan.end () - 1)
         && positionCourante < (int) vectScan.size () -1
         && positionCourante < (int) vectScanSuiv.size () - 1
         && positionCourante < (int) vectScanPrec.size () - 1)
  {
    QPoint pt = vectScan.at (positionCourante);

    QPoint pScanPrecSup = vectScanPrec.at (positionCourante - 1);
    QPoint pScanPrecInf = vectScanPrec.at (positionCourante + 1);
    QPoint pScanPrec = vectScanPrec.at (positionCourante);

    QPoint pScanSup = vectScan.at (positionCourante - 1);
    QPoint pScanInf = vectScan.at (positionCourante + 1);

    QPoint pScanSuivSup = vectScanSuiv.at (positionCourante - 1);
    QPoint pScanSuivInf = vectScanSuiv.at (positionCourante + 1);
    QPoint pScanSuiv = vectScanSuiv.at (positionCourante);

    if ((! (pt.x () >= 0 && pt.x () < largeur
            && pt.y () >= 0 && pt.y () < hauteur))
        || (! (pScanPrecSup.x () >= 0 && pScanPrecSup.x () < largeur
               && pScanPrecSup.y () >= 0 && pScanPrecSup.y () < hauteur))
        || (! (pScanSup.x () >= 0 && pScanSup.x () < largeur
               && pScanSup.y () >= 0 && pScanSup.y () < hauteur))
        || (! (pScanInf.x () >= 0 && pScanInf.x () < largeur
               && pScanInf.y () >= 0 && pScanInf.y () < hauteur))
        || (! (pScanSuivSup.x () >= 0 && pScanSuivSup.x () < largeur
               && pScanSuivSup.y () >= 0 && pScanSuivSup.y () < hauteur))
        || (! (pScanSuivInf.x () >= 0 && pScanSuivInf.x () < largeur
               && pScanSuivInf.y () >= 0 && pScanSuivInf.y () < hauteur))
       )
    {
      iter ++;
      positionCourante ++;
      continue;
    }

    int valScanPrecSup = tabImage[pScanPrecSup.y()][pScanPrecSup.x()];
    int valScanPrecInf = tabImage[pScanPrecInf.y()][pScanPrecInf.x()];
    int valScanPrec = tabImage[pScanPrec.y()][pScanPrec.x()];

    int valScanSup = tabImage[pScanSup.y()][pScanSup.x()];
    int valScanInf = tabImage[pScanInf.y()][pScanInf.x()];

    int valScanSuivSup = tabImage[pScanSuivSup.y()][pScanSuivSup.x()];
    int valScanSuivInf = tabImage[pScanSuivInf.y()][pScanSuivInf.x()];
    int valScanSuiv = tabImage[pScanSuiv.y()][pScanSuiv.x()];

    double gradientY = (valScanPrecSup + 2 * valScanSup + valScanSuivSup)
                       - (valScanPrecInf + 2 * valScanInf + valScanSuivInf);
    double gradientX = - (valScanPrecSup + 2 * valScanPrec + valScanPrecInf)
                       + (valScanSuivSup + 2 * valScanSuiv + valScanSuivInf);

    vectPix.push_back (QPoint (pt.x (), pt.y ()));
    vectGrad.push_back (QPoint (gradientX, gradientY));
    positionCourante ++;
    iter ++;
  }

  vectResult[0] = vectPix;
  vectResult[1] = vectGrad;
  return vectResult;
}


vector<QPoint> DirectionalUtil::getGradientDirMaxFromScans(
                     DirectionalScan &dirScan, int **tabImage,
                     int largeur, int hauteur, int direction, QPoint vectDir)
{
  vector<QPoint> vectResult;
  // dirScan.computeScans (0, 0, largeur, hauteur, direction);
  int nbScan = dirScan.getNbScan (direction);

  // double normDir = sqrt((vectDir.x () * vectDir.x ())
  //                       + (vectDir.y () * vectDir.y ()));

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint> *vect = getGradientDirVectorsFromScans (
                      dirScan, tabImage, largeur, hauteur, direction, i);
    vector<QPoint> vectGradient = vect[1];
    vector<QPoint>::iterator iterGradient = vectGradient.begin ();
    vector<QPoint> vectPoints = vect[0];
    vector<QPoint>::iterator iterPoints = vectPoints.begin ();

    if (vectPoints.size () == 0) continue;

    int gradMax = (*iterGradient).y ();
    QPoint ptGradMax = (*iterPoints);
    // int gradMin = (*iterGradient).y ();
    QPoint ptGradMin =  (*iterPoints);

    bool foundSup = false;
    while (iterPoints != vectPoints.end ())
    {
      QPoint pt = (*iterPoints);
      QPoint vectGrad = (*iterGradient);
      double normGrad = sqrt ((vectGrad.x () * vectGrad.x ())
                              + (vectGrad.y () * vectGrad.y ()));
      double gradX = vectGrad.x ();
      double gradY = vectGrad.y ();
      // double scal = vectGrad.x () * vectDir.x ()
      //               + vectGrad.y () * vectDir.y ();
      // double scal = gradX * vectDir.x () + gradY * vectDir.y ();

      double val = gradX * gradX + gradY * gradY;

      // double val = abs (gradX);
      // double angle = atan2 (gradY / normGrad, gradX / normGrad) + 3.14 / 2.0;

      // if ((acos (abs (scal / (normGrad * normDir)))) < (3.14 / 4.0))
      // {
      //   if (abs (angle) - (3.14 / 2.0) < (3.14 / 6.0))
      //   {
      if (abs (gradX / normGrad) < 0.5 && normGrad > 30)
      {
	//if (abs (gradX / normGrad) < abs (gradY / normGrad))
	foundSup = true;
	if (val > gradMax)
        {
	  gradMax = val;
	  ptGradMax = pt;
	}
      }
      iterGradient ++;
      iterPoints ++;
    }
    if (foundSup) vectResult.push_back (ptGradMax);
    else vectResult.push_back (QPoint (-1, -1));
  }
  return vectResult;
}


vector<QPoint> DirectionalUtil::getGradientMaxFromScans (
                    DirectionalScan &dirScan, int **tabImage,
                    int largeur, int hauteur,
                    QPoint **tabGradient, bool **tabGradientUsed,
                    int direction, QPoint vectDir, double seuil)
{
  vector<QPoint> vectResult;
  // dirScan.computeScans (0, 0, largeur, hauteur, direction);
  int nbScan = dirScan.getNbScan (direction);

  double normDir = sqrt ((vectDir.x () * vectDir.x ())
                         + (vectDir.y () * vectDir.y ()));
  double dx = vectDir.x () / normDir;
  double dy = vectDir.y () / normDir;
  // double angleD = atan2 (dy, dx);

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint> vectPoints = dirScan.getScan (i, direction);
    vector<QPoint>::iterator iterPoints = vectPoints.begin ();
    if (vectPoints.size () == 0) continue;

    QPoint ptGradMax =  (*iterPoints);
    QPoint g = tabGradient[ptGradMax.y()][ptGradMax.x()];
    double gradMax = sqrt (g.x () * g.x () + g.y () * g.y ());

    bool foundSup = false;
    while (iterPoints != vectPoints.end ())
    {
      QPoint pt = (*iterPoints);
      QPoint vectGrad = tabGradient[pt.y()][pt.x()];
      double normeGrad = sqrt (vectGrad.x () * vectGrad.x ()
                               + vectGrad.y () * vectGrad.y ());
     if (normeGrad == 0)
     {
       iterPoints ++;
       continue;
     }

     double gx = vectGrad.x () / normeGrad;
     double gy = vectGrad.y () / normeGrad;
     double scalaire = dx * gx + dy * gy;

     if (normeGrad > seuil && abs (scalaire) > 0.95
         && ! tabGradientUsed[pt.y()][pt.x()])
     {
       if (normeGrad >= gradMax)
       {
	 foundSup = true;
	 gradMax = normeGrad;
	 ptGradMax = pt;
       }
     }
     iterPoints ++;
   }
   if(foundSup) vectResult.push_back (ptGradMax);
   else vectResult.push_back (QPoint (-1, -1));
 }
 return vectResult;
}


vector< vector<QPoint> > DirectionalUtil::getGradientVectorsFromScans (
                   DirectionalScan &dirScan, int **tabImage,
                   int largeur, int hauteur,
                   QPoint **tabGradient, bool **tabGradientUsed,
                   int direction, QPoint vectDir, double seuil)
{
  vector< vector<QPoint> > vectResult;
  // dirScan.computeScans (0, 0, largeur, hauteur, direction);
  int nbScan = dirScan.getNbScan (direction);

  double normDir = sqrt ((vectDir.x () * vectDir.x ())
                         + (vectDir.y () * vectDir.y ()));
  double dx = vectDir.x () / normDir;
  double dy = vectDir.y () / normDir;
  // double angleD = atan2 (dy, dx);

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint> vectPoints = dirScan.getScan (i, direction);
    vector<QPoint>::iterator iterPoints = vectPoints.begin ();
    if(vectPoints.size () == 0) continue;

    QPoint ptGradMax =  (*iterPoints);
    QPoint g = tabGradient[ptGradMax.y()][ptGradMax.x()];
    // double gradMax = sqrt (g.x () * g.x () + g.y () * g.y ());
    vector<QPoint *> vCandidatsPre;

    // bool foundSup = false;
    while (iterPoints != vectPoints.end ())
    {
      QPoint pt = (*iterPoints);
      QPoint vectGrad = tabGradient[pt.y()][pt.x()];
      double normeGrad = sqrt (vectGrad.x () * vectGrad.x ()
                               + vectGrad.y () * vectGrad.y ());
      if (normeGrad == 0)
      {
        iterPoints ++;
        continue;
      }
 
      double gx = vectGrad.x () / normeGrad;
      double gy = vectGrad.y () / normeGrad;
      double scalaire = dx * gx + dy * gy;

      if (normeGrad > seuil && abs (scalaire) > 0.95
          && ! tabGradientUsed[pt.y()][pt.x()])
      {
        QPoint* t = new QPoint[2];
        t[0] = pt;
        t[1] = tabGradient[pt.y()][pt.x()];
        vCandidatsPre.push_back (t);
      }
      iterPoints ++;
    }

    sort (vCandidatsPre.begin (), vCandidatsPre.end (), compGradVectors);
    vector<QPoint> vCandidats;
    vector<QPoint *>::iterator iv = vCandidatsPre.begin ();
    while (iv != vCandidatsPre.end ())
    {
      vCandidats.push_back ((*iv)[0]);
      iv ++;
    }
    vectResult.push_back (vCandidats);
  }
  return vectResult;
}


bool DirectionalUtil::compGradVectors (QPoint *p , QPoint *q)
{
  QPoint g1 = p[1];
  QPoint g2 = q[1];
  double norme1 = g1.x () * g1.x () + g1.y () * g1.y ();
  double norme2 = g2.x () * g2.x () + g2.y () * g2.y ();
  return (norme1 > norme2);
}


vector<QPoint> DirectionalUtil::getGradientMaxFromScans (
                   DirectionalScan &dirScan, int **tabImage,
                   int largeur, int hauteur,
                   QPoint **tabGradient, bool **tabGradientUsed,
                   int direction, int seuil)
{
  vector<QPoint> vectResult;

  // dirScan.computeScans (0, 0, largeur - 1, hauteur - 1, direction);
  int nbScan = dirScan.getNbScan (direction);

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint> vectPoints = dirScan.getScan (i, direction);
    vector<QPoint>::iterator iterPoints = vectPoints.begin ();
    if (vectPoints.size () == 0) continue;

    QPoint ptGradMax = (*iterPoints);
    QPoint g = tabGradient[ptGradMax.y()][ptGradMax.x()];
    double gradMax = sqrt (g.x () * g.x () + g.y () * g.y ());

    bool foundSup = false;
    while (iterPoints != vectPoints.end ())
    {
      QPoint pt = (*iterPoints);
      QPoint vectGrad = tabGradient[pt.y()][pt.x()];
      double normeGrad = vectGrad.x () * vectGrad.x ()
                         + vectGrad.y () * vectGrad.y ();

      if (normeGrad > seuil && ! tabGradientUsed[pt.y()][pt.x()])
      {
        foundSup = true;
        if(normeGrad > gradMax)
        {
          gradMax = normeGrad;
          ptGradMax = pt;
        }
      }
      iterPoints ++;
    }
    if (foundSup) vectResult.push_back (ptGradMax);
    else vectResult.push_back (QPoint (-1, -1));
  }
  return vectResult;
}


vector<QPoint> DirectionalUtil::getGradientDirMaxFromScans (
                      DirectionalScan &dirScan, int **tabImage,
                      int largeur, int hauteur, int direction)
{
  vector<QPoint> vectResult;

  // dirScan.computeScans (0, 0, largeur, hauteur, direction);
  int nbScan = dirScan.getNbScan (direction);
  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint>* vect = getGradientDirVectorsFromScans (
                      dirScan, tabImage, largeur, hauteur, direction, i);
    vector<QPoint> vectGradient = vect[1];
    vector<QPoint>::iterator iterGradient = vectGradient.begin ();
    vector<QPoint> vectPoints = vect[0];
    vector<QPoint>::iterator iterPoints = vectPoints.begin ();
    if (vectPoints.size () == 0) continue;

    int gradMax = (*iterGradient).y ();
    QPoint ptGradMax = (*iterPoints);
    // int gradMin = (*iterGradient).y ();
    QPoint ptGradMin =  (*iterPoints);
    bool max = false;

    while (iterPoints != vectPoints.end ())
    {
      QPoint pt = (*iterPoints);
      QPoint vectGrad = (*iterGradient);

      double gradX = vectGrad.y ();
      double gradY = vectGrad.x ();
      double val = gradX * gradX + gradY * gradY;
      if (val > gradMax && sqrt (val) > 30)
      {
	max = true;
	gradMax = val;
	ptGradMax = pt;
      }
      iterGradient ++;
      iterPoints ++;
    }
    if (max) vectResult.push_back (ptGradMax);
  }
  return vectResult;
}


vector<QPoint> DirectionalUtil::getScansBorder (
                     DirectionalScan &dirScan, QImage &image, int direction)
{
  vector<QPoint> vectResult;
  dirScan.computeScans (0, 0, image.width (), image.height (), direction);
  int nbScan = dirScan.getNbScan (direction);

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint> vect = dirScan.getScan (i, direction);
    if (vect.size () != 0)
    {
      QPoint pbas = vect.at (0);
      QPoint phaut = vect.at (vect.size () - 1);
      vectResult.push_back (pbas);
      vectResult.push_back (phaut);
    }
  }
  return vectResult;
}


// return all the "num" th elements from each scans
vector<QPoint> DirectionalUtil::getElementsI (
                   DirectionalScan &dirScan, QImage &image,
                   int direction, int num)
{
  vector<QPoint> vectResult;
  dirScan.computeScans (0, 0, image.width (), image.height (), direction);
  int nbScan = dirScan.getNbScan (direction);

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint> vect = dirScan.getScan (i, direction);
    if (num < (int) vect.size ())
    {
      QPoint p = vect.at (num);
      vectResult.push_back (p);
    }
  }
  return vectResult ;
}


vector<QPoint> DirectionalUtil::getDuplicatedPoints (vector<QPoint> &vect,
						     int largeur, int hauteur)
{
  vector<QPoint> vectResult;
  vector<QPoint>::iterator iter = vect.begin ();
  bool *tab[largeur];
  for (int i = 0; i < largeur; i++)
  {
    tab[i] = new bool[hauteur];
    for (int j = 0; j < hauteur; j++) tab[i][j] = false;
  }
  while (iter != vect.end ())
  {
    QPoint p = *iter;
    if (tab[p.x()][p.y()]) vectResult.push_back (p);
    else tab[p.x()][p.y()] = true;
    iter ++;
  }
  return vectResult;
}


QPoint* DirectionalUtil::getAjustedPoints (QPoint pCenter, double direction,
                                           double distance)
{
  QPoint *tabResu = new QPoint[2];
  QPoint p1 = QPoint (pCenter.x () + distance * cos (direction),
                      pCenter.y () + distance * sin (direction));
  QPoint p2 = QPoint (pCenter.x () + distance * cos (direction + M_PI),
                      pCenter.y () + distance * sin (direction + M_PI));
  tabResu[0] = p1;
  tabResu[1] = p2;
  return tabResu;
}


QPoint **DirectionalUtil::retVectorsGradientRepereNornal (
                               int **tabImage, int largeur, int hauteur)
{
  QPoint **tabPointResult = new QPoint*[hauteur];

  for (int i = 0; i < hauteur; i++)
  {
    tabPointResult[i] = new QPoint[largeur];
    for (int j = 0; j < largeur; j++)
    {
      if (i > 0 && i < hauteur - 1 && j > 0 && j < largeur - 1)
      {
	double gradY = - tabImage[i-1][j-1] - 2.0
                         * tabImage[i-1][j] - tabImage[i-1][j+1]
                       + tabImage[i+1][j-1] + 2.0
                         * tabImage[i+1][j] + tabImage[i+1][j+1];
	double gradX = tabImage[i-1][j+1] + 2.0 * tabImage[i][j+1]
                       + tabImage[i+1][j+1]
                       - tabImage[i-1][j-1] - 2.0
                         * tabImage[i][j-1] - tabImage[i+1][j-1];
	tabPointResult[i][j] = QPoint(gradX, gradY);
      }
      else tabPointResult[i][j] = QPoint (0, 0);
    }
  }
  return tabPointResult;
}


QPoint **DirectionalUtil::retVectorsGradientRepereNornal (
                int **tabImage, int largeur, int hauteur, int nbIteration)
{
  QPoint **tabPointResult = new QPoint*[hauteur];
  return tabPointResult;
}


/*
 * Renvoie la liste des candidats representant des maximaux locaux
 *  puis filtrage dans la direction determinee par l'utilisateur.
 */
vector< vector<QPoint> > DirectionalUtil::getPointsGradMaxLocalFromScans(
                             DirectionalScan &dirScan, QPoint **tabGradient,
                             int largeur, int hauteur,
                             bool **tabGradientUsed, int direction,
                             QPoint vectDir, double cosDirLimits)
{
  vector< vector<QPoint> > vectResult;
  // dirScan.computeScans (0, 0, largeur, hauteur, direction);
  int nbScan = dirScan.getNbScan (direction);

  double normDir = sqrt((vectDir.x()*vectDir.x())+(vectDir.y()*vectDir.y()));
  double dx = vectDir.x()/normDir;
  double dy = vectDir.y()/normDir;

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<QPoint> vectPoints = dirScan.getScan (i, direction);
    vector<QPoint>::iterator iterPoints = vectPoints.begin ();
    if (vectPoints.size () == 0) continue;

    vector<QPoint> vCandidats;
    vector<double> vectListValeur;
    // Recuperation des indices des max locaux de norme de gradient
    while (iterPoints != vectPoints.end ())
    {
      QPoint pt = (*iterPoints);
      QPoint vectGrad = tabGradient[pt.y()][pt.x()];
      double normeGrad = vectGrad.x () * vectGrad.x ()
                         + vectGrad.y () * vectGrad.y ();
      vectListValeur.push_back (normeGrad);
      iterPoints ++;
    }
    vector<int> vIndiceMaxLoc = LocalVariations::getLocalMaximaIndices (
                                       vectListValeur, 0.0001, false);

    // Recuperation des pointsEtGradient associes aux indices
    vector<int>::iterator itIndices = vIndiceMaxLoc.begin ();
    vector<PointAndGrad> vectPtsEtGrad;
    while (itIndices != vIndiceMaxLoc.end ())
    {
      int indiceMaxLoc = *itIndices;
      QPoint pt = vectPoints.at (indiceMaxLoc);
      QPoint vectGrad = tabGradient[pt.y()][pt.x()];
      double normeGrad = sqrt (vectGrad.x () * vectGrad.x ()
                               + vectGrad.y () * vectGrad.y ());
      double gx = vectGrad.x () / normeGrad;
      double gy = vectGrad.y () / normeGrad;
      double scalaire = dx * gx + dy * gy;
      if (abs (scalaire) > cosDirLimits && ! tabGradientUsed[pt.y()][pt.x()])
      {
        PointAndGrad ptg;
        ptg.pt = pt;
        ptg.grad = normeGrad;
        vectPtsEtGrad.push_back (ptg);
      }
      ++itIndices;
    }
    // Classement des vecteurs
    sort (vectPtsEtGrad.begin (), vectPtsEtGrad.end (), compPtAndGrad);
    // Recuperation des points candidats classes dans l'ordre.
    vector<PointAndGrad>::iterator itPG = vectPtsEtGrad.begin ();
    while (itPG != vectPtsEtGrad.end ())
    {
      PointAndGrad pg = *itPG;
      vCandidats.push_back (pg.pt);
      ++itPG;
    }
    vectResult.push_back (vCandidats);
  }
  return vectResult;
}


bool DirectionalUtil::compPtAndGrad (PointAndGrad pg1, PointAndGrad pg2)
{
  double norme1 = pg1.grad;
  double norme2 = pg2.grad;
  return (norme1 > norme2);
}


bool DirectionalUtil::compPtAndCorrel (PointAndCorrel pg1, PointAndCorrel pg2)
{
  double norme1 = pg1.correl;
  double norme2 = pg2.correl;
  return (norme1 < norme2);
}


vector<int> DirectionalUtil::getImageSection (
                  DirectionalScan &dirScan, int numSection, int direction,
                  int **tabImage, int largeur, int hauteur)
{
  vector<int> vResult;
  if (! dirScan.isScanDefined (numSection, direction)) return vResult;
  vector<QPoint> vectPoints = dirScan.getScan (numSection, direction);
  vector<QPoint>::iterator itVP = vectPoints.begin ();
  while (itVP != vectPoints.end ())
  {
    QPoint pt = *itVP;
    if (pt.x () < largeur && pt.y () < hauteur && pt.x () >= 0 && pt.y () >= 0)
      vResult.push_back (tabImage[pt.y()][pt.x()]);
    ++itVP;
  }
  return vResult;
}
