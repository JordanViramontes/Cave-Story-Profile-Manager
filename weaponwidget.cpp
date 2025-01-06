#include "tablewidgetdragrows.h"

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
#include <QComboBox>
#include <QScrollBar>
#include <QIntValidator>

WeaponWidget::WeaponWidget(QWidget *parent)
    : QWidget{parent}
{
    // horizontal layout widgets
    check = createCheck();
    icon = createIcon();
    levelText = new QLabel("Lvl:", this);
    level = createLevel();
    xpLabel = new QLabel("XP:", this);
    slider = createSlider();
    xpText = createXpText();
    xpNeededLabel = createXpNeededLabel();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setSpacing(5);
    layout->addWidget(check);
    layout->addWidget(icon);
    layout->addWidget(levelText);
    layout->addWidget(level);
    layout->addWidget(xpLabel);
    layout->addWidget(slider);
    layout->addWidget(xpText);
    layout->addWidget(xpNeededLabel);

    setLayout(layout);
}

QWidget * WeaponWidget::createCheck() {
    QCheckBox *check = new QCheckBox();
    check->setCheckState(Qt::Checked);

    if (tablePosition > 5) {
        check->setEnabled(false);
    }

    // connection for changing bool isEnabled
    connect(check, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(enabledChecked()));

    return check;
}

QLabel * WeaponWidget::createIcon() {
    QLabel * label = new QLabel(name, this);
    return label;
}

QWidget * WeaponWidget::createLevel() {
    QComboBox *menu = new QComboBox(this);
    menu->addItem("1");
    menu->addItem("2");
    menu->addItem("3");
    menu->addItem("MAX");

    // connection
    connect(menu, &QComboBox::currentIndexChanged, this, [this](int i) {
        setLvl(i + 1);
    });

    return menu;
}

QSlider * WeaponWidget::createSlider() {
    // Create slider with range 0-100 starting at 0
    QSlider *slider;
    slider = new QSlider(this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    // slider->setGeometry(10,140, 115, 30);

    connect(slider, &QSlider::sliderMoved, this, [this](int i) {
        setXp(i);
        qDebug() << "SLIDER: " << xp;
    });

    return slider;
}

QLineEdit * WeaponWidget::createXpText() {
    QLineEdit * edit = new QLineEdit(this);
    edit->setAlignment(Qt::AlignRight);
    edit->setFixedWidth(40);
    edit->setMaxLength(3);
    edit->setPlaceholderText("xx");
    edit->setText("0");

    // validator
    QIntValidator *intValidator = new QIntValidator(0, xpNeeded[lvl-1], this);  // Valid floating-point numbers from 0.0 to 1000.0 with 2 decimal places
    intValidator->setLocale(QLocale::C);
    edit->setValidator(intValidator);

    // Connect textChanged to handle user input
    connect(edit, &QLineEdit::textChanged, this, [this, &edit](const QString &text) {
        if (text.toInt() > xpNeeded[lvl-1]) {
            setXp(xpNeeded[lvl-1]);
            return;
        }

        setXp(text.toInt());
    });

    return edit;
}

QLabel * WeaponWidget::createXpNeededLabel() {
    QLabel *text = new QLabel(this);
    QString str = "/" + QString::number(xpNeeded[0]);
    text->setText(str);

    return text;
}

