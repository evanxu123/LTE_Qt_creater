/********************************************************************************
** Form generated from reading UI file 'constellationdiagram.ui'
**
** Created by: Qt User Interface Compiler version 5.2.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CONSTELLATIONDIAGRAM_H
#define UI_CONSTELLATIONDIAGRAM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>

QT_BEGIN_NAMESPACE

class Ui_ConstellationDiagram
{
public:

    void setupUi(QDialog *ConstellationDiagram)
    {
        if (ConstellationDiagram->objectName().isEmpty())
            ConstellationDiagram->setObjectName(QStringLiteral("ConstellationDiagram"));
        ConstellationDiagram->resize(400, 300);

        retranslateUi(ConstellationDiagram);

        QMetaObject::connectSlotsByName(ConstellationDiagram);
    } // setupUi

    void retranslateUi(QDialog *ConstellationDiagram)
    {
        ConstellationDiagram->setWindowTitle(QApplication::translate("ConstellationDiagram", "ConstellationDiagram", 0));
    } // retranslateUi

};

namespace Ui {
    class ConstellationDiagram: public Ui_ConstellationDiagram {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONSTELLATIONDIAGRAM_H
