/********************************************************************************
** Form generated from reading UI file 'snr.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNR_H
#define UI_SNR_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_SNR
{
public:

    void setupUi(QDialog *SNR)
    {
        if (SNR->objectName().isEmpty())
            SNR->setObjectName(QStringLiteral("SNR"));
        SNR->resize(400, 300);

        retranslateUi(SNR);

        QMetaObject::connectSlotsByName(SNR);
    } // setupUi

    void retranslateUi(QDialog *SNR)
    {
        SNR->setWindowTitle(QApplication::translate("SNR", "SNR", 0));
    } // retranslateUi

};

namespace Ui {
    class SNR: public Ui_SNR {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNR_H
