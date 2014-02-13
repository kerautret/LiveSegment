#include <QtGui>
#include <iostream>
#include <cstdlib>
#include "math.h"
#include "extractionwidget.h"
#include "segmentdetector.h"
#include "linetool.h"
#include "blurredsegment.h"

using namespace std;



ExtractionWidget::ExtractionWidget (QWidget *parent)
{
  Q_UNUSED (parent);

  // Set default parameter values
  segmentMaxWidth = DEFAULT_MAX_WIDTH;
  detectionLacks = DEFAULT_DETECTION_LACKS;
  numberOfDetectionSteps = DEFAULT_NUMBER_OF_DETECTION_STEPS;

  // Set default user interface modalities
  supportLinesDisplayOn = true;
  manualModeOn = false;
  gestureModeOn = false;
  scanOn = false;

  // Set default user interface parameters
  setFocus ();
  grabKeyboard ();
  myPenWidth = DEFAULT_PEN_WIDTH;
  myPenColor = Qt::blue;
  gesture = NULL;
  prof = NULL;

  // Set initial values for the segment detector
  detector.setGradientThreshold (DEFAULT_GRADIENT_THRESHOLD);
  detector.setSegmentMaxWidth (segmentMaxWidth);
  detector.setAcceptedLacks (detectionLacks);
  detector.setNumberOfSteps (numberOfDetectionSteps);

  imageData = NULL;
}


QSize ExtractionWidget::openImage (const QString &fileName)
{
  QSize newSize (0, 0);
  loadedImage.load (fileName);
  this->width = loadedImage.width ();
  this->height = loadedImage.height ();
  newSize = loadedImage.size ();
 
  augmentedImage = loadedImage;
  int **tabImage = buildImageArray (augmentedImage);
 
  if (imageData != NULL) delete imageData;
  imageData = new ImageData (width, height);

  Pixel **tabGradientNormale
              = imageData->getVectorsGradientRepereNornal (tabImage);
  imageData->buildGradientMap (tabImage);
  detector.setImageData (imageData);
 
  update ();

  QImage imagePro = loadedImage;
  if (prof) prof->profile()->setImage (&imagePro, tabGradientNormale);
 
  return newSize;
}


bool ExtractionWidget::saveImage (const QString &fileName,
                                  const char *fileFormat)
{
  QImage visibleImage = augmentedImage;
  return (visibleImage.save (fileName, fileFormat));
}


void ExtractionWidget::setProfiler (Profiler *prof)
{
  this->prof = prof;
}


void ExtractionWidget::setPenColor (const QColor &newColor)
{
  myPenColor = newColor;
}


void ExtractionWidget::setPenWidth (int newWidth)
{
  myPenWidth = newWidth;
}


void ExtractionWidget::clearImage ()
{
  augmentedImage.fill (qRgb (255, 255, 255));
  update ();
}


void ExtractionWidget::mousePressEvent (QMouseEvent *event)
{
  if ((event->buttons () & Qt::RightButton))
  {
    this->p1 = Pixel (event->pos().x (), height - 1 - event->pos().y());
    if (width > p1.x () && height > p1.y ())
    {
      scanOn = true;
//      cerr << "p1 defined: " << p1.x () << " " << p1.y () << endl;
    }
    if (gestureModeOn)
    {
      if (gesture != NULL) delete gesture;
      gesture = new vector<Pixel> ();
    }
  }
}


void ExtractionWidget::mouseReleaseEvent (QMouseEvent *event)
{
  if (gestureModeOn)
  {
    cout << "Geste : " << gestureModeOn << endl;
    this->p2 = Pixel (event->pos().x (), height - 1 - event->pos().y());
    if ((abs (p1.x() - p2.x()) > 2 && abs (p1.y() - p2.y()) > 2)
        && (width > p2.x() && height > p2.y()
            && p2.x() > 0 && p2.y() > 0))
    {
//      cerr << "p1 defined: " << p1.x() << " "<< p1.y() << endl;
//      cerr << "p2 defined: " << p2.x() << " "<< p2.y() << endl;
      numberOfDetectionSteps = 0;
      int dx = p2.x() - p1.x();
      int dy = p2.y() - p1.y();
      int l2 = dx * dx + dy * dy;
      int ml2 = 0;
      segmentMaxWidth = 1 + l2 / MAX_WIDTH_TUNING;
      vector<Pixel>::iterator it = gesture->begin ();
      while (it != gesture->end ())
      {
        Pixel pt = *it;
        int px = pt.x() - p1.x();
        int py = pt.y() - p1.y();
        int ll2 = (px * dy - py * dx) * (px * dy - py * dx) / l2;
        if (ll2 > ml2) ml2 = ll2;
        it ++;
      }
      detectionLacks = 0;
      if (ml2 > 20) detectionLacks = ml2 / DETECTION_LACKS_TUNING;
cerr << "Inter = " << detectionLacks << endl;
      detector.setAcceptedLacks (detectionLacks);
      detector.setNumberOfSteps (numberOfDetectionSteps);

      afficheLineDetection ();
    }
  }
}


void ExtractionWidget::mouseMoveEvent (QMouseEvent *event)
{
  if (scanOn)
  {
    this->p2 = Pixel (event->pos().x (), height - 1 - event->pos().y ());
    if (p1.manhattan (p2) > 5
        && (width > p2.x() && height > p2.y()
            && p2.x() > 0 && p2.y() > 0))
    {
      if (gestureModeOn)
      {
        augmentedImage = loadedImage;
        QPainter painter (&augmentedImage);
        gesture->push_back (p2);
        drawListPixels (*gesture, Qt::green, painter);
        update (QRect (QPoint (0, 0), QPoint (width, height)));
      }
      else
      {
        //cerr << "p1 defined: " << p1.x() << " "<< p1.y() << endl;
        //cerr << "p2 defined: " << p2.x() << " "<< p2.y() << endl;
        numberOfDetectionSteps = 0;
        detector.setNumberOfSteps (numberOfDetectionSteps);

        afficheLineDetection ();
      }
    }
  }
}


void ExtractionWidget::keyPressEvent (QKeyEvent *event)
{
  if (event->key () == Qt::Key_A)
  {
    supportLinesDisplayOn = ! supportLinesDisplayOn;
  }

  if (event->key () == Qt::Key_U)
  {
    afficheLineDetection ();
  }

  if (event->key () == Qt::Key_R)
  {          
    if (event->modifiers () & Qt::ShiftModifier) numberOfDetectionSteps = 0;
    if (++numberOfDetectionSteps == 3) numberOfDetectionSteps = 0;
    cerr << "Phase " << numberOfDetectionSteps << endl;
    detector.setNumberOfSteps (numberOfDetectionSteps);
    afficheLineDetection ();
  }

  if (event->key () == Qt::Key_Z)
  {
    if (event->modifiers () & Qt::ShiftModifier) detectionLacks --;
    else detectionLacks ++;
    update ();
    cerr << "Nouveau nb pixel interuption = " << detectionLacks << endl;
    detector.setAcceptedLacks (detectionLacks);
  }

  if (event->key () == Qt::Key_E)
  {
    if (event->modifiers () & Qt::ShiftModifier) segmentMaxWidth --;
    else segmentMaxWidth ++;
    cerr << "Nouvelle epaisseur = " << segmentMaxWidth << endl;
    detector.setSegmentMaxWidth (segmentMaxWidth);
  }

/*
  if (event->key () == Qt::Key_D)
  {
    if (prof)
    {
      if (event->modifiers () & Qt::ControlModifier)
        prof->profile()->setDisplay (0);
      else prof->profile()->setDisplay (
        (event->modifiers () & Qt::ShiftModifier) ? -1 : 1);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_F)
  {
    if (prof)
    {
      prof->profile()->setDisplay (2);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_H)
  {
    if (prof)
    {
      prof->profile()->setDisplay (3);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_J)
  {
    if (prof)
    {
      prof->profile()->setDisplay (4);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_K)
  {
    if (prof)
    {
      prof->profile()->setDisplay (5);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_B)
  {
    if (prof)
    {
      prof->profile()->incCorrelWidth (event->modifiers () & Qt::ShiftModifier);
      prof->profile()->setDisplay (5);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_V)
  {
    if (prof)
    {
      prof->profile()->incCorrelThick (event->modifiers () & Qt::ShiftModifier);
      prof->profile()->setDisplay (5);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_I)
  {
    if (prof)
    {
      prof->profile()->incCorrelRatio (event->modifiers () & Qt::ShiftModifier);
      prof->profile()->setDisplay (5);
      prof->scene()->update ();
      prof->show ();
    }
  }

  if (event->key () == Qt::Key_Y)
  {
    if (prof)
    {
      prof->profile()->toggleMeanCorrel ();
      prof->profile()->setDisplay (5);
      prof->scene()->update ();
      prof->show ();
    }
  }
*/

  if (event->key () == Qt::Key_X)
  {
    manualModeOn = ! manualModeOn;
  }

/*
  if (event->key () == Qt::Key_G)
  {
    gestureModeOn = ! gestureModeOn;
  }

  if (event->key () == Qt::Key_Up)
  {
    if (prof)
    {
      prof->profile()->incStripe (1);
      prof->scene()->update ();
    }
  }

  if (event->key () == Qt::Key_Down)
  {
    if (prof)
    {
      prof->profile()->incStripe (-1);
      prof->scene()->update ();
    }
  }
*/

  if (event->key () == Qt::Key_W)
  {
    if (event->modifiers () & Qt::ShiftModifier)
      afficheVectSegRecognized (detectedSegments);
    else
      detectedSegments.push_back (lastDetectedSegment);
  }
}


void ExtractionWidget::paintEvent (QPaintEvent *)
{
  QPainter painter (this);
  painter.drawImage (QPoint (0, 0), augmentedImage);
}


void ExtractionWidget::drawListPixels (vector<Pixel> vectPixels,
                                       QColor color, QPainter &painter)
{
  vector<Pixel>::iterator iter = vectPixels.begin ();
  while (iter != vectPixels.end ())
  {
    Pixel p = *iter;
    painter.setPen (QPen (color, myPenWidth, Qt::SolidLine,
                          Qt::RoundCap, Qt::RoundJoin));
    if (p.x() < width && p.y() < height && p.x() >= 0 && p.y() >= 0)
      painter.drawPoint (QPoint (p.x(), height - p.y()));
    iter ++;
   }
}


void ExtractionWidget::afficheLineDetection ()
{
  augmentedImage = loadedImage;
  QPainter painter (&augmentedImage);
  drawStraightLine (p1, p2, Qt::red, painter);

//cout << "P1 = (" << p1.x () << "," << p1.y ()
//     << ") et P2 = (" << p2.x () << "," << p2.y () << ")" << endl;

  if (manualModeOn)
  {
    /** ZZZ
    DirectionalScan dirScan = DirectionalScan (p1, p2);
    dirScan.computeAllScans (0, 0, width, height);
    */
    DirectionalScan dirScan = DirectionalScan (p1, p2, 0, 0, width, height);

    vector<Pixel> pointsLineOrigin  = dirScan.getLeftScan (0);
    drawListPixels (pointsLineOrigin, Qt::white, painter);

    Profile *pr = prof->profile ();
    pr->clear ();
    pr->addCentral (p1, p2);
    for (int i = 0; i < dirScan.getNbRightScan (); i++)
      pr->addToRight (vector<Pixel> (dirScan.getRightScan (i)));
    for (int i = 0; i < dirScan.getNbLeftScan (); i++)
      pr->addToLeft (vector<Pixel> (dirScan.getLeftScan (i)));

    update (QRect (QPoint (0, 0), QPoint (width, height)));

    prof->scene()->update ();
    prof->show ();
    return;
  }

  BlurredSegment *bs = detector.detect (p1, p2);
  vector<Pixel> vpoints;
  if (bs != NULL) vpoints = bs->getPrintedPoints ();

  if (vpoints.size () < 6)
  {
    return;
  }

  if (supportLinesDisplayOn)
  {
    drawListPixels (vpoints, Qt::blue, painter);
  }

  lastDetectedSegment.vectPoints = vpoints;
  lastDetectedSegment.p1 = p1;
  lastDetectedSegment.p2 = p2;

  painter.setPen (QPen (Qt::red, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
  painter.drawText (100, 20,
       tr ("E : width = ") + QString::number (segmentMaxWidth));
  painter.drawText (100, 40,
       tr ("Z : # pixels missing = ") + QString::number (detectionLacks));

  update (QRect (QPoint (0, 0), QPoint (width, height)));
}


int ExtractionWidget::getPixValue (const QImage & image, Pixel p)
{  
  return (
    (QColor) image.pixel (QPoint (p.x (), height - p.y ()))).value ();
}


int **ExtractionWidget::buildImageArray (const QImage &image)
{
  int w = image.width ();
  int h = image.height ();
  
  int **tabImage = new int*[h];
  for (int i = 0; i < h; i++)
  {
    tabImage[i] = new int[w];
    for(int j = 0; j < w; j++)
    {
      QColor c = QColor (image.pixel (j, h - i - 1));
      tabImage[i][j] =  c.value ();
    }
  }
  return tabImage;
}


void ExtractionWidget::afficheVectSegRecognized (
                             vector<DetectedSegment> segments)
{
  augmentedImage = loadedImage;
  QPainter painter (&augmentedImage);
  vector<DetectedSegment>::iterator it = segments.begin ();
  while (it != segments.end ())
  {
    DetectedSegment seg = *it;
    Pixel point1 = seg.p1;
    Pixel point2 = seg.p2;

    vector<Pixel> listePixInit = LineTool::draw (point1, point2);
    drawListPixels (listePixInit, Qt::white, painter);
    drawListPixels (seg.vectPoints, Qt::blue, painter);
    it ++;
  }
  update (QRect (QPoint (0, 0), QPoint (width, height)));
}


void ExtractionWidget::drawStraightLine (Pixel p1, Pixel p2,
                                         QColor col, QPainter &painter)
{
  vector<Pixel> pts = LineTool::draw (p1, p2);
  vector<Pixel>::iterator iter = pts.begin ();
  while (iter != pts.end ())
  {
    Pixel p = *iter;
    painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                          Qt::RoundCap, Qt::RoundJoin));
    if (p.x () < width && p.y () < height && p.x () >= 0 && p.y () >= 0)
      painter.drawPoint (QPoint (p.x (), height - p.y ()));
    iter ++;
   }
}
