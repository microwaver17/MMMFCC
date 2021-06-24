#include "graph.h"
#include "settings.h"

#include <QGraphicsSimpleTextItem>
#include <QtDebug>
#include <cmath>

Graph::Graph(QObject *parent) : QObject(parent)
  , isHideCurrentGraph(false)
  , isAutoScele(Settings::getInstance().isAutoScale)
  , graphType(GraphType::Line)
  , width(0)
  , height(0)
  , scale(Settings::getInstance().default_scale * Settings::getInstance().scale_multiple)
  , scene(0, 0, width - x_margin, height - y_margin)
{
}

void Graph::setSceneSize(int width, int height)
{
    this->width = width - x_margin;
    this->height = height - y_margin;
    scene.setSceneRect(0, 0, this->width, this->height);
}

void Graph::setScale(double scale)
{
    this->scale = scale * Settings::getInstance().scale_multiple;
}

void Graph::plotData(QVector<double> data)
{
    current = data;
    paint();
}

void Graph::freeze1Graph()
{
    if (freeze1.size() == 0){
        freeze1 = current;
    }else{
        freeze1.clear();
    }
    paint();
}

void Graph::freeze2Graph()
{
    if (freeze2.size() == 0){
        freeze2 = current;
    }else{
        freeze2.clear();
    }
    paint();
}

void Graph::clearFreeze()
{
    freeze1.clear();
    freeze2.clear();
}

QVector<double> normalize(QVector<double> data)
{
    return data.mid(1);    // F0除去
}

void Graph::paint()
{
    const QPen currentPen(Qt::blue, Settings::getInstance().lineSize);
    const QPen freeze1Pen(Qt::green, Settings::getInstance().lineSize);
    const QPen freeze2Pen(Qt::magenta, Settings::getInstance().lineSize);
    const QPen gridPen(Qt::gray, 1);
    QFont labelFont("Arial", 12);
    labelFont.setStyleHint(QFont::SansSerif);

    QVector<double> n_current = normalize(current);
    QVector<double> n_freeze1 = normalize(freeze1);
    QVector<double> n_freeze2 = normalize(freeze2);

    scene.clear();
    paintGrid(n_current.size(), gridPen);
    paintLabel(n_current.size(), labelFont);

    if (graphType == GraphType::Line){
        paintLine(n_freeze1, freeze1Pen);
        paintLine(n_freeze2, freeze2Pen);
        if (!isHideCurrentGraph){
            paintLine(n_current, currentPen);
        }
    }else if (graphType == GraphType::Bar){
        paintBar(n_freeze1, freeze1Pen.brush());
        paintBar(n_freeze2, freeze2Pen.brush());
        if (!isHideCurrentGraph){
            paintBar(n_current, currentPen.brush());
        }
    }
}

void Graph::paintGrid(int n, QPen pen)
{
    double x_distance = (double)width / (n - 1);
    double y_center = (double)height / 2;
    scene.addLine(0, y_center, width, y_center);
    scene.addLine(0, 0, width, 0);
    scene.addLine(0, height, width,  height);
    for(int i = 0; i < n; i++){
        int x = i * x_distance;
        scene.addLine(x, 0, x, height, pen);
    }
}

void Graph::paintLabel(int n, QFont font)
{
    double x_distance = (double)width / (n - 1);
    for(int i = 0; i < n - 1; i++){
        QPointF pos(i * x_distance + 5, 5);

        scene.addSimpleText(QString::number(i + 1), font)->setPos(pos);
    }
}

double sigmoid(double x){
    return x / (1.0 + abs(x));
}

void Graph::paintLine(QVector<double> &data, QPen pen)
{
    double x_distance = (double)width / (data.size() - 1);
    double y_center = (double)height / 2;
    for (int i = 0; i < data.size() - 1; i++){
        double val = data.at(i);
        double val_next = data.at(i + 1);
        if (isAutoScele){
            val = sigmoid(val) * y_center;
            val_next = sigmoid(val_next) * y_center;
        }else{
            val *= scale;
            val_next *= scale;
        }
        double x_0 = x_distance * i;
        double y_0 = y_center - val;   // 座標が値と+-逆
        double x_1 = x_distance * (i + 1);
        double y_1 = y_center - val_next;
        scene.addLine(x_0, y_0, x_1, y_1, pen);
    }
}

void Graph::paintBar(QVector<double> &data, QBrush brush)
{
    QColor color = brush.color();
    color.setAlphaF(0.5);
    brush.setColor(color);
    double x_distance = (double)width / (data.size() - 1);
    double y_center = (double)height / 2;
    for (int i = 0; i < data.size() - 1; i++){
        double val = data.at(i);
        if (isAutoScele){
            val = sigmoid(val) * y_center;
        }else{
            val *= scale;
        }
        double x_0, y_0, x_1, y_1;
        if (val > 0 ){
            x_0 = x_distance * i;
            y_0 = y_center - val;   // 座標が値と+-逆
            x_1 = x_distance * (i + 1);
            y_1 = y_center;
        }else{
            x_0 = x_distance * i;
            y_0 = y_center;
            x_1 = x_distance * (i + 1);
            y_1 = y_center - val;   // 座標が値と+-逆
        }
        QRectF rect(QPointF(x_0, y_0), QPointF(x_1, y_1));
        scene.addRect(rect, QPen(), brush);
    }

}

QGraphicsScene &Graph::getScene()
{
    return scene;
}
