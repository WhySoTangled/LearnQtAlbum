#include "slideshowdlg.h"
#include "ui_slideshowdlg.h"
#include "prelistwid.h"
#include "protreewidget.h"

SlideShowDlg::SlideShowDlg(QWidget *parent, QTreeWidgetItem *first_item, QTreeWidgetItem *last_item) :
    QDialog(parent),
    ui(new Ui::SlideShowDlg),
    _first_item(first_item), _last_item(last_item)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);
    ui->slidePreBtn->SetIcons(":/icon/previous.png",
                              ":/icon/previous_hover.png",
                              ":/icon/previous_press.png");
    ui->slideNextBtn->SetIcons(":/icon/next.png",
                               ":/icon/next_hover.png",
                               ":/icon/next_press.png");
    ui->closeBtn->SetIcons(":/icon/closeshow.png",
                           ":/icon/closeshow_hover.png",
                           ":/icon/closeshow_press.png");
    ui->playBtn->SetIcons(":/icon/play.png",
                          ":/icon/play_hover.png",
                          ":/icon/play_press.png",
                          ":/icon/pause.png",
                          ":/icon/pause_hover.png",
                          ":/icon/pause_press.png");
    connect(ui->closeBtn, &QPushButton::clicked,
            this, &SlideShowDlg::close);
    connect(ui->slidePreBtn, &QPushButton::clicked,
            this, &SlideShowDlg::SlotSlidePre);
    connect(ui->slideNextBtn, &QPushButton::clicked,
            this, &SlideShowDlg::SlotSlideNext);
    connect(ui->playBtn, &QPushButton::clicked,
            ui->picAnimation, &PicAnimationWid::SlotStartOrStop);
    connect(ui->picAnimation, &PicAnimationWid::SigStartOrStop,
            ui->playBtn, &PicStateBtn::SlotStartOrStop);

    auto * prelistWid = dynamic_cast<PreListWid*>(ui->preListWidget);
    connect(ui->picAnimation, &PicAnimationWid::SigUpPreList,
            prelistWid, &PreListWid::SlotUpPreList);
    connect(ui->picAnimation, &PicAnimationWid::SigSelectItem,
            prelistWid, &PreListWid::SlotUpSelect);
    connect(ui->preListWidget, &PreListWid::SigUpSelectShow,
            ui->picAnimation, &PicAnimationWid::SlotUpSelectShow);

    auto * _protree_widget = dynamic_cast<ProTreeWidget*>(parent);
    connect(ui->picAnimation, &PicAnimationWid::SigStartOrStopMusic,
            _protree_widget, &ProTreeWidget::SlotStartOrStopMusic);
    ui->picAnimation->SetPixmap(_first_item);
    ui->picAnimation->Start();

}

SlideShowDlg::~SlideShowDlg()
{
    delete ui;
}

void SlideShowDlg::SlotSlideNext()
{
    ui->picAnimation->SlideNext();
}

void SlideShowDlg::SlotSlidePre()
{
    ui->picAnimation->SlidePre();
}
