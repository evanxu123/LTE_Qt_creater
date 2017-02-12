/********************************************************************************
** Form generated from reading UI file 'est_phase.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_EST_PHASE_H
#define UI_EST_PHASE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_EST_Phase
{
public:

    void setupUi(QDialog *EST_Phase)
    {
        if (EST_Phase->objectName().isEmpty())
            EST_Phase->setObjectName(QStringLiteral("EST_Phase"));
        EST_Phase->resize(400, 300);

        retranslateUi(EST_Phase);

        QMetaObject::connectSlotsByName(EST_Phase);
    } // setupUi

    void retranslateUi(QDialog *EST_Phase)
    {
        EST_Phase->setWindowTitle(QApplication::translate("EST_Phase", "EST_Phase", 0));
    } // retranslateUi

};

namespace Ui {
    class EST_Phase: public Ui_EST_Phase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_EST_PHASE_H
