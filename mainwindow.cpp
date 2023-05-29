#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/tubiao.png"));
}

MainWindow::~MainWindow()
{
    delete ui;
}

QString MainWindow::getTimeNow()
{
    QDateTime timestamp = QDateTime::currentDateTime();
    return timestamp.toString("yyyy-MM-dd hh:mm:ss") + " - ";
}

void MainWindow::on_pushButton_clicked()
{
//    MyDrawingWidget *drawingWidget = ui->widget->findChild<MyDrawingWidget *>();
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->switchShape("Rectangle");
    ui->log->append(getTimeNow() + " 即将绘制矩形（在右侧中拖动鼠标即可绘制矩形）");
}


void MainWindow::on_pushButton_3_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->switchShape("Circle");
    ui->log->append(getTimeNow() + " 即将绘制圆形（在右侧中拖动鼠标即可绘制圆形）");
}


void MainWindow::on_forceConstraintButton_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->enableDisplacementConstraint(false);
    drawingWidget->enableForceConstraint(true);
    ui->log->append(getTimeNow() + " 即将添加集中力约束（在节点对应的红色圆圈中点击即可添加力约束）");
}


void MainWindow::on_displacementConstraintButton_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->enableForceConstraint(false);
    drawingWidget->enableDisplacementConstraint(true);
    ui->log->append(getTimeNow() + " 即将添加位移约束（在节点对应的红色圆圈中点击即可添加位移约束）");
}

// ...
void MainWindow::on_fenbuforceConstaintButton_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->enableJunbuForceConstraint(true);
    ui->log->append(getTimeNow() + " 即将添加分布力约束（在节点对应的红色圆圈中点击即可添加力约束）");
}

void MainWindow::on_generateMeshButton_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->generateMesh();
    ui->log->append(getTimeNow() + " 划分网格成功， 网格大小:2 ");
}


void MainWindow::on_pushButton_2_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->calculate();
    ui->log->append(getTimeNow() + " 计算成功，用户现在可生成云图  ");
}


void MainWindow::on_yuntu_strain_x_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget-> generateYuntu_strain_x();
    ui->log->append(getTimeNow() + " 生成了x方向的应变云图  ");
}


void MainWindow::on_yuntu_strain_y_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget-> generateYuntu_strain_y();
    ui->log->append(getTimeNow() + " 生成了y方向的应变云图  ");
}


void MainWindow::on_yuntu_stress_x_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget-> generateYuntu_stress_x();
    ui->log->append(getTimeNow() + " 生成了x方向的应力云图  ");
}




void MainWindow::on_yuntu_stress_y_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget-> generateYuntu_stress_y();
    ui->log->append(getTimeNow() + " 生成了y方向的应力云图  ");
}




void MainWindow::on_tijiaojuxing_clicked()
{
    qreal x = ui->juxingXzuobiao->text().toDouble();
    qreal y = ui->juxingYzuobiao->text().toDouble();
    qreal w = ui->juxingkuan->text().toDouble();
    qreal h = ui->juxinggao->text().toDouble();
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget-> generateRectFromCoordinate(x, y, w, h);
    // 测试用
//    drawingWidget-> generateRectFromCoordinate(200, 150, 600, 300);
    ui->log->append(getTimeNow() + " 通过输入坐标的方式生成了矩形！  ");
//    qDebug() << "获取到的值：" << juxingxzuobiao;
//    qDebug() << "获取到的值：" << juxingyzuobiao;
//    qDebug() << "获取到的值：" << juxingkuan;
//    qDebug() << "获取到的值：" << juxinggao;
}

void MainWindow::on_tijiaoyuanxing_clicked()
{
    qreal x = ui->yuanxinzuobiaox->text().toDouble();
    qreal y = ui->yuanxinzuobiaoy->text().toDouble();
    qreal r = ui->yuanbanjingr->text().toDouble();
//    qDebug() << "获取到的值：" << x;
//    qDebug() << "获取到的值：" << y;
//    qDebug() << "获取到的值：" << r;
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget-> generateCircleFromCoordinate(x, y, r);
    // test
//    drawingWidget-> generateCircleFromCoordinate(350, 300, 60);
//    drawingWidget-> generateCircleFromCoordinate(650, 300, 60);
    ui->log->append(getTimeNow() + " 通过输入坐标的方式生成了圆形！  ");

}

void MainWindow::on_tijiaocailiao_clicked()
{

    qreal E = ui->yangshimoliang->text().toDouble();
    qreal v = ui->bosongbi->text().toDouble();
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->determineMateria(E, v);
    ui->log->append(getTimeNow() + " 用户提交了材料属性！  ");
}

void MainWindow::on_remake_clicked()
{
    MyDrawingWidget *drawingWidget = ui->widget;
    drawingWidget->remake();
    ui->log->append(getTimeNow() + " 用户删除了模型！  ");
}

//void MainWindow::on_juxingXzuobiao_editingFinished()
//{

//}




//void MainWindow::on_lineEdit_3_inputRejected()
//{

//}





