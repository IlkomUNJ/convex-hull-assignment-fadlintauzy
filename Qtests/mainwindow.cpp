#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTime>
#include <cstdlib>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent) {
    auto *central = new QWidget;
    auto *layout = new QVBoxLayout;

    canvas = new DrawingCanvas;
    layout->addWidget(canvas, 1);

    auto *hbox = new QHBoxLayout;

    QPushButton *btnRunFast = new QPushButton("Convex Hull (Fast)");


    QPushButton *btnRunSlow = new QPushButton("Slow Convex Hull");

    QPushButton *btnClear = new QPushButton("Clear");

    hbox->addWidget(btnRunFast);
 
    hbox->addWidget(btnRunSlow);
    hbox->addWidget(btnClear);
    hbox->addStretch();

    layout->addLayout(hbox);
    central->setLayout(layout);
    setCentralWidget(central);
    setWindowTitle("Convex Hull Simulator - Qt");

 
    connect(btnRunFast, &QPushButton::clicked, this, [this]{ canvas->computeAndDrawHull(); });

    
    connect(btnRunSlow, &QPushButton::clicked, this, [this]{ canvas->computeAndDrawSlowHull(); });

    connect(btnClear, &QPushButton::clicked, this, [this]{ canvas->clearAll(); });
}
