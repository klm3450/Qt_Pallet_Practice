#include "widget.h"
#include "drawarea.h"
#include <QGridLayout>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    Drawarea *draw = new Drawarea(this);
    QGridLayout *draw_layout = new QGridLayout();
    draw->setFixedSize(600,600);
    draw_layout->addWidget(draw);
}

Widget::~Widget()
{
}

