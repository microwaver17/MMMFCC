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

    enum class GraphType{
        Line,
        Bar
    };

    void setSceneSize(int width, int height);
    void setScale(double scale);
    void plotData(QVector<double> data);
    void freeze1Graph();
    void freeze2Graph();
    void clearFreeze();
    bool isHideCurrentGraph;
    bool isAutoScele;
    GraphType graphType;

private:
    int width;
    int height;
    const int x_margin = 20;
    const int y_margin = 20;
    double scale;   // 縦軸の最大値
    QVector<double> current;
    QVector<double> freeze1;
    QVector<double> freeze2;
    QGraphicsScene scene;

    void paintGrid(int n, QPen pen);
    void paintLabel(int n, QFont font);
    void paintLine(QVector<double> &data, QPen pen);
    void paintBar(QVector<double> &data, QBrush brush);
    void paint();

signals:

};

#endif // GRAPH_H
