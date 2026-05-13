#ifndef WIDGETFUNCTIONS_H
#define WIDGETFUNCTIONS_H

#include <QWidget>
#include <QString>

int runDialogBox(QWidget* parent, const QString& page);

void widgetLock(const bool enable, const QVector<QWidget*> widgets);

#endif // WIDGETFUNCTIONS_H
