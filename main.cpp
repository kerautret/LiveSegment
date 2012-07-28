#include <QApplication>
#include <iostream>
#include "applicationwindow.h"
#include "profiler.h"


int main (int argc, char *argv[])
{
  int val = 0;
  QApplication app (argc, argv);
  ApplicationWindow window (&val);   // Necessary argument !

  Profiler *prof = new Profiler ();
  prof->setWindowTitle ("Profil");
  window.setProfiler (prof);
  if (argc == 2) window.setFile (QString (argv[1]));

  window.show ();
  return app.exec ();
  //  return EXIT_SUCCESS;
}
