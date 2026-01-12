#include <QDebug>
#include <QDir>
#include <QFileDialog>
#include <QGuiApplication>
#include <QHeaderView>
#include <QMenu>
#include "inc/const.h"
#include "inc/opentreethread.h"
#include "inc/protreewidget.h"
#include "inc/protreeitem.h"
#include "inc/removeprodialog.h"
#include "slideshowdlg.h"

ProTreeWidget::ProTreeWidget( QWidget *parent ) : QTreeWidget( parent ),
    _right_btn_item(nullptr), _active_item(nullptr), _selected_item(nullptr),
    _dialog_progress(nullptr), _open_progressdlg(nullptr),
    _thread_create_pro(nullptr), _thread_open_pro(nullptr)
{
    this->header()->hide();
    _right_btn_item = nullptr;

    connect(this, &ProTreeWidget::itemPressed, this, &ProTreeWidget::SlotItemPressed);
    // init qactions
    _action_import = new QAction(QIcon(":/icon/import.png"), tr("导入文件"), this);
    _action_setstart = new QAction(QIcon(":/icon/core.png"), tr("设置活动项目"),this);
    _action_closepro = new QAction(QIcon(":/icon/close.png"), tr("关闭项目"), this);
    _action_slideshow = new QAction(QIcon(":/icon/slideshow.png"), tr("轮播图播放"),this);
    connect(_action_import, &QAction::triggered, this, &ProTreeWidget::SlotImport);
    connect(_action_closepro, &QAction::triggered, this, &ProTreeWidget::SlotClosePro);
    connect(_action_setstart, &QAction::triggered, this, &ProTreeWidget::SlotSetActive);
    connect(_action_slideshow, &QAction::triggered, this, &ProTreeWidget::SlotSlideShow);
    connect(this, &ProTreeWidget::itemDoubleClicked, this, &ProTreeWidget::SlotDoubleClickItem);

    _player = new QMediaPlayer(this);
    _playlist = new QMediaPlaylist(this);
    _playlist->setPlaybackMode(QMediaPlaylist::Loop);
    _player->setPlaylist(_playlist);
}

/**
 * @brief add a new ProTreeItem to this ProTreeWidget
 * @note
 **/
void ProTreeWidget::AddProToTree(const QString &name, const QString &path)
{
    QDir dir(path);
    QString file_path = dir.absoluteFilePath(name);
    if ( _set_path.find(file_path) != _set_path.end() )
    {
        return ;
    }

    QDir pro_dir(file_path);
    if ( !pro_dir.exists() )
    {
        bool enable = pro_dir.mkpath(file_path);
        if ( !enable )
        {
            return;
        }
    }

    _set_path.insert(file_path);
    auto * item = new ProTreeItem( this, name, file_path, TreeItemPro );
    item->setData( 0, Qt::DisplayRole, name );
    item->setData( 0, Qt::DecorationRole, QIcon(":/icon/dir.png") );
    item->setData( 0, Qt::ToolTipRole, file_path );
}

void ProTreeWidget::SlotItemPressed(QTreeWidgetItem *pressedItem, int column)
{
    if (QGuiApplication::mouseButtons() == Qt::RightButton)
    {
        QMenu menu(this);
        int itemtype = pressedItem->type();
        if (itemtype == TreeItemPro)
        {
            _right_btn_item = pressedItem;
            menu.addAction(_action_import);
            menu.addAction(_action_setstart);
            menu.addAction(_action_closepro);
            menu.addAction(_action_slideshow);
            menu.exec(QCursor::pos());
        }
    }
}

void ProTreeWidget::SlotImport()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::Directory);
    file_dialog.setWindowTitle("选择导入的文件夹");
    QString path;
    if(!_right_btn_item){
        qDebug() << "_right_btn_item is empty" << Qt::endl;
        return ;
    }

    path = dynamic_cast<ProTreeItem*>(_right_btn_item)->GetPath();
    file_dialog.setDirectory(path);
    file_dialog.setViewMode(QFileDialog::Detail);

    QStringList fileNames;
    if (file_dialog.exec()) {
        fileNames = file_dialog.selectedFiles();
    }

    if (fileNames.isEmpty()) {
        return;
    }

    QString import_path = fileNames.at(0);
    int file_count = 0;
    //创建模态对话框
    _dialog_progress = new QProgressDialog(this);

    _thread_create_pro = std::make_shared<ProTreeThread>(std::ref(import_path), std::ref(path), _right_btn_item,
                                                         file_count, this, _right_btn_item, nullptr);
    //连接更新进度框操作
    connect(_thread_create_pro.get(), &ProTreeThread::SigUpdateProgress,
            this, &ProTreeWidget::SlotUpdateProgress);
    connect(_thread_create_pro.get(), &ProTreeThread::SigFinishProgress,
            this, &ProTreeWidget::SlotFinishProgress);
    connect(_dialog_progress, &QProgressDialog::canceled,
            this, &ProTreeWidget::SlotCancelProgress);
    connect(this, &ProTreeWidget::SigCancelProgress,
            _thread_create_pro.get(), &ProTreeThread::SlotCancelProgress);

    // thread run
    _thread_create_pro->start();

    _dialog_progress->setWindowTitle("Please wait...");
    _dialog_progress->setFixedWidth(PROGRESS_WIDTH);
    _dialog_progress->setRange(0, PROGRESS_MAX);
    _dialog_progress->exec();

}

void ProTreeWidget::SlotUpdateProgress(int count)
{
    if (!_dialog_progress) {
        return;
    }
    if (count >= PROGRESS_MAX) {
        _dialog_progress->setValue(count % PROGRESS_MAX);
    } else {
        _dialog_progress->setValue(count);
    }
}

void ProTreeWidget::SlotFinishProgress( )
{
    _dialog_progress->setValue(PROGRESS_MAX);
    _dialog_progress->deleteLater();
}

void ProTreeWidget::SlotCancelProgress()
{
    emit SigCancelProgress();
    delete _dialog_progress;
    _dialog_progress = nullptr;
}

void ProTreeWidget::SlotSetActive()
{
    if (!_right_btn_item) {
        return ;
    }

    QFont nullFont;
    nullFont.setBold(false);

    if (_active_item) {
        _active_item->setFont(0, nullFont);
    }

    _active_item = _right_btn_item;
    nullFont.setBold(true);
    _active_item->setFont(0, nullFont);
}

void ProTreeWidget::SlotClosePro()
{
    RemoveProDialog remove_pro_dialog;
    auto res = remove_pro_dialog.exec();
    if (res != QDialog::Accepted) {
        return ;
    }
    bool b_remove = remove_pro_dialog.isRemoved();
    auto index_right_btn = this->indexOfTopLevelItem(_right_btn_item);
    auto * protreeitem = dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto * selecteditem = dynamic_cast<ProTreeItem*>(_selected_item);
    auto delete_path = protreeitem->GetPath();
    _set_path.remove(delete_path);

    if (b_remove) {// remove real file
        QDir delete_dir(delete_path);
        delete_dir.removeRecursively();
    }

    if (protreeitem == _active_item) {
        _active_item = nullptr;
    }

    // the protreeitem which will de deleted if the root of selecteditem
    if (selecteditem && protreeitem == selecteditem->GetRoot()) {
        selecteditem = nullptr;
        _selected_item = nullptr; // will be delete
        emit SigClearSelected();
    }

    delete this->takeTopLevelItem(index_right_btn);
    _right_btn_item = nullptr;
}

// @brief slots, about _open_progressdlg
void ProTreeWidget::SlotUpdateOpenProgress(int count)
{
    if (!_open_progressdlg) {
        return;
    }
    if (count >= PROGRESS_MAX) {
        _open_progressdlg->setValue(count % PROGRESS_MAX);
    } else {
        _open_progressdlg->setValue(count);
    }
}

/**
 *  @brief slots, about _open_progressdlg
*/
void ProTreeWidget::SlotFinishOpenProgress()
{
    if (!_open_progressdlg) {
        return;
    }
    _open_progressdlg->setValue(PROGRESS_MAX);
//    delete _open_progressdlg;
//    _open_progressdlg = nullptr;
    _open_progressdlg->deleteLater();
}

void ProTreeWidget::SlotDoubleClickItem(QTreeWidgetItem *item, int column)
{
    if (QGuiApplication::mouseButtons() == Qt::LeftButton) {
        auto * tree_doubleItem = dynamic_cast<ProTreeItem*>(item);
        if (!tree_doubleItem) {
            return ;
        }

        int itemtype = tree_doubleItem->type();
        if (itemtype == TreeItemPic) {
            // send path of pic which is clicked
            emit SigUpdateSelected(tree_doubleItem->GetPath());
            _selected_item = item;
        }
    }
}

/**
 * @brief a slot function about QAction(_action_slideshow)
*/
void ProTreeWidget::SlotSlideShow()
{
    if (!_right_btn_item) {
        return ;
    }
    auto * right_pro_item = dynamic_cast<ProTreeItem*>(_right_btn_item);
    auto * last_child_item = right_pro_item->GetLastPicChild();
    if (!last_child_item) {
        return ;
    }

    auto * first_child_item = right_pro_item->GetFirstPicChild();
    if (!first_child_item) {
        return ;
    }
    qDebug() << "first_child_item  is " << first_child_item->GetPath();
    qDebug() << "last_child_item  is " << last_child_item->GetPath();
    _slide_show_dlg = std::make_shared<SlideShowDlg>(this, first_child_item, last_child_item);
    _slide_show_dlg->setModal(true);
    _slide_show_dlg->showMaximized();
}

void ProTreeWidget::SlotOpenPro(const QString &path)
{
    if (_set_path.find(path) != _set_path.end()) {
        return;
    }

    _set_path.insert(path);
    int file_count = 0;
    QDir pro_dir(path);
    QString pro_name = pro_dir.dirName();

    _open_progressdlg = new QProgressDialog(this);
    _open_progressdlg->setWindowTitle("Please wait...");
    _open_progressdlg->setFixedWidth(PROGRESS_WIDTH);
    _open_progressdlg->setRange(0, PROGRESS_MAX);
    // 如果窗口存在时间小于0.5s则干脆不显示，避免窗口一闪而过
    _open_progressdlg->setMinimumDuration(500);

    _thread_open_pro = std::make_shared<OpenTreeThread>(path, file_count, this);

    //连接更新进度框操作
    connect(_thread_open_pro.get(), &OpenTreeThread::SigUpdateProgress,
            this, &ProTreeWidget::SlotUpdateOpenProgress);
    connect(_thread_open_pro.get(), &OpenTreeThread::SigFinishProgress,
            this, &ProTreeWidget::SlotFinishOpenProgress);

    _thread_open_pro->start();
    _open_progressdlg->exec();
}

void ProTreeWidget::SlotPreShow()
{
    if (!_selected_item) {
//        qDebug() << "ProTreeWidget::SlotPreShow() : _selected_item is empty!" << endl;
        return;
    }

    auto * curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetPreItem();
    if (!curItem) {
//        qDebug() << "ProTreeWidget::SlotPreShow() : curItem is empty!" << endl;
        return ;
    }

    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotNextShow()
{
//    qDebug() << "ProTreeWidget::SlotNextShow() -- 1" << endl;
    if (!_selected_item) {
        return;
    }
//    qDebug() << "ProTreeWidget::SlotNextShow() -- 2" << endl;
    auto * curItem = dynamic_cast<ProTreeItem*>(_selected_item)->GetNextItem();
    if (!curItem) {
        return ;
    }
//    qDebug() << "ProTreeWidget::SlotNextShow() -- 3" << endl;
    emit SigUpdatePic(curItem->GetPath());
    _selected_item = curItem;
    this->setCurrentItem(curItem);
}

void ProTreeWidget::SlotSetMusic()
{
    QFileDialog file_dialog;
    file_dialog.setFileMode(QFileDialog::ExistingFiles);
    file_dialog.setWindowTitle(tr("选择音频文件"));
    file_dialog.setDirectory(QDir::currentPath());
    file_dialog.setViewMode(QFileDialog::Detail);
    file_dialog.setNameFilter("(*.mp3)");
    QStringList fileNames;
    if (file_dialog.exec()) {
        fileNames = file_dialog.selectedFiles();
    } else {
        return ;
    }

    if (fileNames.length() <= 0) {
        return ;
    }

    _playlist->clear();
    for (auto filename : fileNames) {
        _playlist->addMedia(QUrl::fromLocalFile(filename));
    }
    if (_player->state() != QMediaPlayer::PlayingState) {
        _playlist->setCurrentIndex(0);
    }
}

void ProTreeWidget::SlotStartOrStopMusic(bool flag)
{
    if (flag) {
        qDebug()<< "ProTreeWidget::SlotStartOrStopMusic : start music" << Qt::endl;
        _player->play();
    } else {
        qDebug()<< "ProTreeWidget::SlotStartOrStopMusic : stop music" << Qt::endl;
        _player->pause();
    }
}
