#include "tablewidgetdragrows.h"
#include "global.h"

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

// constructors

WeaponWidget::WeaponWidget(int &w, bool &missile, QString &n, int &l, int &x, QVector<int> &xN,
                      TableWidgetDragRows *pT, QWidget *parent) : QWidget{parent} {
    weaponType = w;
    isMissile = missile;
    name = n;
    lvl = l;
    xp = x;
    xpNeeded = xN;
    parentTable = pT;

    // widgets
    check = createCheck();
    icon = createIcon();
    levelText = new QLabel("Lvl:", this);
    level = createLevel();
    xpLabel = new QLabel("XP", this);
    slider = createXpSlider();
    xpText = createXpText();
    xpNeededLabel = createXpNeededLabel();
    ammoWidget = createAmmoWidget();

    //top row
    layout = new QGridLayout();

    layout->addWidget(levelText, 0, 2);
    layout->addWidget(level, 0, 3);
    layout->addWidget(xpLabel, 0, 4);
    layout->addWidget(slider, 0, 5);
    layout->addWidget(xpText, 0, 6);
    layout->addWidget(xpNeededLabel, 0, 7);

    // missiles
    if (missile) {
        // layout
        layout->addWidget(check, 0, 0, 2, 1, Qt::AlignCenter);
        layout->addWidget(icon, 0, 1, 2 , 1, Qt::AlignCenter);
        layout->addWidget(ammoWidget, 1, 4, 1, -1, Qt::AlignRight);
        ammoWidget->setVisible(true);
    }
    else {
        // layout
        layout->addWidget(check, 0, 0);
        layout->addWidget(icon, 0, 1);
        ammoWidget->setVisible(false);
    }

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

    // connect(check, &QCheckBox::checkStateChanged, this, [&check] {
    //     if (check->isChecked()) {
    //         qDebug() << "move";
    //     }
    // });

    return check;
}

QLabel * WeaponWidget::createIcon() {    
    QString imagePath = getImagePath(weaponType);
    QPixmap pixmap(imagePath);
    qDebug() << "path: " << imagePath;

    // QLabel label;
    QLabel * label = new QLabel(name, this);
    if (pixmap.isNull()) return label;
    label->setPixmap(pixmap);
    label->setMask(pixmap.mask());
    label->setToolTip(name);

    return label;
}

QComboBox * WeaponWidget::createLevel() {
    WeaponComboBox *menu = new WeaponComboBox(this);
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

QSlider * WeaponWidget::createXpSlider() {
    // Create slider with range 0-100 starting at 0
    WeaponSlider *slider;
    slider = new WeaponSlider(Qt::Horizontal, this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 99);
    slider->setValue(0);
    slider->setToolTip("Adjust Weapon Xp");

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
    edit->setToolTip("Set Ammo Count");

    // validator
    QIntValidator *intValidator = new QIntValidator(0, 50, this);  // Valid floating-point numbers from 0.0 to 1000.0 with 2 decimal places
    intValidator->setLocale(QLocale::C);
    edit->setValidator(intValidator);

    // Connect textChanged to handle user input
    connect(edit, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (text.toInt() > ammoMax) {
            setAmmo(ammoMax);
            return;
        }

        setAmmo(text.toInt());
    });

    return edit;
}

QSlider * WeaponWidget::createAmmoSlider() {
    // Create slider with range 0-100 starting at 0
    WeaponSlider *slider;
    slider = new WeaponSlider(Qt::Horizontal, this);
    slider->setOrientation(Qt::Horizontal);
    slider->setRange(0, 100);
    slider->setValue(0);
    slider->setToolTip("Adjust Ammo Count");

    connect(slider, &QSlider::sliderMoved, this, [this](int i) {
        setAmmo(i);
        // qDebug() << "SLIDER: " << xp;
    });

    return slider;
}

QWidget * WeaponWidget::createAmmoWidget() {
    ammoLabel = new QLabel("Ammo:", this);
    ammoEdit = createAmmoEdit();
    ammoLine = new QLabel("/50",this);
    ammoSlider = createAmmoSlider();

    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->addWidget(ammoLabel);
    layout->addWidget(ammoSlider);
    layout->addWidget(ammoEdit);
    layout->addWidget(ammoLine);
    layout->setContentsMargins(0, 0, 0, 0);

    QWidget *widget = new QWidget(this);
    widget->setLayout(layout);

    return widget;
}

// others

QString WeaponWidget::getImagePath(int weaponType) {
    switch(weaponType) {
    case(1): return IMAGEPATH + "/snake.png"; break;
    case(2): return IMAGEPATH + "/polarStar.png"; break;
    case(3): return IMAGEPATH + "/fireball.png"; break;
    case(4): return IMAGEPATH + "/machineGun.png"; break;
    case(5): return IMAGEPATH + "/missile.png"; break;
    case(7): return IMAGEPATH + "/bubble.png"; break;
    case(9): return IMAGEPATH + "/blade.png"; break;
    case(0xA): return IMAGEPATH + "/missileSuper.png"; break;
    case(0xC): return IMAGEPATH + "/nemesis.png"; break;
    case(0xD): return IMAGEPATH + "/spur.png"; break;
    default: break;
    }

    return "ERROR WITH IMAGE PATH SWITCH";
}
