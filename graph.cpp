#include "graph.h"
#include <QtDebug>

Graph::Graph(QObject *parent) : QObject(parent)
  , width(0)
  , height(0)
  , maxValue(0)
  , scene(0, 0, width, height)
{
}

void Graph::setSceneSize(int width, int height)
{
    this->width = width;
    this->height = height;
    scene.setSceneRect(0, 0, width, height);
}

void Graph::setMaxValue(double maxValue)
{
    this->maxValue = maxValue;
}

void Graph::paintCurrent(QVector<double> &data)
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

void Graph::paint()
{
    const QPen currentPen(Qt::blue, 3);
    const QPen freeze1Pen(Qt::green, 3);
    const QPen freeze2Pen(Qt::magenta, 3);
    const QPen gridPen(Qt::gray, 1);

    scene.clear();
    paintGrid(current.size(), gridPen);
    paintLine(freeze1, freeze1Pen);
    paintLine(freeze2, freeze2Pen);
    if (!isHideCurrentGraph){
        paintLine(current, currentPen);
    }
}

void Graph::paintGrid(int n, QPen pen)
{
    double x_distance = (double)width / (n - 1);
    double y_center = (double)height / 2;
    scene.addLine(0, y_center, width, y_center);
    for(int i = 0; i < n; i++){
        scene.addLine(i * x_distance, 0, i * x_distance, height, pen);
    }
}

void Graph::paintLine(QVector<double> &data, QPen pen)
{
    double x_distance = (double)width / (data.size() - 1);
    double y_center = (double)height / 2;
    double y_scale = y_center / maxValue;
    for (int i = 0; i < data.size() - 1; i++){
        double x_0 = x_distance * i;
        double y_0 = y_center - data.at(i) * y_scale;   // 座標が値と+-逆
        double x_1 = x_distance * (i + 1);
        double y_1 = y_center - data.at(i + 1) * y_scale;
        scene.addLine(x_0, y_0, x_1, y_1, pen);
    }
}

QGraphicsScene &Graph::getScene()
{
    return scene;
}
