#ifndef YAHA_H
#define YAHA_H

#include <QMainWindow>

namespace Ui {
class yaha;
}

class yaha : public QMainWindow
{
    Q_OBJECT

public:
    explicit yaha(QWidget *parent = 0);
    ~yaha();

private:
    Ui::yaha *ui;
};

#endif // YAHA_H
