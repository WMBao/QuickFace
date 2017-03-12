#include "widget.h"
#include "ui_QuickFace.h"
#include "WinsockMatTransmissionClient.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::QuickFace)
{
    ui->setupUi(this);

    timer1   = new QTimer(this);
    timer2   = new QTimer(this);
    state=0;
    QObject::connect(timer1,SIGNAL(timeout()),this,SLOT(ShowPicSlot()));
    QObject::connect(timer2,SIGNAL(timeout()),this,SLOT(ParseRecvData()));
    QObject::connect(ui->StartCam,SIGNAL(clicked()),this,SLOT(openCamaraSlot()));
    QObject::connect(ui->Quit,SIGNAL(clicked()),this,SLOT(QuitSlot()));
    QObject::connect(ui->SaveObj,SIGNAL(clicked()),this,SLOT(SaveSlot()));
    QObject::connect(ui->Mode,SIGNAL(clicked()),this,SLOT(ChangeMode()));
}

Widget::~Widget()
{
    delete ui;
}

void Widget::openCamaraSlot()
{

    //WinsockMatTransmissionClient socketMat;
    if(socketMat.socketConnect("202.120.38.35",12000)<0)
    {
        qDebug()<< "socket Mat error"<<endl;
        QMessageBox::information(NULL, "ERROR", "Server Failure!", QMessageBox::Ok, QMessageBox::Ok);
        this->close();
    }

    if(socketLog.socketConnect("202.120.38.35",12001)<0)
    {
        qDebug()<< "socket Log error"<<endl;
        //QMessageBox::information(NULL, "ERROR", "Server Failure!", QMessageBox::Ok, QMessageBox::Ok);
        this->close();
    }

    if(socketFile.socketConnect("202.120.38.35",12002)<0)
    {
        qDebug()<< "socket File error"<<endl;
        //QMessageBox::information(NULL, "ERROR", "Server Failure!", QMessageBox::Ok, QMessageBox::Ok);
        this->close();
    }

    if(socketFile2.socketConnect("202.120.38.35",12003)<0)
    {
        qDebug()<< "socket File2 error"<<endl;
        //QMessageBox::information(NULL, "ERROR", "Server Failure!", QMessageBox::Ok, QMessageBox::Ok);
        this->close();
    }
    connected=true;
    memset(key, 0, STR_BUFFER_SIZE);
//    flag = 0;

    char fn0[] = "Client Initialized.";
    socketLog.transmit(fn0);

    capture.open(0);
    if( !capture.isOpened())
    {
       qDebug()<< "open camera error"<<endl;
       QMessageBox::information(NULL, "ERROR", "Camera Failure!", QMessageBox::Ok, QMessageBox::Ok);
       this->close();
    }
    else
    {
        timer1->start(33);
        timer2->start(10);
    }
}

void Widget::ShowPicSlot()
{
    if(!connected){
        QMessageBox::information(NULL, "Error", "Please open the camera first!", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }

    timer1->stop();
    capture >> frame;
    if(!frame.empty())
    {
        img1 = QImage((const unsigned char*)(frame.data),frame.cols,frame.rows,
                     QImage::Format_RGB888).rgbSwapped();
        img1 = img1.scaled(
                            ui->CapturePic->width(),
                            ui->CapturePic->height(),
                            Qt::IgnoreAspectRatio,
                            Qt::FastTransformation
                        );//调整图片大小，使图片和显示的label控件一样大
        ui->CapturePic->setPixmap(QPixmap::fromImage(img1));
    }

    socketMat.transmit(frame);

    if(socketMat.receive(Model)>0)
    {
        if(!Model.empty()&& state==0)
        {
            img2 = QImage((const unsigned char*)(Model.data),Model.cols,Model.rows,
                         QImage::Format_RGBA8888).rgbSwapped();
            img2 = img2.scaled(
                                ui->ModelPic->width(),
                                ui->ModelPic->height(),
                                Qt::IgnoreAspectRatio,
                                Qt::FastTransformation
                            );//调整图片大小，使图片和显示的label控件一样大
            ui->ModelPic->setPixmap(QPixmap::fromImage(img2));
        }
        if(!Model.empty()&& state==1)//Game of Thrones mode
        {
            printf("reading\n\n");
            //background =imread("GameofThrones.png");
            Ptr<IplImage>  img = cvLoadImage("image/GameofThrones2.jpg",-1);
            Mat background(img);
            if( !background.data )
            {
              printf( "No image data \n " );
            }
            uchar* groundData = new uchar[background.total()*4];
            Mat backgroundRGBA(background.size(), CV_8UC4, groundData);
            cv::cvtColor(background, backgroundRGBA, CV_BGR2RGBA, 4);
            printf("read background OK");
            //Mat background4(IMG_HEIGHT,IMG_WIDTH,CV_8UC4,cv::Scalar(0));
            //cvtColor( background, background4, CV_BGR2BGRA );
            addWeighted( backgroundRGBA, 1, Model, 0.7, 0.0,Model);
            printf("addweight OK");
            img2 = QImage((const unsigned char*)(Model.data),Model.cols,Model.rows,
                         QImage::Format_RGBA8888);//.rgbSwapped();
            img2 = img2.scaled(
                                ui->ModelPic->width(),
                                ui->ModelPic->height(),
                                Qt::IgnoreAspectRatio,
                                Qt::FastTransformation
                            );//调整图片大小，使图片和显示的label控件一样大
            ui->ModelPic->setPixmap(QPixmap::fromImage(img2));
        }
        if(!Model.empty()&& state==2)
        {
            printf("reading\n\n");
            //background =imread("GameofThrones.png");
            Ptr<IplImage>  img = cvLoadImage("/image/BottleHead.jpg",-1);
            Mat background(img);
            if( !background.data )
            {
              printf( "No image data \n " );
            }
            uchar* groundData = new uchar[background.total()*4];
            Mat backgroundRGBA(background.size(), CV_8UC4, groundData);
            cv::cvtColor(background, backgroundRGBA, CV_BGR2BGRA, 4);
            printf("read background OK");
            //Mat background4(IMG_HEIGHT,IMG_WIDTH,CV_8UC4,cv::Scalar(0));
            //cvtColor( background, background4, CV_BGR2BGRA );
            addWeighted( backgroundRGBA, 0.7, Model, 1, 0.0,Model);
            printf("addweight OK");
            img2 = QImage((const unsigned char*)(Model.data),Model.cols,Model.rows,
                         QImage::Format_RGBA8888).rgbSwapped();
            img2 = img2.scaled(
                                ui->ModelPic->width(),
                                ui->ModelPic->height(),
                                Qt::IgnoreAspectRatio,
                                Qt::FastTransformation
                            );//调整图片大小，使图片和显示的label控件一样大
            ui->ModelPic->setPixmap(QPixmap::fromImage(img2));
        }
    }
    timer1->start(1);
}

void Widget::SaveSlot()
{
    if(!connected){
        QMessageBox::information(NULL, "Error", "Please open the camera first!", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    char k[] = "s";
    socketLog.transmit(k);
    recvObjFlag = true;
    recvPngFlag = true;
    //recvFlag = true;
//    flag =true;
}

void Widget::QuitSlot()
{
    this->close();
}

void Widget::ChangeMode()
{
    if(!connected){
        QMessageBox::information(NULL, "Error", "Please open the camera first!", QMessageBox::Ok, QMessageBox::Ok);
        return;
    }
    if(state==2)
        state=0;
    else
        state=state+1;
}

void Widget::ParseRecvData()
{
    timer2->stop();
//    tempL = socketLog.receive(key);
//    printf("Receiving %s\n", tempL);
//    ui->LogInfo->setText((QString &)key);
//    if (tempL != NULL)
//    {
//        if (tempL[0] == 'f') flag = true;
//    }
    if (recvObjFlag)
    {
        char fn1[] = "current_merged.obj";
        if (socketFile.receive(fn1))
            printf("obj failed.\n");
        else{
            printf("obj succeed.\n");
            recvObjFlag = false;
        }
    }
    if (recvPngFlag)
    {
        char fn2[] = "current_merged.isomap.png";
        if (socketFile2.receive(fn2))
            printf("png failed.\n");
        else{
            printf("png succeed.\n");
            recvPngFlag = false;
            recvFlag=true;
        }
    }
    if(recvFlag){
         QMessageBox::information(NULL, "Notice", "Current model files saved!", QMessageBox::Ok, QMessageBox::Ok);
         QMessageBox::information(NULL, "Error", "Server failure!", QMessageBox::Ok, QMessageBox::Ok);
         recvFlag=false;
    }

    timer2->start(1);
}

//void Widget::closeCamaraSlot()
//{
//    timer->stop();
//    ui->cameraPic->setPixmap(QPixmap());
//}
