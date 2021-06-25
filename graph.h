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
    Q_ENUM(GraphType)

    void setSceneSize(int width, int height);
    void setScale(double scale);
    void plotData(QVector<double> const &data);
    void freeze1Graph();
    void freeze2Graph();
    void clearFreeze();

    void setIsHideCurrentGraph(bool newIsHideCurrentGraph);
    void setIsAutoScele(bool newIsAutoScele);
    void setIsMovingAvarage(bool newIsMovingAvarage);
    void setGraphType(GraphType newGraphType);

    bool getIsHideCurrentGraph() const;

private:
    int width;
    int height;
    const int x_margin = 20;
    const int y_margin = 20;
    double scale;   // 縦軸の最大値
    QGraphicsScene scene;
    QVector<double> current;
    QVector<double> freeze1;
    QVector<double> freeze2;
    QVector<QVector<double>> currentHistory;
    int movavg_cursor;

    // 設定
    bool isHideCurrentGraph;
    bool isAutoScele;
    bool isMovingAvarage;
    GraphType graphType;

    void addCurrentHistory(QVector<double> const &data);
    QVector<double> calcMovingAverage();
    void paintGrid(int n, QPen pen);
    void paintLabel(int n, QFont font);
    void paintError(double err, int idx, QBrush brush, QFont font);
    void paintLine(QVector<double> const &data, QPen pen);
    void paintBar(QVector<double> const &data, QBrush brush);
    void paint();

signals:

};

#endif // GRAPH_H
