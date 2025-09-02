#ifndef PROTREETHREAD_H
#define PROTREETHREAD_H

#include <QThread>
#include <QTreeWidget>
#include <QTreeWidgetItem>

class ProTreeThread : public QThread
{
    Q_OBJECT
public:
    ProTreeThread(const QString& src_path, const QString& dist_path, QTreeWidgetItem* parent_item,
                  int file_count, QTreeWidget* self, QTreeWidgetItem* root, QObject* parent = nullptr);
    ~ProTreeThread();
protected:
    void run() override;
private:
    void CreateProTree(const QString& src_path, const QString& dist_path, QTreeWidgetItem* parent_item,
                       int& file_count, QTreeWidget* self, QTreeWidgetItem* root, QTreeWidgetItem* preItem = nullptr);

    QString _src_path;
    QString _dist_path;
    int _file_count;
    QTreeWidgetItem* _parent_item;
    QTreeWidget* _self;
    QTreeWidgetItem* _root;
    bool _bstop;

signals:
    void SigUpdateProgress(int);
    void SigFinishProgress(int);
public slots:
    void SlotCancelProgress();
};

#endif // PROTREETHREAD_H
