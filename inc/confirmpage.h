#ifndef CONFIRMPAGE_H
#define CONFIRMPAGE_H

#include <QWizardPage>

namespace Ui {
class ConfirmPage;
}

class ConfirmPage : public QWizardPage
{
    Q_OBJECT

public:
    explicit ConfirmPage(QWidget *parent = 0);
    ~ConfirmPage();

private:
    Ui::ConfirmPage *ui;
};

#endif // CONFIRMPAGE_H
