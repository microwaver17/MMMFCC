#include "graph.h"
#include "settings.h"

#include <QGraphicsSimpleTextItem>
#include <QtDebug>
#include <cmath>

Graph::Graph(QObject *parent) : QObject(parent)
  , isHideCurrentGraph(false)
  , isAutoScele(Settings::getInstance().isAutoScale)
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
    QFont labelFont("Arial", 12);
    labelFont.setStyleHint(QFont::SansSerif);

    scene.clear();
    paintGrid(current.size(), gridPen);
    paintLabel(current.size(), labelFont);
    paintLine(freeze1, freeze1Pen);
    paintLine(freeze2, freeze2Pen);
    if (!isHideCurrentGraph){
        paintLine(current, currentPen);
    }
}

void Graph::paintGrid(int n, QPen pen)
{
    if (n == 0){
        return;
    }
    n -= 1; // F0除去

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
    if (n == 0){
        return;
    }
    n -= 1; // F0除去

    double x_distance = (double)width / (n - 1);
    for(int i = 0; i < n - 1; i++){
        QPointF pos(i * x_distance + 5, 5);

        scene.addSimpleText(QString::number(i + 1), font)->setPos(pos);
    }
}

void Graph::paintLine(QVector<double> &data, QPen pen)
{
    if (data.size() == 0){
        return;
    }
    QVector<double> data_from_f1 = data.mid(1); //F0除去

    double x_distance = (double)width / (data_from_f1.size() - 1);
    double y_center = (double)height / 2;
    for (int i = 0; i < data_from_f1.size() - 1; i++){
        double val = data_from_f1.at(i);
        double val_next = data_from_f1.at(i + 1);
        if (isAutoScele){
            val = val / (1 + abs(val)) * y_center;
            val_next = val_next / (1 + abs(val_next)) * y_center;
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

}

QGraphicsScene &Graph::getScene()
{
    return scene;
}
