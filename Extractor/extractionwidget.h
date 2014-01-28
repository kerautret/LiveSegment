#ifndef PAINT_H
#define PAINT_H

#include <QGraphicsView>
#include <QColor>
#include <QImage>
#include <QWidget>
#include <QVector>
#include <fstream>
#include <vector>
#include "imagedata.h"
#include "directionalscan.h"
#include "segmentdetector.h"
#include "profiler.h"
#include "pixel.h"

using namespace std;


/** 
 * @class ExtractionWidget extractionwidget.h
 * \brief Segment extraction view and controller.
 * \author {B. Kerautret and P. Even}
 */
class ExtractionWidget : public QWidget 
{
  Q_OBJECT

public:

  /**
   * Creates a segment extraction widget.
   */
  ExtractionWidget (QWidget *parent = 0);
  
  /**
   * Qt
   */
  void paint (QPainter *painter,
	      const QStyleOptionGraphicsItem *option,
	      QWidget *widget);

  QSize openImage (const QString &fileName);
  bool saveImage (const QString &fileName, const char *fileFormat);
  void setProfiler (Profiler *prof);

  /**
   * Returns the pen width. 
   */
  int penWidth () const { return myPenWidth; }

  /**
   * Sets the pen width.
   */
  void setPenWidth (int newWidth);

  /**
   * Returns the pen color. 
   */
  QColor penColor () const { return myPenColor; }

  /**
   * Sets the pen color.
   */
  void setPenColor (const QColor &newColor);

  void drawVectorsFieldFromPoints (vector<Pixel> vectPoints,
				   QColor color, QPainter &painter);
  void drawListPixels (vector<Pixel> vectPixels,
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


    
  void drawVectorsField (vector<Pixel> *vectGradient,
                         QColor color, QPainter &painter);
  void drawVectorsField (vector<Pixel *> vectPixGrad,
                         QColor color, QPainter &painter);

  int getPixValue (const QImage & image, const Pixel p);
  int **buildImageArray (const QImage &image);

  void afficheLineDetection ();

  void drawStraightLine (Pixel p1, Pixel p2, QColor col, QPainter & painter);
 


 
  // Segment extraction parameters
  /** Segment max width input. */
  int segmentMaxWidth;
  /** Accepted detection lacks input. */
  int detectionLacks;
  /** Number of required steps of the detection algorithm (for debug). */
  int numberOfDetectionSteps;    // default : 0

  /** Default value for gradient threshold. */
  static const double DEFAULT_GRADIENT_THRESHOLD = 30.0;
  /** Default value for segment max width parameter. */
  static const int DEFAULT_MAX_WIDTH = 5;
  /** Sensitiveness of segment max width parameter. */
  static const int MAX_WIDTH_TUNING = 400;
  /** Default value for accepted number of detection lacks. */
  static const int DEFAULT_DETECTION_LACKS = 5;
  /** Sensitiveness of detection lacks parameter. */
  static const int DETECTION_LACKS_TUNING = 20;
  /** Default number of detection method steps (for debug). */
  static const int DEFAULT_NUMBER_OF_DETECTION_STEPS = 0; // means nominal

  // Interaction modes
  /** Interaction mode : live or manual. */
  bool manualModeOn;
  /** Gesture analyzing in manual mode. */
  bool gestureModeOn;
  /** Gesture track. */
  vector<Pixel> *gesture;
  /** Scan action running. */
  bool scanOn;
  /** Initial scan end points */
  Pixel p1, p2;

  // User interface parameters
  /** Flag used to enable the display of the blurred segment support lines. */
  bool supportLinesDisplayOn;
  /** Pen width. */
  int myPenWidth;
  /** Pen color. */
  QColor myPenColor;

  /** Default value for pen width. */
  static const int DEFAULT_PEN_WIDTH = 1;


  /** Presently loaded image. */
  QImage loadedImage;
  /** Present image augmented with processed data. */
  QImage augmentedImage;
  /** Information about the presently loaded image. */
  ImageData * imageData;
  /** Width of the present image. */
  int width;
  /** Height of the present image. */
  int height;
  /** Blurred segment detector. */
  SegmentDetector detector;
  /** View for analyzing extracted information. */
  Profiler *prof;

  /** Aggregation of segment detection result with initial conditions. */
  struct DetectedSegment
  {
    /** Blurred segment detected. */
    vector<Pixel> vectPoints;
    /** First scan end. */
    Pixel p1;
    /** Second scan end. */
    Pixel p2;
  };

  /** Last detected segment. */
  DetectedSegment lastDetectedSegment;
  /** List of detected segments. */
  vector<DetectedSegment> detectedSegments;




  void afficheVectSegRecognized (vector<DetectedSegment> segments);
};
#endif
