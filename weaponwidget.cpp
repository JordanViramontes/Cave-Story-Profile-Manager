#include "weaponwidget.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDrag>
#include <QMimeData>
#include <QDebug>
#include <QHBoxLayout>
#include <QSlider>
#include <QCheckBox>
#include <QLineEdit>

WeaponWidget::WeaponWidget(QWidget *parent)
    : QWidget{parent}
{
    // horizontal layout widgets
    QWidget *check = createCheck();
    QWidget *icon = createIcon();
    QWidget *level = createLevel();
    QSlider *slider = createSlider();
    QLabel * xpText = createXpText();


    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(check);
    layout->addWidget(icon);
    layout->addWidget(level);
    layout->addWidget(slider);
    layout->addWidget(xpText);

    setLayout(layout);
}

QWidget * WeaponWidget::createCheck() {
    QCheckBox *check = new QCheckBox();
    check->setCheckState(Qt::Unchecked);
    return check;
}

QWidget * WeaponWidget::createIcon() {
    QLabel * label = new QLabel("temp", this);
    return label;
}

QWidget * WeaponWidget::createLevel() {
    QLabel * label = new QLabel("temp", this);
    return label;
}

QSlider * WeaponWidget::createSlider() {
    // Create slider with range 0-100 starting at 0
    QSlider *slider;
    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    // slider->setGeometry(10,140, 115, 30);

    return slider;
}

QLabel * WeaponWidget::createXpText() {
    QLabel *text = new QLabel(this);
    QString str = "00/" + QString::number(xpNeeded[0]);
    text->setText(str);

    return text;
}

