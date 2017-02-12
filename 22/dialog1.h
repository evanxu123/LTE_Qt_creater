#ifndef DIALOG1
#define DIALOG1
#include<QTimer>
#include<QPainter>
#include<qmath.h>
#include <QtGui/QPainter>
#include <QPen>
#include <QBrush>
#include <QDateTime>
#include <QTimer>
#include<stdio.h>
#include <QDialog>

namespace Ui {
class Dialog1;
}

class Dialog1 : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog1(QWidget *parent = 0);
    ~Dialog1();
    QPoint Point;
    void paintEvent1(QPaintEvent *);
    void paintEventa1(QPaintEvent *);

private:
    Ui::Dialog1 *ui;
    QPainterPath *path;
};


#endif // DIALOG1

