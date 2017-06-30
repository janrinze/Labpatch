#ifndef SUBMENUMAIN_H
#define SUBMENUMAIN_H

#include <QFrame>
#include <QLabel>
#include <QDebug>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>

#include "defines.h"
#include "iconobject.h"

namespace Ui {
class SubMenuMain;
}

class SubMenuMain : public QFrame
{
    Q_OBJECT
public slots:
    void setMenuCurrentTool(QString s);

//signals:
//    void sendIconInstance(QString type, qlonglong id);

public:
    explicit SubMenuMain(QList<QList<QString> > menuList, QWidget *parent = 0);
    ~SubMenuMain();
    static qlonglong idCounter;

private:
    Ui::SubMenuMain *ui;
    QString currentTool;

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // SUBMENUMAIN_H
