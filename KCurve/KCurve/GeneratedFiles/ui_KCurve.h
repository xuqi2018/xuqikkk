/********************************************************************************
** Form generated from reading UI file 'KCurve.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_KCURVE_H
#define UI_KCURVE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_KCurveClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *KCurveClass)
    {
        if (KCurveClass->objectName().isEmpty())
            KCurveClass->setObjectName(QString::fromUtf8("KCurveClass"));
        KCurveClass->resize(600, 400);
        menuBar = new QMenuBar(KCurveClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        KCurveClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(KCurveClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        KCurveClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(KCurveClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        KCurveClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(KCurveClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        KCurveClass->setStatusBar(statusBar);

        retranslateUi(KCurveClass);

        QMetaObject::connectSlotsByName(KCurveClass);
    } // setupUi

    void retranslateUi(QMainWindow *KCurveClass)
    {
        KCurveClass->setWindowTitle(QApplication::translate("KCurveClass", "KCurve", nullptr));
    } // retranslateUi

};

namespace Ui {
    class KCurveClass: public Ui_KCurveClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_KCURVE_H
