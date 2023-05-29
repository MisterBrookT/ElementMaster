#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <MyDrawingWidget.h>
#include <QMainWindow>
#include <QDateTime>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();

    void on_forceConstraintButton_clicked();

    void on_displacementConstraintButton_clicked();

    void on_generateMeshButton_clicked();

    void on_pushButton_2_clicked();

    void on_yuntu_strain_x_clicked();

    void on_yuntu_strain_y_clicked();

    void on_yuntu_stress_x_clicked();

    void on_yuntu_stress_y_clicked();

//    void on_juxingXzuobiao_editingFinished();

    void on_tijiaojuxing_clicked();

    void on_tijiaoyuanxing_clicked();

//    void on_lineEdit_3_inputRejected();

    QString getTimeNow();

    void on_fenbuforceConstaintButton_clicked();

    void on_tijiaocailiao_clicked();

    void on_remake_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
