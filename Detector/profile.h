#ifndef PROFILE_H
#define PROFILE_H

#include <QGraphicsItem>
#include <QImage>
#include "directionalscan.h"


class Profile : public QGraphicsItem
{

public:

  Profile ();
  QRectF boundingRect () const;
  void paint (QPainter *painter,
              const QStyleOptionGraphicsItem *option, QWidget *widget);

  void setImage (QImage *image, QPoint **grad);
  void clear ();
  void addCentral (QPoint p1, QPoint p2);
  void addToRight (vector<QPoint> pts);
  void addToLeft (vector<QPoint> pts);
  void setDisplay (int disp);
  void incStripe (int inc);
  void incCorrelWidth (int down);
  void incCorrelThick (int down);
  void incCorrelRatio (int down);
  void toggleMeanCorrel ();


protected:

private:
  int iter;
  int widHeight;       // Profile analysis widget height
  int widWidth;        // Profile analysis widget width

  int profileWidth;    // Profile area width
  int profileRatio;    // Intensity profile vertical zoom factor
  int profileLow;      // Intensity profile lowest visible value

  int gradientUnRatio; // Gradient profile vertical unzoom factor
  int gradientLow;     // Gradient profile lowest visible value

  double correlRatio;  // Correlation profile vertical zoom factor
  int correlWidth;     // Correlation profile width
  int correlThick;     // Correlation profile scan thickness
  int correlMethod;    // Correlation computation used (mean or not, 1D or 2D)

  int stripeWidth;     // Stripe area width
  int stripeMargin;    // Stripe area margin width

  QImage *image;
  QPoint **grad;
  int imageWidth, imageHeight;
  QPoint pt1, pt2;
  vector <vector <QPoint> > leftscan;
  vector <vector <QPoint> > rightscan;

  vector <vector <int> > leftCorrel;
  vector <vector <int> > rightCorrel;
  vector <vector <double> > leftReCorrel;    // Just for local minima extraction
  vector <vector <double> > rightReCorrel;

  int display;         // Current display (all scans, first scans or profiles)
  int resolScan;       // All scans points zoom factor
  int resolLoop;       // First scans points zoom factor
  int resolStripe;     // Stripe points zoom factor

  int stripe;          // Current stripe index
  int minStripe;       // Min stripe index (right scan size)
  int maxStripe;       // Max stripe index (left scan size)

  void setCorrelationStripes (QPoint p1, QPoint p2, int segwidth);
  void setMeanCorrelationStripes ();
  void setFull2dCorrelationStripes ();
  void setMean2dCorrelationStripes ();

  void paintFirstScans (QPainter *painter);
  void paintScans (QPainter *painter, int val);
  void paintStripes (QPainter *painter);
  void paintProfile (QPainter *painter);
  void paintGradientProfile (QPainter *painter);
  void paintCorrelationProfile (QPainter *painter);
};

#endif
