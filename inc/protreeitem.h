#ifndef PROTREEITEM_H
#define PROTREEITEM_H

#include <QTreeWidget>
#include <QTreeWidgetItem>

//========== project and pro's directory and pro's picture ==========
// [modefied by OpenTreeThread and ProTreeThread]
// itemPro(ProTreeItem, type == TreeItemPro)
// ├── itemPic_1(ProTreeItem, TreeItemPic)
// ├── itemDir_2(ProTreeItem, TreeItemDir)
// └── itemDir_3(ProTreeItem, TreeItemDir)
//     ├── itemPic_3_1(ProTreeItem, TreeItemPic)
//     └── itemPic_3_2(ProTreeItem, TreeItemPic)
//===================================================================

class ProTreeItem : public QTreeWidgetItem
{
public:
    ProTreeItem( QTreeWidget * parent, const QString& name, const QString& path, int type = Type );
    ProTreeItem( QTreeWidgetItem * parent, const QString& name, const QString& path, QTreeWidgetItem * root, int type = Type );
    const QString& GetPath();
    QTreeWidgetItem * GetRoot();
    void SetPreItem(QTreeWidgetItem* item);
    void SetNextItem(QTreeWidgetItem* item);
    ProTreeItem* GetPreItem();
    ProTreeItem* GetNextItem();
    ProTreeItem* GetLastPicChild();
    ProTreeItem* GetFirstPicChild();
private:
    QString _path;
    QString _name;
    QTreeWidgetItem* _root;
    QTreeWidgetItem* _pre_item;
    QTreeWidgetItem* _next_item;
};

#endif // PROTREEITEM_H
