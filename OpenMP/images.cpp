#include <QPushButton>
#include <QGridLayout>
#include <QFileInfo>
#include <QLabel>
#include <QtGui>
#include <QDir>
#include "images.h"

QGridImages::QGridImages(QWidget *parent)
    : QWidget(parent)
{
  QGridLayout *grid = new QGridLayout(this);
  
  QImage copy;
  QString tempFileName;
  QList<QImage> images;
  QList<QString> filesList;
  filesList << "/image1.png" << "/image2.png" << "/image3.png";
  QString path = QDir::current().path();

  foreach(QFileInfo fileInfo, filesList)
  {
    tempFileName = fileInfo.filePath();
    qDebug() << path+"/images"+fileInfo.filePath();
    QImage image(path+"/images"+tempFileName);
    copy = image.scaled(200,200,Qt::KeepAspectRatio);
    images.append(copy);
  }

  for (int i = 0; i < 3; i++) {
    for(int j = 0; j < 3; j++) {
      QPixmap p(QPixmap::fromImage(images[i]));
      QLabel *label = new QLabel(this);
      label->setPixmap(p);
      grid->addWidget(label, i, j);
    }
  }

  setLayout(grid);
}
