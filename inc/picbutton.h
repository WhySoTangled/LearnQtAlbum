#ifndef PICBUTTON_H
#define PICBUTTON_H
#include <QEvent>
#include <QPushButton>


class PicButton : public QPushButton
{
public:
    PicButton(QWidget *parent = Q_NULLPTR);
    void SetIcons(const QString& normal, const QString& hover, const QString& pressed);
private:
    void setNormalIcon();
    void setHoverIcon();
    void setPressIcon();
    QString _normal;
    QString _hover;
    QString _pressed;
protected:
    bool event(QEvent *e) Q_DECL_OVERRIDE;
};

#endif // PICBUTTON_H
