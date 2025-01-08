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
#include <QGridLayout>
#include <QPushButton>

WeaponWidget::WeaponWidget(int &w, bool &missile, QString &n, int &l, int &x, QVector<int> &xN,
                      TableWidgetDragRows *pT, QWidget *parent) : QWidget{parent} {
    weaponType = w;
    isMissile = missile;
    name = n;
    lvl = l;
    xp = x;
    xpNeeded = xN;
    parentTable = pT;

    // qDebug() << "setup";

    // horizontal layout widgets
    check = createCheck();
    icon = createIcon();
    levelText = new QLabel("Lvl:", this);
    level = createLevel();
    xpLabel = new QLabel("XP", this);
    slider = createSlider();
    xpText = createXpText();
    xpNeededLabel = createXpNeededLabel();


    //top row
    layout = new QGridLayout();

    // missiles
    if (missile) {
        // widgets
        isMissile = true;
        ammoLabel = new QLabel("Ammo:", this);
        ammoEdit = createAmmoEdit();
        ammoLine = new QLabel("/50",this);

        // layout
        layout->addWidget(check, 0, 0, 2, 1, Qt::AlignCenter);
        layout->addWidget(icon, 0, 1, 2 , 1, Qt::AlignCenter);
        layout->addWidget(ammoLabel, 1, 5, Qt::AlignRight);
        layout->addWidget(ammoEdit, 1, 6, Qt::AlignRight);
        layout->addWidget(ammoLine, 1, 7, Qt::AlignLeft);
    }
    else {
        layout->addWidget(check, 0, 0);
        layout->addWidget(icon, 0, 1);
    }

    layout->addWidget(levelText, 0, 2);
    layout->addWidget(level, 0, 3);
    layout->addWidget(xpLabel, 0, 4);
    layout->addWidget(slider, 0, 5);
    layout->addWidget(xpText, 0, 6);
    layout->addWidget(xpNeededLabel, 0, 7);



    // set all spacings for consistency
    for (int i = 0; i < layout->rowCount(); i++) {
        layout->setRowStretch(i, 1);
    }
    for (int i = 0; i < layout->columnCount(); i++) {
        layout->setColumnStretch(i, 1);
    }
    layout->setColumnStretch(5, 7);

    setLayout(layout);
    // qDebug() << "second";
}

QCheckBox * WeaponWidget::createCheck() {
    QCheckBox *check = new QCheckBox();
    check->setCheckState(Qt::Checked);
    check->setToolTip("Enables/Disables the weapon");

    // connection for changing bool isEnabled
    connect(check, SIGNAL(checkStateChanged(Qt::CheckState)),
            this, SLOT(enabledUpdate()));

    return check;
}

QLabel * WeaponWidget::createIcon() {
    QLabel * label = new QLabel(name, this);
    return label;
}

QComboBox * WeaponWidget::createLevel() {
    QComboBox *menu = new QComboBox(this);
    menu->setFocusPolicy(Qt::NoFocus);
    menu->addItem("1");
    menu->addItem("2");
    menu->addItem("3");
    menu->addItem("MAX");
    menu->setToolTip("Change Weapon Lvl");

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
    slider->setToolTip("Adjust Weapon Xp");
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
    edit->setToolTip("Set Weapon Xp");

    // validator
    QIntValidator *intValidator = new QIntValidator(0, xpNeeded[lvl-1], this);  // Valid floating-point numbers from 0.0 to 1000.0 with 2 decimal places
    intValidator->setLocale(QLocale::C);
    edit->setValidator(intValidator);

    // Connect textChanged to handle user input
    connect(edit, &QLineEdit::textChanged, this, [this, &edit](const QString &text) {
        if (text.toInt() > xpNeeded[lvl-1]) {
            setXp(xpNeeded[lvl-1]);
            // edit->setText(QString::(xpNeeded[lvl-1]));
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
    text->setAlignment(Qt::AlignLeft);

    return text;
}

QLineEdit * WeaponWidget::createAmmoEdit() {
    QLineEdit * edit = new QLineEdit(this);
    edit->setAlignment(Qt::AlignRight);
    edit->setFixedWidth(40);
    edit->setMaxLength(3);
    edit->setPlaceholderText("xx");
    edit->setText("0");
    edit->setToolTip("Set Ammo Xp");

    // validator
    QIntValidator *intValidator = new QIntValidator(0, 50, this);  // Valid floating-point numbers from 0.0 to 1000.0 with 2 decimal places
    intValidator->setLocale(QLocale::C);
    edit->setValidator(intValidator);

    // Connect textChanged to handle user input
    connect(edit, &QLineEdit::textChanged, this, [this, &edit](const QString &text) {
        if (text.toInt() > ammoMax) {
            setAmmo(ammoMax);
            return;
        }

        setAmmo(text.toInt());
    });

    return edit;
}
