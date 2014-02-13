#include <cstdlib>
#include <iostream>
#include <limits>
#include <algorithm>
#include <cmath>
#include "imagedata.h"

using namespace std;


ImageData::ImageData (int width, int height)
{
  this->width = width;
  this->height = height;
  imageMapDefined = false;
  gradientMapDefined = false;
}


ImageData::~ImageData ()
{
  if (imageMapDefined) delete imageMap;
  if (gradientMapDefined) delete gradientMap;
}


Pixel **ImageData::getVectorsGradientRepereNornal (int **tabImage)
{
  Pixel **tabPointResult = new Pixel*[height];

  for (int i = 0; i < height; i++)
  {
    tabPointResult[i] = new Pixel[width];
    for (int j = 0; j < width; j++)
    {
      if (i > 0 && i < height - 1 && j > 0 && j < width - 1)
      {
        double gradY = - tabImage[i-1][j-1] - 2.0
                         * tabImage[i-1][j] - tabImage[i-1][j+1]
                       + tabImage[i+1][j-1] + 2.0
                         * tabImage[i+1][j] + tabImage[i+1][j+1];
        double gradX = tabImage[i-1][j+1] + 2.0 * tabImage[i][j+1]
                       + tabImage[i+1][j+1]
                       - tabImage[i-1][j-1] - 2.0
                         * tabImage[i][j-1] - tabImage[i+1][j-1];
        tabPointResult[i][j] = Pixel(gradX, gradY);
      }
      else tabPointResult[i][j] = Pixel (0, 0);
    }
  }
  return tabPointResult;
}


void ImageData::buildGradientMap (int **data)
{
  if (gradientMapDefined) delete gradientMap;

  gradientMap = new ImageData::Gradient[width * height];
  ImageData::Gradient *gm = gradientMap;

  for (int j = 0; j < width; j++)
  {
    gm->x = 0;
    gm->y = 0;
    gm++;
  }
  for (int i = 1; i < height - 1; i++)
  {
    gm->x = 0;
    gm->y = 0;
    gm++;
    for (int j = 1; j < width - 1; j++)
    {
      gm->x = data[i-1][j+1] + 2.0 * data[i][j+1] + data[i+1][j+1]
              - data[i-1][j-1] - 2.0 * data[i][j-1] - data[i+1][j-1];
      gm->y = data[i+1][j-1] + 2.0 * data[i+1][j] + data[i+1][j+1]
              - data[i-1][j-1] - 2.0 * data[i-1][j] - data[i-1][j+1];
      gm++;
    }
    gm->x = 0;
    gm->y = 0;
    gm++;
  }
  for (int j = 0; j < width; j++)
  {
    gm->x = 0;
    gm->y = 0;
    gm++;
  }

  gradientMapDefined = true;
}


int ImageData::getWidth ()
{
  return width;
}


int ImageData::getHeight ()
{
  return height;
}


vector<Pixel> ImageData::maxGradient (DirectionalScan &dirScan,
                                    int direction, int threshold)
{
  vector<Pixel> vectResult;
  int nbScan = dirScan.getNbScan (direction);

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<Pixel> vectPoints = dirScan.getScan (i, direction);
    if (vectPoints.size () == 0) continue;

    vector<Pixel>::iterator iterPoints = vectPoints.begin ();
    Pixel ptGradMax = (*iterPoints);
    ImageData::Gradient g = gradientMap[ptGradMax.y() * width + ptGradMax.x()];
    int gradMax = g.x * g.x + g.y * g.y;

    bool foundSup = false;
    while (iterPoints != vectPoints.end ())
    {
      Pixel pt = (*iterPoints);
      ImageData::Gradient vectGrad = gradientMap[pt.y() * width + pt.x()];
      int normeGrad = vectGrad.x * vectGrad.x
                      + vectGrad.y * vectGrad.y;
      if (normeGrad > threshold)
      {
        foundSup = true;
        if (normeGrad > gradMax)
        {
          gradMax = normeGrad;
          ptGradMax = pt;
        }
      }
      iterPoints ++;
    }
    if (foundSup) vectResult.push_back (ptGradMax);
    else vectResult.push_back (Pixel (-1, -1));
  }
  return vectResult;
}


vector<vector<Pixel> > ImageData::gradientRidges (DirectionalScan &dirScan,
                              int direction, Pixel vectDir, double cosDirLimits)
{
  vector<vector<Pixel> > vectResult;
  int nbScan = dirScan.getNbScan (direction);

  double normDir = sqrt (vectDir.x () * vectDir.x ()
                         + vectDir.y () * vectDir.y ());
  double dx = - vectDir.y () / normDir;
  double dy = vectDir.x () / normDir;

  for (int i = 1; i < nbScan - 1; i++)
  {
    vector<Pixel> vectPoints = dirScan.getScan (i, direction);
    if (vectPoints.size () == 0) continue;

    vector<Pixel>::iterator iterPoints = vectPoints.begin ();
    vector<Pixel> vCandidats;
    vector<long> vectListValeur;
    // Recuperation des indices des max locaux de norme de gradient
    while (iterPoints != vectPoints.end ())
    {
      Pixel pt = (*iterPoints);
      ImageData::Gradient vectGrad = gradientMap[pt.y() * width + pt.x()];
      long normeGrad = vectGrad.x * vectGrad.x + vectGrad.y * vectGrad.y;
      vectListValeur.push_back (normeGrad);
      iterPoints ++;
    }

    vector<int> vIndiceMaxLoc = getLocalMaximaIndices (vectListValeur);

    // Recuperation des pointsEtGradient associes aux indices
    vector<int>::iterator itIndices = vIndiceMaxLoc.begin ();
    vector<PointAndGrad> vectPtsEtGrad;
    while (itIndices != vIndiceMaxLoc.end ())
    {
      int indiceMaxLoc = *itIndices;
      Pixel pt = vectPoints.at (indiceMaxLoc);
      ImageData::Gradient vectGrad = gradientMap[pt.y() * width + pt.x()];
      double normeGrad = sqrt (vectGrad.x * vectGrad.x
                               + vectGrad.y * vectGrad.y);
      double gx = vectGrad.x / normeGrad;
      double gy = vectGrad.y / normeGrad;
      double scalaire = dx * gx + dy * gy;
      if (abs (scalaire) > cosDirLimits)
      {
        PointAndGrad ptg;
        ptg.pt = pt;
        ptg.grad = normeGrad;
        vectPtsEtGrad.push_back (ptg);
      }
      ++itIndices;
    }

    // Classement des vecteurs
    sort (vectPtsEtGrad.begin (), vectPtsEtGrad.end (),
          ImageData::comparePtAndGrad);
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



bool ImageData::comparePtAndGrad (PointAndGrad pg1, PointAndGrad pg2)
{
  return (pg1.grad > pg2.grad);
}


vector<int> ImageData::getLocalMaximaIndices (vector<long> &vectValues)
{
  vector<int> vResult;
  vector<ImageData::LocalMinMaxArea> vectAreas
                             = getLocalMaxMinArea (vectValues);
  vector<ImageData::LocalMinMaxArea>::iterator it = vectAreas.begin ();
  int tailleTab = vectValues.size ();
  while (it != vectAreas.end ())
  {
    ImageData::LocalMinMaxArea area = *it;
    if (area.isMaxArea)
    {
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


vector<int> ImageData::getLocalMinimaIndices (vector<long> &vectValues)
{
  vector<int> vResult;
  vector<ImageData::LocalMinMaxArea> vectAreas
                             = getLocalMaxMinArea (vectValues);
  vector<ImageData::LocalMinMaxArea>::iterator it = vectAreas.begin ();
  int tailleTab = vectValues.size ();
  while (it != vectAreas.end ())
  {
    ImageData::LocalMinMaxArea area = *it;
    if (! area.isMaxArea)
    {
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


vector<ImageData::LocalMinMaxArea>
ImageData::getLocalMaxMinArea (vector<long> &vectValues)
{
  vector<ImageData::LocalMinMaxArea> vectResuMinMaxArea;
  // vector<int>::iterator iter = vectValues.begin ();

  int tailleVValues = vectValues.size ();

  int decalage = 0;
  bool estMontant = true;

  // Recherche de la premiere valeur differente du depart
  while (decalage < tailleVValues - 1
         && vectValues.at (decalage) == vectValues.at (0))
  {
    if (vectValues.at (decalage) - vectValues.at (decalage + 1) < 0)
    {
      estMontant = true;
      break;
    }
    if (vectValues.at (decalage) - vectValues.at (decalage + 1) > 0)
    {
      estMontant = false;
      break;
    }
    decalage++;
  }

  for(int i = decalage; i < tailleVValues - 1; i++)
  {
    if (estMontant)
    {
      if ((vectValues.at (i + 1) - vectValues.at (i)) < 0)
      {
        estMontant = false;
        int k = i;
        LocalMinMaxArea lMinMax;
        lMinMax.indiceFin = k;
        while (vectValues.at (k) == vectValues.at (i))
        {
          k--;
        }
        lMinMax.indiceDebut = k + 1;
        lMinMax.isMaxArea = true;
        vectResuMinMaxArea.push_back (lMinMax);
      }
    }
    else
    {
      if (vectValues.at (i + 1) - vectValues.at (i) > 0)
      {
        estMontant = true;
        int k = i;
        LocalMinMaxArea lMinMax;
        lMinMax.indiceFin = k;
        while (vectValues.at (k) == vectValues.at (i))
        {
          k--;
        }
        lMinMax.indiceDebut = k + 1;
        lMinMax.isMaxArea = false;
        vectResuMinMaxArea.push_back (lMinMax);
      }
    }
  }
  return vectResuMinMaxArea;
}
