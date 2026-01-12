#include "../inc/prelistwid.h"
#include <QDebug>
#include <QGuiApplication>
#include <QPainter>
#include <QPixmap>
#include "const.h"
#include "protreeitem.h"
#include "prelistitem.h"

PreListWid::PreListWid(QWidget *parent)
    : QListWidget(parent), _global(0), _last_index(17)
{
    this->setViewMode(QListWidget::IconMode);
    this->setIconSize(QSize(PREICON_SIZE, PREICON_SIZE));
    this->setSpacing(5);
//    connect(this, &QListWidget::itemClicked,
//            this, &PreListWid::SlotItemPressed);
    connect(this, &QListWidget::itemPressed,
            this, &PreListWid::SlotItemPressed);
}

PreListWid::~PreListWid()
{

}

/**
 * @brief add a new PreListItem to this PreListWid
 * @param path : key
 * @note insert {key : path, value : pItem} into _set_items
 */
void PreListWid::AddListItem(const QString &path)
{
    QPixmap src_pixmap(path);
    src_pixmap = src_pixmap.scaled(PREICON_SIZE, PREICON_SIZE, Qt::KeepAspectRatio);
    QPixmap dst_pixmap(QSize(PREICON_SIZE, PREICON_SIZE));
    dst_pixmap.fill(QColor(220, 220, 220, 50));
    QPainter painter(&dst_pixmap);
    auto src_width = src_pixmap.width();
    auto src_height = src_pixmap.height();

    auto dst_width = dst_pixmap.width();
    auto dst_height = dst_pixmap.height();
    auto x = (dst_width - src_width) / 2;
    auto y = (dst_height - src_height) / 2;
    painter.drawPixmap(x, y, src_pixmap);
    ++_global;
    PreListItem * pItem = new PreListItem(QIcon(dst_pixmap), path, _global, this);
    pItem->setSizeHint(QSize(PREITEM_SIZE, PREITEM_SIZE));
//    todo: 似乎不需要显式调用add？
//    this->addItem(pItem);
//    insert {key : path, value : pItem} into _set_items
    _set_items[path] = pItem;
    if (_global == 1) {
        _pos_origin = this->pos();
    }
}

void PreListWid::SlotUpPreList(QTreeWidgetItem * tree_item)
{
    if (!tree_item) {
        qDebug() << "tree_item is empty" << Qt::endl;
        return ;
    }
    auto * pro_item = dynamic_cast<ProTreeItem*>(tree_item);
    auto path = pro_item->GetPath();
    auto iter = _set_items.find(path);
    if (iter != _set_items.end()) {
        qDebug() << "path " << path << " exists" << Qt::endl;
        return ;
    }
    AddListItem(path);
}

void PreListWid::SlotUpSelect(QTreeWidgetItem * tree_item)
{
    if (!tree_item) {
        return ;
    }
    auto * pro_item = dynamic_cast<ProTreeItem*>(tree_item);
    auto Path = pro_item->GetPath();
    auto iter = _set_items.find(Path);
    if (iter == _set_items.end()) {
        return ;
    }
    auto * list_item = dynamic_cast<PreListItem*>(iter.value());
    auto index = list_item->GetIndex();
    if (index > 17) {
        auto pos_cur = this->pos();
//        where space?
        this->move(pos_cur.x() - (index - _last_index) * PREITEM_SIZE, pos_cur.y());
    } else {
        this->move(_pos_origin);
        _last_index = 17;
    }
    this->setCurrentItem(iter.value());
}

void PreListWid::SlotItemPressed(QListWidgetItem * item)
{
    if (QGuiApplication::mouseButtons() != Qt::LeftButton) {
        return ;
    }
    if (!item) {
        return ;
    }
    auto * list_item = dynamic_cast<PreListItem*>(item);
    auto cur_index = list_item->GetPath();
    auto path = list_item->GetPath();
    this->setCurrentItem(item);
    emit SigUpSelectShow(path);
}
