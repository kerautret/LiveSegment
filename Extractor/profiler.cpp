#include <QtGui>
#include "profiler.h"



Profiler::Profiler ()
{
  // CAUTION : don't activate antialiasing here !!!
  setBackgroundBrush (QBrush (Qt::white));
  setScene (new QGraphicsScene (0, 0, 610, 610));
  prof = new Profile ();
  scene()->addItem (prof);
}


void Profiler::paint (QPainter *painter,
                      const QStyleOptionGraphicsItem *option,
                      QWidget *widget)
{
  Q_UNUSED (option);
  Q_UNUSED (widget);
  Q_UNUSED (painter);

}


void Profiler::keyPressEvent (QKeyEvent *event)
{
  if (event->key () == Qt::Key_A)
  {
  }
}


Profile *Profiler::profile ()
{
  return (prof);
}
