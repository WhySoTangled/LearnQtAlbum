#ifndef PROSETPAGE_H
#define PROSETPAGE_H

#include <QWizardPage>

namespace Ui {
class ProSetPage;
}

class ProSetPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ProSetPage(QWidget *parent = 0);
    ~ProSetPage();

    bool isComplete() const override;
    void GetProSettings(QString& name, QString& path);

private slots:
    void on_pushButton_clicked();

private:
    Ui::ProSetPage *ui;
};

#endif // PROSETPAGE_H
