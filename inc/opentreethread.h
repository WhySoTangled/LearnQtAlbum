#ifndef OPENTREETHREAD_H
#define OPENTREETHREAD_H

#include <QThread>
#include <QTreeWidget>

class OpenTreeThread : public QThread
{
    Q_OBJECT
public:
    OpenTreeThread(const QString& src_path, int file_count,
                   QTreeWidget* self);
    void OpenProTree(const QString& src_path, int &file_count, QTreeWidget* self);
protected:
    void run() override;
private:
    void RecursiveProTree(const QString& src_path, int& file_count, QTreeWidget* self,
                          QTreeWidgetItem* root, QTreeWidgetItem* parent, QTreeWidgetItem* preitem);

    QString _src_path;
    int _file_count;
    QTreeWidget* _self;
    bool _bstop;
    QTreeWidgetItem* _root;
signals:
    void SigFinishProgress(int);
    void SigUpdateProgress(int);
};

#endif // OPENTREETHREAD_H
