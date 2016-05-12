#include "DMAHCALMainWindow.h"
#include "DMAHCAL.h"

#include <QApplication>
#include <QtGui>
#include <QTimer>

int main(int argc, char *argv[])
{
  //start QApplication
  QApplication a(argc, argv);
  //ini glut for EventDisplay
  glutInit(&argc, argv);

  //Splash screen (change version here)
  DMAHCAL::SplashScreen("1.3");

  //instance Main UI
  DMAHCALMainWindow w;
  //Wait 1s before showing ui
  QTimer::singleShot(1000, &w, SLOT(show()));

  return a.exec();
}

