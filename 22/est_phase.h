#ifndef EST_PHASE_H
#define EST_PHASE_H

#include <QDialog>
#include <QPaintEvent>

namespace Ui {
class EST_Phase;
}

class EST_Phase : public QDialog
{
    Q_OBJECT

public:
    explicit EST_Phase(QWidget *parent = 0);
    ~EST_Phase();

    void paintEvent(QPaintEvent *);
    int receiver[512][2];
    int phases[300];
    int grid_num[2];
    double interval[2];
    double transfered_coordinates[300][2];
    double valued_coordinates_interval[2];
    double phase(double x, double y);

private:
    Ui::EST_Phase *ui;
};

#endif // EST_PHASE_H
