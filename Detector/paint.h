#ifndef PAINT_H
#define PAINT_H

#include <QGraphicsView>
#include <fstream>
#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>
#include <QVector>
#include "directionalscan.h"
#include "linedetection.h"
#include "profiler.h"
#include <vector>

using namespace std;


class Paint : public QWidget 
{
  Q_OBJECT

public:

  struct SegmentsNivGris
  {
    vector<QPoint> vectPoints;
    QPoint p1;
    QPoint p2;
  };


  Paint (QWidget *parent = 0);

  QSize openImage (const QString &fileName);
  bool saveImage (const QString &fileName, const char *fileFormat);
  void setProfiler (Profiler *prof);
  void setPenColor (const QColor &newColor);
  void setPenWidth (int newWidth);

  bool isModified () const { return modified; }
  QColor penColor () const { return myPenColor; }
  int penWidth () const { return myPenWidth; }
  
  void paint (QPainter *painter,
	      const QStyleOptionGraphicsItem *option,
	      QWidget *widget);

  void drawVectorsFieldFromPoints (vector<QPoint> vectPoints,
				   QColor color, QPainter &painter);
  void drawListPixels (vector<QPoint> vectPixels,
                       QColor color, QPainter &painter);


public slots:
    void clearImage ();


protected:
  void mousePressEvent (QMouseEvent *event);
  void mouseReleaseEvent (QMouseEvent *event);
  void mouseMoveEvent (QMouseEvent *event);
  void paintEvent (QPaintEvent *event);
  void keyPressEvent (QKeyEvent *event);


private:
  void afficheVectSegRecognized (vector<SegmentsNivGris> vectSegRecognized);
    
  vector<SegmentsNivGris> vectSegRecognized;
  void drawGradientMaxDirectionnal ();
  void drawVectorsField (vector<QPoint> *vectGradient,
                         QColor color, QPainter &painter);
  void drawVectorsField (vector<QPoint *> vectPixGrad,
                         QColor color, QPainter &painter);

  int getPixValue (const QImage & image, const QPoint p);
  int **buildImageArray (const QImage &image);
  void updateTabGradUsed (vector<QPoint> vPoints);
  void resetTabGradientUsed ();

  void drawIntLinesDist ();
  void drawIntLines ();
  void drawIntLinesDistLongueur ();
  void afficheLineDetection ();
 

  bool isCurrentSegmentNivGrisDefined;
  SegmentsNivGris segNivGris;

  bool modified;
  bool scribbling;
  int myPenWidth;
  QColor myPenColor;
  QImage image;
  QSize sizeNormal;
  QImage imageOrigin;

  QPoint ** tabGradientNormale;
  bool **   tabGradientUsed;
 
  int ** tabImage;
  int largeur;
  int hauteur;

  int numberSolution;
  int numberStep;
  int method;

  /** Initial scan end points */
  QPoint p1;
  QPoint p2;
 
  DirectionalScan dirScan;
  bool p1defined;

  /** Segment extraction parameters */
  int epaisseurDetection;
  int epaisseurSensitivity;
  int nbPixelInteruption;
  int interSensitivity;

  /** User interface parameters */
  bool afficheGradientMax;
  bool afficheCandSel;
  bool afficheSegmentFlou;
  double seuil;

  bool modeManuel;
  bool modeGestuel;
  vector<QPoint> *geste;

  Profiler *prof;
};
#endif
