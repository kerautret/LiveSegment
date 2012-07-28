#include <cstdlib>
#include <iostream>
#include "directionalscan.h"
#include "pointset.h"
#include "util.h"    // Direction et cmath


DirectionalScan::DirectionalScan (QPoint p1, QPoint p2)
{
  this->p1 = p1;
  this->p2 = p2;
  this->calcDroites ();
  this->directionOctant = this->retOctantDroiteD1 ();
  this->computeMotif (p1, p2);
}


DirectionalScan::DirectionalScan (QPoint p1, QPoint p2, double directionAngle)
{
  this->p1 = p1;
  this->p2 = p2;
  // this->calcDroites ();
  // this->directionOctant = this->retOctantDroiteD1 ();
  // cerr << "octant first = " << directionOctant << endl;
  // cerr << "val ini a,b:" << a1<< " " << b1 << endl;
  this->calcDroites (directionAngle);
  this->directionOctant = this->retOctantDroiteD1 ();
  // cerr << "octant second = " << directionOctant << endl;
  // cerr << "direction angle " << directionAngle << endl;
  // cerr << "val second a,b:" << a1<< " " << b1 << "cos (angle)="
  //      << cos(directionAngle)<< endl;
  this->computeMotif (p1, p2);
}


DirectionalScan::DirectionalScan ()
{
}


vector<QPoint> DirectionalScan::getVectPoints ()
{
  return vectPoints;
}


vector<QPoint> DirectionalScan::getScan (int num, int direction)
{
  if (! isScanDefined (num, direction))
  {
    cerr << "Trying to get an undefined scan at " << num
         << " in direction  " << direction << endl;
    exit (1);
  }
  return (direction == RIGHT) ? vectRightScan.at (num) : vectLeftScan.at (num);
}


vector<QPoint> DirectionalScan::getScanOrientedIndex (int num, int direction)
{
  vector<QPoint> vResult;
  if (! isScanDefined (num, direction))
  {
    cerr << "Trying to get an undefined scan at " << num
         << " in direction  " << direction << endl;
    exit (1);
  }
  if (num >= 0)
    vResult = getScan (num, direction);
  else
    vResult = getScan (abs (num), (direction == RIGHT) ? LEFT : RIGHT);
  return vResult;
}


bool DirectionalScan::isScanDefined (int num, int direction)
{
  if (num > 0)
    return (direction == RIGHT) ? (abs (num) < vectRightScan.size ())
                                : (abs (num) < vectLeftScan.size ());
  else
    return (direction == RIGHT) ? (abs (num) < vectLeftScan.size ())
                                : (abs (num) < vectRightScan.size ());
}


vector<QPoint> DirectionalScan::getLeftScan (int num)
{
  return vectLeftScan.at (num);
}


vector<QPoint> DirectionalScan::getRightScan (int num)
{
  return vectRightScan.at (num);
}


int DirectionalScan::getNbScan (int direction)
{
  return (direction == RIGHT) ? vectRightScan.size () : vectLeftScan.size ();
}


int DirectionalScan::getNbLeftScan ()
{
  return vectLeftScan.size ();
}


int DirectionalScan::getNbRightScan ()
{
  return vectRightScan.size ();
}


QPoint *DirectionalScan::retTabDeplacements (int direction)
{
  int tailleDeplacements = this->retTailleMotif ();
  QPoint* vresult = new QPoint[tailleDeplacements];
  for (int i = 0; i < tailleDeplacements; i++)
  {
    QPoint dep = tabDeplacements[i];
    vresult[i]= QPoint ((direction == RIGHT) ?  dep.y () : -dep.y (),
		        (direction == RIGHT) ? -dep.x () :  dep.x ());
  }
  return vresult;
}


int DirectionalScan::retTailleMotif ()
{
  return vectPoints.size () - 1;
}


int DirectionalScan::retOctantDroiteD1 ()
{
  if (a1 >= 0 && b1 >= 0 && b1 >= a1)
    return 1;
  if (a1 >= 0 && b1 >= 0 && b1 < a1)
    return 2;
  if (b1 <= 0 && a1 >= 0 && -b1 <= a1)
    return 3;
  if (b1 <= 0 && a1 > 0 && -b1 > a1)
    return 4;
  if (a1 <= 0 && b1 <= 0 && -b1 >= -a1)
    return 5;
  if (a1 <= 0 && b1 <= 0 && -b1 < -a1)
    return 6;
  if (b1 >= 0 && a1 < 0 && b1 <= -a1)
    return 7;
  if (b1 >= 0 && a1 <= 0 && b1 >= -a1)
    return 8;
  else 
    return 0;
}


bool DirectionalScan::isRight (QPoint p)
{
  int ux = p2.x () - p1.x ();
  int uy = p2.y () - p1.y ();
  int vx = p.x () - p1.x ();
  int vy = p.y () - p1.y ();
  return (ux * vy - uy * vx > 0);
}


bool DirectionalScan::isBetweenLines (QPoint p)
{
  return (a1 * p.x() - b1 * p.y() <= mu1 && a2 *p.x() - b2 * p.y() >= mu2);
}


int *DirectionalScan::retCoeffDroitesD1 ()
{
  int *d1 = new int[3];
  d1[0] = a1;
  d1[1] = b1;
  d1[2] = mu1;
  return d1;
}


int *DirectionalScan::retCoeffDroitesD2 ()
{
  int *d2 = new int[3];
  d2[0] = a2;
  d2[1] = b2;
  d2[2] = mu2;
  return d2;
}


void DirectionalScan::calcDroites ()
{
  int a0 = p2.y () - p1.y ();
  int b0 = p2.x () - p1.x ();
  this->a1 = -b0;
  this->a2 = -b0;
  this->b1 = a0;
  this->b2 = a0;
  this->mu1 = a1 * p1.x () - b1 * p1.y ();
  this->mu2 = a2 * p2.x () - b2 * p2.y ();
}


void DirectionalScan::calcDroites (double directionAngle)
{
  // Orientates the angle so that angle lies in [-angle(P1P2), angle(P1P2)]
  double ux = p2.x () - p1.x ();
  double uy = p2.y () - p1.y ();
  double vx = cos (directionAngle);
  double vy = sin (directionAngle);
  double prodScal = ux * vy - uy * vx;
  if (prodScal > 0.0) directionAngle += M_PI;

  this->a1 =  sin(directionAngle) * 10000;
  this->a2 =  sin(directionAngle) * 10000;
  this->b1 =  cos(directionAngle) * 10000;
  this->b2 =  cos(directionAngle) * 10000;
  this->mu1 = a1 * p1.x () - b1 * p1.y ();
  this->mu2 = a2 * p2.x () - b2 * p2.y ();
}


void DirectionalScan::computeMotif (QPoint p1, QPoint p2)
{
  vectPoints = PointSet::tracerSegment (p1, p2);
  vector<QPoint>::iterator iter = vectPoints.begin ();
  tabDeplacements = new QPoint[vectPoints.size() - 1];
  int i = 0;
  QPoint pointLast = *iter;
  iter ++;
  while (iter != vectPoints.end ())
  {
    QPoint p = *iter;
    tabDeplacements[i] = QPoint (p.x () - pointLast.x (),
                                 p.y () - pointLast.y ());
    i ++;
    iter ++;
    pointLast = p;
  }
}


QPoint *DirectionalScan::inverseSensMotif (QPoint *tabDepl, int taille)
{
  QPoint *result = new QPoint [taille];
  for (int i = 0; i < taille; i++)
  {
    result[i] = QPoint (- tabDepl[taille-i-1].x(), - tabDepl[taille-i-1].y());
  }
  return result;
}


void DirectionalScan::addPixel (int x, int y)
{
  vectPoints.push_back (QPoint (x, y));
}


void DirectionalScan::computeAllScans (int xmin, int ymin, int xmax, int ymax)
{
  this->xmin = xmin;
  this->ymin = ymin;
  this->xmax = xmax;
  this->ymax = ymax;
  computeScans (xmin, ymin, xmax, ymax, RIGHT);
  computeScans (xmin, ymin, xmax, ymax, LEFT);
}


vector<QPoint> DirectionalScan::inverseVector (vector<QPoint> &v)
{
  vector<QPoint> vResult;
  vector<QPoint>::iterator iter = v.end ();
  while (iter != v.begin ())
  {
    iter --;
    QPoint p = *iter;
    vResult.push_back (p);
  }
  return vResult;
}


void DirectionalScan::computeScans (int xmin, int ymin, int xmax, int ymax,
                                    int direction)
{
  computeMotif (p1, p2);
  int tailleDeplacements = this->retTailleMotif ();
  QPoint pointCourant (p1.x (), p1.y ());

  bool aInverser = false;
  if ((directionOctant % 2 == 0 && direction == RIGHT)
      || (directionOctant % 2 == 1 && direction == LEFT))
  {
    tabDeplacements = inverseSensMotif (tabDeplacements, tailleDeplacements);
    pointCourant = QPoint (p2.x (), p2.y ());
    aInverser = true;
  }

  QPoint pInit = pointCourant;
  int posInit = 0;
  int posTabCourante = 0;
  
  while (pointCourant.x () < xmax && pointCourant.x () > xmin
	 && pointCourant.y () > ymin && pointCourant.y () < ymax)
  {
    vector<QPoint> vectToadd;
    posTabCourante = posInit;
    pointCourant = pInit;
    if (directionOctant == 1 || directionOctant == 8)
      pointCourant = QPoint (pointCourant.x () + (direction == RIGHT ? 1 : -1),
                             pointCourant.y ());
    else if (directionOctant == 2 || directionOctant == 3)
      pointCourant = QPoint (pointCourant.x (),
                             pointCourant.y () + (direction == RIGHT ? 1 : -1));
    else if (directionOctant == 4 || directionOctant == 5)
      pointCourant = QPoint (pointCourant.x () + (direction == RIGHT ? -1 : 1),
                             pointCourant.y ());
    else if (directionOctant == 6 || directionOctant == 7)
      pointCourant = QPoint (pointCourant.x (),
                             pointCourant.y () + (direction == RIGHT ? -1 : 1));
        
    QPoint ptdeplacement = tabDeplacements[posTabCourante];
    bool premierPointAjoute = false;
 
    while (isBetweenLines (pointCourant) || (! premierPointAjoute))
    {
      if (isBetweenLines (pointCourant))
      {
	if (pointCourant.x () < xmax && pointCourant.x () > xmin
	    && pointCourant.y () > ymin && pointCourant.y () < ymax)
	  vectToadd.push_back (pointCourant);
	if (! premierPointAjoute)
        {
	  premierPointAjoute = true;
	  posInit = posTabCourante;
	  pInit = pointCourant;
	}
      }
      pointCourant = QPoint (pointCourant.x () + ptdeplacement.x (),
                             pointCourant.y () + ptdeplacement.y ());
      posTabCourante= (posTabCourante + 1) % tailleDeplacements;
      ptdeplacement = tabDeplacements[posTabCourante];
    }
    if (aInverser)
      vectToadd = inverseVector (vectToadd);

    bool isAllInImage = isInImageBounds (vectToadd);
    if (isAllInImage)
    {
      if (direction == RIGHT)
	this->vectRightScan.push_back (vectToadd);
      else
	this->vectLeftScan.push_back (vectToadd);
    }
  }
}


bool DirectionalScan::isInImageBounds (QPoint &p)
{
  return ((p.x () < xmax) && (p.y () < ymax)
           && (p.y() >= ymin) && (p.x () >= xmin));
}


bool DirectionalScan::isInImageBounds (vector<QPoint> &vectP)
{
  bool result = true;
  vector<QPoint>::iterator it = vectP.begin ();
  while (it != vectP.end () && result)
  {
    QPoint p = *it;
    result = result && isInImageBounds (p);
    ++ it;
  }
  return result;
}
