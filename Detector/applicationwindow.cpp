#include "applicationwindow.h"
#include <QtGui>


ApplicationWindow::ApplicationWindow (int *val)
{
  paint = new Paint;
  setCentralWidget (paint);
  // setFocus();  
  // createActions ();
  // createMenus ();
  setWindowTitle (tr ("Blurred Segments Detection"));
  resize (400, 400);
}


ApplicationWindow::ApplicationWindow ()
{
    paint = new Paint;
    setCentralWidget (paint);
    // setFocus ();
    // createActions ();
    // createMenus ();
    setWindowTitle (tr ("Blurred Segments Detection"));
    resize (400, 400);
}


void ApplicationWindow::setFile (QString fileName)
{
  resize (paint->openImage (fileName));
}


void ApplicationWindow::setProfiler (Profiler *prof)
{
  paint->setProfiler (prof);
}


void ApplicationWindow::closeEvent (QCloseEvent *event)
{
  event->accept ();
}


void ApplicationWindow::open ()
{
  QSize windowSize;
  QString fileName = QFileDialog::getOpenFileName (this,
                                     tr ("Open File"), QDir::currentPath ());
  if (! fileName.isEmpty ())
  {
    windowSize = paint->openImage (fileName);
    updateActions ();
  }
  resize (windowSize);
}


void ApplicationWindow::save ()
{
  QAction *action = qobject_cast < QAction *> (sender ());
  QByteArray fileFormat = action->data().toByteArray ();
  saveFile (fileFormat);
}


void ApplicationWindow::updateActions ()
{
}


void ApplicationWindow::createActions ()
{
  openAct = new QAction (tr ("&Open..."), this);
  openAct->setShortcut (tr ("Ctrl+O"));
  connect (openAct, SIGNAL (triggered ()), this, SLOT (open()));

  foreach (QByteArray format, QImageWriter::supportedImageFormats ())
  {
    QString text = tr("%1...").arg (QString(format).toUpper ());
    QAction *action = new QAction (text, this);
    action->setData (format);
    connect (action, SIGNAL (triggered ()), this, SLOT (save ()));
    saveAsActs.append (action);
  }
  exitAct = new QAction (tr ("E&xit"), this);
  exitAct->setShortcut (tr ("Ctrl+Q"));
  connect (exitAct, SIGNAL (triggered ()), this, SLOT (close ()));
}


void ApplicationWindow::createMenus ()
{
  saveAsMenu = new QMenu (tr ("&Save As"), this);
  foreach (QAction *action, saveAsActs) saveAsMenu->addAction (action);
  fileMenu = new QMenu (tr ("&File"), this);
  fileMenu->addAction (openAct);
  fileMenu->addMenu (saveAsMenu);
  fileMenu->addSeparator ();
  fileMenu->addAction (exitAct);
  menuBar()->addMenu (fileMenu);
}


bool ApplicationWindow::saveFile (const QByteArray &fileFormat)
{
  QString initialPath = QDir::currentPath () + "/untitled." + fileFormat;
  QString fileName = QFileDialog::getSaveFileName (this, tr ("Save As"),
                       initialPath, tr ("%1 Files (*.%2);;All Files (*)")
                           .arg(QString(fileFormat.toUpper()))
                           .arg(QString(fileFormat)));
  if (fileName.isEmpty ()) return false;
  else return paint->saveImage (fileName, fileFormat);
}
