#ifndef MYDRAWINGWIDGET_H
#define MYDRAWINGWIDGET_H

#include <QWidget>
#include <QMouseEvent>
#include <QPainter>
#include <QPointF>
#include <gmsh.h>
#include <iostream>
class MyDrawingWidget : public QWidget {
    Q_OBJECT
public:
    explicit MyDrawingWidget(QWidget *parent = nullptr);
    QVector<QRectF> getRects() const { return m_rects; }
    QVector<QRectF> getCircles() const { return m_circles; }


protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;


public slots:
    void switchShape(const QString& shape);
    void enableForceConstraint(bool enable);
    void enableJunbuForceConstraint(bool enable);
    void enableDisplacementConstraint(bool enable);
    void generateMesh();
    void calculate();
    void determineMateria(qreal E_input, qreal v_input);
    void generateYuntu_strain_x();
    void generateYuntu_strain_y();
    void generateYuntu_stress_x();
    void generateYuntu_stress_y();
    void generateRectFromCoordinate(qreal x, qreal y, qreal w, qreal h);
    void generateCircleFromCoordinate(qreal x, qreal y, qreal r);
    void remake();
    QColor getColorAtPosition(const QLinearGradient& gradient, qreal position);
    std::pair<double, double> findBestValue(std::vector<double> vec);
    double normalize(double min, double max, double value);
    std::vector<double> calculate_element_K(double xi, double yi, double xj, double yj,  double xk, double yk);
private:
    std::pair<int, std::pair<double, double>> isPointOnBoundary(const QPointF &point);
    std::pair<int, int> junbuTagPair;
    bool hasClicked = false;
    bool m_isDrawing = true;
    double E = 1;
    double v = 0.3;
    QPointF m_startPos;
    QPointF m_endPos;
    QVector<QRectF> m_rects;
    QVector<QRectF> m_circles;
    QVector<QPointF> forcePosition ;
    std::vector<std::pair<int, std::pair<double, double>>> lineNodes ;
    std::vector<std::pair<int, int> > entities;
    std::vector<std::size_t> nodeTags;
    std::vector<double> nodeCoords, nodeParams;
    std::vector<int> elemTypes;
    std::vector<std::vector<std::size_t> > elemTags, elemNodeTags;
    std::vector<double> stress;
    std::vector<double> strain;
    std::vector<std::size_t> triangleNodesTags;
    std::vector<std::size_t> lineNodesTags;
    bool m_isDrawingForceConstraint = false;
    bool m_isDrawingJunbuForceConstraint = false;
    bool m_isDrawingDisplacementConstraint = false;
    bool isDrawingStrain_x = false;
    bool isDrawingStrain_y = false;
    bool isDrawingStress_x = false;
    bool isDrawingStress_y = false;



    struct ForceConstraint {
        int tag;
        QPointF position;
        QString direction;
        double magnitude;
    };
    QVector<ForceConstraint> m_forces;

    struct DisplacementConstraint {
            int tag;
            QPointF position;
            QString direction;
    };
    QVector<DisplacementConstraint> m_displacementConstraints;
    enum Shape { Rectangle, Circle } m_currentShape;

    QVector<QLineF> meshLines;
};

#endif // MYDRAWINGWIDGET_H
