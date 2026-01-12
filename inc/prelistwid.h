#ifndef PRELISTWID_H
#define PRELISTWID_H

#include <QListWidget>
#include <QTreeWidgetItem>

class PreListWid : public QListWidget
{
    Q_OBJECT
public:
    PreListWid(QWidget *parent = nullptr);
    virtual ~PreListWid();
private:
    void AddListItem(const QString& path);
    QMap<QString, QListWidgetItem*> _set_items;
    int _global;
    QPoint _pos_origin;
    int _last_index;

signals:
    void SigUpSelectShow(QString path);

public slots:
    void SlotUpPreList(QTreeWidgetItem*);
    void SlotUpSelect(QTreeWidgetItem*);
    void SlotItemPressed(QListWidgetItem*);
};

#endif // PRELISTWID_H
