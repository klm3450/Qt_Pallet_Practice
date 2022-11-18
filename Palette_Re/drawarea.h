#ifndef DRAWAREA_H
#define DRAWAREA_H
#include <QWidget>
#include <QPushButton>
#include <QGridLayout>
#include <QImage>
#include <QPoint>
#include <QColor>


class Drawarea : public QWidget
{
    Q_OBJECT

public:
    Drawarea(QWidget *parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent *event) override; // 마우스 클릭 시 이벤트
    void mouseMoveEvent(QMouseEvent *event) override; // 마우스 이동 시 이벤트
    void mouseReleaseEvent(QMouseEvent *event) override;
    void paintEvent(QPaintEvent *event) override;

private:
    QPushButton *pen;
    QPushButton *line;
    QPushButton *Rectangle;
    QPushButton *Circle;
    QPushButton *allclear;
    //-- 버튼 모음 --//

    QImage *image;
    QPoint recent_point;
    //-- 이미지 & 마우스포인터 최근 위치 --//

    QColor myPenColor = Qt::blue;
    int myPenWidth = 3;
    //-- 펜 색 및 굵기 --//


    bool scribbling = false;
    //-- 그림을 그리고 있는지의 여부 --//

    QGridLayout *based_layout;
    QGridLayout *draw_layout;
    //-- 베이스가 되는 layout, 그림그리는 layout --//

public:
    enum class mode{
        pen_mode,
        line_mode,
        rect_mode,
        circle_mode
    };
    Q_ENUM(mode)

    mode draw_mode = mode::pen_mode;
    //-- 그림 그리는 모드에 대한 부분 --//


protected slots:
    void Pen_btn_clicked();
    void Line_btn_clicked();
    void Rectangle_btn_clicked();
    void Cricle_btn_clicked();
    void allclear_btn_clicked();
    //-- 버튼이 클릭되었을 때 동작할 slots --//

};

#endif // DRAWAREA_H
