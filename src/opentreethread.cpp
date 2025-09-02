#include "inc/const.h"
#include "inc/opentreethread.h"
#include "inc/protreeitem.h"
#include <QDir>

OpenTreeThread::OpenTreeThread(const QString &src_path, int file_count, QTreeWidget *self)
    : _bstop(false), _src_path(src_path), _file_count(file_count),
      _self(self), _root(nullptr)
{

}

void OpenTreeThread::OpenProTree(const QString &src_path, int &file_count, QTreeWidget *self)
{
    QDir src_dir(src_path);
    auto name = src_dir.dirName();
    auto * item = new ProTreeItem(self, name, src_path, TreeItemPro);
    item->setData(0, Qt::DisplayRole, name);
    item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
    item->setData(0, Qt::ToolTipRole, src_path);
    _root = item;
    RecursiveProTree(src_path, file_count, self, _root, item, nullptr);
}

void OpenTreeThread::run()
{
    OpenProTree(_src_path, _file_count, _self);
    if(_bstop&&_root){
        auto path = dynamic_cast<ProTreeItem*>(_root)->GetPath();
        auto index = _self->indexOfTopLevelItem(_root);
        delete _self->takeTopLevelItem(index);

// the part of code is from 'protreethread.cpp',
// it's target is remove all files when user want to stop transport some files into new project
// but in here
// what we want to do is just show old project in album
// need to avoid to delete file wrong!!!
//        QDir dir(path);
//        dir.removeRecursively();

        return;
    }

    emit SigFinishProgress(_file_count);
}

void OpenTreeThread::RecursiveProTree(const QString &src_path, int &file_count, QTreeWidget *self, QTreeWidgetItem *root, QTreeWidgetItem *parent, QTreeWidgetItem *preitem)
{
    QDir src_dir(src_path);
    src_dir.setFilter(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    src_dir.setSorting(QDir::Name);
    QFileInfoList list = src_dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        if (_bstop) {
            return;
        }
        QFileInfo fileInfo = list.at(i);
        bool bIsDir = fileInfo.isDir();
        if (bIsDir) {
            if (_bstop) {
                return;
            }
            file_count++;
            emit SigUpdateProgress(file_count);
            auto * item = new ProTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(),
                                          _root, TreeItemDir);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/dir.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
            RecursiveProTree(fileInfo.absoluteFilePath(), file_count, self, _root, item, preitem);
        } else {
            if (_bstop) {
                return;
            }
            const QString& suffix = fileInfo.completeSuffix();
            if (suffix != "png" && suffix != "jpeg" && suffix != "jpg") {
                continue;
            }
            file_count++;
            emit SigUpdateProgress(file_count);
            auto * item = new ProTreeItem(parent, fileInfo.fileName(), fileInfo.absoluteFilePath(),
                                                      _root, TreeItemPic);
            item->setData(0, Qt::DisplayRole, fileInfo.fileName());
            item->setData(0, Qt::DecorationRole, QIcon(":/icon/pic.png"));
            item->setData(0, Qt::ToolTipRole, fileInfo.absoluteFilePath());
            if (preitem) {
                auto * pre_proitem = dynamic_cast<ProTreeItem*>(preitem);
                pre_proitem->SetNextItem(item);
                item->SetPreItem(preitem);
                preitem = item;
            }
        }
    }
//    emit SigFinishProgress(file_count);
}
