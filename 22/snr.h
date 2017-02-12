#ifndef SNR_H
#define SNR_H

#include <QDialog>
#include <QPaintEvent>

namespace Ui {
class SNR;
}

class SNR : public QDialog
{
    Q_OBJECT

public:
    explicit SNR(QWidget *parent = 0);
    ~SNR();

    void paintEvent(QPaintEvent *);
    int temp;
    double snr[100];
    int grid_num[2];
    double interval[2];
    double transfered_coordinates[100][2];
    double valued_coordinates_interval[2];

private:
    Ui::SNR *ui;
};

#endif // SNR_H
