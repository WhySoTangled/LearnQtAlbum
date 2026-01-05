#include "../inc/picanimationwid.h"
#include "protreeitem.h"
#include <QPainter>
#include <QTimer>


PicAnimationWid::PicAnimationWid(QWidget *parent)
    : QWidget{parent}, _factor(0.0), _cur_item(nullptr), _b_start(false)
{
    _timer = new QTimer(this);
    connect(_timer, &QTimer::timeout, this, &PicAnimationWid::TimeOut);
}

PicAnimationWid::~PicAnimationWid()
{
// _timer's parent is 'this'
}

void PicAnimationWid::SetPixmap(QTreeWidgetItem * item)
{
    if (!item) {
        return;
    }

    auto * pro_tree_item = dynamic_cast<ProTreeItem*>(item);
    auto path = pro_tree_item->GetPath();
    _pixmap1.load(path);
    _cur_item = pro_tree_item;
    if (_map_items.find(path) == _map_items.end()) {
        _map_items[path] = pro_tree_item;
        // todo
    }

    // todo : 这里还不知道获取next是干嘛的
    auto * next_item = pro_tree_item->GetNextItem();
    if (!next_item) {
        return;
    }
    auto next_path = next_item->GetPath();
    _pixmap2.load(next_path);
    if (_map_items.find(next_path) == _map_items.end()) {
        _map_items[next_path] = next_item;
        // todo
    }
}

void PicAnimationWid::Start()
{
    _factor = 0.0;
    _timer->start();
    _b_start = true;
}

void PicAnimationWid::Stop()
{
    _timer->stop();
    _factor = 0.0;
    _b_start = false;
}

void PicAnimationWid::paintEvent(QPaintEvent *event)
{
    if (_pixmap1.isNull()) {
        return ;
    }
    QPainter painter(this);
    // Indicates that the engine should antialias edges of primitives if possible.
    // 抗锯齿，无需理解，用就行了
    painter.setRenderHint(QPainter::Antialiasing);

    QRect rect = geometry();
    int w = rect.width();
    int h = rect.height();
// The size is scaled to a rectangle as large as possible inside a given rectangle, preserving the aspect ratio.
    _pixmap1 = _pixmap1.scaled(w, h, Qt::KeepAspectRatio);
    int alpha = 255 * (1.0f - _factor);
    QPixmap alphaPixmap(_pixmap1.size());
// a transparent black value
    alphaPixmap.fill(Qt::transparent);

    QPainter p1(&alphaPixmap);
// The output is the source pixel.
// (This means a basic copy operation and is identical to SourceOver when the source pixel is opaque).
    p1.setCompositionMode(QPainter::CompositionMode_Source);
    p1.drawPixmap(0, 0, _pixmap1);
// The output is the destination pixel.
// This means that the blending has no effect.
// This mode is the inverse of CompositionMode_Source.
    p1.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p1.fillRect(alphaPixmap.rect(), QColor(0, 0, 0, alpha));
    p1.end();

    int x = (w - alphaPixmap.width()) / 2;
    int y = (h - alphaPixmap.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap);

    if (_pixmap2.isNull()) {
        return ;
    }
    _pixmap2 = _pixmap2.scaled(w, h, Qt::KeepAspectRatio);
    alpha = 255 * _factor;
    QPixmap alphaPixmap_2(_pixmap2.size());
    alphaPixmap_2.fill(Qt::transparent);
    QPainter p2(&alphaPixmap_2);
    p2.setCompositionMode(QPainter::CompositionMode_Source);
    p2.drawPixmap(0, 0, _pixmap2);
    p2.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    p2.fillRect(alphaPixmap_2.rect(), QColor(0, 0, 0, alpha));
    p2.end();

    x = (w - alphaPixmap_2.width()) / 2;
    y = (h - alphaPixmap_2.height()) / 2;
    painter.drawPixmap(x, y, alphaPixmap_2);
}

/**
 * @brief a slot func which is connected to _timer(QTimer)
*/
void PicAnimationWid::TimeOut()
{
    if (!_cur_item) {
        Stop();
        update();
        return ;
    }
    _factor += 0.01;
    if (_factor >= 1.0) {
        _factor = 0.0;
        auto * cur_pro_item = dynamic_cast<ProTreeItem *>(_cur_item);
        auto * next_pro_item = cur_pro_item->GetNextItem();
        if (!next_pro_item) {
            Stop();
            update();
            return ;
        }
        SetPixmap(next_pro_item);
        update();
        return ;
    } else {
        update();
    }
}
