#include "graph.h"
#include "settings.h"
#include "log.h"
#include "util.h"

#include <QFont>
#include <QGraphicsSimpleTextItem>
#include <QtDebug>
#include <cmath>

Graph::Graph(QObject *parent) : QObject(parent)
  , width(0)
  , height(0)
  , real_width(0)
  , real_height(0)
  , scale(SETTINGS.default_scale * SETTINGS.scale_multiple)
  , scene(nullptr)
  , currentHistory(SETTINGS.movingAverageSize)
  , movavg_cursor(0)
  , isHideCurrentGraph(false)
  , isAutoScele(true)
  , isMovingAvarage(false)
  , isZeroLineAtMiddle(true)
  , graphType(GraphType::Line)
{
}

void Graph::setSceneSize(int width, int height)
{
    this->width = width - x_margin * 2;
    this->height = height - y_margin * 2;
    real_width = width;
    real_height = height;
}

QGraphicsScene *Graph::getScene()
{
    return scene;
}

void Graph::setScale(double scale)
{
    this->scale = scale * SETTINGS.scale_multiple;
}

void Graph::plotData(QVector<double> const &data)
{
    if (data.size() != 0){
        current = data;
        addCurrentHistory(data);
    }
    paint();
}

void Graph::addCurrentHistory(QVector<double> const &data)
{
    currentHistory[movavg_cursor] = data;
    movavg_cursor = (movavg_cursor + 1) % currentHistory.size();
}

QVector<double> Graph::calcMovingAverage()
{
    QVector<double> movavg(current.size());
    for (auto cur = currentHistory.begin(); cur != currentHistory.end(); cur++){
        for (int i_mfcc = 0; i_mfcc < cur->size(); i_mfcc++){
            movavg[i_mfcc] += cur->at(i_mfcc) / currentHistory.size();
        }
    }

    return movavg;
}

void Graph::freeze1Graph()
{
    if (freeze1.size() == 0){
        if (isMovingAvarage){
            freeze1 = calcMovingAverage();
        }else{
            freeze1 = current;
        }
    }else{
        freeze1.clear();
    }
    paint();
}

void Graph::freeze2Graph()
{
    if (freeze2.size() == 0){
        if (isMovingAvarage){
            freeze2 = calcMovingAverage();
        }else{
            freeze2 = current;
        }
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

void Graph::setIsHideCurrentGraph(bool newIsHideCurrentGraph)
{
    LOG.addLog(u8"グラフを非表示 " + Util::toStr(newIsHideCurrentGraph), this);
    isHideCurrentGraph = newIsHideCurrentGraph;
}

bool Graph::getIsHideCurrentGraph() const
{
    return isHideCurrentGraph;
}

void Graph::setIsZeroLineAtMiddle(bool newIsZeroLineAtMiddle)
{
    LOG.addLog(u8"グラフのゼロ点を半分の位置 " + Util::toStr(newIsZeroLineAtMiddle), this);
    isZeroLineAtMiddle = newIsZeroLineAtMiddle;
}

void Graph::setIsAutoScele(bool newIsAutoScele)
{
    LOG.addLog(u8"グラフを自動スケール " + Util::toStr(newIsAutoScele), this);
    isAutoScele = newIsAutoScele;
}

void Graph::setIsMovingAvarage(bool newIsMovingAvarage)
{
    LOG.addLog(u8"グラフの後処理（移動平均） " + Util::toStr(newIsMovingAvarage), this);
    isMovingAvarage = newIsMovingAvarage;
}

void Graph::setGraphType(GraphType newGraphType)
{
    LOG.addLog(u8"グラフ種類 " + Util::toStrEnum<GraphType>(newGraphType), this);
    graphType = newGraphType;
}

inline QVector<double> normalize(QVector<double> data)
{
    return data.mid(1);    // F0除去
}

inline double sigmoid(double x){
    return x / (1.0 + abs(x));
}

inline double error(QVector<double> const &data1, QVector<double> const &data2){
    if (data1.size() == 0 || data2.size() ==0){
        return 0.0;
    }

    double err = 0;
    double size = data1.size();
    for (int i = 0; i < data1.size(); i++){
        double diff = sigmoid(data1.at(i) - data2.at(i));
        err += diff * diff;
    }

    return err / size;
}

void Graph::paint()
{
    // scene.clear() よりインスタンスを作り直したほうが断然早い
    if (scene != nullptr){
        scene->deleteLater();
    }
    scene = new QGraphicsScene(0, 0, real_width, real_height);

    const QPen currentPen(Qt::blue, 2);
    const QPen freeze1Pen(Qt::green, 2);
    const QPen freeze2Pen(Qt::magenta, 2);
    const QPen gridPen(Qt::gray, 1);
    QFont labelFont("Arial", 12);
    labelFont.setStyleHint(QFont::SansSerif);

    QVector<double> n_current;
    QVector<double> n_freeze1;
    QVector<double> n_freeze2;
    if (isMovingAvarage){
        QVector<double> current_movavg = calcMovingAverage();
        n_current = normalize(current_movavg);
    }else{
        n_current = normalize(current);
    }
    n_freeze1 = normalize(freeze1);
    n_freeze2 = normalize(freeze2);

    double err_freeze1 = error(n_current, n_freeze1);
    double err_freeze2 = error(n_current, n_freeze2);

    // FFTモード
    qDebug() <<current.size() << SETTINGS.cepstramNumber;
    if (current.size() <= SETTINGS.cepstramNumber){
        paintGrid(n_current.size(), gridPen);
        paintLabel(n_current.size(), labelFont);
        paintError(err_freeze1, 0, freeze1Pen.brush(), labelFont);
        paintError(err_freeze2, 1, freeze2Pen.brush(), labelFont);
    }

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

    emit updated();
}

void Graph::_addLine(double x1, double y1, double x2, double y2, QPen pen)
{
    scene->addLine(x1 + x_margin, y1 + y_margin, x2 + x_margin, y2 + y_margin, pen);
}

void Graph::_addSimpleText(QString text, double x, double y, QFont font, QBrush brush)
{
    QGraphicsSimpleTextItem *item = scene->addSimpleText(text, font);
    item->setPos(x + x_margin, y + y_margin);
    item->setBrush(brush);
}

void Graph::_addRect(double x, double y, double w, double h, QPen pen, QBrush brush)
{
    scene->addRect(x + x_margin, y + y_margin, w, h, pen, brush);
}

void Graph::paintGrid(int n, QPen pen)
{
    double x_distance = (double)width / (n - 1);
    double y_center = isZeroLineAtMiddle ? (double)height / 2 : height;
    _addLine(0, y_center, width, y_center);
    _addLine(0, 0, width, 0);
    _addLine(0, height, width,  height);
    for(int i = 0; i < n; i++){
        int x = i * x_distance;
        _addLine(x, 0, x, height, pen);
    }
}

void Graph::paintLabel(int n, QFont font)
{
    double x_distance = (double)width / (n - 1);
    for(int i = 0; i < n - 1; i++){
        _addSimpleText(QString::number(i + 1), i * x_distance + 5, 5, font);
    }
}

void Graph::paintError(double err, int idx, QBrush brush, QFont font)
{
    int x_origin = 110;
    int y_origin = height - 40 + idx * 20;
    int max_bar_length = width * 0.8;

    QColor color = brush.color();
    color.setAlphaF(0.5);
    brush.setColor(color);

    _addSimpleText(QString::number(err), x_origin + max_bar_length + 10, y_origin, font, QBrush(QColor(0, 0, 0, 127)));

    _addRect(x_origin, y_origin + 5, max_bar_length, 10, QPen(Qt::NoPen), QBrush(QColor(230,230,230,180)));
    _addRect(x_origin, y_origin + 5, err * max_bar_length, 10, QPen(Qt::NoPen), brush);
}

void Graph::paintLine(QVector<double> const &data, QPen pen)
{
    QColor color = pen.color();
    color.setAlphaF(0.5);
    pen.setColor(color);
    double x_distance = (double)width / (data.size() - 1);
    double y_center = isZeroLineAtMiddle ? (double)height / 2 : height;
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
        _addLine(x_0, y_0, x_1, y_1, pen);
    }
}

void Graph::paintBar(QVector<double> const &data, QBrush brush)
{
    QColor color = brush.color();
    color.setAlphaF(0.5);
    brush.setColor(color);
    double x_distance = (double)width / (data.size() - 1);
    double y_center = isZeroLineAtMiddle ? (double)height / 2 : height;
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
        _addRect(x_0, y_0, x_1 - x_0, y_1 - y_0, QPen(), brush);
    }

}
