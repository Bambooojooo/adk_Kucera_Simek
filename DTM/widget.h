#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT
    private:
        double zmin, zmax, dz, n_points;
        int k;

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_pushButton_cleardt_clicked();

    void on_lineEdit_editingFinished();

    void on_lineEdit_2_editingFinished();

    void on_lineEdit_3_editingFinished();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_lineEdit_4_editingFinished();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_lineEdit_5_editingFinished();

    void on_pushButton_Load_clicked();

    void on_pushButton_6_clicked();

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_2_stateChanged(int arg1);

    void on_save_clicked();

    void on_save_canvas_clicked();

    void on_dmtUP_clicked();

private:
    Ui::Widget *ui;
};
#endif // WIDGET_H
