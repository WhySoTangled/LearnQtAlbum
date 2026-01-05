#include "../inc/picstatebtn.h"
#include "const.h"


PicStateBtn::PicStateBtn(QWidget *parent)
    : QPushButton(parent)
{

}

void PicStateBtn::SetIcons(const QString &normal, const QString &hover, const QString &pressed, const QString &normal_2, const QString &hover_2, const QString &pressed_2)
{
    _normal    = normal;
    _hover     = hover;
    _pressed   = pressed;

    _normal_2  = normal_2;
    _hover_2   = hover_2;
    _pressed_2 = pressed_2;

    QPixmap tmpPixmap;
    tmpPixmap.load(normal);
    this->resize(tmpPixmap.size());
    this->setIcon(tmpPixmap);
    this->setIconSize(tmpPixmap.size());
    _cur_state = PicBtnStateNormal;
}
