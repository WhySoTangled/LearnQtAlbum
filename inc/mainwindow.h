#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QResizeEvent>

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
    QWidget* _protree;
    QWidget* _picshow;
protected:
    virtual void resizeEvent(QResizeEvent *event) override;
private slots:
    void SlotCreatePro( bool );
    void SlotOpenPro( bool );
signals:
    void SigOpenPro(const QString& path);
};

#endif // MAINWINDOW_H
