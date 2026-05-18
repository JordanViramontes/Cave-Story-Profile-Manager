#ifndef WIDGETFUNCTIONS_H
#define WIDGETFUNCTIONS_H

#include <QWidget>
#include <QString>

int runDialogBox(QWidget* parent, const QString& page);

// locks
void widgetLock(const bool enable, const QVector<QWidget*>& widgets);

// better than a simple function because when the locker is defined and then is out of scope, itll always
// unblock the signals
class SignalLocker {
public:
    SignalLocker(const QVector<QWidget*>& widgets) : widgets(widgets) {
        for (auto* w : widgets)
            if (w) w->blockSignals(true);
    }
    ~SignalLocker() {
        for (auto* w : widgets)
            if (w) w->blockSignals(false);
    }
private:
    QVector<QWidget*> widgets;
};




#endif // WIDGETFUNCTIONS_H
