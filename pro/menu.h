#pragma once

#include <omp.h> 
#include <QWidget>

#include <QPushButton>
#include <QApplication>
#include <QGridLayout>
#include <QFileInfo>
#include <QLabel>
#include <stdio.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

using namespace cv;

class QMainMenu : public QWidget
{
Q_OBJECT

 public:
   QMainMenu(QWidget *parent = 0);
   int hCont = 1;

   struct hist {
     double c;
     QString path;
   };


private slots:
    void OnShowPressed();
    void histComp(QString);
    static bool compare(const hist& first, const hist& second);
    void comparaHist();
    vector<Mat> histogram(Mat im);
    void saveHistogram(Mat im);

  private:
    QPushButton *show_btn;
	QPushButton *empty_btn;
    QList<QString> fillList(QString file);
    QString getName(QString pre, int n);
};
