#include "inc/picshow.h"
#include "ui_picshow.h"
#include <QDebug>


PicShow::PicShow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PicShow)
{
    ui->setupUi(this);
    ui->previousBtn->SetIcons(":/icon/previous.png",
                              ":/icon/previous_hover.png",
                              ":/icon/previous_press.png");
    ui->nextBtn->SetIcons(":/icon/next.png",
                              ":/icon/next_hover.png",
                              ":/icon/next_press.png");
    QGraphicsOpacityEffect * opacity_pre = new QGraphicsOpacityEffect(this);
    opacity_pre->setOpacity(0);
    ui->previousBtn->setGraphicsEffect(opacity_pre);

    QGraphicsOpacityEffect * opacity_next = new QGraphicsOpacityEffect(this);
    opacity_next->setOpacity(0);
    ui->nextBtn->setGraphicsEffect(opacity_next);

    _animation_show_pre = new QPropertyAnimation(opacity_pre, "opacity", this);
    _animation_show_pre->setEasingCurve(QEasingCurve::Linear);
    _animation_show_pre->setDuration(500);

    _animation_show_next = new QPropertyAnimation(opacity_next, "opacity", this);
    _animation_show_next->setEasingCurve(QEasingCurve::Linear);
    _animation_show_next->setDuration(500);

    connect(ui->previousBtn, &QPushButton::clicked, this, &PicShow::SigPreClicked);
    connect(ui->nextBtn, &QPushButton::clicked, this, &PicShow::SigNextClicked);
}

PicShow::~PicShow()
{
    delete ui;
}

void PicShow::ReloadPic()
{
    if (_selected_path != "") {
        const auto width = ui->label->width();
        const auto height = ui->label->height();
//        qDebug() << "widget : " << ui->widget_2->width() << "  " << ui->widget_2->height() << endl;
//        qDebug() << "label : " << ui->label->width() << "  " << ui->label->height() << endl;
        _pix_map.load(_selected_path);
        _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(_pix_map);
    }
}

void PicShow::ShowPreNextBtns(bool b_show)
{
    if (!b_show && _b_btnvisible) {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(1);
        _animation_show_pre->setEndValue(0);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(1);
        _animation_show_next->setEndValue(0);
        _animation_show_next->start();

        _b_btnvisible = false;
        return;
    }

    if (b_show && !_b_btnvisible) {
        _animation_show_pre->stop();
        _animation_show_pre->setStartValue(0);
        _animation_show_pre->setEndValue(1);
        _animation_show_pre->start();

        _animation_show_next->stop();
        _animation_show_next->setStartValue(0);
        _animation_show_next->setEndValue(1);
        _animation_show_next->start();

        _b_btnvisible = true;
        return;
    }
}

bool PicShow::event(QEvent *event)
{
    switch(event->type()) {
        case QEvent::Enter:
            ShowPreNextBtns(true);
            break;
        case QEvent::Leave:
            ShowPreNextBtns(false);
            break;
        default:
            break;
    }
    return QWidget::event(event);
}

void PicShow::SlotSelectItem(const QString &path)
{
    _selected_path = path;
    _pix_map.load(path);
    auto width  = ui->label->width();
    auto height = ui->label->height();
//    qDebug()<<width<<"  "<<height;
    _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);
    ui->label->setPixmap(_pix_map);
}

void PicShow::SlotUpdatePro(const QString &path)
{
    _selected_path = path;
    if (_selected_path != "") {
        const auto width = ui->label->width();
        const auto height = ui->label->height();
        _pix_map.load(_selected_path);
        _pix_map = _pix_map.scaled(width, height, Qt::KeepAspectRatio);
        ui->label->setPixmap(_pix_map);
    } else {
//        qDebug() << "PicShow::SlotUpdatePro : _selected_path == """ << endl;
    }

}

void PicShow::SlotDeleteItem()
{
    _selected_path = "";
}
