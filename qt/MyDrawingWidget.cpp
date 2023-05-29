#include "MyDrawingWidget.h"
#include <QInputDialog>
#include <Eigen/Dense>
#include <QtGui>

MyDrawingWidget::MyDrawingWidget(QWidget *parent) :
    QWidget(parent),
    m_isDrawing(true),
    m_startPos(QPointF(0, 0)),
    m_endPos(QPointF(0, 0)),
    m_isDrawingForceConstraint(false),
    m_isDrawingDisplacementConstraint(false),
    m_currentShape(Shape::Rectangle) {
}

// 在widget绘图
void MyDrawingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

     // 创建背景的颜色

    QLinearGradient gradient(rect().topLeft(), rect().bottomLeft());
    gradient.setColorAt(0, QColor(208, 215, 255));  // 开始颜色，浅蓝色
    gradient.setColorAt(1, QColor(255, 255, 255));  // 结束颜色，白色
    painter.setBrush(gradient);
    painter.drawRect(rect());

    // 绘制用户绘制的矩形
    foreach (const QRectF &rect, m_rects) {

       painter.drawRect(rect);
    }

    // 绘制用户绘制的圆形
    foreach (const QRectF &circle, m_circles) {
        qreal radius = circle.width() / 2;
        painter.drawEllipse(circle.center(), radius, radius);
//        painter.drawEllipse(circle);
    }

    QRectF rect = QRectF(m_startPos, m_endPos).normalized();
    qreal radius = rect.width() / 2;
    switch (m_currentShape) {
    case Shape::Rectangle:
        painter.drawRect(rect);
        break;
    case Shape::Circle:
        painter.drawEllipse(rect.center(), radius, radius);
        break;
    }

    if (m_isDrawingForceConstraint || m_isDrawingDisplacementConstraint || m_isDrawingJunbuForceConstraint){
        // 绘制添加载荷的范围 分三种不同的情况绘制不同的颜色
        if (m_isDrawingDisplacementConstraint) {
            painter.setPen(QPen(Qt::green, 4));
        }else if(m_isDrawingJunbuForceConstraint){
            painter.setPen(QPen(Qt::blue, 4));
        }else{
            painter.setPen(QPen(Qt::red, 4));
        }
        for(auto lineNode: lineNodes) {
            painter.drawEllipse(QPointF(lineNode.second.first, lineNode.second.second), 2, 2);
        }


        // 绘制力载荷
        painter.setPen(QPen(Qt::blue, 2));
            for (const auto &constraint : m_forces) {
                if (constraint.direction == "X"){
                    painter.drawLine(constraint.position - QPointF(30, 0) ,  constraint.position - QPointF(2, 0));
                    painter.drawLine(constraint.position - QPointF(8, 5), constraint.position - QPointF(2, 0));
                    painter.drawLine(constraint.position - QPointF(8, -5), constraint.position - QPointF(2, 0));
                }else{
                    painter.drawLine(constraint.position - QPointF(0, 30), constraint.position - QPointF(0, 2));
                    painter.drawLine(constraint.position - QPointF(5, 8), constraint.position - QPointF(0, 2));
                    painter.drawLine(constraint.position - QPointF(-5, 8), constraint.position - QPointF(0, 2));
                }

            }

        // 绘制位移约束
        painter.setPen(QPen(Qt::red, 2));
         for (const auto &constraint : m_displacementConstraints) {
            if (constraint.direction == "X") {
                painter.drawLine(constraint.position - QPointF(20, 0), constraint.position - QPointF(2, 0));
                painter.drawLine(constraint.position - QPointF(20, 8), constraint.position - QPointF(20, -8));

                painter.drawLine(constraint.position - QPointF(24, 12), constraint.position - QPointF(20, 8));
                painter.drawLine(constraint.position - QPointF(24, 8), constraint.position - QPointF(20, 4));
                painter.drawLine(constraint.position - QPointF(24, 4), constraint.position - QPointF(20, 0));
                painter.drawLine(constraint.position - QPointF(24, 0), constraint.position - QPointF(20, -4));
                painter.drawLine(constraint.position - QPointF(24, -4), constraint.position - QPointF(20, -8));
            } else if (constraint.direction == "Y") {
                painter.drawLine(constraint.position + QPointF(0, 20), constraint.position + QPointF(0, 2));
                painter.drawLine(constraint.position + QPointF(-8, 20), constraint.position + QPointF(8, 20));

                painter.drawLine(constraint.position + QPointF(-8, 20), constraint.position + QPointF(-12, 24));
                painter.drawLine(constraint.position + QPointF(-4, 20), constraint.position + QPointF(-8, 24));
                painter.drawLine(constraint.position + QPointF(0, 20), constraint.position + QPointF(-4, 24));
                painter.drawLine(constraint.position + QPointF(4, 20), constraint.position + QPointF(0, 24));
                painter.drawLine(constraint.position + QPointF(8, 20), constraint.position + QPointF(4, 24));
            } else {
                painter.drawLine(constraint.position - QPointF(20, 0), constraint.position - QPointF(2, 0));
                painter.drawLine(constraint.position - QPointF(20, 8), constraint.position - QPointF(20, -8));

                painter.drawLine(constraint.position - QPointF(24, 12), constraint.position - QPointF(20, 8));
                painter.drawLine(constraint.position - QPointF(24, 8), constraint.position - QPointF(20, 4));
                painter.drawLine(constraint.position - QPointF(24, 4), constraint.position - QPointF(20, 0));
                painter.drawLine(constraint.position - QPointF(24, 0), constraint.position - QPointF(20, -4));
                painter.drawLine(constraint.position - QPointF(24, -4), constraint.position - QPointF(20, -8));


                painter.drawLine(constraint.position + QPointF(0, 20), constraint.position + QPointF(0, 2));
                painter.drawLine(constraint.position + QPointF(-8, 20), constraint.position + QPointF(8, 20));

                painter.drawLine(constraint.position + QPointF(-8, 20), constraint.position + QPointF(-12, 24));
                painter.drawLine(constraint.position + QPointF(-4, 20), constraint.position + QPointF(-8, 24));
                painter.drawLine(constraint.position + QPointF(0, 20), constraint.position + QPointF(-4, 24));
                painter.drawLine(constraint.position + QPointF(4, 20), constraint.position + QPointF(0, 24));
                painter.drawLine(constraint.position + QPointF(8, 20), constraint.position + QPointF(4, 24));
            }
        }
    }

     // 绘制网格信息
     painter.setPen(QPen(Qt::gray, 1));
     foreach (const QLineF &line, meshLines) {
         painter.drawLine(line);
     }


     // 绘制网格内对应x方向应变的云图
     if (isDrawingStrain_x) {
         std::vector<double> strain_x;
         for (int i=0; i < strain.size();i+=3 ){
             strain_x.push_back(strain[i]);
         }
         std::pair<double, double> bestResult = findBestValue(strain_x);
         double min = bestResult.first;
         double max = bestResult.second;

         // 绘制参考的颜色值
         QRect refer(40, 40, 60, 460);
         QLinearGradient linearGradient(refer.topLeft(), refer.bottomLeft());
         linearGradient.setColorAt(0, QColor(0,191,255));
         linearGradient.setColorAt(0.125, QColor(135,206,250));
         linearGradient.setColorAt(0.25, QColor(152,251,152));
         linearGradient.setColorAt(0.375, QColor(0,255,0));
         linearGradient.setColorAt(0.5, QColor(255,255,0));
         linearGradient.setColorAt(0.625, QColor(245,222,179));
         linearGradient.setColorAt(0.75, QColor(255,165,0));
         linearGradient.setColorAt(0.875, QColor(255,99,71));
         linearGradient.setColorAt(1, QColor(255,0,0));
         painter.setBrush(linearGradient);
         painter.drawRect(refer);

         QString text_min = QString::number(min);
         QString text_max = QString::number(max);
         painter.drawText(QPointF(108, 40), text_min);
         painter.drawText(QPointF(108, 500), text_max);

//         std::cout << "strainxSize :   " << strain_x.size() << std::endl;

         for (int i=0; i < strain_x.size();i+=1 ){
//             std::cout << strain_x[i] << std::endl;
//             std::cout << "fick u" << std::endl;
             double normalizeStrain = normalize(min, max, strain_x[i]);
//             std::cout << normalizeStrain << std::endl;
             QColor color = getColorAtPosition(linearGradient, normalizeStrain);
             painter.setBrush(color);
             int j = 3*i;
             int nodeTag1 = triangleNodesTags[j];
             int nodeTag2 = triangleNodesTags[j+1];
             int nodeTag3 = triangleNodesTags[j+2];

             double x1 = nodeCoords[3 * (nodeTag1 - 1)];
             double y1 = nodeCoords[3 * (nodeTag1 - 1) + 1];
             double x2 = nodeCoords[3 * (nodeTag2 - 1)];
             double y2 = nodeCoords[3 * (nodeTag2 - 1) + 1];
             double x3 = nodeCoords[3 * (nodeTag3 - 1)];
             double y3 = nodeCoords[3 * (nodeTag3 - 1) + 1];
             QPointF point1(x1, y1);
             QPointF point2(x2, y2);
             QPointF point3(x3, y3);

             QPolygonF triangle;
             triangle << point1 << point2 << point3;
             painter.drawPolygon(triangle);

         }
     }


     // 绘制网格内对应y方向应变的云图
     if (isDrawingStrain_y) {
         std::vector<double> strain_y;
         for (int i=1; i < strain.size();i+=3 ){
             strain_y.push_back(strain[i]);
         }
         std::pair<double, double> bestResult = findBestValue(strain_y);
         double min = bestResult.first;
         double max = bestResult.second;

         // 绘制参考的颜色值
         QRect refer(40, 40, 60, 460);
         QLinearGradient linearGradient(refer.topLeft(), refer.bottomLeft());
         linearGradient.setColorAt(0, QColor(0,191,255));
         linearGradient.setColorAt(0.125, QColor(135,206,250));
         linearGradient.setColorAt(0.25, QColor(152,251,152));
         linearGradient.setColorAt(0.375, QColor(0,255,0));
         linearGradient.setColorAt(0.5, QColor(255,255,0));
         linearGradient.setColorAt(0.625, QColor(245,222,179));
         linearGradient.setColorAt(0.75, QColor(255,165,0));
         linearGradient.setColorAt(0.875, QColor(255,99,71));
         linearGradient.setColorAt(1, QColor(255,0,0));
         painter.setBrush(linearGradient);
         painter.drawRect(refer);

         QString text_min = QString::number(min);
         QString text_max = QString::number(max);
         painter.drawText(QPointF(108, 40), text_min);
         painter.drawText(QPointF(108, 500), text_max);


//          std::cout << "strainySize :   " << strain_y.size() << std::endl;
         for (int i=0; i < strain_y.size();i+=1 ){
//             std::cout << strain_x[i] << std::endl;
//             std::cout << "fick u" << std::endl;
             double normalizeStrain = normalize(min, max, strain_y[i]);
//             std::cout << normalizeStrain << std::endl;
             QColor color = getColorAtPosition(linearGradient, normalizeStrain);
             painter.setBrush(color);
             int j = 3*i;
             int nodeTag1 = triangleNodesTags[j];
             int nodeTag2 = triangleNodesTags[j+1];
             int nodeTag3 = triangleNodesTags[j+2];

             double x1 = nodeCoords[3 * (nodeTag1 - 1)];
             double y1 = nodeCoords[3 * (nodeTag1 - 1) + 1];
             double x2 = nodeCoords[3 * (nodeTag2 - 1)];
             double y2 = nodeCoords[3 * (nodeTag2 - 1) + 1];
             double x3 = nodeCoords[3 * (nodeTag3 - 1)];
             double y3 = nodeCoords[3 * (nodeTag3 - 1) + 1];
             QPointF point1(x1, y1);
             QPointF point2(x2, y2);
             QPointF point3(x3, y3);

             QPolygonF triangle;
             triangle << point1 << point2 << point3;
             painter.drawPolygon(triangle);
         }
     }


     // 绘制网格内对应x方向应力的云图
     if (isDrawingStress_x) {
         std::vector<double> stress_x;
         for (int i=0; i < stress.size();i+=3 ){
             stress_x.push_back(stress[i]);
         }
         std::pair<double, double> bestResult = findBestValue(stress_x);
         double min = bestResult.first;
         double max = bestResult.second;

         // 绘制参考的颜色值
         QRect refer(40, 40, 60, 460);
         QLinearGradient linearGradient(refer.topLeft(), refer.bottomLeft());
         linearGradient.setColorAt(0, QColor(0,191,255));
         linearGradient.setColorAt(0.125, QColor(135,206,250));
         linearGradient.setColorAt(0.25, QColor(152,251,152));
         linearGradient.setColorAt(0.375, QColor(0,255,0));
         linearGradient.setColorAt(0.5, QColor(255,255,0));
         linearGradient.setColorAt(0.625, QColor(245,222,179));
         linearGradient.setColorAt(0.75, QColor(255,165,0));
         linearGradient.setColorAt(0.875, QColor(255,99,71));
         linearGradient.setColorAt(1, QColor(255,0,0));
         painter.setBrush(linearGradient);
         painter.drawRect(refer);

         QString text_min = QString::number(min);
         QString text_max = QString::number(max);
         painter.drawText(QPointF(108, 40), text_min);
         painter.drawText(QPointF(108, 500), text_max);


//          std::cout << "stressXSize :   " << stress_x.size() << std::endl;
         for (int i=0; i < stress_x.size();i+=1 ){
//             std::cout << strain_x[i] << std::endl;
//             std::cout << "fick u" << std::endl;
             double normalizeStrain = normalize(min, max, stress_x[i]);
//             std::cout << normalizeStrain << std::endl;
             QColor color = getColorAtPosition(linearGradient, normalizeStrain);
             painter.setBrush(color);
             int j = 3*i;
             int nodeTag1 = triangleNodesTags[j];
             int nodeTag2 = triangleNodesTags[j+1];
             int nodeTag3 = triangleNodesTags[j+2];

             double x1 = nodeCoords[3 * (nodeTag1 - 1)];
             double y1 = nodeCoords[3 * (nodeTag1 - 1) + 1];
             double x2 = nodeCoords[3 * (nodeTag2 - 1)];
             double y2 = nodeCoords[3 * (nodeTag2 - 1) + 1];
             double x3 = nodeCoords[3 * (nodeTag3 - 1)];
             double y3 = nodeCoords[3 * (nodeTag3 - 1) + 1];
             QPointF point1(x1, y1);
             QPointF point2(x2, y2);
             QPointF point3(x3, y3);

             QPolygonF triangle;
             triangle << point1 << point2 << point3;
             painter.drawPolygon(triangle);
         }
     }


    // 绘制网格内对应y方向应力的云图
     if (isDrawingStress_y) {
         std::vector<double> stress_y;
         for (int i=1; i < stress.size();i+=3 ){
             stress_y.push_back(stress[i]);
         }
         std::pair<double, double> bestResult = findBestValue(stress_y);
         double min = bestResult.first;
         double max = bestResult.second;

         // 绘制参考的颜色值
         QRect refer(40, 40, 60, 460);
         QLinearGradient linearGradient(refer.topLeft(), refer.bottomLeft());
         linearGradient.setColorAt(0, QColor(0,191,255));
         linearGradient.setColorAt(0.125, QColor(135,206,250));
         linearGradient.setColorAt(0.25, QColor(152,251,152));
         linearGradient.setColorAt(0.375, QColor(0,255,0));
         linearGradient.setColorAt(0.5, QColor(255,255,0));
         linearGradient.setColorAt(0.625, QColor(245,222,179));
         linearGradient.setColorAt(0.75, QColor(255,165,0));
         linearGradient.setColorAt(0.875, QColor(255,99,71));
         linearGradient.setColorAt(1, QColor(255,0,0));
         painter.setBrush(linearGradient);
         painter.drawRect(refer);

         QString text_min = QString::number(min);
         QString text_max = QString::number(max);
         painter.drawText(QPointF(108, 40), text_min);
         painter.drawText(QPointF(108, 500), text_max);


//          std::cout << "stressYSize :   " << stress_y.size() << std::endl;
         for (int i=0; i < stress_y.size();i+=1 ){
//             std::cout << strain_x[i] << std::endl;
//             std::cout << "fick u" << std::endl;
             double normalizeStrain = normalize(min, max, stress_y[i]);
//             std::cout << normalizeStrain << std::endl;
             QColor color = getColorAtPosition(linearGradient, normalizeStrain);
             painter.setBrush(color);
             int j = 3*i;
             int nodeTag1 = triangleNodesTags[j];
             int nodeTag2 = triangleNodesTags[j+1];
             int nodeTag3 = triangleNodesTags[j+2];

             double x1 = nodeCoords[3 * (nodeTag1 - 1)];
             double y1 = nodeCoords[3 * (nodeTag1 - 1) + 1];
             double x2 = nodeCoords[3 * (nodeTag2 - 1)];
             double y2 = nodeCoords[3 * (nodeTag2 - 1) + 1];
             double x3 = nodeCoords[3 * (nodeTag3 - 1)];
             double y3 = nodeCoords[3 * (nodeTag3 - 1) + 1];
             QPointF point1(x1, y1);
             QPointF point2(x2, y2);
             QPointF point3(x3, y3);

             QPolygonF triangle;
             triangle << point1 << point2 << point3;
             painter.drawPolygon(triangle);
         }
     }
}

// 鼠标点击事件
void MyDrawingWidget::mousePressEvent(QMouseEvent *event) {

    if (event->button() == Qt::LeftButton) {
        if (!m_isDrawingForceConstraint && !m_isDrawingDisplacementConstraint) {
//            m_isDrawing = true;
            m_startPos = event->pos();
            m_endPos = event->pos();
            update();
        }

        if (m_isDrawingForceConstraint || m_isDrawingDisplacementConstraint || m_isDrawingJunbuForceConstraint) {
            QPointF point = event->pos();
            //判断是否在边界内 返回std::pair<tag, std::pair<x,y>>
            std::pair<int, std::pair<double, double>> lineNode = isPointOnBoundary(point);
            if (lineNode.first > 0) {
                if (m_isDrawingForceConstraint) {
                    double forceMagnitude = QInputDialog::getDouble(this, tr("Force Magnitude"),
                                        tr("Enter force magnitude:"), 0, -100000, 100000, 2);
                    if (forceMagnitude){
                        ForceConstraint constraint;

                        QStringList directions;
                        directions << tr("X") << tr("Y");
                        QString selectedDirection = QInputDialog::getItem(this, tr("Select Force Direction"),
                                                                                        tr("Direction:"), directions, 0, false);
                        constraint.magnitude = forceMagnitude;
                        constraint.tag = lineNode.first;
                        constraint.direction = selectedDirection;
                        constraint.position = QPointF(lineNode.second.first, lineNode.second.second);
                        m_forces.append(constraint);
                        update();
                    }
                } else if(m_isDrawingDisplacementConstraint){
                    QStringList directions;
                    directions << tr("")<< tr("X") << tr("Y") << tr("X and Y");
                    QString selectedDirection = QInputDialog::getItem(this, tr("Select Displacement Constraint Direction"),
                                                              tr("Direction:"), directions, 0, false);
                    if (!selectedDirection.isEmpty()) {
                        DisplacementConstraint constraint;
                        constraint.tag = lineNode.first;
                        constraint.position = QPointF(lineNode.second.first, lineNode.second.second);
                        constraint.direction = selectedDirection;
                        m_displacementConstraints.append(constraint);
                        update();
                    }
                } else {
                    if (!hasClicked){
                        hasClicked = true;
                        junbuTagPair.first = lineNode.first;
                    }else{
                        junbuTagPair.second = lineNode.first;
                        double forceMagnitude = QInputDialog::getDouble(this, tr("均布载荷力集度"),
                                            tr("输入均布载荷力的集度:"), 0, -100000, 100000, 2);
                        QStringList directions;
                        directions << tr("X") << tr("Y");
                        QString selectedDirection = QInputDialog::getItem(this, tr("选择集度的方向"),
                                                                                        tr("方向:"), directions, 0, false);
                        if (forceMagnitude){
                            int nodeNum = std::abs(junbuTagPair.second - junbuTagPair.first) + 1;
                            double x_start = nodeCoords[3 * (junbuTagPair.first - 1)];
                            double y_start = nodeCoords[3 * (junbuTagPair.first - 1) + 1];
                            double x_end = nodeCoords[3 * (junbuTagPair.second - 1)];
                            double y_end = nodeCoords[3 * (junbuTagPair.second - 1) + 1];
                            // 如果是在矩形的上下边界加分布载荷
                            if (y_start == y_end) {
                                double everyNodeForce = forceMagnitude* std::abs((x_end-x_start))/100/ nodeNum;
                                // 当点击的第一个点的tag小于第二个点时
                                if (junbuTagPair.first < junbuTagPair.second) {
                                    for (int i = junbuTagPair.first; i <= junbuTagPair.second; i++) {
                                        double x_tranverse = nodeCoords[3 * (i - 1)];
                                        double y_tranverse = nodeCoords[3 * (i - 1) + 1];
                                        ForceConstraint constraint;
                                        constraint.magnitude = everyNodeForce;
                                        constraint.tag = i;
                                        constraint.direction = selectedDirection;
                                        constraint.position = QPointF(x_tranverse, y_tranverse);
                                        m_forces.append(constraint);
                                    }
                                }else {
                                    for (int i = junbuTagPair.first; i >= junbuTagPair.second; i--) {
                                        double x_tranverse = nodeCoords[3 * (i - 1)];
                                        double y_tranverse = nodeCoords[3 * (i - 1) + 1];
                                        ForceConstraint constraint;
                                        constraint.magnitude = everyNodeForce;
                                        constraint.tag = i;
                                        constraint.direction = selectedDirection;
                                        constraint.position = QPointF(x_tranverse, y_tranverse);
                                        m_forces.append(constraint);
                                    }
                                }

                            }// 在矩形的左右边界加分布载荷
                            else {
                                double everyNodeForce = forceMagnitude* std::abs((y_end-y_start))/100/nodeNum;
                                // 当点击的第一个点的tag小于第二个点时
                                if (junbuTagPair.first < junbuTagPair.second) {
                                    for (int i = junbuTagPair.first; i <= junbuTagPair.second; i++) {
                                        double x_tranverse = nodeCoords[3 * (i - 1)];
                                        double y_tranverse = nodeCoords[3 * (i - 1) + 1];
                                        ForceConstraint constraint;
                                        constraint.magnitude = everyNodeForce;
                                        constraint.tag = i;
                                        constraint.direction = selectedDirection;
                                        constraint.position = QPointF(x_tranverse, y_tranverse);
                                        m_forces.append(constraint);
                                    }
                                }else {
                                    for (int i = junbuTagPair.first; i >= junbuTagPair.second; i--) {
                                        double x_tranverse = nodeCoords[3 * (i - 1)];
                                        double y_tranverse = nodeCoords[3 * (i - 1) + 1];
                                        ForceConstraint constraint;
                                        constraint.magnitude = everyNodeForce;
                                        constraint.tag = i;
                                        constraint.direction = selectedDirection;
                                        constraint.position = QPointF(x_tranverse, y_tranverse);
                                        m_forces.append(constraint);
                                    }
                                }
                            }
                            hasClicked = false;
                            update();
                        }
                    }
                }
            }
        }
    }
}

// 鼠标移动事件
void MyDrawingWidget::mouseMoveEvent(QMouseEvent *event) {

    if (m_isDrawing) {
        m_endPos = event->pos();
        update();
    }
}

// 鼠标释放事件
void MyDrawingWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton && m_isDrawing) {
//        m_isDrawing = false;
        m_endPos = event->pos();
        QRectF rect = QRectF(m_startPos, m_endPos).normalized();

        switch (m_currentShape) {
                case Shape::Rectangle:
                    m_rects.append(rect);
                    break;
                case Shape::Circle:
//                    qreal radius = QLineF(m_startPos, m_endPos).length() / 2.0;
//                    QRectF circleRect(m_startPos, QSizeF(radius * 2, radius * 2));
//                    circleRect.moveCenter(rect.center());
                    m_circles.append(rect);
                    break;
                }
        update();
    }
}

// 检查点是否在边界上
std::pair<int, std::pair<double, double>> MyDrawingWidget::isPointOnBoundary(const QPointF &point)
{

    const double distanceThreshold = 4.0;
    for(auto lineNode: lineNodes) {
        if (qAbs(point.x() - lineNode.second.first) <= distanceThreshold &&
            qAbs(point.y() - lineNode.second.second) <= distanceThreshold){
//            return lineNode.first;
             return lineNode;
        }
    }
    return std::make_pair(-1 , std::make_pair(-1, -1));
}

// 切换绘制的形状
void MyDrawingWidget::switchShape(const QString& shape)
{
    if (shape == "Rectangle") {
        m_currentShape = Shape::Rectangle;
    } else if (shape == "Circle") {
        m_currentShape = Shape::Circle;
    }
}

// 添加集中力载荷
void MyDrawingWidget::enableForceConstraint(bool enable)
{

    m_isDrawingForceConstraint = enable;
    m_isDrawingJunbuForceConstraint = false;
    m_isDrawingDisplacementConstraint = false;
    update();
}

// 添加均布载荷
void MyDrawingWidget::enableJunbuForceConstraint(bool enable)
{
    m_isDrawingForceConstraint = false;
    m_isDrawingJunbuForceConstraint = enable;
    m_isDrawingDisplacementConstraint = false;
    update();
}

// 添加位移载荷
void MyDrawingWidget::enableDisplacementConstraint(bool enable)
{
    m_isDrawingForceConstraint = false;
    m_isDrawingJunbuForceConstraint = false;
    m_isDrawingDisplacementConstraint = enable;
    update();
}

// 绘制网格
void MyDrawingWidget::generateMesh() {
    gmsh::initialize();
    gmsh::model::add("rectangle_with_holes");
    m_isDrawing = false;
    namespace factory = gmsh::model::geo;
    // 网格大小
    double lc = 30;

////    QRectF rect = m_rects.first();
////    double points[4][2] = {
////                {rect.left(), rect.top()},
////                {rect.right(), rect.top()},
////                {rect.right(), rect.bottom()},
////                {rect.left(), rect.bottom()}};
    for (const QRectF &rect :m_rects)
        {
            double left = rect.left();
            double right = rect.right();
            double top  = rect.top();
            double bottom = rect.bottom();
            factory::addPoint(left, top, 0, lc);
            factory::addPoint(right, top, 0, lc);
            factory::addPoint(right, bottom, 0, lc);
            factory::addPoint(left, bottom, 0, lc);
            factory::addLine(1, 2);
            factory::addLine(2, 3);
            factory::addLine(3, 4);
            factory::addLine(4, 1);
            factory::addCurveLoop({ 1, 2, 3, 4 });
//            factory::addPlaneSurface({1});
        }

    int pointIndex = 4;
    int lineIndex = 4;
    int curveLoopIndex = 1;
    for (const QRectF &circle : m_circles)
        {
            double center_x = circle.center().x();
            double center_y = circle.center().y();
            double radius = circle.width() / 2;

            gmsh::model::geo::addPoint(center_x - radius, center_y, 0, lc);
            pointIndex ++;
            gmsh::model::geo::addPoint(center_x, center_y, 0, lc);
            pointIndex ++;
            gmsh::model::geo::addPoint(center_x + radius, center_y, 0, lc);
            pointIndex ++;
            factory::addCircleArc(pointIndex-2, pointIndex-1, pointIndex);
            lineIndex ++;
            factory::addCircleArc(pointIndex, pointIndex-1, pointIndex-2);
            lineIndex ++;
            factory::addCurveLoop({ lineIndex-1, lineIndex});
            curveLoopIndex ++;
        }
    std::vector<int> surfaceVector;
    for (int i = 1; i <= curveLoopIndex; i++) {
        surfaceVector.push_back(i);
    }
    factory::addPlaneSurface(surfaceVector);
    factory::synchronize();
    gmsh::model::mesh::generate(2);

    // meshing in the window
//    std::string name;
//      gmsh::model::getCurrent(name);
//      std::cout << "Model " << name << " (" << gmsh::model::getDimension()
//                << "D)\n";

        gmsh::model::getEntities(entities);       
        gmsh::model::mesh::getNodes(nodeTags, nodeCoords, nodeParams);
        gmsh::model::mesh::getElements(elemTypes, elemTags, elemNodeTags);

        triangleNodesTags = elemNodeTags[1];
        lineNodesTags = elemNodeTags[0];

        for (int i = 0; i < lineNodesTags.size(); i += 2){
            double x1 = nodeCoords[3 * (lineNodesTags[i] - 1)];
            double y1 = nodeCoords[3 * (lineNodesTags[i] - 1) + 1];
            lineNodes.push_back(std::make_pair(lineNodesTags[i], std::make_pair(x1, y1)));
        }

        for(int i = 0; i < triangleNodesTags.size(); i += 3) {
          // Dimension and tag of the entity:

          double x1 = nodeCoords[3 * (triangleNodesTags[i] - 1)];
          double y1 = nodeCoords[3 * (triangleNodesTags[i] - 1) + 1];
          double x2 = nodeCoords[3 * (triangleNodesTags[i+1] - 1)];
          double y2 = nodeCoords[3 * (triangleNodesTags[i+1] - 1) + 1];
          double x3 = nodeCoords[3 * (triangleNodesTags[i+2] - 1)];
          double y3 = nodeCoords[3 * (triangleNodesTags[i+2] - 1) + 1];
          QLineF line1(QPointF(x1, y1), QPointF(x2, y2));
          QLineF line2(QPointF(x2, y2), QPointF(x3, y3));
          QLineF line3(QPointF(x3, y3), QPointF(x1, y1));

//          std::cout << "dim: " << dim << " tag: " << tag<< std::endl;
          // Get the mesh nodes for the entity (dim, tag):
          meshLines.push_back(line1);
          meshLines.push_back(line2);
          meshLines.push_back(line3);
        }
              // Get the mesh elements for the entity (dim, tag):

//                  // * Type of the entity:
//

//                      // * Number of mesh nodes and elements:
//                      int numElem = 0;
//                      for(auto &tags : elemTags) numElem += tags.size();
//                      std::cout << " - Mesh has " << nodeTags.size() << " nodes and " << numElem
//                                << " elements\n";



//    gmsh::fltk::run();



    // t1 .example
//    gmsh::initialize();


//      gmsh::model::add("t1");


//      double lc = 1e-2;
//      gmsh::model::geo::addPoint(0, 0, 0, lc, 1);

//      gmsh::model::geo::addPoint(.1, 0, 0, lc, 2);
//      gmsh::model::geo::addPoint(.1, .3, 0, lc, 3);

//      // If the tag is not provided explicitly, a new tag is automatically created,
//      // and returned by the function:
//      int p4 = gmsh::model::geo::addPoint(0, .3, 0, lc);


//      gmsh::model::geo::addLine(1, 2, 1);
//      gmsh::model::geo::addLine(3, 2, 2);
//      gmsh::model::geo::addLine(3, p4, 3);
//      gmsh::model::geo::addLine(4, 1, p4);

//      gmsh::model::geo::addCurveLoop({4, 1, -2, 3}, 1);


//      gmsh::model::geo::addPlaneSurface({1}, 1);


//      gmsh::model::geo::synchronize();


////      gmsh::model::addPhysicalGroup(1, {1, 2, 4}, 5);
////      gmsh::model::addPhysicalGroup(2, {1}, -1, "My surface");
//      gmsh::model::mesh::generate(2);

//      gmsh::write("t1.msh");


//     gmsh::fltk::run();
      update();
      gmsh::finalize();
}

// 计算
void MyDrawingWidget::calculate() {
    using Eigen::MatrixXd;
    using Eigen::Matrix2d;
    using Eigen::Matrix3d;
    using Eigen::VectorXd;
    using Eigen::Vector3d;
    std::cout<< "E:" << E <<", v: " << v;
//    Eigen::Array22d tmp;
    Matrix2d tmp;
    MatrixXd K(2*nodeTags.size(),2*nodeTags.size());
    MatrixXd B(3,6);
    Matrix3d D;


//    Matrix3d test;
//    Matrix2d test2;
//    for (int i=0; i < 3; i++) {
//        for (int j=0; j < 3; j++){
//            test(i,j) = 0;
//        }
//    }
//    for (int i=0; i < 2; i++) {
//        for (int j=0; j < 2; j++){
//            test2(i,j) = 1;
//        }
//    }
//    test.block<2,2>(0,0) += test2;
//    std::cout << "test" << test << std::endl;
//    test.block<2,2>(0,0) += test2;
//    std::cout << "test" << test << std::endl;

    // 初始化矩阵B,D,K
    for (int i=0; i < 3; i++) {
        for (int j=0; j < 3; j++){
            D(i,j) = 0;
        }
    }

    for (int i=0; i < 3; i++) {
        for (int j=0; j < 6; j++){
            B(i,j) = 0;
        }
    }

    for (int i=0; i < 2*nodeTags.size(); i++) {
        for (int j=0; j < 2*nodeTags.size(); j++){
            K(i,j) = 0;
        }
    }


//    v = 0.3;
//    int a = 10e8;
//    std::cout << "a" << ": " << a <<std::endl;
//    E = 1;

    // 计算D矩阵
    double coefficent1 = E*1.0/(1-v*v);
    D(0,0) = coefficent1;
    D(0,1) = coefficent1*v;
    D(1,0) = coefficent1*v;
    D(1,1) = coefficent1;
    D(2,2) = coefficent1*(1-v)/2.0;

    for(int i = 0; i < triangleNodesTags.size(); i += 3) {
      // Dimension and tag of the entity:
      int nodeTag1 = triangleNodesTags[i];
      int nodeTag2 = triangleNodesTags[i+1];
      int nodeTag3 = triangleNodesTags[i+2];

      double x1 = nodeCoords[3 * (nodeTag1 - 1)];
      double y1 = nodeCoords[3 * (nodeTag1 - 1) + 1];
      double x2 = nodeCoords[3 * (nodeTag2 - 1)];
      double y2 = nodeCoords[3 * (nodeTag2 - 1) + 1];
      double x3 = nodeCoords[3 * (nodeTag3 - 1)];
      double y3 = nodeCoords[3 * (nodeTag3 - 1) + 1];
      double xi = x1/50;
      double yi = y1/50;
      double xj = x2/50;
      double yj = y2/50;
      double xk = x3/50;
      double yk = y3/50;

      double A = (yk*(xj-xi) - yj*(xk-xi) - yi*(xj-xk))/2;
      std::cout << "A: " << A << std::endl;
      int t = 1;
//      int ai = xj*yk - xk*yj;
//      int aj = xk*yi - xi*yk;
//      int ak = xi*yj - xj*yi;

      double bi = yj-yk;
      double bj = yk-yi;
      double bk = yi-yj;

      double ci = -xj + xk;
      double cj = -xk + xi;
      double ck = -xi + xj;

      double coefficent = 1.0*E*t/(4*(1-v*v)*A);
      // calculate K_ii
      tmp(0,0) = (coefficent*(bi*bi + (1-v)*ci*ci/2));
      tmp(0,1) = (coefficent*(v*bi*ci + (1-v)*ci*bi/2));
      tmp(1,0) = (coefficent*(v*bi*ci + (1-v)*ci*bi/2));
      tmp(1,1) = (coefficent*(ci*ci + (1-v)*bi*bi/2));
      K.block<2,2>(2*nodeTag1-2,2*nodeTag1-2) += tmp;

      // calculate K_ij
      tmp(0,0) = (coefficent*(bi*bj + (1-v)*ci*cj/2));
      tmp(0,1) = (coefficent*(v*bi*cj + (1-v)*ci*bj/2));
      tmp(1,0) = (coefficent*(v*ci*bj + (1-v)*bi*cj/2));
      tmp(1,1) = (coefficent*(ci*cj + (1-v)*bi*bj/2));
      K.block<2,2>(2*nodeTag1-2,2*nodeTag2-2) += tmp;

      // calculate K_ik
      tmp(0,0) = (coefficent*(bi*bk + (1-v)*ci*ck/2));
      tmp(0,1) = (coefficent*(v*bi*ck + (1-v)*ci*bk/2));
      tmp(1,0) = (coefficent*(v*ci*bk + (1-v)*bi*ck/2));
      tmp(1,1) = (coefficent*(ci*ck + (1-v)*bi*bk/2));
      K.block<2,2>(2*nodeTag1-2,2*nodeTag3-2) += tmp;

      // calculate K_ji
      tmp(0,0) = (coefficent*(bj*bi + (1-v)*cj*ci/2));
      tmp(0,1) = (coefficent*(v*bj*ci + (1-v)*cj*bi/2));
      tmp(1,0) = (coefficent*(v*cj*bi + (1-v)*bj*ci/2));
      tmp(1,1) = (coefficent*(cj*ci + (1-v)*bj*bi/2));
      K.block<2,2>(2*nodeTag2-2,2*nodeTag1-2) += tmp;

      // calculate K_jj
      tmp(0,0) = (coefficent*(bj*bj + (1-v)*cj*cj/2));
      tmp(0,1) = (coefficent*(v*bj*cj + (1-v)*cj*bj/2));
      tmp(1,0) = (coefficent*(v*cj*bj + (1-v)*bj*cj/2));
      tmp(1,1) = (coefficent*(cj*cj + (1-v)*bj*bj/2));
      K.block<2,2>(2*nodeTag2-2,2*nodeTag2-2) += tmp;

      // calculate K_jk
      tmp(0,0) = (coefficent*(bj*bk + (1-v)*cj*ck/2));
      tmp(0,1) = (coefficent*(v*bj*ck + (1-v)*cj*bk/2));
      tmp(1,0) = (coefficent*(v*cj*bk + (1-v)*bj*ck/2));
      tmp(1,1) = (coefficent*(cj*ck + (1-v)*bj*bk/2));
      K.block<2,2>(2*nodeTag2-2,2*nodeTag3-2) += tmp;

      // calculate K_ki
      tmp(0,0) = (coefficent*(bk*bi + (1-v)*ck*ci/2));
      tmp(0,1) = (coefficent*(v*bk*ci + (1-v)*ck*bi/2));
      tmp(1,0) = (coefficent*(v*ck*bi + (1-v)*bk*ci/2));
      tmp(1,1) = (coefficent*(ck*ci + (1-v)*bk*bi/2));
      K.block<2,2>(2*nodeTag3-2,2*nodeTag1-2) += tmp;

      // calculate K_kj
      tmp(0,0) = (coefficent*(bk*bj + (1-v)*ck*cj/2));
      tmp(0,1) = (coefficent*(v*bk*cj + (1-v)*ck*bj/2));
      tmp(1,0) = (coefficent*(v*ck*bj + (1-v)*bk*cj/2));
      tmp(1,1) = (coefficent*(ck*cj + (1-v)*bk*bj/2));
      K.block<2,2>(2*nodeTag3-2,2*nodeTag2-2) += tmp;

      // calculate K_kk
      tmp(0,0) = (coefficent*(bk*bk + (1-v)*ck*ck/2));
      tmp(0,1) = (coefficent*(v*bk*ck + (1-v)*ck*bk/2));
      tmp(1,0) = (coefficent*(v*ck*bk + (1-v)*bk*ck/2));
      tmp(1,1) = (coefficent*(ck*ck + (1-v)*bk*bk/2));
//      std::cout << "coefficent*(bk*bk + (1-v)*ck*ck/2): " << coefficent*(bk*bk + (1-v)*ck*ck/2) << std::endl;
//      std::cout << "tmp: " << tmp << std::endl;
      K.block<2,2>(2*nodeTag3-2,2*nodeTag3-2) += tmp;

    }

    VectorXd F(2*nodeTags.size());
    VectorXd delta(2*nodeTags.size());

    for (int i =0; i<2*nodeTags.size(); i++) {
        F(i) = 0;
        delta(i) = 0;
    }



    for (const auto &constraint : m_forces) {
        if (constraint.direction == "X"){
            F(2*constraint.tag-2 ) = constraint.magnitude;
        }else{
            F(2*constraint.tag-1 ) = constraint.magnitude;
        }
    }

//     std::cout << "Here is the vector F:\n" << F << std::endl;
//     std::cout << "Here is the vector K:\n" << K << std::endl<< std::endl;






        // 将位移约束作用于刚度矩阵K上
    for (const auto &constraint : m_displacementConstraints) {
       std::cout << "constraintNodeTag" << constraint.tag << std::endl;
       if (constraint.direction == "X") {
            for (int i =0; i < 2*nodeTags.size(); i ++) {
                K(2*constraint.tag-2, i) = 0;
                K(i, 2*constraint.tag-2) = 0;
            }
            K(2*constraint.tag-2, 2*constraint.tag-2) = 1;
       } else if (constraint.direction == "Y") {
           for (int i =0; i < 2*nodeTags.size(); i ++) {
               K(2*constraint.tag-1, i) = 0;
               K(i, 2*constraint.tag-1) = 0;
           }
           K(2*constraint.tag-1, 2*constraint.tag-1) = 1;
       } else {
           for (int i =0; i < 2*nodeTags.size(); i ++) {
               K(2*constraint.tag-2, i) = 0;
               K(i, 2*constraint.tag-2) = 0;
           }
           for (int i =0; i < 2*nodeTags.size(); i ++) {
               K(2*constraint.tag-1, i) = 0;
               K(i, 2*constraint.tag-1) = 0;
           }
           K(2*constraint.tag-2, 2*constraint.tag-2) = 1;
            K(2*constraint.tag-1, 2*constraint.tag-1) = 1;
       }
    }
//    for (int i =0; i < 2*nodeTags.size(); i ++) {
//        std::cout << K(3, i) << std::endl;
//        std::cout << K(i, 3) << std::endl;
//    }
//    std::cout << "fuck you " << std::endl;
//    for (int i =0; i < 2*nodeTags.size(); i ++) {
//        std::cout << K(4, i) << std::endl;
//        std::cout << K(i, 4) << std::endl;
//    }

//    std::cout << "Here is the vector K_reverse:\n" << K << std::endl << std::endl;
    delta = K.reverse()*F;
//    std::cout << delta;







    // 计算形变和形力
//    delta(1) = 10000;

    Vector3d strain_element;
    Vector3d stress_element;
    VectorXd delta_element(6);
    for(int i = 0; i < triangleNodesTags.size(); i += 3) {
      // Dimension and tag of the entity:
      int nodeTag1 = triangleNodesTags[i];
      int nodeTag2 = triangleNodesTags[i+1];
      int nodeTag3 = triangleNodesTags[i+2];

      double x1 = nodeCoords[3 * (nodeTag1 - 1)];
      double y1 = nodeCoords[3 * (nodeTag1 - 1) + 1];
      double x2 = nodeCoords[3 * (nodeTag2 - 1)];
      double y2 = nodeCoords[3 * (nodeTag2 - 1) + 1];
      double x3 = nodeCoords[3 * (nodeTag3 - 1)];
      double y3 = nodeCoords[3 * (nodeTag3 - 1) + 1];
      double xi = x1/50;
      double yi = y1/50;
      double xj = x2/50;
      double yj = y2/50;
      double xk = x3/50;
      double yk = y3/50;

      double A = (yk*(xj-xi) - yj*(xk-xi) - yi*(xj-xk))/2;
      std::cout <<"A" << A << std::endl;
      int t = 1;
      int ai = xj*yk - xk*yj;
      int aj = xk*yi - xi*yk;
      int ak = xi*yj - xj*yi;

      double bi = yj-yk;
      double bj = yk-yi;
      double bk = yi-yj;

      double ci = -xj + xk;
      double cj = -xk + xi;
      double ck = -xi + xj;

      B(0,0) = 1.0/2/A*bi;
      B(0,2) = 1.0/2/A*bj;
       B(0,4) = 1.0/2/A*bk;
    B(1,1) = 1.0/2/A*ci;
     B(1,3) = 1.0/2/A*cj;
      B(1,5) = 1.0/2/A*ck;
       B(2,0) = 1.0/2/A*ci;
        B(2,1) = 1.0/2/A*bi;
     B(2,2) = 1.0/2/A*cj;
      B(2,3) = 1.0/2/A*bj;
       B(2,4) = 1.0/2/A*ck;
        B(2,5) = 1.0/2/A*bk;

        delta_element(0) = delta(2*nodeTag1-2);
        delta_element(1) = delta(2*nodeTag1-1);
        delta_element(2) = delta(2*nodeTag2-2);
        delta_element(3) = delta(2*nodeTag2-1);
        delta_element(4) = delta(2*nodeTag3-2);
        delta_element(5) = delta(2*nodeTag3-1);

        strain_element = B*delta_element;
        stress_element = D*strain_element;

        strain.push_back(strain_element(0));
        strain.push_back(strain_element(1));
        strain.push_back(strain_element(2));


        stress.push_back(stress_element(0));
        stress.push_back(stress_element(1));
        stress.push_back(stress_element(2));

    }
//    std::cout <<"K" << K << std::endl;
    std::cout << "calculate done!" << std::endl;


//    // eigen example
}


// 生成x方向应变的云图
void MyDrawingWidget::generateYuntu_strain_x(){
    isDrawingStrain_x = true;
    isDrawingStrain_y = false;
    isDrawingStress_x = false;
    isDrawingStress_y = false;

    update();

}

// 确定弹性模量E和泊松比v
void MyDrawingWidget::determineMateria(qreal E_input, qreal v_input){
       E = E_input;
       v = v_input;
}

//生成y方向应变的云图
void MyDrawingWidget::generateYuntu_strain_y(){
    isDrawingStrain_x = false;
    isDrawingStrain_y = true;
    isDrawingStress_x = false;
    isDrawingStress_y = false;

    update();

}

// 生成x方向应力的云图
void MyDrawingWidget::generateYuntu_stress_x(){
    isDrawingStrain_x = false;
    isDrawingStrain_y = false;
    isDrawingStress_x = true;
    isDrawingStress_y = false;

    update();

}

// 根据输入坐标的值输入矩形和圆
void MyDrawingWidget::generateRectFromCoordinate(qreal x, qreal y, qreal w, qreal h){
    m_rects.append(QRect(x,y,w,h));
    update();
}


void MyDrawingWidget::generateCircleFromCoordinate(qreal x, qreal y, qreal r){
    m_circles.append(QRect(x-r,y-r,2*r,2*r));
    update();
}
// 生成y方向应力的云图
void MyDrawingWidget::generateYuntu_stress_y(){
    isDrawingStrain_x = false;
    isDrawingStrain_y = false;
    isDrawingStress_x = false;
    isDrawingStress_y = true;

    update();

}

// utils:
// 1. 找到一个vector的最大最小值并返回
std::pair<double, double> MyDrawingWidget::findBestValue(std::vector<double> vec){
        //判断vector是否为空
        if (vec.empty()) {
            // Return a pair of NaN values to indicate no valid result
            return std::make_pair(std::numeric_limits<double>::quiet_NaN(),
                                  std::numeric_limits<double>::quiet_NaN());
        }

        // 初始化最大最小值
        double minVal = vec[0];
        double maxVal = vec[0];

        // Iterate over the vector to find the minimum and maximum values
        for (size_t i = 1; i < vec.size(); ++i) {
            if (vec[i] < minVal) {
                minVal = vec[i];
            }
            if (vec[i] > maxVal) {
                maxVal = vec[i];
            }
        }

        // Return a pair of the minimum and maximum values
        return std::make_pair(minVal, maxVal);
}

// 2. 将值映射为[0-1的范围]
double MyDrawingWidget::normalize(double min, double max, double value){
    double result = (value-min)/(max-min);
    return result;
}

// 3. 根据正则化之后的值获得指定位置的颜色
QColor MyDrawingWidget::getColorAtPosition(const QLinearGradient& gradient, qreal position)
{
    QGradientStops stops = gradient.stops();
    int numStops = stops.size();

    // 寻找插值区间
    int lowerIndex = 0;
    int upperIndex = numStops - 1;
//    for (int i = 0; i < numStops; ++i)
//    {
//        if (stops[i].first >= position)
//        {
//            upperIndex = i;
//            if (i > 0)
//                lowerIndex = i - 1;
//            break;
//        }
//    }
    if ( 0 <= position &&  position <= 0.125) {
        lowerIndex = 0;
        upperIndex = 1;
    }else if (0.125 < position &&  position <= 0.25){
        lowerIndex = 1;
        upperIndex = 2;
    }else if (0.25 < position &&  position <= 0.375){
        lowerIndex = 2;
        upperIndex = 3;
    }else if (0.375 < position &&  position <= 0.5){
        lowerIndex = 3;
        upperIndex = 4;
    }else if (0.5 < position &&  position <= 0.625){
        lowerIndex = 4;
        upperIndex = 5;
    }else if (0.625 < position &&  position <= 0.75){
        lowerIndex = 5;
        upperIndex = 6;
    }else if (0.75 < position &&  position <= 0.875){
        lowerIndex = 6;
        upperIndex = 7;
    }else {
        lowerIndex = 7;
        upperIndex = 8;
    }
    // 计算插值权重
    qreal lowerPosition = stops[lowerIndex].first;
    qreal upperPosition = stops[upperIndex].first;
    qreal t = (position - lowerPosition) / (upperPosition - lowerPosition);

    // 执行颜色插值
    QColor lowerColor = stops[lowerIndex].second;
    QColor upperColor = stops[upperIndex].second;
    QColor interpolatedColor = QColor::fromRgbF(
        lowerColor.redF()   * (1 - t) + upperColor.redF()   * t,
        lowerColor.greenF() * (1 - t) + upperColor.greenF() * t,
        lowerColor.blueF()  * (1 - t) + upperColor.blueF()  * t,
        lowerColor.alphaF() * (1 - t) + upperColor.alphaF() * t
    );

    return interpolatedColor;
}

// 清除已有的结构
void MyDrawingWidget::remake(){
    stress.clear();
    strain.clear();
    triangleNodesTags.clear();
    lineNodesTags.clear();
    elemTypes.clear();
    nodeCoords.clear();
    nodeParams.clear();
    nodeTags.clear();
    entities.clear();
    lineNodes.clear();
    m_rects.clear();
    m_circles.clear();
    forcePosition.clear();
    m_forces.clear();
    m_displacementConstraints.clear();
    meshLines.clear();

    isDrawingStrain_x = false;
    isDrawingStrain_y = false;
    isDrawingStress_x = false;
    isDrawingStress_y = false;

    m_startPos= QPointF(0, 0);
    m_endPos = QPointF(0, 0);
    m_isDrawingForceConstraint = false;
    m_isDrawingDisplacementConstraint = false;
    m_isDrawing = true;
    for (auto vec:elemNodeTags){
        vec.clear();
    }
    elemNodeTags.clear();

    for (auto vec:elemTags){
        vec.clear();
    }
    elemTags.clear();
    update();
}

std::vector<double> MyDrawingWidget::calculate_element_K(double xi, double yi, double xj, double yj, double xk, double yk){
    xi = xi/100;
    yi = yi/100;
    xj = xj/100;
    yj = yj/100;
    xk = xk/100;
    yk = yk/100;

    double A = (yk*(xj-xi) - yj*(xk-xi) - yi*(xj-xk))/2;
    int t = 1;
    int ai = xj*yk - xk*yj;
    int aj = xk*yi - xi*yk;
    int ak = xi*yj - xj*yi;

    int bi = yj-yk;
    int bj = yk-yi;
    int bk = yi-yj;

    int ci = -xj + xk;
    int cj = -xk + xi;
    int ck = -xi + xj;

    int coefficent = E*t/(4*(1-v*v)*A);
    std::vector<double> K;
    // calculate K_ii
    K.push_back(coefficent*(bi*bi + (1-v)*ci*ci/2));
    K.push_back(coefficent*(v*bi*ci + (1-v)*ci*bi/2));
    K.push_back(coefficent*(v*bi*ci + (1-v)*ci*bi/2));
    K.push_back(coefficent*(ci*ci + (1-v)*bi*bi/2));

    // calculate K_ij
    K.push_back(coefficent*(bi*bj + (1-v)*ci*cj/2));
    K.push_back(coefficent*(v*bi*cj + (1-v)*ci*bj/2));
    K.push_back(coefficent*(v*ci*bj + (1-v)*bi*cj/2));
    K.push_back(coefficent*(ci*cj + (1-v)*bi*bj/2));

    // calculate K_ik
    K.push_back(coefficent*(bi*bk + (1-v)*ci*ck/2));
    K.push_back(coefficent*(v*bi*ck + (1-v)*ci*bk/2));
    K.push_back(coefficent*(v*ci*bk + (1-v)*bi*ck/2));
    K.push_back(coefficent*(ci*ck + (1-v)*bi*bk/2));

    // calculate K_ji
    K.push_back(coefficent*(bj*bi + (1-v)*cj*ci/2));
    K.push_back(coefficent*(v*bj*ci + (1-v)*cj*bi/2));
    K.push_back(coefficent*(v*cj*bi + (1-v)*bj*ci/2));
    K.push_back(coefficent*(cj*ci + (1-v)*bj*bi/2));

    // calculate K_jj
    K.push_back(coefficent*(bj*bj + (1-v)*cj*cj/2));
    K.push_back(coefficent*(v*bj*cj + (1-v)*cj*bj/2));
    K.push_back(coefficent*(v*cj*bj + (1-v)*bj*cj/2));
    K.push_back(coefficent*(cj*cj + (1-v)*bj*bj/2));

    // calculate K_jk
    K.push_back(coefficent*(bj*bk + (1-v)*cj*ck/2));
    K.push_back(coefficent*(v*bj*ck + (1-v)*cj*bk/2));
    K.push_back(coefficent*(v*cj*bk + (1-v)*bj*ck/2));
    K.push_back(coefficent*(cj*ck + (1-v)*bj*bk/2));

    // calculate K_ki
    K.push_back(coefficent*(bk*bi + (1-v)*ck*ci/2));
    K.push_back(coefficent*(v*bk*ci + (1-v)*ck*bi/2));
    K.push_back(coefficent*(v*ck*bi + (1-v)*bk*ci/2));
    K.push_back(coefficent*(ck*ci + (1-v)*bk*bi/2));

    // calculate K_kj
    K.push_back(coefficent*(bk*bj + (1-v)*ck*cj/2));
    K.push_back(coefficent*(v*bk*cj + (1-v)*ck*bj/2));
    K.push_back(coefficent*(v*ck*bj + (1-v)*bk*cj/2));
    K.push_back(coefficent*(ck*cj + (1-v)*bk*bj/2));

    // calculate K_kk
    K.push_back(coefficent*(bk*bk + (1-v)*ck*ck/2));
    K.push_back(coefficent*(v*bk*ck + (1-v)*ck*bk/2));
    K.push_back(coefficent*(v*ck*bk + (1-v)*bk*ck/2));
    K.push_back(coefficent*(ck*ck + (1-v)*bk*bk/2));

    return K;
}

