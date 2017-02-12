/********************************************************************************
** Form generated from reading UI file 'estimating_diagram.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_ESTIMATING_DIAGRAM_H
#define UI_ESTIMATING_DIAGRAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_Estimating_Diagram
{
public:

    void setupUi(QDialog *Estimating_Diagram)
    {
        if (Estimating_Diagram->objectName().isEmpty())
            Estimating_Diagram->setObjectName(QStringLiteral("Estimating_Diagram"));
        Estimating_Diagram->resize(400, 300);

        retranslateUi(Estimating_Diagram);

        QMetaObject::connectSlotsByName(Estimating_Diagram);
    } // setupUi

    void retranslateUi(QDialog *Estimating_Diagram)
    {
        Estimating_Diagram->setWindowTitle(QApplication::translate("Estimating_Diagram", "Estimating_Diagram", 0));
    } // retranslateUi

};

namespace Ui {
    class Estimating_Diagram: public Ui_Estimating_Diagram {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ESTIMATING_DIAGRAM_H
