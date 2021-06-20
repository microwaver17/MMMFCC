#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QGraphicsScene>

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);
    QGraphicsScene &getScene();

    void setSceneSize(int width, int height);
    void setMaxValue(double maxValue);
    void paintCurrent(QVector<double> &data);
    void freeze1Graph();
    void freeze2Graph();
    void clearFreeze();
    bool isHideCurrentGraph;

private:
    int width;
    int height;
    double maxValue;   // 縦軸の最大値
    QVector<double> current;
    QVector<double> freeze1;
    QVector<double> freeze2;
    QGraphicsScene scene;

    void paintGrid(int n, QPen pen);
    void paintLine(QVector<double> &data, QPen pen);
    void paint();

signals:

};

#endif // GRAPH_H
