#include <fstream>
#include <iostream>
#include <QImageWriter>
#include <boost/lexical_cast.hpp>
#include "menu.h"
#include <ctime>
#include <time.h>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "QFileDialog"
#include <QImageWriter>
#include <QDirIterator>
#include <QMessageBox>

using namespace std;
using namespace cv;

QMainMenu::QMainMenu(QWidget *parent)
	: QWidget(parent)
{
#pragma omp parallel 
	{
        //printf("Hello from thread %d, nthreads %d\n", omp_get_thread_num(), omp_get_num_threads());
	}

    QVBoxLayout *layout = new QVBoxLayout(this);



    show_btn = new QPushButton("Carrega imatges i crea histogrames", this);
    //show_btn->move(20, 50);
    show_btn->setMinimumWidth(250);

    empty_btn = new QPushButton("Mostra imatges mes semblants", this);
    //empty_btn->move(20, 80);
    empty_btn->setMinimumWidth(250);

    percentatge = new QLabel("");
    //percentatge->move(200,110);
    percentatge->show();

    accio = new QLabel("");
    //accio->move(20,110);
    accio->show();

    QHBoxLayout *layout2 = new QHBoxLayout();

    layout->addWidget(show_btn);
    layout->addWidget(empty_btn);
    layout->addLayout(layout2);
    layout2->addWidget(accio);
    layout2->addWidget(percentatge);

	connect(show_btn, SIGNAL(clicked()), this, SLOT(OnShowPressed()));
    connect(empty_btn, SIGNAL(clicked()), this, SLOT(comparaHist()));

    this->setMinimumSize(300,150);

}

void QMainMenu::comparaHist(){

    double linies = 0;
    QDirIterator it("../histogrames/", QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);
    while(it.hasNext()){
        linies++;
        it.next();
    }
    mult = 100 /linies;

    QWidget *imagesWidget = new QWidget();
    QGridLayout *grid = new QGridLayout(imagesWidget);

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "../", tr("Image Files (*.png *.jpg *.bmp)"));
    if (fileName == NULL){
        return;
    }

    QList<QString> filesList;

    filesList = histComp(fileName);
    QString tempFileName;
    QList<QImage> images;
    QFileInfo fileInfo;
    #pragma omp parallel for
    for(int i=0 ; i<filesList.size() ; i++){
        fileInfo = filesList.at(i);
        tempFileName = fileInfo.absoluteFilePath();
        Mat img;
        img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);
        QImage image = QImage((uchar*) img.data, img.cols, img.rows, img.step, QImage::Format_RGB888);
        QImage copy = image.scaled(200,200,Qt::KeepAspectRatio);
        images.append(copy);
    }

    for (int i = 0; i < 3; i+=1) {
        for(int j = 0; j < 3; j+=1) {
            QPixmap p0(QPixmap::fromImage(images[3*i+j]));
            QLabel *label = new QLabel(imagesWidget);
            label->setPixmap(p0);
            grid->addWidget(label, i, j);
        }
    }
    imagesWidget->setLayout(grid);
    imagesWidget->show();
}


bool QMainMenu::compare(const hist& first, const hist& second)
{
    return(first.c < second.c);
}


void QMainMenu::OnShowPressed()
{
    show_btn->setDisabled(true);
    empty_btn->setDisabled(true);

    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open Image"), "../", tr("Text Files (*.txt)"));



	QString tempFileName;
	QList<QString> filesList;
    //filesList << "../pro/image1.png" << "../pro/image2.png" << "../pro/image3.png";
    filesList = fillList("../pro/input.txt");

    int counter = 0;
    accio->setText("Generant histogrames");
    percentatge->setText("0%");

    const clock_t begin_time = clock();

    QFileInfo fileInfo;

    int i = 0;
    int n = filesList.size();

   #pragma omp parallel
   {
        #pragma omp for
        for( i=0 ; i<n; i++){
            fileInfo = filesList.at(i);
            //counter++;
            //percentatge->setText(QString::number(counter*mult)+"%");
            tempFileName = fileInfo.absoluteFilePath();
            Mat img;
            img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

            saveHistogram(img);
        }
}
    cout<<"Generar histogrames: ";
    cout<<float(clock() - begin_time)/CLOCKS_PER_SEC<<endl;

    QMessageBox msgBox;
    msgBox.setText("Fet!");
    msgBox.exec();

    show_btn->setDisabled(false);
    empty_btn->setDisabled(false);
    accio->setText("");
    percentatge->setText("");

}



void QMainMenu::saveHistogram(Mat im)
{


      /// Load two images with different environment settings
      if( !im.data )
        { return; }

      vector<Mat> m = histogram(im);

      Mat hist_h, hist_s, hist_v;

      hist_h = m.at(0);
      hist_s = m.at(1);
      hist_v = m.at(2);


      QString path = QString("../histogrames/") + getName("hist_",hCont++) + ".xml";
      // Store histograms on disc
      FileStorage fs(path.toLatin1().data(), FileStorage::WRITE);

      fs << "hist_h" << hist_h;
      fs << "hist_s" << hist_s;
      fs << "hist_v" << hist_v;

      fs.release();


      //histComp("../histogrames/hi_0.xml");

    //waitKey(0);

}

vector<Mat> QMainMenu::histogram(Mat src)
{

    Mat hsv;
    vector<Mat> hsv_planes;

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

    vector<Mat> m;
    m.push_back(hist_h);
    m.push_back(hist_s);
    m.push_back(hist_v);
    return m;
}


QList<QString> QMainMenu::histComp(QString path1){

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

  QString tempFileName = QFileInfo(path1).absoluteFilePath();
  Mat img;
  img = imread(tempFileName.toStdString(), CV_LOAD_IMAGE_COLOR);

  vector<Mat> m = this->histogram(img);

  hist_h1 = m.at(0);
  hist_s1 = m.at(1);
  hist_v1 = m.at(2);

  vector<hist> histoList;


  QString path2;

    QDirIterator it("../histogrames/", QStringList() << "*.xml", QDir::Files, QDirIterator::Subdirectories);

    int counter = 0;
    accio->setText("Comparant histogrames");
    percentatge->setText("0%");

    const clock_t begin_time = clock();

    // paralelitzacio aqui
    while (it.hasNext()) {

        counter++;
        percentatge->setText(QString::number(counter*mult)+"%");

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

    }

    cout<<"Comparar Histogrames: ";
    cout<<float(clock() - begin_time)/CLOCKS_PER_SEC<<endl;

    std::sort(histoList.begin(),histoList.end(), compare);

    QList<QString> filesList;

    QString p;
    for(int i; i<9; i++){
        QStringList pL = QFileInfo(histoList.at(i).path).baseName().split("_");
        filesList << "../imatges/img_"+pL.at(1)+".jpg";
    }

    return filesList;
}



QList<QString> QMainMenu::fillList(QString file)
{
    QString dir_path = QFileInfo(file).absolutePath();
    QList<QString> filesList;
    string f = file.toStdString();
    ifstream test(f.c_str());
    double counter = 0;
    double c = count(istreambuf_iterator<char>(test), istreambuf_iterator<char>(), '\n');
    mult = 100 / c;
    ifstream fin(f.c_str());
    string line;
    int i = 1;
    accio->setText("Guardant imatges");
    percentatge->setText("0%");

    const clock_t begin_time = clock();

#pragma omp parallel
    {
    while(fin >> line){
        counter++;
        percentatge->setText(QString::number(counter*mult)+"%");
        // passem la linie(std::str) a QString
        QString qline = dir_path+"/images/"+ QString::fromStdString(line);

        // Configurem l'ImageWriter
        QString name = getName("img_",i++);
        QImageWriter imw("../imatges/"+ name +".jpg","jpg");

        // Creem i escribim la imatge
        QImage image;
        image.load(qline,"jpg");
        imw.write(image);

        // L'afegim a la llista
        filesList << qline;
    }
}
    cout<<"carregar fitxers a memòria ";
    cout<<float(clock() - begin_time)/CLOCKS_PER_SEC<<endl;
    return filesList;
}

QString QMainMenu::getName(QString pre, int n)
{
    int aux = n;
    int count = 0;
    string name = pre.toStdString();
    QString qname;
    while(aux){
        count++;
        aux/=10;
    }
    for(int i=0 ; i<6-count ; i++)
        name+="0";
    name=name+boost::lexical_cast<std::string>(n);
    return qname.fromStdString(name);
}

