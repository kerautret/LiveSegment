#include <QtGui>
#include "paint.h"
#include "math.h"
#include <iostream>
#include <cstdlib>
#include "linedetection.h"
#include "directionalutil.h"
#include "pointset.h"
using namespace std;


Paint::Paint (QWidget *parent)
{
  Q_UNUSED (parent);

  // Set default parameter values
  epaisseurDetection = 5;
  epaisseurSensitivity = 400;
  nbPixelInteruption = 5;
  interSensitivity = 20;

  // Set default user interface modalities
  afficheCandSel = false;
  afficheGradientMax = false;
  afficheSegmentFlou = true;
  LineDetection::seuil = 30.0;

  setFocus ();
  grabKeyboard ();
  myPenWidth = 1;
  myPenColor = Qt::blue;
  p1defined = false;
  numberSolution = 0;
  numberStep = 0;
  method = 0;
  modeManuel = false;
  modeGestuel = false;
  geste = NULL;
  prof = NULL;
  isCurrentSegmentNivGrisDefined = false;
}


QSize Paint::openImage (const QString &fileName)
{
  QImage loadedImage;
  QSize newSize (0, 0);
  loadedImage.load (fileName);

  newSize = loadedImage.size ();
  sizeNormal = newSize;
 
  image = loadedImage;
  imageOrigin = loadedImage;
  this->tabImage = buildImageArray (image);
  this->largeur = image.width ();
  this->hauteur = image.height ();
 
  modified = false;
 
  tabGradientNormale = DirectionalUtil::retVectorsGradientRepereNornal (
                                                 tabImage, largeur, hauteur);
  tabGradientUsed = new bool* [hauteur];
  for (int i = 0; i < hauteur; i++)
  {
    tabGradientUsed[i] = new bool[largeur];
    for (int j = 0; j < largeur; j++) tabGradientUsed[i][j] = false;
  }
 
  update ();

  if (prof) prof->profile()->setImage (&imageOrigin, tabGradientNormale);
 
  return newSize;
}


bool Paint::saveImage (const QString &fileName, const char *fileFormat)
{
  QImage visibleImage = image;
  if (visibleImage.save (fileName, fileFormat))
  {
    modified = false;
    return true;
  }
  else return false;
}


void Paint::setProfiler (Profiler *prof)
{
  this->prof = prof;
}


void Paint::setPenColor (const QColor &newColor)
{
  myPenColor = newColor;
}


void Paint::setPenWidth (int newWidth)
{
  myPenWidth = newWidth;
}


void Paint::clearImage ()
{
  image.fill (qRgb (255, 255, 255));
  modified = true;
  update ();
}


void Paint::mousePressEvent (QMouseEvent *event)
{
  if ((event->buttons () & Qt::RightButton))
  {
    this->p1 = QPoint (event->pos().x (),
                       image.height () - 1 - event->pos().y());
    if (p1defined)
      cerr << "afficheGradients" << endl;
    if (image.width () > p1.x () && image.height () > p1.y ())
    {
      p1defined = true;
      cerr << "p1 defined: " << p1.x () << " " << p1.y () << endl;
    }
    if (modeGestuel)
    {
      if (geste != NULL) delete geste;
      geste = new vector<QPoint> ();
    }
  }
}


void Paint::mouseReleaseEvent (QMouseEvent *event)
{
  if (modeGestuel)
  {
    cout << "Gestuel : " << modeGestuel << endl;
    this->p2 = QPoint (event->pos().x (),
                       image.height () - 1 - event->pos().y());
    if ((abs (p1.x() - p2.x()) > 2 && abs (p1.y() - p2.y()) > 2)
        && (image.width() > p2.x() && image.height() > p2.y()
            && p2.x() > 0 && p2.y() > 0))
    {
      cerr << "p1 defined: " << p1.x() << " "<< p1.y() << endl;
      cerr << "p2 defined: " << p2.x() << " "<< p2.y() << endl;
      numberSolution = 0;
      numberStep = 0;
      int dx = p2.x() - p1.x();
      int dy = p2.y() - p1.y();
      int l2 = dx * dx + dy * dy;
      int ml2 = 0;
      epaisseurDetection = 1 + l2 / epaisseurSensitivity;
      vector<QPoint>::iterator it = geste->begin ();
      while (it != geste->end ())
      {
        QPoint pt = *it;
        int px = pt.x() - p1.x();
        int py = pt.y() - p1.y();
        int ll2 = (px * dy - py * dx) * (px * dy - py * dx) / l2;
        if (ll2 > ml2) ml2 = ll2;
        it ++;
      }
      nbPixelInteruption = 0;
      if (ml2 > 20) nbPixelInteruption = ml2 / interSensitivity;
cerr << "Inter = " << nbPixelInteruption << endl;

      resetTabGradientUsed ();
      afficheLineDetection ();
    }
  }
}


void Paint::mouseMoveEvent (QMouseEvent *event)
{
  if (p1defined)
  {
    this->p2 = QPoint (event->pos().x (),
                       image.height () - 1 - event->pos().y ());
    if ((abs (p1.x() - p2.x()) > 2 && abs (p1.y() - p2.y()) > 2)
        && (image.width() > p2.x() && image.height() > p2.y()
            && p2.x() > 0 && p2.y() > 0))
    {
      if (modeGestuel)
      {
        image = imageOrigin;
        QPainter painter (&image);
        geste->push_back (p2);
        drawListPixels (*geste, Qt::green, painter);
        update (QRect (QPoint (0, 0), QPoint (image.width (), image.height ())));
      }
      else
      {
        //cerr << "p1 defined: " << p1.x() << " "<< p1.y() << endl;
        //cerr << "p2 defined: " << p2.x() << " "<< p2.y() << endl;
        numberSolution = 0;
        numberStep = 0;
        // method = 0;

        resetTabGradientUsed ();
        afficheLineDetection ();

        // drawIntLinesDistLongueur ();
        // drawIntLines ();
        // drawGadientDirectionnalOLD ();
      }
    }
  }
}


void Paint::keyPressEvent (QKeyEvent *event)
{
  // if (event->key () == Qt::Key_C)
  // {
  //   afficheCandSel = ! afficheCandSel;
  // }
  
  if (event->key () == Qt::Key_A)
  {
    afficheSegmentFlou = ! afficheSegmentFlou;
  }

  // if (event->key () == Qt::Key_G)
  // {
  //   afficheGradientMax = ! afficheGradientMax;
  //   resetTabGradientUsed ();
  //   afficheLineDetection ();
  // }

  if (event->key () == Qt::Key_U)
  {
    resetTabGradientUsed ();
    afficheLineDetection ();
  }

  if (event->key () == Qt::Key_N)
  {
    // afficheLineDetection ();
    if (event->modifiers () & Qt::ShiftModifier) numberSolution = 0;
    numberSolution ++;
    numberStep = 0;
    resetTabGradientUsed ();
    afficheLineDetection ();
  }
  
  if (event->key () == Qt::Key_R)
  {          
    // afficheLineDetection ();
    if (event->modifiers () & Qt::ShiftModifier) numberStep = 0;
    if (++numberStep == 3) numberStep = 0;
    cerr << "Phase " << numberStep << endl;
    resetTabGradientUsed ();
    afficheLineDetection ();
  }

  if (event->key () == Qt::Key_M)
  {
    // afficheLineDetection ();
    if (event->modifiers () & Qt::ShiftModifier) method = 0;
    if (++method == 4) method = 0;
    numberSolution = 0;
    numberStep = 0;
    if (method == 1)
      cerr << "Methode " << method << " avec profils" << endl;
    else if (method == 2)
      cerr << "Methode " << method << " multi-solutions" << endl;
    else if (method == 3)
      cerr << "Methode " << method << " : pas de prolongement sur R2" << endl;
    else
      cerr << "Methode " << method << " std" << endl;
    resetTabGradientUsed ();
    afficheLineDetection ();
  }
  
  // if (event->key () == Qt::Key_L)
  // {
  //   drawIntLinesDistLongueur ();
  // }
 
  // if (event->key () == Qt::Key_M)
  // {
  //   drawGradientMaxDirectionnal ();
  // }

  if (event->key () == Qt::Key_Z)
  {
    if (event->modifiers () & Qt::ShiftModifier) nbPixelInteruption --;
    else nbPixelInteruption ++;
    update ();
    cerr << "Nouveau nb pixel interuption = " << nbPixelInteruption << endl;
  }

  if (event->key () == Qt::Key_S)
  {
    if (event->modifiers () & Qt::ShiftModifier)
      LineDetection::seuil = LineDetection::seuil - 5.0;
    else LineDetection::seuil = LineDetection::seuil + 5.0;
    cerr << "Nouveau seuil = " << LineDetection::seuil << endl;
    QPainter painter (&image);
  }

  if (event->key () == Qt::Key_E)
  {
    if (event->modifiers () & Qt::ShiftModifier) epaisseurDetection --;
    else epaisseurDetection ++;
    cerr << "Nouvelle epaisseur = " << epaisseurDetection << endl;
  }

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

  if (event->key () == Qt::Key_X)
  {
    modeManuel = ! modeManuel;
  }

  if (event->key () == Qt::Key_G)
  {
    modeGestuel = ! modeGestuel;
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

  if (event->key () == Qt::Key_W)
  {
    if (event->modifiers () & Qt::ShiftModifier)
      afficheVectSegRecognized (vectSegRecognized);
    else
      vectSegRecognized.push_back (segNivGris);
  }
}


void Paint::paintEvent (QPaintEvent *)
{
  QPainter painter (this);
  painter.drawImage (QPoint (0, 0), image);
}


void Paint::drawListPixels (vector<QPoint> vectPixels,
                            QColor color, QPainter &painter)
{
  vector<QPoint>::iterator iter = vectPixels.begin ();
  while (iter != vectPixels.end ())
  {
    QPoint p = *iter;
    painter.setPen (QPen (color, myPenWidth, Qt::SolidLine,
                          Qt::RoundCap, Qt::RoundJoin));
    if (p.x() < image.width() && p.y() < image.height()
        && p.x() >= 0 && p.y() >= 0)
      painter.drawPoint (QPoint (p.x(), image.height() - p.y()));
    iter ++;
   }
}


void Paint::drawVectorsFieldFromPoints (vector<QPoint> vectPoints,
                                        QColor color, QPainter &painter)
{
  vector<QPoint>::iterator iter = vectPoints.begin ();
  while (iter != vectPoints.end ())
  {
    QPoint p = *iter;
    if (p.y() < hauteur && p.y() >= 0 && p.x() < largeur && p.x() >= 0)
    {
      QPoint g = tabGradientNormale[p.y()][p.x()];
      double norme = sqrt (g.x() * g.x() + g.y() * g.y());

      QPoint p2 = QPoint (p.x() + (g.x() / norme) * 30.0,
                          p.y() + (g.y() / norme) * 30.0);
      QPoint p3 = QPoint ((int) (p2.x() + 2), (int) (p2.y()));
      QPoint p4 = QPoint ((int) (p2.x() - 2), (int) (p2.y()));

      vector<QPoint> listePixLine = PointSet::tracerSegment (p, p2);
      drawListPixels (listePixLine, color, painter);
      listePixLine = PointSet::tracerSegment (p3, p4);
      drawListPixels (listePixLine, color, painter);
    }
    iter ++;
  }
}


void Paint::drawVectorsField (vector<QPoint>* vectGradient,
                              QColor color, QPainter &painter)
{
  Q_UNUSED (color);
  Q_UNUSED (painter);
 
  vector<QPoint> vectPix = vectGradient[0];
  vector<QPoint> vectGrad = vectGradient[1];
  vector<QPoint>::iterator iterGrad = vectGrad.begin ();
  vector<QPoint>::iterator iterPix = vectPix.begin ();
  int cpt = 0;
  while (iterPix != vectPix.end () && iterGrad != vectGrad.end ())
  {
    cpt ++;
    QPoint pix1 = (*iterPix);
    double dx = (*iterGrad).x ();
    double dy = (*iterGrad).y ();
    double norm2 = sqrt (dx * dx + dy * dy);

    // if (pix1.x() % 10 != 0 || pix1.y() % 10 != 0)
    // {
    //   iterGrad ++;
    //   iterPix ++;
    //   continue;
    // }

    if(norm2 == 0) norm2 = 1.0;
    double angle = atan2 (dy / norm2, dx / norm2);
 
    double dxp = this->p2.x() - this->p1.x();
    double dyp = this->p2.y() - this->p1.y();
    double norm = sqrt (dxp * dxp + dyp * dyp);

    double angle2 = atan2 (- dxp / norm, dyp / norm);

    dx = cos (angle2-angle);
    dy = sin (angle2-angle);

    vector<QPoint> listePixLine;
    dx = (dx) * 20.0;
    dy = (dy) * 20.0;
    QPoint pix2 = QPoint ((int) (pix1.x() + dx),
                          (int) (pix1.y() + dy));
    // cout << p1.x() << " " << p1.y() << " " << angle << endl;

    QPoint pix3 = QPoint ((int) (pix1.x() + 2), (int) (pix1.y()));
    QPoint pix4 = QPoint ((int) (pix1.x() - 2), (int) (pix1.y()));

    // listePixLine = PointSet::tracerSegment (pix1, pix2);
    // drawListPixels (listePixLine, Qt::red, painter);
    // listePixLine = PointSet::tracerSegment (pix3, pix4);
    // drawListPixels (listePixLine, Qt::red, painter);
    // listePixLine = PointSet::tracerSegment (pix1, pix2N);
    // drawListPixels (listePixLine, Qt::blue, painter);
    iterGrad ++;
    iterPix ++;
  }
}


void Paint::afficheLineDetection ()
{
  QColor col = Qt::magenta;
  image = imageOrigin;
  QPainter painter (&image);
  vector<QPoint> listePixInit = PointSet::tracerSegment (p1, p2);
  drawListPixels (listePixInit, Qt::red, painter);

  if (modeManuel)
  {
    DirectionalScan dirScan = DirectionalScan (p1, p2);
    dirScan.computeAllScans (0, 0, largeur, hauteur);
    vector<QPoint> pointsLineOrigin  = dirScan.getLeftScan (0);
    drawListPixels (pointsLineOrigin, Qt::white, painter);

    Profile *pr = prof->profile ();
    pr->clear ();
    pr->addCentral (p1, p2);
    for (int i = 0; i < dirScan.getNbRightScan (); i++)
      pr->addToRight (vector<QPoint> (dirScan.getRightScan (i)));
    for (int i = 0; i < dirScan.getNbLeftScan (); i++)
      pr->addToLeft (vector<QPoint> (dirScan.getLeftScan (i)));

    modified = true;
    update (QRect (QPoint (0, 0), QPoint (image.width(), image.height())));

    prof->scene()->update ();
    prof->show ();
    return;
  }

  vector<QPoint> vpoints;
  if (method == 1)
  {
    vpoints = LineDetection::detectLineProfils (
                p1, p2, tabImage, largeur,  hauteur, epaisseurDetection,
                nbPixelInteruption, numberSolution, numberStep);
  }
  else if (method == 2)
  {
    vpoints = LineDetection::detectLine3 (
                p1, p2, largeur, hauteur, tabGradientNormale,
                tabGradientUsed, epaisseurDetection, nbPixelInteruption,
                numberSolution, numberStep);
  }
  else if (method == 3)
  {
    vpoints = LineDetection::detectLine (
                p1, p2, tabImage, largeur, hauteur, tabGradientNormale,
                tabGradientUsed, epaisseurDetection, nbPixelInteruption,
                numberStep);
  }
  else // 0
  {
    vpoints = LineDetection::detectLine2 (
                p1, p2, tabImage, largeur, hauteur, tabGradientNormale,
                tabGradientUsed, epaisseurDetection, nbPixelInteruption,
                numberStep);
  }

  if (vpoints.size () < 6)
  {
    return;
  }

  if (afficheSegmentFlou)
  {
    drawListPixels (vpoints, Qt::blue, painter);
  }

  segNivGris.vectPoints = vpoints;
  segNivGris.p1 = p1;
  segNivGris.p2 = p2;

  painter.setPen (QPen (Qt::red, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
  painter.drawText (100, 20,
       tr ("E : width = ") + QString::number (epaisseurDetection));
  painter.drawText (100, 40,
       tr ("Z : # pixels missing = ") + QString::number (nbPixelInteruption));

  modified = true;
  update (QRect (QPoint (0, 0), QPoint (image.width (), image.height ())));
}


void Paint::updateTabGradUsed (vector<QPoint> vPoints)
{
  vector<QPoint>::iterator it = vPoints.begin ();
  while (it != vPoints.end ())
  {
    QPoint p = *it;
    tabGradientUsed[p.y()][p.x()] = true;
    it ++;
  }
}


void Paint::resetTabGradientUsed ()
{
  for (int i = 0; i < hauteur; i++)
    for (int j = 0; j < largeur; j++)
      tabGradientUsed[i][j] = false;
}


void Paint::drawIntLinesDist ()
{
  QColor col = Qt::magenta;
  QPainter painter (&image);

  modified = true;
  dirScan.computeAllScans (0, 0, image.width (), image.height ());
  int tailleL = dirScan.getNbLeftScan ();
  int tailleR = dirScan.getNbRightScan ();
  for (int i = 0; i < tailleL; i++)
  {
    if (i % 10 == 0) col = Qt::magenta;
    else col = Qt::blue;
    // int val = (int) (((double) i / (double) tailleL) * 255.0);
    // col = QColor (val, val, val);
    painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                          Qt::RoundCap, Qt::RoundJoin));
    vector<QPoint> vect = dirScan.getLeftScan (i);
    vector<QPoint>::iterator iter = vect.begin ();
    while (iter != vect.end ())
    {
      QPoint pt = *iter;
      painter.drawPoint (QPoint (pt.x (), image.height () - pt.y ()));
      iter ++;
    }
  }
  for (int i = 0; i < tailleR; i++)
  {
    if (i % 10 == 0) col = Qt::green;
    else col = Qt::red;
    // int val = (int) (((double) i / (double) tailleR) * 255.0);
    // col = QColor (val, val, val);
    painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                          Qt::RoundCap, Qt::RoundJoin));
    vector<QPoint> vect = dirScan.getRightScan (i);
    vector<QPoint>::iterator iter = vect.begin ();
    while (iter != vect.end ())
    {
      QPoint pt = *iter;
      painter.drawPoint (QPoint (pt.x (), image.height () - pt.y ()));
      iter ++;
    }
  }
  modified = true;
  update (QRect (QPoint (0, 0), QPoint (image.width (), image.height ())));
}


void Paint::drawIntLinesDistLongueur ()
{
  QColor col = Qt::magenta;
  QPainter painter (&image);
  cerr << "int lines" << endl;
  modified = true;
  dirScan.computeAllScans (0, 0, image.width (), image.height ());
  int tailleL = dirScan.getNbLeftScan ();
  int tailleR = dirScan.getNbRightScan ();
  for (int i = 0; i < tailleL; i++)
  {
    if (i % 10 == 0) col = Qt::magenta;
    else col = Qt::blue;
    int val = (int) (((double) i / (double) tailleL) * 255.0);
    if (i % 2 == 0) val = 255 - val;
    col = QColor (val, val, val);
    painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                          Qt::RoundCap, Qt::RoundJoin));
    vector<QPoint> vect = dirScan.getLeftScan (i);
    
    for (int k = 0; k < (int) vect.size (); k++)
    {
      QPoint pt = vect.at (k);
      if (i % 2 == 0) col = Qt::red;
      else col = Qt::blue;
      int val = (int) (((double) k / (double) vect.size ()) * 255.0);
      if (i % 2 == 0) val = 255 - val;
      col = QColor (val, val, val);
      if (k % 2 == 0) col = Qt::blue;
      painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));
      painter.drawPoint (QPoint (pt.x (), image.height () - pt.y ()));
    }
  }
  for  (int i = 0; i < tailleR; i++)
  {
    // int val = (int) (((double) i / (double) tailleR) * 255.0);
    // col = QColor (val, val, val);
    painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                          Qt::RoundCap, Qt::RoundJoin));
    vector<QPoint> vect = dirScan.getRightScan (i);
    for (int k = 0; k < (int) vect.size (); k++)
    {
      QPoint pt = vect.at (k);
      if (i % 1 == 0) col = Qt::red;
      else col = Qt::blue;

      int val = (int) (((double) k / (double) vect.size ()) * 255.0);
      if (i % 2 == 0) val = 255 - val;
      col = QColor (val, val, val);
      if (k % 2 == 0) col = Qt::blue;
      painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                            Qt::RoundCap, Qt::RoundJoin));
      painter.drawPoint (QPoint (pt.x (), image.height () - pt.y ()));
    }
    // QPoint pt  = vect.at (vect.size () / 2);
    // painter.drawPoint (QPoint (pt.x (), image.height () - pt.y ()));
    // }
  }
  modified = true;
  update (QRect (QPoint (0, 0), QPoint (image.width (), image.height ())));
}


void Paint::drawIntLines ()
{
  QColor col = Qt::magenta;
  QPainter painter (&image);
  painter.setPen (QPen (col, myPenWidth, Qt::SolidLine,
                        Qt::RoundCap, Qt::RoundJoin));
  cerr << "dans draw int ..." << endl;
  dirScan.computeAllScans (0, 0, image.width (), image.height ());
  vector<QPoint> vr = dirScan.getRightScan (0);
  vector<QPoint> vl = dirScan.getLeftScan (0);
  vector<QPoint>::iterator itL = vl.begin ();
  vector<QPoint>::iterator itR = vr.begin ();

  while (itL < vl.end () && itR < vr.end ())
  {
    QPoint p = *itL;
    QPoint pp = *itR;
    painter.drawPoint (QPoint (p.x (), image.height () - p.y ()));
    itL ++;
    itR ++;
  }
  update (QRect (QPoint (0, 0), QPoint (image.width (), image.height ())));
}


void Paint::drawGradientMaxDirectionnal ()
{
  QColor col = Qt::magenta;
  image = imageOrigin;
  QPainter painter (&image);
  
  // vector<QPoint> pointLeftCandidates
  //    = DirectionalUtil::getGradientMaxFromScans (
  //             dirScan, imageOrigin, LEFT,
  //             QPoint (p1.x () - p2.x (), p1.y () - p2.y ()));
  // vector<QPoint> pointRightCandidates
  //    = DirectionalUtil::getGradientMaxFromScans (
  //             dirScan, imageOrigin, RIGHT,
  //             QPoint (p1.x () - p2.x (), p1.y () - p2.y ()));

  vector<QPoint> pointLeftCandidates
      = DirectionalUtil::getGradientDirMaxFromScans (
               dirScan, tabImage, largeur, hauteur, LEFT);
  vector<QPoint> pointRightCandidates
      = DirectionalUtil::getGradientDirMaxFromScans (
               dirScan, tabImage, largeur, hauteur, RIGHT);

  vector<QPoint> pointRightBorder
      = DirectionalUtil::getScansBorder (dirScan, imageOrigin, RIGHT);
  vector<QPoint> pointLeftBorder
      = DirectionalUtil::getScansBorder (dirScan, imageOrigin, LEFT);
  vector<QPoint> pointCenter
      = DirectionalUtil::getElementsI (dirScan, imageOrigin, LEFT, 3);
  
  dirScan.computeAllScans (0, 0, image.width (), image.height ());
  vector<QPoint> pointsLineOrigin = dirScan.getLeftScan (0);
  
  drawListPixels (pointLeftCandidates, Qt::cyan, painter);
  drawListPixels (pointRightCandidates, Qt::cyan, painter);
  
  drawListPixels (pointRightBorder, Qt::magenta, painter);
  drawListPixels (pointLeftBorder, Qt::magenta, painter);
  // drawListPixels (pointCenter, Qt::white, painter);
  drawListPixels (pointsLineOrigin, Qt::white, painter);
  
  modified = true;
  update (QRect (QPoint (0, 0), QPoint (image.width (), image.height ())));
}


int Paint::getPixValue (const QImage & image, const QPoint p)
{  
  return (
    (QColor) image.pixel (QPoint (p.x (), image.height () - p.y ()))).value ();
}


int **Paint::buildImageArray (const QImage &image)
{
  int largeur = image.width ();
  int hauteur = image.height ();
  
  int **tabImage = new int*[hauteur];
  for (int i = 0; i < hauteur; i++)
  {
    tabImage[i] = new int[largeur];
    for(int j = 0; j < largeur; j++)
    {
      QColor c = QColor (image.pixel (j, hauteur - i - 1));
      tabImage[i][j] =  c.value ();
    }
  }
  return tabImage;
}


void Paint::afficheVectSegRecognized (vector<SegmentsNivGris> vectSegRecognized)
{
  image = imageOrigin;
  QPainter painter (&image);
  vector<SegmentsNivGris>::iterator it = vectSegRecognized.begin ();
  while (it != vectSegRecognized.end ())
  {
    SegmentsNivGris seg = *it;
    QPoint point1 = seg.p1;
    QPoint point2 = seg.p2;

    vector<QPoint> listePixInit = PointSet::tracerSegment (point1, point2);
    drawListPixels (listePixInit, Qt::white, painter);
    drawListPixels (seg.vectPoints, Qt::blue, painter);
    it ++;
  }
  modified = true;
  update (QRect (QPoint (0, 0), QPoint (image.width (), image.height ())));
}
