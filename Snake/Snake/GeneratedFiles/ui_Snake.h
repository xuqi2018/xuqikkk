/********************************************************************************
** Form generated from reading UI file 'Snake.ui'
**
** Created by: Qt User Interface Compiler version 5.12.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SNAKE_H
#define UI_SNAKE_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_SnakeClass
{
public:
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QWidget *centralWidget;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *SnakeClass)
    {
        if (SnakeClass->objectName().isEmpty())
            SnakeClass->setObjectName(QString::fromUtf8("SnakeClass"));
        SnakeClass->resize(600, 400);
        menuBar = new QMenuBar(SnakeClass);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        SnakeClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(SnakeClass);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        SnakeClass->addToolBar(mainToolBar);
        centralWidget = new QWidget(SnakeClass);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        SnakeClass->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(SnakeClass);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        SnakeClass->setStatusBar(statusBar);

        retranslateUi(SnakeClass);

        QMetaObject::connectSlotsByName(SnakeClass);
    } // setupUi

    void retranslateUi(QMainWindow *SnakeClass)
    {
        SnakeClass->setWindowTitle(QApplication::translate("SnakeClass", "Snake", nullptr));
    } // retranslateUi

};

namespace Ui {
    class SnakeClass: public Ui_SnakeClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SNAKE_H
