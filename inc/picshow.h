#ifndef PICSHOW_H
#define PICSHOW_H

#include <QDialog>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

namespace Ui {
class PicShow;
}

class PicShow : public QDialog
{
    Q_OBJECT

public:
    explicit PicShow(QWidget *parent = 0);
    ~PicShow();
    void ReloadPic();

private:
    void ShowPreNextBtns(bool b_show);
    Ui::PicShow *ui;
    QPropertyAnimation* _animation_show_pre;
    QPropertyAnimation* _animation_show_next;
    bool _b_btnvisible;
    QString _selected_path;
    QPixmap _pix_map;

protected:
    // Event handlers
    bool event(QEvent *event) override;

public slots:
    void SlotSelectItem(const QString& path);
    void SlotUpdatePro(const QString& path);
    void SlotDeleteItem();
signals:
    void SigPreClicked();
    void SigNextClicked();
};

#endif // PICSHOW_H
