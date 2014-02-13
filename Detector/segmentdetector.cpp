#include <cstdlib>
#include <iostream>
#include <algorithm>
#include <vector>
#include <math.h>
#include "util.h"
#include "segmentdetector.h"
#include "directionalscan.h"



SegmentDetector::SegmentDetector ()
{
  isDetected = false;
  segment = NULL;
  gradientThreshold = DEFAULT_GRADIENT_THRESHOLD;
  directionCloseness = DEFAULT_DIRECTION_CLOSENESS;
  maxWidth = DEFAULT_MAX_WIDTH;
  acceptedLacks = DEFAULT_ACCEPTED_LACKS;
  numberOfSteps = DEFAULT_NUMBER_OF_STEPS;
  imageData = NULL;
}


SegmentDetector::~SegmentDetector ()
{
  if (segment != NULL) delete segment;
}



BlurredSegment *SegmentDetector::detect (Pixel p1, Pixel p2)
{
  vector<Pixel> vectResu;
  if (segment != NULL) delete segment;
  segment = NULL;
  isDetected = false;

  // Premiere detection max des gradients sans direction
  //----------------------------------------------------
  DirectionalScan dirScan = DirectionalScan (p1, p2, 0,
              imageData->getWidth (), 0, imageData->getHeight ());
  vector<Pixel> pointLeftCandidates
            = imageData->maxGradient (dirScan, LEFT, gradientThreshold);
  vector<Pixel> pointRightCandidates
            = imageData->maxGradient (dirScan, RIGHT, gradientThreshold);
  if ((int) pointLeftCandidates.size () == 0
      || (int) pointRightCandidates.size () == 0)
  {
/*
    cerr << "aucune droite detectable (pas de candidats a gauche ou a droite)"
         << endl;
*/
    return NULL;
  }

  if (! buildSegment (pointLeftCandidates, pointRightCandidates))
  {
/*
    cerr << "aucune droite detectable (pas assez de candidats initiaux)"
         << endl;
*/
    return NULL;
  }
  if (numberOfSteps == 1) return segment;


  // Deuxieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction
  //---------------------------------------------------------------
  int *coords = new int[4];
  segment->getDirection (coords);
  Pixel v1 (coords[0], coords[1]);
  Pixel v2 (coords[2], coords[3]);
  DirectionalScan dirScan2 = DirectionalScan (p1, p2, v1, v2, 0,
          imageData->getWidth (), 0, imageData->getHeight ());
  delete coords;

  vector<vector<Pixel> > pointRightCandidatesV
    = imageData->gradientRidges (dirScan2, RIGHT,
                      Pixel (v2.x () - v1.x (), v2.y () - v1.y ()),
                      directionCloseness);
  vector<vector<Pixel> > pointLeftCandidatesV
    = imageData->gradientRidges (dirScan2, LEFT,
                      Pixel (v2.x () - v1.x (), v2.y () - v1.y ()),
                      directionCloseness);
  if ((int) pointLeftCandidatesV.size () == 0
      || (int) pointRightCandidatesV.size () == 0)
  {
//    cerr << "aucune droite detectable." << endl;
    return NULL;
  }

  if (segment != NULL) delete segment;
  segment = NULL;
  if (! buildSegment (pointLeftCandidatesV, pointRightCandidatesV))
    return NULL;
  if (numberOfSteps == 2) return segment;


  // Troisieme detection apres ajustement de la direction initiale
  // puis detection en fonction des gradients max dans la direction 
  //---------------------------------------------------------------
  coords = new int[4];
  segment->getDirection (coords);
  Pixel v3 (coords[0], coords[1]);
  Pixel v4 (coords[2], coords[3]);
  dirScan2 = DirectionalScan (p1, p2, v3, v4, 0,
               imageData->getWidth (), 0, imageData->getHeight ());
  delete coords;

  pointRightCandidatesV
    = imageData->gradientRidges (dirScan2, RIGHT,
                    Pixel (v2.x () - v1.x (), v2.y () - v1.y ()),
                    directionCloseness);
  pointLeftCandidatesV
    = imageData->gradientRidges (dirScan2, LEFT,
                    Pixel (v2.x () - v1.x (), v2.y () - v1.y ()),
                    directionCloseness);
  if ((int) pointLeftCandidates.size () == 0
      || (int) pointRightCandidates.size () == 0)
  {
//    cerr << "aucune droite detectable." << endl;
    return NULL;
  }

  if (segment != NULL) delete segment;
  segment = NULL;
  if (! buildSegment (pointLeftCandidatesV, pointRightCandidatesV))
    return NULL;
  return segment;
}




bool SegmentDetector::buildSegment (vector<Pixel> leftV, vector<Pixel> rightV)
{
  isDetected = false;
  if ((int) leftV.size () < 5 || (int) rightV.size () < 5)
  {
    cerr << "Bande de recherche trop courte" << endl;
      return isDetected;
  }

  Point2D pCentral (leftV.at(0).x (), leftV.at(0).y ());
  Point2D pLeft (leftV.at(1).x (), leftV.at(1).y ());
  Point2D pRight (rightV.at(0).x (), rightV.at(0).y ());
  int pos = 2;

  while (isLeft (pCentral, pLeft, pRight) == 0
         && pos < (int) leftV.size () && pos < (int) rightV.size ())
  {
    pLeft = Point2D (leftV.at(pos).x (), leftV.at(pos).y ());
    pRight = Point2D (rightV.at(pos).x (), rightV.at(pos).y ());
    pos ++;
  }
    
  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    cerr <<  "Trop peu de points (align)" << endl;
    return isDetected;
  }

  segment = new BlurredSegment (pCentral, pLeft, pRight, maxWidth);
  isDetected = true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  vector<Pixel>::iterator iterRight = rightV.begin ();
  vector<Pixel>::iterator iterLeft = leftV.begin ();

  bool scanRightStopped = false;
  bool scanLeftStopped = false;

  while(! scanRightStopped || ! scanLeftStopped)
  {
    if (! scanRightStopped)
    {
      Pixel pr = *iterRight;
      nbVideRight ++;
      if (! (pr.x () == -1 && pr.y () == -1))
        if (segment->addPoint (Point2D (pr.x (), pr.y ()), RIGHT))
          nbVideRight = 0;
    }
    if(! scanLeftStopped)
    {
      Pixel pr = *iterLeft;
      nbVideLeft ++;
      if (! (pr.x () == -1 && pr.y () == -1))
        if (segment->addPoint (Point2D (pr.x (), pr.y ()), LEFT))
          nbVideLeft = 0;
    }

    iterRight ++;
    iterLeft ++;
    if (iterRight == rightV.end () || nbVideRight > acceptedLacks)
      scanRightStopped = true;
    if (iterLeft == leftV.end () || nbVideLeft > acceptedLacks)
      scanLeftStopped = true;
  }
  isDetected = true;
  return (isDetected);
}
      



bool SegmentDetector::buildSegment (vector< vector<Pixel> > leftW,
                                    vector< vector<Pixel> > rightW)
{
  isDetected = false;
  if ((int) leftW.size () < 2 || (int) rightW.size () < 2
                              || (int) leftW.at(0).size () == 0)
    return isDetected;

  int decalC = 0;
  Point2D pCentral (leftW.at(decalC).at(0).x (), leftW.at(decalC).at(0).y ());
  int decalL = 1;
  while ((int) leftW.at(decalL).size () == 0
         && decalL < (int) leftW.size () - 1)
    decalL ++;

  int decalR = 1;
  while ((int) rightW.at(decalR).size () == 0
         && decalR < (int) rightW.size () - 1)
    decalR ++;    

  if (decalR >= (int) rightW.size () || decalL >= (int) leftW.size ()
      || (int) rightW.at(decalR).size () == 0
      || (int) leftW.at(decalL).size () == 0)
  {
    cerr << "Pas assez de points pour l'initialisation" << endl;
    return isDetected;
  }

  Point2D pLeft (leftW.at(decalL).at(0).x (), leftW.at(decalL).at(0).y ());
  Point2D pRight (rightW.at(decalR).at(0).x (), rightW.at(decalR).at(0).y ());

  int pos = 0;
  while (isLeft (pCentral, pLeft, pRight) == 0
         && pos + decalL < (int) leftW.size ()
         && pos + decalR < (int) rightW.size ())
  {
    if ((int) leftW.at (pos + decalL).size () != 0
        && (int) rightW.at (pos + decalR).size () != 0)
    {
      pLeft = Point2D (leftW.at (decalL + pos).at(0).x (),
                       leftW.at (decalL + pos).at(0).y ());
      pRight = Point2D (rightW.at (decalR + pos).at(0).x (),
                        rightW.at (decalR + pos).at(0).y ());
    }
    pos ++;
  }

  if (isLeft (pCentral, pLeft, pRight) == 0)
  {
    cerr << "Trop peu de points" << endl;
    return isDetected;
  }

  segment = new BlurredSegment (pCentral, pLeft, pRight, maxWidth);
  isDetected = true;
  int nbVideLeft = 0;
  int nbVideRight = 0;
  vector< vector<Pixel> >::iterator iterRight = rightW.begin () + decalR + pos;
  vector< vector<Pixel> >::iterator iterLeft = leftW.begin () + decalL + pos;
    
  bool scanRightStopped = false;
  bool scanLeftStopped = false;

  while (! scanRightStopped || ! scanLeftStopped)
  {
    if (! scanRightStopped)
    {
      vector<Pixel> vectCandidats = *iterRight;
      vector<Pixel>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          Pixel pr = *iterCandidats;      
          added = segment->addPoint (Point2D (pr.x (), pr.y ()), RIGHT);
          if (added) nbVideRight = 0;
          else nbVideRight ++;
          iterCandidats ++;	  
        }
      }
      else nbVideRight ++;
    }
    if (! scanLeftStopped)
    {
      vector<Pixel> vectCandidats = *iterLeft;
      vector<Pixel>::iterator iterCandidats = vectCandidats.begin ();
      bool added = false;
      if (iterCandidats != vectCandidats.end ())
      {
        while (! added && iterCandidats != vectCandidats.end ())
        {
          Pixel pr = *iterCandidats;
          added = segment->addPoint (Point2D (pr.x (), pr.y ()), LEFT);
          if (added) nbVideLeft = 0;
          else nbVideLeft ++;
          iterCandidats ++;
        }      
      }
      else nbVideLeft ++;
    }
    iterRight ++;
    iterLeft ++;
    if (iterRight == rightW.end () || nbVideRight > acceptedLacks)
      scanRightStopped = true;
    if (iterLeft == leftW.end () || nbVideLeft > acceptedLacks)
      scanLeftStopped = true;
  }
  isDetected = true;
  return isDetected;
}



void SegmentDetector::setGradientThreshold (double value)
{
  gradientThreshold = value;
}


void SegmentDetector::setSegmentMaxWidth (double value)
{
  maxWidth = value;
}


void SegmentDetector::setAcceptedLacks (int number)
{
  acceptedLacks = number;
}


void SegmentDetector::setNumberOfSteps (int number)
{
  numberOfSteps = number;
}


void SegmentDetector::setImageData (ImageData *data)
{
  imageData = data;
}
