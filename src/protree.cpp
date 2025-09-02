#include "inc/protree.h"
#include "ui_protree.h"

ProTree::ProTree(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ProTree)
{
    ui->setupUi(this);

    // fixed size
    setMinimumWidth(378);
    setMaximumWidth(378);
}

ProTree::~ProTree()
{
    delete ui;
}

QTreeWidget *ProTree::GetTreeWidget()
{
    return ui->treeWidget;
}

void ProTree::AddProToTree(const QString &name, const QString &path)
{
    ui->treeWidget->AddProToTree( name, path );
}
