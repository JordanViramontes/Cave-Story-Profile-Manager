#ifndef HELPSCREEN_H
#define HELPSCREEN_H

#include "dialog.h"
#include <QHash>

namespace Ui {
class HelpScreen;
}

class HelpScreen : public QDialog
{
    Q_OBJECT

public:
    explicit HelpScreen(QWidget *parent = nullptr);
    ~HelpScreen();

private:
    Ui::HelpScreen *ui;
};

#endif // HELPSCREEN_H

