#ifndef PROFILER_H
#define PROFILER_H

#include <QGraphicsView>
#include "profile.h"


class Profiler : public QGraphicsView
{

public:
  Profiler ();
  void paint (QPainter *painter,
              const QStyleOptionGraphicsItem *option, QWidget *widget);
  Profile *profile ();

protected:
  void keyPressEvent (QKeyEvent * event);

private:
  Profile *prof;
};

#endif
