#ifndef QPROFILEVIEWTREE_H
#define QPROFILEVIEWTREE_H

#include <QWidget>
#include <QTreeView>

class QProfileViewTree : public QTreeView
{
    Q_OBJECT
public:
    explicit QProfileViewTree(QWidget *parent = nullptr);

protected:
    // disable being able to highlight files while holding mouse button over them
    void mouseMoveEvent(QMouseEvent *event) override {
        return;
        qDebug() << "qprofileviewtree.h: testing mouse move";

        QTreeView::mouseMoveEvent(event);
    }


signals:
};

#endif // QPROFILEVIEWTREE_H
