#ifndef WIDGETFUNCTIONS_H
#define WIDGETFUNCTIONS_H

#include <QWidget>
#include <QString>

int runDialogBox(QWidget* parent, const QString& page);

// locks
void widgetLock(const bool enable, const QVector<QWidget*> widgets);
void signalLock(const bool enable, const QVector<QWidget*> widgets);

#endif // WIDGETFUNCTIONS_H
