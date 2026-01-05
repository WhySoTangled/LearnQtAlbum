#ifndef PICANIMATIONWID_H
#define PICANIMATIONWID_H

#include <QWidget>
#include <QTreeWidgetItem>

class PicAnimationWid : public QWidget
{
    Q_OBJECT
public:
    explicit PicAnimationWid(QWidget *parent = nullptr);
    ~PicAnimationWid();
    void SetPixmap(QTreeWidgetItem*);
    void Start();
    void Stop();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    float _factor;
    QTimer* _timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem* _cur_item; // 当前播放到的图片(item)
    QMap<QString, QTreeWidgetItem*> _map_items;
    bool _b_start;

private slots:
    void TimeOut();

signals:

};

#endif // PICANIMATIONWID_H
