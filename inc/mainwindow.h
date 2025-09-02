#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QWidget * _protree;

private slots:
    void SlotCreatePro( bool );
    void SlotOpenPro( bool );
signals:
    void SigOpenPro(const QString& path);
};

#endif // MAINWINDOW_H
