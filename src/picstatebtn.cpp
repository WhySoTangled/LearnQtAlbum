#include "../inc/picstatebtn.h"
#include "const.h"
#include <QEvent>


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

bool PicStateBtn::event(QEvent *event)
{
    switch (event->type()) {
        case QEvent::Enter : // 鼠标悬停在btn上时
            if (_cur_state < PicBtnState2Normal) {
                setHoverIcon();
            } else {
                setHover2Icon();
            }
        break;
        case QEvent::Leave : // 鼠标离开btn时
            if (_cur_state < PicBtnState2Normal) {
                setNormalIcon();
            } else {
                setNormal2Icon();
            }
        break;
        case QEvent::MouseButtonPress : // 鼠标点击this btn时
            if (_cur_state < PicBtnState2Normal) {
                setPressIcon();
            } else {
                setPress2Icon();
            }
        break;
        case QEvent::MouseButtonRelease : // 点击btn后的鼠标松开时
            if (_cur_state < PicBtnState2Normal) {
                setHover2Icon();
            } else {
                setHoverIcon();
            }
        break;
        default:
            break;
    }
    return QPushButton::event(event);
}

/**
 * @brief 置暂停播放自然态
 */
void PicStateBtn::setNormalIcon() {
    QPixmap tmpPixmap;
    tmpPixmap.load(_normal);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateNormal;
}

/**
 * @brief 置暂停播放鼠标悬停态
 */
void PicStateBtn::setHoverIcon() {
    QPixmap tmpPixmap;
    tmpPixmap.load(_hover);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStateHover;
}

/**
 * @brief 置暂停播放鼠标选中态
 */
void PicStateBtn::setPressIcon() {
    QPixmap tmpPixmap;
    tmpPixmap.load(_pressed);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnStatePress;
}

/**
 * @brief 置播放中自然态
 */
void PicStateBtn::setNormal2Icon() {
    QPixmap tmpPixmap;
    tmpPixmap.load(_normal_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Normal;
}

/**
 * @brief 置播放中鼠标悬停态
 */
void PicStateBtn::setHover2Icon() {
    QPixmap tmpPixmap;
    tmpPixmap.load(_hover_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Hover;
}

/**
 * @brief 置播放中鼠标选中态
 */
void PicStateBtn::setPress2Icon() {
    QPixmap tmpPixmap;
    tmpPixmap.load(_pressed_2);
    this->setIcon(tmpPixmap);
    _cur_state = PicBtnState2Press;
}

/**
 * @brief 改变‘播放’按钮的状态
 * @param flag
 */
void PicStateBtn::SlotStartOrStop(bool flag)
{
    if (flag) { // Ani播放态,Btn置暂停
        setNormal2Icon();
    } else { // Ani暂停态,Btn置播放
        setNormalIcon();
    }
}
