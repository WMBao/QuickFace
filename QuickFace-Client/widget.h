#ifndef WIDGET_H
#define WIDGET_H
#include "WinsockMatTransmissionClient.h"

#include <QWidget>
#include <QDialog>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;

namespace Ui {
class QuickFace;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    WinsockMatTransmissionClient socketMat;
    WinsockLogTransmissionClient socketLog;
    WinsockFileTransmissionClient socketFile;
    WinsockFileTransmissionClient socketFile2;
    Mat frame;//Video Frame
    Mat Model;//3D Model
    QImage img1;
    QImage img2;
    Mat background;
    VideoCapture capture;
    int state;
    QTimer *timer1;
    QTimer *timer2;
    char key[STR_BUFFER_SIZE];
    char * tempL = NULL;
    bool flag;
    bool recvObjFlag = false;
    bool recvPngFlag = false;
    bool recvFlag = false;
    bool connected=false;

private:
    Ui::QuickFace *ui;

private slots:
   void openCamaraSlot();
   void SaveSlot();
   void QuitSlot();
   void ShowPicSlot();
   void ChangeMode();
   void ParseRecvData();
};

#endif // WIDGET_H
