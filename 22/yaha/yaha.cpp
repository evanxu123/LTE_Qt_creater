#include "yaha.h"
#include "ui_yaha.h"

yaha::yaha(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::yaha)
{
    ui->setupUi(this);
}

yaha::~yaha()
{
    delete ui;
}
