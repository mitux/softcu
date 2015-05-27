
#include "menu.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "QFileDialog"
#include <QImageWriter>
#include <QDirIterator>

using namespace std;
using namespace cv;

QMainMenu::QMainMenu(QWidget *parent)
	: QWidget(parent)
{
#pragma omp parallel 
	{
		printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads()); 
	}
	show_btn = new QPushButton("Show images", this);
	show_btn->move(20, 50);

	empty_btn = new QPushButton("I do nothing :)", this);
	empty_btn->move(23, 80);

	connect(show_btn, SIGNAL(clicked()), this, SLOT(OnShowPressed()));
    //connect(empty_btn, SIGNAL(clicked()), this, SLOT(Histogram(Mat)));

}

bool QMainMenu::compare(const hist& first, const hist& second)
{
  if (first.c < second.c)
    return true;
  else
    return false;
}

void QMainMenu::OnShowPressed()
{
	QWidget *imagesWidget = new QWidget();
	QGridLayout *grid = new QGridLayout(imagesWidget);


    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "../", tr("Text Files (*.txt)"));

    cout << QFileInfo(fileName).absolutePath().toStdString() << endl;

	QImage copy;
	QString tempFileName;
	QList<QImage> images;
	QList<QString> filesList;
	filesList << "../image1.png" << "../image2.png" << "../image3.png";

	foreach(QFileInfo fileInfo, filesList)
	{
		tempFileName = fileInfo.absoluteFilePath();
		cout << tempFileName.toStdString() << endl;
		Mat img;
		img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);
        Histogram(img);
		QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
		//QImage image(tempFileName);
		copy = image.scaled(200,200,Qt::KeepAspectRatio);
		images.append(copy);
	}

	for (int i = 0; i < 3; i++) {
		for(int j = 0; j < 3; j++) {
			QPixmap p(QPixmap::fromImage(images[i]));
			QLabel *label = new QLabel(imagesWidget);
			label->setPixmap(p);
			grid->addWidget(label, i, j);
		}
	}

	setLayout(grid);
}



void QMainMenu::Histogram(Mat im)
{

    Mat src, hsv;
      vector<Mat> hsv_planes;
      // Load image
      src = im;


      /// Load two images with different environment settings
      if( !src.data )
        { return; }


      /// Convert to HSV
      cvtColor(src, hsv, CV_BGR2HSV );

      // Extract HSV planes
      split(hsv, hsv_planes);

      /// Bins to use
      int h_bins = 50; int s_bins = 50; int v_bins = 100;

      // Ranges
      float hrang[] = {0, 180};
      const float *h_ranges = { hrang };

      float srang[] = {0, 256};
      const float *s_ranges = { srang };

      float vrang[] = {0, 256};
      const float *v_ranges = { vrang };

      /// Histograms
      Mat hist_h, hist_s, hist_v;

      /// Calculate the histogram for the H image
      calcHist( &hsv_planes[0], 1, 0, Mat(), hist_h, 1, &h_bins, &h_ranges, true, false );
      normalize( hist_h, hist_h, 0, 1, NORM_MINMAX, -1, Mat() );

      calcHist( &hsv_planes[1], 1, 0, Mat(), hist_s, 1, &s_bins, &s_ranges, true, false );
      normalize( hist_s, hist_s, 0, 1, NORM_MINMAX, -1, Mat() );

      calcHist( &hsv_planes[2], 1, 0, Mat(), hist_v, 1, &v_bins, &v_ranges, true, false );
      normalize( hist_v, hist_v, 0, 1, NORM_MINMAX, -1, Mat() );


      /// Display
      //namedWindow("histogram Demo", CV_WINDOW_AUTOSIZE );
      //imshow("histogram Demo", hist_h );

      QString path = QString("../histogrames/") + QString("hist_") + QString::number(hCont) + ".xml";
       cout << path.toStdString() << endl;
       hCont++;
      // Store histograms on disc
      FileStorage fs(path.toLatin1().data(), FileStorage::WRITE);

      fs << "hist_h" << hist_h;
      fs << "hist_s" << hist_s;
      fs << "hist_v" << hist_v;

      fs.release();


      histComp("../histogrames/hi_0.xml");

    waitKey(0);

}



void QMainMenu::histComp(QString path1){

  int method, compare_method;

  method = 2;
  if (method == 1)
    compare_method = CV_COMP_CORREL; // Correlation
  else if (method == 2)
    compare_method = CV_COMP_CHISQR; // Chi-Square
  else if (method == 3)
    compare_method = CV_COMP_INTERSECT; // Intersection
  else if (method == 4)
    compare_method = CV_COMP_BHATTACHARYYA; // Bhattacharyya distance
  else {
    printf("ERROR: no valid value for compare_method\n");
    exit(1);
  }

  Mat hist_h1, hist_s1, hist_v1;
  Mat hist_h2, hist_s2, hist_v2;

  // Read histogram1
  FileStorage fs1(path1.toLatin1().data(), FileStorage::READ);

  fs1["hist_h"] >> hist_h1;
  fs1["hist_s"] >> hist_s1;
  fs1["hist_v"] >> hist_v1;

  fs1.release();

  vector<hist> histoList;


  QString path2;

    QDirIterator it("../histogrames/", QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {

        path2 = it.next();

        // Read histogram2
        FileStorage fs2(path2.toLatin1().data(), FileStorage::READ);

        fs2["hist_h"] >> hist_h2;
        fs2["hist_s"] >> hist_s2;
        fs2["hist_v"] >> hist_v2;

        fs2.release();

        /// Histogram comparison
        double comphist_h = compareHist( hist_h1, hist_h2, compare_method);
        double comphist_s = compareHist( hist_s1, hist_s2, compare_method);
        double comphist_v = compareHist( hist_v1, hist_v2, compare_method);

        hist h;
        h.c = comphist_h + comphist_s + comphist_v;
        h.path = path2;

        histoList.push_back(h);


        cout << path2.toStdString() << endl;
    }

    std::sort(histoList.begin(),histoList.end(), compare);

    for(int i; i<5; i++){
        cout << QString::number(histoList.at(i).c).toStdString() << endl;
        cout << histoList.at(i).path.toStdString() << endl;
    }


  //return 0;
}


