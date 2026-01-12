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
    void SlideNext();
    void SlidePre();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    void UpSelectPixmap(QTreeWidgetItem*);
    float _factor;
    QTimer* _timer;
    QPixmap _pixmap1;
    QPixmap _pixmap2;
    QTreeWidgetItem* _cur_item; // 当前播放到的图片(item)
    QMap<QString, QTreeWidgetItem*> _map_items;
    bool _b_start;

public slots:
    void SlotUpSelectShow(QString);
    void SlotStartOrStop();

private slots:
    void TimeOut();

signals:
    void SigUpPreList(QTreeWidgetItem*);
    void SigSelectItem(QTreeWidgetItem*);

//    两个信号应该可以合一
    void SigStartOrStop(bool); // 改变‘播放’按钮的状态
    void SigStartOrStopMusic(bool); // 改变音乐播放的状态
};

#endif // PICANIMATIONWID_H
