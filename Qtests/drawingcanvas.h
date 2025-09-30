#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QPen>
#include <QBrush>
#include <vector>

class DrawingCanvas : public QGraphicsView {
    Q_OBJECT
public:
    DrawingCanvas(QWidget *parent=nullptr);

    void addPoint(const QPointF &p);
    void clearAll();
    void computeAndDrawHull();
    void computeAndDrawSlowHull();



protected:
    void mousePressEvent(QMouseEvent *event) override;


private:
    QGraphicsScene *scene;
    std::vector<QPointF> points;
    QPen gridPen, pointPen, hullPen;
    QBrush pointBrush;
    int iterationCount = 0; 


    void drawPoint(const QPointF &p);
    void drawIterationCount(int count);
};

#endif // DRAWINGCANVAS_H
