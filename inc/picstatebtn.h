#ifndef PICSTATEBTN_H
#define PICSTATEBTN_H

#include <QPushButton>

class PicStateBtn : public QPushButton
{
public:
    PicStateBtn(QWidget *parent = nullptr);
    void SetIcons(const QString& normal, const QString& hover, const QString& pressed,
                  const QString& normal_2, const QString& hover_2, const QString& pressed_2);

private:
    QString _normal;
    QString _hover;
    QString _pressed;
    QString _normal_2;
    QString _hover_2;
    QString _pressed_2;
    int _cur_state;
};

#endif // PICSTATEBTN_H
