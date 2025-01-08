#ifndef WINDOW_H
#define WINDOW_H

#include "profileloader.h"
#include "tablewidgetdragrows.h"

#include "qabstractitemmodel.h"
#include "qfilesystemmodel.h"
#include "qlineedit.h"
#include <QWidget>
#include <QFileInfo>

class Window : public QWidget
{
    Q_OBJECT
private:
    QWidget * createFileWidget();
    QWidget * createImageLoader();
    QWidget * createScrollArea();

    // directories and file management
    QFileSystemModel *fileSystem;
    QString gameDirectory = "C:/Users/jorda/OneDrive/Documents/1-Projects/CSProfileManager/doukutsu";
    QString saveDirectory = "C:/Users/jorda/OneDrive/Documents/1-Projects/CSProfileManager/doukutsu/saves";


    // widgets
    TableWidgetDragRows * tableWidget;
    QLineEdit * AmmoEdit;
    QSlider *slider;

public:
    explicit Window(QWidget *parent = nullptr);

private slots:
    void updateAmmoEdit(int n) {
        AmmoEdit->setText(QString::number(n));
    }

    // when clicked get a profileloader
    void getProfileDirectory(QModelIndex index) {
        QFileInfo fileInfo = fileSystem->fileInfo(index);

        ProfileLoader profile = ProfileLoader(fileSystem->filePath(index).toStdString());
        QVector<WeaponSlot> weapons = profile.getWeapons();
        int ammoMax = profile.getMaxAmmo();
        qDebug() << "ammoMax: " << ammoMax;
        // int ammoMax = profile.getMaxAmmo();
        // qDebug() << "size: " << weapons.size();
        slider->setValue(ammoMax);
        emit slider->sliderMoved(ammoMax);


        tableWidget->newWeaponTable(weapons);
        // emit updateAmmoFromProfile(ammoMax);
    }

signals:
    void updateAmmoFromProfile(int n);
};

#endif // WINDOW_H
