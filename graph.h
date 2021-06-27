#ifndef GRAPH_H
#define GRAPH_H

#include <QObject>
#include <QGraphicsScene>

class Graph : public QObject
{
    Q_OBJECT
public:
    explicit Graph(QObject *parent = nullptr);

    enum class GraphType{
        Line,
        Bar
    };
    Q_ENUM(GraphType)

    void setSceneSize(int width, int height);
    QGraphicsScene *getScene();
    void setScale(double scale);
    void plotData(QVector<double> const &data);
    void freeze1Graph();
    void freeze2Graph();
    void clearFreeze();

    void setIsHideCurrentGraph(bool newIsHideCurrentGraph);
    void setIsAutoScele(bool newIsAutoScele);
    void setIsMovingAvarage(bool newIsMovingAvarage);
    void setIsZeroLineAtMiddle(bool newIsZeroLineAtMiddle);
    void setGraphType(GraphType newGraphType);

    bool getIsHideCurrentGraph() const;


private:
    int width;
    int height;
    int real_width;
    int real_height;
    const int x_margin = 10;
    const int y_margin = 10;
    double scale;   // 縦軸の最大値
    QGraphicsScene *scene;
    QVector<double> current;
    QVector<double> freeze1;
    QVector<double> freeze2;
    QVector<QVector<double>> currentHistory;
    int movavg_cursor;

    // 設定
    bool isHideCurrentGraph;
    bool isAutoScele;
    bool isMovingAvarage;
    bool isZeroLineAtMiddle;
    GraphType graphType;

    void addCurrentHistory(QVector<double> const &data);
    QVector<double> calcMovingAverage();
    void paintGrid(int n, QPen pen);
    void paintLabel(int n, QFont font);
    void paintError(double err, int idx, QBrush brush, QFont font);
    void paintLine(QVector<double> const &data, QPen pen);
    void paintBar(QVector<double> const &data, QBrush brush);
    void paint();

    void _addLine(double x1, double y1, double x2, double y2, QPen pen = QPen());
    void _addSimpleText(QString text, double x, double y, QFont font = QFont(), QBrush brush = QBrush());
    void _addRect(double x, double y, double w, double h, QPen pen = QPen(), QBrush brush = QBrush());

signals:
    void updated();

};

#endif // GRAPH_H
