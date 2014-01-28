#include <cstdlib>
#include <iostream>
#include <QtGui>
#include "profile.h"
#include "linetool.h"   /*Tempo*/
#include "imagedata.h"  /*pour la recherche de min sur la correl*/

using namespace std;



Profile::Profile ()
{
  iter = 0;
  correlWidth = 7;
  correlThick = 3;

  widHeight = 610;

  profileWidth = 400;
  profileRatio = 3;
  profileLow = (256 - (widHeight / profileRatio)) / 2;
  gradientUnRatio = 40;
  gradientLow = 0;
  correlRatio = 3.0;
  correlMethod = 2;

  stripeWidth = 200;
  stripeMargin = 5;

  widWidth = profileWidth + stripeWidth + 2 * stripeMargin;
  image = NULL;
  grad = NULL;
  imageWidth = 0;
  imageHeight = 0;

  display = 0;
  resolScan = 8;
  resolLoop = 20;   // min 16
  resolStripe = 4;
  stripe = 0;
}


QRectF Profile::boundingRect () const
{
  return QRectF (0, 0, widWidth, widHeight);
}


void Profile::paint (QPainter *painter,
                     const QStyleOptionGraphicsItem *option, QWidget *widget)
{
  Q_UNUSED (option);
  Q_UNUSED (widget);


  if (display == 0) paintScans (painter, 0);
  else if (display == 1) paintScans (painter, 1);
  else if (display == -1) paintScans (painter, -1);
  else if (display == 2) paintFirstScans (painter);
  else if (display == 3)
  {
    paintStripes (painter);
    paintProfile (painter);
    painter->drawText (100, 20, QString ("INTENSITE"));
  }
  else if (display == 4)
  {
    paintStripes (painter);
    paintGradientProfile (painter);
    painter->drawText (100, 20, QString ("MAG. GRADIENT"));
  }
  else if (display == 5)
  {
    bool ok = false;
    if (correlMethod == 0) ok = setMeanCorrelationStripes ();
    else if (correlMethod == 1) ok = setFull2dCorrelationStripes ();
    else // correlMethod == 2
      ok = setMean2dCorrelationStripes ();
    if (ok)
    {
      paintStripes (painter);
      paintCorrelationProfile (painter);
    }
    painter->drawText (100, 20, QString ("B : Correl width = ")
                                + QString::number (correlWidth));
    painter->drawText (100, 40, QString ("V : Correl thick = ")
                                + QString::number (correlThick));
    if (correlMethod == 0)
      painter->drawText (100, 60, QString ("Y : Correl method = mean 1D"));
    else if (correlMethod == 1)
      painter->drawText (100, 60, QString ("Y : Correl method = full 2D"));
    else if (correlMethod == 2)
      painter->drawText (100, 60, QString ("Y : Correl method = mean 2D"));
  }
}


void Profile::setImage (QImage *image, Pixel **grad)
{
  this->image = image;
  this->grad = grad;
  this->imageWidth = image->width ();
  this->imageHeight = image->height ();
}


void Profile::clear ()
{
  rightscan.clear ();
  leftscan.clear ();
  rightCorrel.clear ();
  leftCorrel.clear ();
  rightReCorrel.clear ();
  leftReCorrel.clear ();
  stripe = 0;
}


void Profile::addCentral (Pixel p1, Pixel p2)
{
  this->pt1 = p1;
  this->pt2 = p2;
}


void Profile::addToRight (vector<Pixel> pts)
{
  rightscan.push_back (pts);
  minStripe = 1 - rightscan.size ();
}


void Profile::addToLeft (vector<Pixel> pts)
{
  leftscan.push_back (pts);
  maxStripe = leftscan.size () - 1;
}


void Profile::setDisplay (int disp)
{
  display = disp;
}


void Profile::incStripe (int inc)
{
  stripe += inc;
  if (stripe > maxStripe) stripe = maxStripe;
  else if (stripe < minStripe) stripe = minStripe;
}


void Profile::incCorrelWidth (int down)
{
  if (down)
  {
    if (correlWidth >= 5) correlWidth -= 2;
  }
  else if (correlWidth <= 19) correlWidth += 2;
}


void Profile::incCorrelThick (int down)
{
  if (down)
  {
    if (correlThick >= 3) correlThick -= 2;
  }
  else if (correlThick <= 17) correlThick += 2;
}


void Profile::incCorrelRatio (int down)
{
  if (down) correlRatio /= 2;
  else correlRatio *= 2;
}


void Profile::toggleMeanCorrel ()
{
  correlMethod = (correlMethod + 1) % 3;

  if (correlMethod == 1) correlRatio /= 4;       // Very empirical I assume...
  else if (correlMethod == 2) correlRatio *= 4;
}





/** First release :
  * Compares portions of scan bars with a profile extracted at
  * the first scan centered on the blurred segment.
  */
void Profile::setCorrelationStripes (Pixel p1, Pixel p2, int segwidth)
{
  leftCorrel.clear ();
  rightCorrel.clear ();
  leftReCorrel.clear ();
  rightReCorrel.clear ();
  int correlWidth = segwidth;
  if (correlWidth > (int) leftscan.at(0).size ())
  {
    cerr << "Can't get correlation stripes" << endl;
    return;
  }

  // Gets the central index
  Pixel pc ((p1.x () + p2.x ()) / 2, (p1.y () + p2.y ()) / 2);
  vector<Pixel>::iterator it = leftscan.at(0).begin ();
  int dist, pos = 0, minPos = 0;
  int minDist = (pc.x() - (*it).x()) * (pc.x() - (*it).x())
                + (pc.y() - (*it).y()) * (pc.y() - (*it).y());
  it ++;
  while (it != leftscan.at(0).end ())
  {
    dist = (pc.x() - (*it).x()) * (pc.x() - (*it).x())
           + (pc.y() - (*it).y()) * (pc.y() - (*it).y());
    if (dist < minDist)
    {
      minDist = dist;
      minPos = pos;
    }
    it ++;
    pos ++;
  }
  if (minPos - correlWidth / 2 < 0
      || minPos + correlWidth / 2 >= (int) leftscan.size ())
  {
    cerr << "Excentred stripe" << endl;
    return;
  }

  // Gets the central template
  int *centralShape = new int[correlWidth];
  for (int i = 0; i < correlWidth; i++)
  {
    Pixel pix = leftscan.at(0).at(minPos-correlWidth/2+i);
    centralShape[i] = ((QColor) image->pixel (QPoint (pix.x (),
                                  imageHeight - 1 - pix.y ()))).value ();
  }

  // Computes the left correlation stripes
  vector <vector <Pixel> >::iterator scit = leftscan.begin ();
  while (scit != leftscan.end ())
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((*scit).size ()) >= correlWidth)
    {
      for (int j = 0; j < ((int) (*scit).size ()) - correlWidth + 1; j++)
      {
        int val = 0;
        for (int k = 0; k < correlWidth; k++)
        {
          Pixel pix = (*scit).at(j+k);
          int diff = ((QColor) image->pixel (QPoint (pix.x (),
                                  imageHeight - 1 - pix.y ()))).value ()
                     - centralShape[k];
          val += (diff < 0 ? - diff : diff);
        }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    }
    leftCorrel.push_back (corr);
    leftReCorrel.push_back (recorr);
    scit ++;
  }

  // Computes the right correlation stripes
  scit = rightscan.begin ();
  while (scit != rightscan.end ())
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((*scit).size ()) >= correlWidth)
    {
      for (int j = 0; j < ((int) (*scit).size ()) - correlWidth + 1; j++)
      {
        int val = 0;
        for (int k = 0; k < correlWidth; k++)
        {
          Pixel pix = (*scit).at(j+k);
          int diff = ((QColor) image->pixel (QPoint (pix.x (),
                                  imageHeight - 1 - pix.y ()))).value ()
                     - centralShape[k];
          val += (diff < 0 ? - diff : diff);
        }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    }
    rightCorrel.push_back (corr);
    rightReCorrel.push_back (recorr);
    scit ++;
  }
}


/** Second release :
  * Compares portions of scan bars with a mean profile extracted at
  * the center of the first Nth scans (N = correlThick).
  */
bool Profile::setMeanCorrelationStripes ()
{
  leftCorrel.clear ();
  rightCorrel.clear ();
  leftReCorrel.clear ();
  rightReCorrel.clear ();
  int minPos = (int) leftscan.at(0).size () / 2;
  if (correlWidth > (int) leftscan.at(0).size ())
  {
    cerr << "Can't get correlation stripes" << endl;
    return false;
  }

  // Gets the central template
  int *centralShape = new int[correlWidth];
  for (int i = 0; i < correlWidth; i++)
  {
    centralShape[i] = 0;
    for (int j = 0; j < correlThick; j++)
    {
      Pixel pix = leftscan.at(j).at(minPos-correlWidth/2+i);
      centralShape[i] += ((QColor) image->pixel (QPoint (pix.x (),
                                    imageHeight - 1 - pix.y ()))).value ();
    }
    centralShape[i] /= correlThick;
  }

  // Computes the left correlation stripes
  vector <vector <Pixel> >::iterator scit = leftscan.begin ();
  while (scit != leftscan.end ())
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((*scit).size ()) >= correlWidth)
    {
      for (int j = 0; j < ((int) (*scit).size ()) - correlWidth + 1; j++)
      {
        int val = 0;
        for (int k = 0; k < correlWidth; k++)
        {
          Pixel pix = (*scit).at(j+k);
          int diff = ((QColor) image->pixel (QPoint (pix.x (),
                                  imageHeight - 1 - pix.y ()))).value ()
                     - centralShape[k];
          val += (diff < 0 ? - diff : diff);
        }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    }
    leftCorrel.push_back (corr);
    leftReCorrel.push_back (recorr);
    scit ++;
  }

  // Computes the right correlation stripes
  scit = rightscan.begin ();
  while (scit != rightscan.end ())
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((*scit).size ()) >= correlWidth)
    {
      for (int j = 0; j < ((int) (*scit).size ()) - correlWidth + 1; j++)
      {
        int val = 0;
        for (int k = 0; k < correlWidth; k++)
        {
          Pixel pix = (*scit).at(j+k);
          int diff = ((QColor) image->pixel (QPoint (pix.x (),
                                  imageHeight - 1 - pix.y ()))).value ()
                     - centralShape[k];
          val += (diff < 0 ? - diff : diff);
        }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    }
    rightCorrel.push_back (corr);
    rightReCorrel.push_back (recorr);
    scit ++;
  }
  return true;
}


/** Third release :
  * Compares 2D portions of scan bars (N = correlThick) with a 2D profile
  * extracted at the center of the first Nth scans (N = correlThick).
  */
bool Profile::setFull2dCorrelationStripes ()
{
  leftCorrel.clear ();
  rightCorrel.clear ();
  leftReCorrel.clear ();
  rightReCorrel.clear ();
  if ((int) leftscan.size () <= correlThick)
  {
    cerr << "Can't get correlation stripes : not enough left scans" << endl;
    return false;
  }
  int minPos = (int) leftscan.at(0).size () / 2;
  if (correlWidth > (int) leftscan.at(0).size ())
  {
    cerr << "Can't get correlation stripes : scans too narrow" << endl;
    return false;
  }

  // Gets the central template
  int *centralShape = new int[correlThick * correlWidth];
  for (int i = 0; i < correlWidth; i++)
    for (int j = 0; j < correlThick; j++)
    {
      Pixel pix = leftscan.at(j).at(minPos-correlWidth/2+i);
      centralShape[j * correlWidth + i] = ((QColor) image->pixel (
               QPoint (pix.x (), imageHeight - 1 - pix.y ()))).value ();
    }

  // Computes the left correlation stripes
  for (int i = 0; i < (int) leftscan.size () - correlThick - 2; i++)
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((leftscan.at(i)).size ()) >= correlWidth)
      for (int j = 0; j < ((int) (leftscan.at(i)).size ()) - correlWidth; j++)
      {
        int val = 0;
        for (int cw = 0; cw < correlThick; cw ++)
          for (int k = 0; k < correlWidth; k++)
            if ((int) leftscan.at(i+cw).size () > j+k)
            {
              Pixel pix = (leftscan.at(i+cw)).at(j+k);
              int diff = ((QColor) image->pixel (QPoint (pix.x (),
                                    imageHeight - 1 - pix.y ()))).value ()
                       - centralShape[cw * correlWidth + k];
              val += (diff < 0 ? - diff : diff);
            }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    leftCorrel.push_back (corr);
    leftReCorrel.push_back (recorr);
  }

  // Computes the right correlation stripes
  for (int i = correlThick - 1; i < (int) rightscan.size () - 1; i++)
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((rightscan.at(i)).size ()) >= correlWidth)
    {
      for (int j = 0; j < ((int) (rightscan.at(i)).size ()) - correlWidth; j++)
      {
        int val = 0;
        for (int cw = 0; cw < correlThick; cw ++)
          for (int k = 0; k < correlWidth; k++)
            if ((int) rightscan.at(i-cw).size () > j+k)
            {
              Pixel pix = (rightscan.at(i-cw)).at(j+k);
              int diff = ((QColor) image->pixel (QPoint (pix.x (),
                                  imageHeight - 1 - pix.y ()))).value ()
                     - centralShape[cw * correlWidth + k];
              val += (diff < 0 ? - diff : diff);
            }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    }
    rightCorrel.push_back (corr);
    rightReCorrel.push_back (recorr);
  }
  return true;
}




/** Fourth release :
  * Compares mean portions of scan bars (N = correlThick) with a mean
  * profile extracted at the center of the first Nth scans (N = correlThick).
  */
bool Profile::setMean2dCorrelationStripes ()
{
  leftCorrel.clear ();
  rightCorrel.clear ();
  leftReCorrel.clear ();
  rightReCorrel.clear ();
  if ((int) leftscan.size () <= correlThick)
  {
    cerr << "Can't get correlation stripes : not enough left scans" << endl;
    return false;
  }
  int minPos = (int) leftscan.at(0).size () / 2;
  if (correlWidth > (int) leftscan.at(0).size ())
  {
    cerr << "Can't get correlation stripes : scans too narrow" << endl;
    return false;
  }

  // Gets the central template
  int *centralShape = new int[correlWidth];
  for (int i = 0; i < correlWidth; i++)
  {
    centralShape[i] = 0;
    for (int j = 0; j < correlThick; j++)
    {
      Pixel pix = leftscan.at(j).at(minPos-correlWidth/2+i);
      centralShape[i] += ((QColor) image->pixel (
               QPoint (pix.x (), imageHeight - 1 - pix.y ()))).value ();
    }
    centralShape[i] /= correlThick;
  }

  // Computes the left correlation stripes
  for (int i = 0; i < (int) leftscan.size () - correlThick - 2; i++)
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((leftscan.at(i)).size ()) >= correlWidth)
      for (int j = 0; j < ((int) (leftscan.at(i)).size ()) - correlWidth; j++)
      {
        int val = 0;
        for (int k = 0; k < correlWidth; k++)
        {
          int nbval = 0;
          int locval = 0;
          for (int cw = 0; cw < correlThick; cw ++)
          {
            if ((int) leftscan.at(i+cw).size () > j+k)
            {
              nbval ++;
              Pixel pix = (leftscan.at(i+cw)).at(j+k);
              locval += ((QColor) image->pixel (QPoint (pix.x (),
                                    imageHeight - 1 - pix.y ()))).value ();
            }
          }
          int diff = locval / nbval - centralShape[k];
          val += (diff < 0 ? - diff : diff);
        }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    leftCorrel.push_back (corr);
    leftReCorrel.push_back (recorr);
  }

  // Computes the right correlation stripes
  for (int i = correlThick - 1; i < (int) rightscan.size () - 1; i++)
  {
    vector<int> corr;
    vector<long> recorr;
    if ((int) ((rightscan.at(i)).size ()) >= correlWidth)
    {
      for (int j = 0; j < ((int) (rightscan.at(i)).size ()) - correlWidth; j++)
      {
        int val = 0;
        for (int k = 0; k < correlWidth; k++)
        {
          int nbval = 0;
          int locval = 0;
          for (int cw = 0; cw < correlThick; cw ++)
          {
            if ((int) rightscan.at(i-cw).size () > j+k)
            {
              nbval ++;
              Pixel pix = (rightscan.at(i-cw)).at(j+k);
              locval += ((QColor) image->pixel (QPoint (pix.x (),
                                  imageHeight - 1 - pix.y ()))).value ();
            }
          }
          int diff = locval / nbval - centralShape[k];
          val += (diff < 0 ? - diff : diff);
        }
        corr.push_back (val);
        recorr.push_back ((long) val);
      }
    }
    rightCorrel.push_back (corr);
    rightReCorrel.push_back (recorr);
  }
  return true;
}





void Profile::paintFirstScans (QPainter *painter)
{
  if (! rightscan.empty ())
  {
    vector <Pixel> sc = rightscan.at (0);
    int widi = sc.size ();
    int cx = (sc.at (widi / 2)).x ();
    int cy = (sc.at (widi / 2)).y ();
    vector<Pixel>::iterator it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x, y, resolLoop, resolLoop, QBrush (Qt::blue));
      it ++;
    }
    sc = rightscan.at (1);
    it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x + 1, y + 1, resolLoop - 2, resolLoop - 2,
                         QBrush (Qt::green));
      it ++;
    }
    sc = rightscan.at (2);
    it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x + 2, y + 2, resolLoop - 4, resolLoop - 4,
                         QBrush (Qt::red));
      it ++;
    }
    sc = rightscan.at (3);
    it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x + 3, y + 3, resolLoop - 6, resolLoop - 6,
                         QBrush (Qt::blue));
      it ++;
    }

    sc = leftscan.at (0);
    it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x + 4, y + 4, resolLoop - 8, resolLoop - 8,
                         QBrush (Qt::black));
      it ++;
    }
    sc = leftscan.at (1);
    it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x + 5, y + 5, resolLoop - 10, resolLoop - 10,
                         QBrush (Qt::black));
      it ++;
    }
    sc = leftscan.at (2);
    it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x + 6, y + 6, resolLoop - 12, resolLoop - 12, QBrush (Qt::black));
      it ++;
    }
    sc = leftscan.at (3);
    it = sc.begin ();
    while (it != sc.end ())
    {
      int x = ((*it).x() - cx) * resolLoop + widWidth / 2;
      int y = ((*it).y() - cy) * resolLoop + widHeight / 2;
      painter->fillRect (x + 7, y + 7, resolLoop - 14, resolLoop - 12, QBrush (Qt::black));
      it ++;
    }
  }
}


void Profile::paintScans (QPainter *painter, int val)
{
  QColor cols[] = {Qt::green, Qt::red, Qt::blue};
  int colnum = 0;
  Pixel p1, p2;
  if (val == 0) iter = 0;
  else if (val == 1) iter ++;
  else if (val == -1) iter --;

  int first = 1, cx = 0, cy = 0;
  vector <vector <Pixel> >::iterator bigit = rightscan.begin ();
  while (bigit != rightscan.end ())
  {
    vector<Pixel> scan = *bigit;
    int widi = scan.size ();
    if (widi != 0)
    {
      if (first)
      {
        cx = (scan.at (widi / 2)).x ();
        cy = (scan.at (widi / 2)).y ();
        first = 0;
      }

      vector<Pixel>::iterator it = scan.begin ();
      while (it != scan.end ())
      {
        int x = ((*it).x() - cx) * resolScan + 300;
        int y = ((*it).y() - cy) * resolScan + 300;
        if (x < 610 - resolScan && x >= 0 && y < 610 - resolScan && y >= 0)
          painter->fillRect (x, y, resolScan, resolScan, QBrush (cols[colnum%3]));
        it ++;
      }
    }
    colnum++;
    bigit ++;
  }

  colnum = 0;
  bigit = leftscan.begin ();
  while (bigit != leftscan.end ())
  {
    vector<Pixel> scan = *bigit;
    int widi = scan.size ();
    if (widi != 0)
    {
      vector<Pixel>::iterator it = scan.begin ();
      while (it != scan.end ())
      {
        int x = ((*it).x() - cx) * resolScan + 300;
        int y = ((*it).y() - cy) * resolScan + 300;
        if (x < 610 - resolScan && x >= 0 && y < 610 - resolScan && y >= 0)
          //painter->fillRect (x + 1, y + 1, resolScan - 2, resolScan - 2,
          painter->fillRect (x, y, resolScan, resolScan,
                             QBrush (cols[colnum%3]));
        it ++;
      }
    }
    colnum++;
    bigit ++;
  }

  vector<Pixel> pts = LineTool::draw (pt1, pt2, iter);
  vector<Pixel>::iterator it = pts.begin ();
  while (it != pts.end ())
  {
    int x = ((*it).x() - cx) * resolScan + 300;
    int y = ((*it).y() - cy) * resolScan + 300;
    painter->fillRect (x+2, y+2, resolScan-4, resolScan-4, QBrush (Qt::black));
    it ++;
  }
}


void Profile::paintStripes (QPainter *painter)
{
  int median = rightscan.at(0).size () / 2;
  int lx = profileWidth + stripeMargin + stripeWidth / 2
           - resolStripe / 2 - median * resolStripe;
  int cx, cy = widHeight / 2;

  painter->setPen (QPen (Qt::red, 2));
  painter->drawRect (profileWidth + stripeMargin - 1, stripeMargin - 1,
                     stripeWidth + 2, widHeight - 2 * stripeMargin + 2);

  vector <vector <Pixel> >::iterator bigit = rightscan.begin ();
  while (cy <= widWidth - stripeMargin - resolStripe
         && bigit != rightscan.end ())
  {
    cx = lx;
    vector<Pixel> scan = *bigit;
    vector<Pixel>::iterator it = scan.begin ();
    while (cx < widWidth - stripeMargin - resolStripe && it != scan.end ())
    {
      if (cx >= profileWidth + stripeMargin)
        painter->fillRect (cx, cy, resolStripe, resolStripe,
             QBrush (image->pixel ((*it).x (), imageHeight - 1 - (*it).y ())));
      it ++;
      cx += resolStripe;
    }
    bigit ++;
    cy += resolStripe;
  }

  cy = widHeight / 2 - 2 * resolStripe;
  bigit = leftscan.begin ();
  while (cy >= 5 && bigit != leftscan.end ())
  {
    cx = lx;
    vector<Pixel> scan = *bigit;
    vector<Pixel>::iterator it = scan.begin ();
    while (cx < widHeight - stripeMargin - resolStripe && it != scan.end ())
    {
      if (cx >= profileWidth + stripeMargin)
        painter->fillRect (cx, cy, resolStripe, resolStripe,
             QBrush (image->pixel ((*it).x (), imageHeight - 1 - (*it).y ())));
      it ++;
      cx += resolStripe;
    }
    bigit ++;
    cy -= resolStripe;
  }

  if (stripe >= 0)
    cy = widHeight / 2 - 1 - 2 * resolStripe - stripe * resolStripe;
  else cy = widHeight / 2 - 1 - (1 + stripe) * resolStripe;
  painter->setPen (QPen (Qt::green, 2));
  painter->drawRect (profileWidth + stripeMargin - 1, cy,
                     stripeWidth + 2, resolStripe + 2);
}


void Profile::paintProfile (QPainter *painter)
{
  if (rightscan.size () || leftscan.size ())
  {
    vector<Pixel> scan;
    if (stripe >= 0) scan = leftscan.at (stripe);
    else scan = rightscan.at (- stripe - 1);
    int h, cx = 0, w = profileWidth / scan.size ();
    vector<Pixel>::iterator it = scan.begin ();
    while (it != scan.end ())
    {
      if ((*it).x () < 0 || (*it).x () >= imageWidth
          || (*it).y () < 0 || (*it).y () >= imageHeight)
        cerr << "OUT OF IMAGE BOUNDS : (" << (*it).x () << ","
             << imageHeight - 1 - (*it).y () << ")" << endl;
      else
      {
        h = ((QColor) image->pixel (QPoint ((*it).x (),
                                    imageHeight - 1 - (*it).y ()))).value ();
        h = (h - profileLow) * profileRatio;
        if (h < 0) h = 0;
        else if (h > widHeight) h = widHeight;
        if (h) painter->fillRect (cx, widHeight - h, w, h, QBrush (Qt::blue));
        it ++;
        cx += w;
      }
    }

    painter->setPen (QPen (Qt::black, 2));
    scan = leftscan.at (0);
    it = scan.begin ();
    int prevh = ((QColor) image->pixel (QPoint ((*it).x (),
                                  imageHeight - 1 - (*it).y ()))).value ();
    prevh = (prevh - profileLow) * profileRatio;
    if (prevh < 0) prevh = 0;
    else if (prevh > widWidth) prevh = widWidth;
    cx = 0;
    while (it != scan.end ())
    {
      h = ((QColor) image->pixel (QPoint ((*it).x (),
                                  imageHeight - 1 - (*it).y ()))).value ();
      h = (h - profileLow) * profileRatio;
      if (h < 0) h = 0;
      else if (h > widWidth) h = widWidth;
      painter->drawLine (cx, widWidth - prevh, cx, widWidth - h);
      painter->drawLine (cx, widWidth - h, cx + w, widWidth - h);
      prevh = h;
      it ++;
      cx += w;
    }
  }
}


void Profile::paintGradientProfile (QPainter *painter)
{
  if (rightscan.size () || leftscan.size ())
  {
    Pixel gr;
    vector<Pixel> scan;
    if (stripe >= 0) scan = leftscan.at (stripe);
    else scan = rightscan.at (- stripe - 1);
    int h, cx = 0, w = profileWidth / scan.size ();
    vector<Pixel>::iterator it = scan.begin ();
    while (it != scan.end ())
    {
      if ((*it).x () < 0 || (*it).x () >= imageWidth
          || (*it).y () < 0 || (*it).y () >= imageHeight)
        cerr << "OUT OF IMAGE BOUNDS : (" << (*it).x () << ","
             << imageHeight - 1 - (*it).y () << ")" << endl;
      else
      {
        gr = grad[(*it).y ()][(*it).x ()];
        h = (int) (gr.x () * gr.x () + gr.y () * gr.y ());
        h = (h - gradientLow) / gradientUnRatio;
        if (h < 0) h = 0;
        else if (h > widHeight) h = widHeight;
        if (h) painter->fillRect (cx, widHeight - h, w, h, QBrush (Qt::blue));
        it ++;
        cx += w;
      }
    }

    painter->setPen (QPen (Qt::black, 2));
    scan = leftscan.at (0);
    it = scan.begin ();
    gr = grad[(*it).y ()][(*it).x ()];
    int prevh = (int) (gr.x () * gr.x () + gr.y () * gr.y ());
    prevh = (prevh - gradientLow) / gradientUnRatio;
    if (prevh < 0) prevh = 0;
    else if (prevh > widWidth) prevh = widWidth;
    cx = 0;
    while (it != scan.end ())
    {
      gr = grad[(*it).y ()][(*it).x ()];
      h = (int) (gr.x () * gr.x () + gr.y () * gr.y ());
      h = (h - gradientLow) / gradientUnRatio;
      if (h < 0) h = 0;
      else if (h > widWidth) h = widWidth;
      painter->drawLine (cx, widWidth - prevh, cx, widWidth - h);
      painter->drawLine (cx, widWidth - h, cx + w, widWidth - h);
      prevh = h;
      it ++;
      cx += w;
    }
  }
}



// stripe
// RightCorrel
// LeftCorrel

void Profile::paintCorrelationProfile (QPainter *painter)
{
  if (rightCorrel.size () || leftCorrel.size ())
  {
    Pixel gr;
    vector<int> scan;
    vector<long> rescan;
    if (stripe >= 0)
    {
      scan = leftCorrel.at (stripe);
      rescan = leftReCorrel.at (stripe);
    }
    else
    {
      scan = rightCorrel.at (- stripe - 1);
      rescan = rightReCorrel.at (- stripe - 1);
    }

    int h, cx = 0, w = profileWidth / scan.size ();
    vector<int>::iterator it = scan.begin ();
    while (it != scan.end ())
    {
      h = (int) ((*it) * correlRatio + 0.5);
      if (h > widHeight) h = widHeight;
      if (h) painter->fillRect (cx, widHeight - h, w, h, QBrush (Qt::blue));
      it ++;
      cx += w;
    }

    ImageData *idata = new ImageData (1, 1);
    vector<int> locs = idata->getLocalMinimaIndices (rescan);
    delete idata;
    it = locs.begin ();
    while (it != locs.end ())
    {
      h = (int) (scan.at(*it) * correlRatio + 0.5);
      painter->fillRect ((*it) * w + 4, widHeight - h - 14, w - 8, 10,
                         QBrush (Qt::red));
      it ++;
    }

    painter->setPen (QPen (Qt::black, 2));
    scan = leftCorrel.at (0);
    it = scan.begin ();
    int prevh = (int) ((*it) * correlRatio + 0.5);
    if (prevh > widWidth) prevh = widWidth;
    cx = 0;
    while (it != scan.end ())
    {
      h = (int) ((*it) * correlRatio + 0.5);
      if (h > widWidth) h = widWidth;
      painter->drawLine (cx, widWidth - prevh, cx, widWidth - h);
      painter->drawLine (cx, widWidth - h, cx + w, widWidth - h);
      prevh = h;
      it ++;
      cx += w;
    }
  }
}

