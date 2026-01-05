#ifndef PROTREE_H
#define PROTREE_H

#include <QDialog>
#include <QTreeWidget>

namespace Ui {
class ProTree;
}

//===============================================
// _protree(ProTree)
// ├── label_pro(QLabel, qss set hide)
// └── treeWidget(ProTreeWidget)
//     ├── item_1(ProTreeItem, type() == TreeItemPro)
//     ├── item_2(ProTreeItem, TreeItemPro)
//     └── item_3(ProTreeItem)
//===============================================

class ProTree : public QDialog
{
    Q_OBJECT

public:
    explicit ProTree(QWidget *parent = 0);
    ~ProTree();
    QTreeWidget * GetTreeWidget();

private:
    Ui::ProTree *ui;

public slots:
    void AddProToTree( const QString& name, const QString& path );
};

#endif // PROTREE_H
