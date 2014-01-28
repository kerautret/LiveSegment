#ifndef APPLICATIONWINDOW_H
#define APPLICATIONWINDOW_H

#include <QList>
#include <QMainWindow>
#include "profiler.h"
#include "extractionwidget.h"


class ApplicationWindow : public QMainWindow
{
    Q_OBJECT
    
public:
  ApplicationWindow ();
  ApplicationWindow (int *val);
  void setFile (QString fileName);
  void setProfiler (Profiler *prof);


protected:
  void closeEvent (QCloseEvent *event);


private slots:
  void open ();
  void save ();
  void updateActions ();


private:
  void createActions ();
  void createMenus ();
  bool saveFile (const QByteArray &fileFormat);
  QMenu *saveAsMenu;
  QMenu *fileMenu;
  ExtractionWidget *extractor;

  QAction *openAct;
  QList<QAction *> saveAsActs;
  QAction *exitAct;
};
#endif
