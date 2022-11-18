#include "drawarea.h"
#include <QLabel>
#include <QMessageBox>

#include <QMouseEvent>
#include <QPainter>


#define btn_height 50


QImage *image_tmp;
Drawarea::Drawarea(QWidget *parent): QWidget(parent)
{
    image = QImage(600,600,QImage::Format_RGB32);
    image.fill(Qt::white);
    //-- <QPainter::begin: Paint device returned engine == 0, type: 3 해결> 이미지가 안그려져서 DrawArea 생성 시 아예 같이 생성--//

    image_tmp = new QImage(600,600,QImage::Format_RGB32);
    image_tmp -> fill(Qt::white);

    pen = new QPushButton("Pen", this);
    line = new QPushButton("Line", this);
    Rectangle = new QPushButton("Rect", this);
    Circle = new QPushButton("Circ", this);
    allclear = new QPushButton("Allclear", this);
    QString style_str = "QPushButton {background-color : #A9E2F3; font-weight : bold; border-radius : 10px; color : #0174DF; }";

    pen->setStyleSheet(style_str);
    line->setStyleSheet(style_str);
    Rectangle->setStyleSheet(style_str);
    Circle->setStyleSheet(style_str);
    allclear->setStyleSheet(style_str);

    pen->setFixedHeight(btn_height);
    line->setFixedHeight(btn_height);
    Rectangle->setFixedHeight(btn_height);
    Circle->setFixedHeight(btn_height);
    allclear->setFixedHeight(btn_height);

    connect(pen,SIGNAL(clicked(bool)),this,SLOT(Pen_btn_clicked()));
    connect(line,SIGNAL(clicked(bool)),this,SLOT(Line_btn_clicked()));
    connect(Rectangle,SIGNAL(clicked(bool)),this,SLOT(Rectangle_btn_clicked()));
    connect(Circle,SIGNAL(clicked(bool)),this,SLOT(Cricle_btn_clicked()));
    connect(allclear,SIGNAL(clicked(bool)),this,SLOT(allclear_btn_clicked()));



    based_layout = new  QGridLayout();
    based_layout->setHorizontalSpacing(30);
    based_layout->setVerticalSpacing(40);
    draw_layout = new QGridLayout();


    QGridLayout *btn_layout = new QGridLayout();
    btn_layout->setSpacing(1);
    btn_layout->addWidget(pen,1,0,1,1);
    btn_layout->addWidget(line,1,1,1,1);
    btn_layout->addWidget(Rectangle,1,2,1,1);
    btn_layout->addWidget(Circle,1,3,1,1);
    btn_layout->addWidget(allclear,1,4,1,1);

    based_layout -> addLayout(draw_layout,0,0,4,0);
    based_layout -> addLayout(btn_layout,5,0,1,1);
    this->setLayout(based_layout);

}

void Drawarea::Pen_btn_clicked(){
    draw_mode = mode::pen_mode;
}

void Drawarea::Line_btn_clicked(){
    draw_mode = mode::line_mode;
}

void Drawarea::Rectangle_btn_clicked(){
    draw_mode = mode::rect_mode;
}

void Drawarea::Cricle_btn_clicked(){
    draw_mode = mode::circle_mode;
}

void Drawarea::allclear_btn_clicked(){
    if(image.isNull()){}
    else{
       QPainter painter(&image);
       painter.eraseRect(0,0,600,600);
       *image_tmp = image.copy();
       update();
        }
    }

void Drawarea::mousePressEvent(QMouseEvent *mouse_event){
    if(mouse_event->button() == Qt::LeftButton){
        recent_point = mouse_event->pos();
        scribbling = true;
    }
}

void Drawarea::mouseMoveEvent(QMouseEvent *mouse_event){

    int rad = (myPenWidth / 2) + 2;//왜 이 숫자?
    if(mouse_event->buttons() == Qt::LeftButton && scribbling){
        *image_tmp = image.copy();

        QPoint endpoint = mouse_event->pos();

        if(draw_mode == mode::pen_mode){
            QPainter painter(&image); //기존 이미지에 그림

            painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter.drawLine(recent_point,endpoint);//메모리 내에서 이뤄지는 과정
            *image_tmp = image.copy();
            update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                         .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트
            recent_point = endpoint;
        }
        else if(draw_mode == mode::line_mode){

            QPainter painter_tmp(image_tmp); //temp 이미지에 그림
            painter_tmp.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            painter_tmp.drawLine(recent_point,endpoint);
            repaint();

            update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                         .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트
        }
        else if(draw_mode == mode::rect_mode){

           QPainter painter_tmp(image_tmp); //temp 이미지에 그림
           painter_tmp.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
           painter_tmp.drawRect(recent_point.x(),recent_point.y(),endpoint.x()-recent_point.x(),endpoint.y()-recent_point.y());
           repaint();


            update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                         .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트

        }
        else if(draw_mode == mode::circle_mode){

           QPainter painter_tmp(image_tmp); //temp 이미지에 그림
           painter_tmp.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
           painter_tmp.drawEllipse(recent_point,(endpoint.x()-recent_point.x()),(endpoint.y()-recent_point.y()));
           repaint();

            update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                         .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트

        }

    }
}


void Drawarea::paintEvent(QPaintEvent *event){
    QPainter painter(this);
    QRect dirtyRect = event->rect();
   // painter.drawImage(dirtyRect,image,dirtyRect);
    painter.drawImage(dirtyRect,*image_tmp,dirtyRect);
}


void Drawarea::mouseReleaseEvent(QMouseEvent *mouse_event){

    QPainter painter(&image);
    painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
    QPoint endpoint = mouse_event->pos();
    int rad = (myPenWidth / 2) + 2; //왜 이 숫자?

  if(mouse_event->button() == Qt::LeftButton && scribbling){

      if(draw_mode == mode::pen_mode){
          QPainter painter(&image); //기존 이미지에 그림

          painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
          painter.drawLine(recent_point,endpoint);//메모리 내에서 이뤄지는 과정
          update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                       .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트
          recent_point = endpoint;
      }
      else if(draw_mode == mode::line_mode){
          painter.drawLine(recent_point,endpoint);
          update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                       .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트
      }
      else if(draw_mode == mode::rect_mode){
          rad = (endpoint.x() - recent_point.x())*(endpoint.y() - recent_point.y());
          painter.drawRect(recent_point.x(),recent_point.y(),endpoint.x()-recent_point.x(),endpoint.y()-recent_point.y());
          update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                       .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트
      }
      else if(draw_mode == mode::circle_mode){
          rad = qMax((endpoint.x() - recent_point.x()),(endpoint.y() - recent_point.y()));
          painter.drawEllipse(recent_point,(endpoint.x()-recent_point.x()),(endpoint.y()-recent_point.y()));
          update(QRect(recent_point, endpoint).normalized() //QRect(QPoint &topLeft, QPoint &bottomRight) 정규화된(음수->양수)직사각형
                                       .adjusted(-rad, -rad, +rad, +rad)); //사각형의 기존 좌표의 dx1, dy1, dx2, dy2가 추가된 새 사각형 반환 후 업데이트
      }

  }
     if(image_tmp->isNull()){}
      else{
          QPainter painter(&image); //기존 이미지에 그림
          painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
          painter.drawImage(QPoint(0,0),*image_tmp);
          repaint();
      }
    scribbling = false;
    }


