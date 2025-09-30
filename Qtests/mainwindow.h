#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "drawingcanvas.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent=nullptr);

private:
    DrawingCanvas *canvas;

    void addRandomSample();
};

#endif // MAINWINDOW_H
