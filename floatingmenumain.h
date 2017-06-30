#ifndef FLOATINGMENUMAIN_H
#define FLOATINGMENUMAIN_H

#include <QFrame>
#include <QDebug>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QMenu>

#include "defines.h"
#include "submenumain.h"

namespace Ui {
class FloatingMenuMain;
}

class FloatingMenuMain : public QFrame
{
    Q_OBJECT
public slots:
    void setMenuCurrentTool(QString s);

public:
    explicit FloatingMenuMain(QWidget *parent = 0);
    ~FloatingMenuMain();

private:
    Ui::FloatingMenuMain *ui;
    QString currentTool;

protected:
    void mousePressEvent(QMouseEvent *event) override;
};

#endif // FLOATINGMENUMAIN_H
