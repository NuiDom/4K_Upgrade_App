#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

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

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_Upgrade_push_button_clicked();

    void on_Open_File_push_button_clicked();

    void on_pushButton_Erase_clicked();

    void on_pushButton_ReadMemory_clicked();

signals:
     void signalProgramPIC();
     void signalReadPIC();

private:
    Ui::MainWindow *ui;

    void writeToFile(QString fileName);
    void readFromFile(QString fileName);
};
#endif // MAINWINDOW_H
