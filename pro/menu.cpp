
#include "menu.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

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
    connect(empty_btn, SIGNAL(clicked()), this, SLOT(Histogram()));

}



void QMainMenu::OnShowPressed()
{
	QWidget *imagesWidget = new QWidget();
	QGridLayout *grid = new QGridLayout(imagesWidget);

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

void QMainMenu::Histogram()
{

    Mat src, dst;


    QString tempFileName;
    QList<QString> filesList;
    filesList << "../image1.png" << "../image2.png" << "../image3.png";
    QFileInfo f = filesList.at(1);
    tempFileName = f.absoluteFilePath();

    /// Load image
    src = imread(tempFileName.toStdString(), 1);

    if( !src.data )
      { return; }

    /// Separate the image in 3 places ( B, G and R )
    vector<Mat> bgr_planes;
    split( src, bgr_planes );

    /// Establish the number of bins
    int histSize = 256;

    /// Set the ranges ( for B,G,R) )
    float range[] = { 0, 256 } ;
    const float* histRange = { range };

    bool uniform = true; bool accumulate = false;

    Mat b_hist, g_hist, r_hist;

    /// Compute the histograms:
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize,
          &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize,
          &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize,
          &histRange, uniform, accumulate );

    // Draw the histograms for B, G and R
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    /// Normalize the result to [ 0, histImage.rows ]
    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    /// Draw for each channel
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
                         Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
                         Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ) ,
                         Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
                         Scalar( 0, 0, 255), 2, 8, 0  );
    }

    /// Display
    namedWindow("histogram Demo", CV_WINDOW_AUTOSIZE );
    imshow("histogram Demo", histImage );

    waitKey(0);

}
