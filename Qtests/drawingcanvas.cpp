#include "drawingcanvas.h"
#include <QMouseEvent>
#include <QGraphicsEllipseItem>
#include <QGraphicsPolygonItem>
#include <QGraphicsTextItem> 
#include <QtMath>
#include <algorithm>
#include <vector>


static double cross(const QPointF &o, const QPointF &a, const QPointF &b) {
    return (a.x() - o.x()) * (b.y() - o.y()) - (a.y() - o.y()) * (b.x() - o.x());
}


static std::vector<QPointF> convexHull(std::vector<QPointF> pts, int& counter) {
    counter = 0;
    if (pts.size() < 3) return pts;


    std::sort(pts.begin(), pts.end(), [](const QPointF &a, const QPointF &b) {
        if (a.x() != b.x()) return a.x() < b.x();
        return a.y() < b.y();
    });

    std::vector<QPointF> Lupper;
    Lupper.reserve(pts.size());
    Lupper.push_back(pts[0]);
    Lupper.push_back(pts[1]);

    for (size_t i = 2; i < pts.size(); ++i) {

        Lupper.push_back(pts[i]);

        while (Lupper.size() > 2) {
            size_t n = Lupper.size();
            const QPointF &c = Lupper[n - 1];
            const QPointF &b = Lupper[n - 2];
            const QPointF &a = Lupper[n - 3];

            counter++;
            if (cross(a, b, c) >= 0) {
                Lupper.erase(Lupper.end() - 2);
            } else {
                break;
            }
        }
    }


    std::vector<QPointF> Llower;
    Llower.reserve(pts.size());


    Llower.push_back(pts.back());
    Llower.push_back(pts[pts.size() - 2]);


    for (int i = pts.size() - 3; i >= 0; --i) {

        Llower.push_back(pts[i]);


        while (Llower.size() > 2) {
            size_t n = Llower.size();
            const QPointF &c = Llower[n - 1];
            const QPointF &b = Llower[n - 2];
            const QPointF &a = Llower[n - 3];

            counter++;
            if (cross(a, b, c) >= 0) {
                Llower.erase(Llower.end() - 2);
            } else {
                break;
            }
        }
    }


    if (Llower.size() > 0) Llower.erase(Llower.begin());
    if (Llower.size() > 0) Llower.pop_back();


    Lupper.insert(Lupper.end(), Llower.begin(), Llower.end());


    std::reverse(Lupper.begin(), Lupper.end());


    return Lupper;
}


static std::vector<QPointF> slowCConvexHull(std::vector<QPointF> pts, int& counter) {
    counter = 0;
    size_t N = pts.size();
    if (N < 3) return pts;


    std::vector<std::pair<QPointF, QPointF>> E;


    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {


            counter++;

            if (i == j) continue;

            const QPointF &p = pts[i];
            const QPointF &q = pts[j];


            bool valid = true;


            for (size_t k = 0; k < N; ++k) {
                if (k == i || k == j) continue;

                const QPointF &r = pts[k];

                counter++;

                if (cross(p, q, r) > 0) {
                    valid = false;
                    break;
                }
            }


            if (valid) {
                E.push_back({p, q});
            }
        }
    }


    if (E.empty()) return {};

    std::vector<QPointF> hull;
    QPointF current = E[0].first;
    QPointF startPoint = current;

    do {
        hull.push_back(current);

        QPointF next;
        bool foundNext = false;


        counter++;

        for (const auto &edge : E) {
            if (edge.first == current) {
                next = edge.second;
                foundNext = true;
                break;
            }
        }

        if (!foundNext) break;
        current = next;

    } while (current != startPoint && hull.size() <= E.size());

    return hull;
}



DrawingCanvas::DrawingCanvas(QWidget *parent) : QGraphicsView(parent)
    , iterationCount(0)
{
    scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing, true);
    setBackgroundBrush(QBrush(QColor(250,250,250)));

    gridPen = QPen(QColor(235,235,235));
    pointPen = QPen(Qt::black);
    pointBrush = QBrush(QColor(30,144,255));
    hullPen = QPen(Qt::red);
    hullPen.setWidth(2);

    setSceneRect(0,0,600,400);
}

void DrawingCanvas::addPoint(const QPointF &p) {
    points.push_back(p);
    drawPoint(p);
}

void DrawingCanvas::clearAll() {
    points.clear();
    scene->clear();
    iterationCount = 0;
}

void DrawingCanvas::drawIterationCount(int count) {
    QGraphicsTextItem *text = new QGraphicsTextItem(QString("Iterations: %1").arg(count));
    text->setFont(QFont("Arial", 12, QFont::Bold));
    text->setDefaultTextColor(Qt::darkCyan);
    text->setPos(10, 10);
    scene->addItem(text);
}

void DrawingCanvas::computeAndDrawHull() {
    scene->clear();
    for (const QPointF &p : points) drawPoint(p);
    if (points.size() < 3) return;

    auto hull = convexHull(points, iterationCount);

    QPolygonF poly;
    for (const QPointF &p : hull) poly << p;

    if (poly.size() >= 3) {
        scene->addPolygon(poly, hullPen, QBrush(QColor(255,0,0,50)));
        
        for (const QPointF &p : points) drawPoint(p);
    } else if (hull.size() == 2) {
        scene->addLine(QLineF(hull[0], hull[1]), hullPen);
        
        for (const QPointF &p : points) drawPoint(p);
    }

    
    drawIterationCount(iterationCount);
}

void DrawingCanvas::computeAndDrawSlowHull() {
    scene->clear();
    for (const QPointF &p : points) drawPoint(p);
    if (points.size() < 3) return;

    auto hull = slowCConvexHull(points, iterationCount);

    QPolygonF poly;
    for (const QPointF &p : hull) poly << p;

    if (poly.size() >= 3) {
        scene->addPolygon(poly, hullPen, QBrush(QColor(255,0,0,50)));
        
        for (const QPointF &p : points) drawPoint(p);
    } else if (hull.size() == 2) {
        scene->addLine(QLineF(hull[0], hull[1]), hullPen);
        
        for (const QPointF &p : points) drawPoint(p);
    }

    
    drawIterationCount(iterationCount);
}

void DrawingCanvas::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        QPointF scenePt = mapToScene(event->pos());
        addPoint(scenePt);
    }
    QGraphicsView::mousePressEvent(event);
}

void DrawingCanvas::drawPoint(const QPointF &p) {
    double r = 4.5;
    scene->addEllipse(p.x()-r, p.y()-r, 2*r, 2*r, pointPen, pointBrush);
}
