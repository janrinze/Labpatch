#ifndef VIEWPORTAREAWIDGET_H
#define VIEWPORTAREAWIDGET_H

#include <QApplication>
#include <QStyleOption>
#include <QWidget>
#include <QMap>
#include <QMessageBox>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSpinBox>
#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>
#include <QRubberBand>
#include <QPainter>
#include <QDebug>
#include <QBitmap>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QXmlStreamWriter>
#include <QPair>

#include <float.h>
#include "floatingmenumain.h"
#include "floatingmenutools.h"

class ViewPortAreaWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ViewPortAreaWidget();
    virtual ~ViewPortAreaWidget();
    QString stateToXML();
    void stateFromXML(QByteArray in);
    void deleteAllIconObjects();

public slots:
    void setMenuCurrentTool(QString s);
    void showAll();
    void hideAll();
    void resizedMainWindow(QSize sz);
    void continue_actionDump_iconMap_triggered();

private slots:
    void arrangeToolbars();

signals:
    void putPositionOfScrollArea(QPointF delta);

private:
    QString currentTool;
    QMap<QString, QString> mapFuncEdit;
    QMap<QString, QString> mapFuncPixmap;
    //   funcPlus      subindex  1lphtype  convertedTypes
    QMap<QString, QMap<int, QMap<QString, QList<QString> > > > mapFuncVaryNode;
    QBrush wireColor;
    QString wireColorType;
    Qt::PenStyle wireStyle;
    QPair<qlonglong,int> startWire;
    QString startWireChildType;
    QPoint startFinger;
    QPoint deltaFinger;
    QPoint endWire;
    QPoint refSelectDrop;
    QMap<qlonglong, QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > > wires;
    QMap<qlonglong, QPair<QBrush,QString> > wireColors;
    QRubberBand *rubberBand;
    QPoint origin;
    QList<qlonglong> selectedIndecies;

    qlonglong widCounter;

    FloatingMenuMain *fmenu;
    FloatingMenuTools *ftools;

    IconObject* mousePressEventGetChildAt(QMouseEvent *event);
    void mousePressEventSelectLeft(QMouseEvent *event);
    void mousePressEventSelectLeftChildNull();
    void mousePressEventSelectLeftSelectedIndeciesNotEmpty(QMouseEvent *event, IconObject *child);
    void mousePressEventSelectLeftSelectedIndeciesEmpty(QMouseEvent *event, IconObject *child);
    void mousePressEventSelectRight(QMouseEvent *event);
    void mousePressEventWireLeft(QMouseEvent *event);
    void mousePressEventWireRight(QMouseEvent *event);
    void mousePressEventFingerLeft(QMouseEvent *event);
    //void mousePressEventFingerRight(QMouseEvent *event);

    void mouseMoveEventSelectLeft(QMouseEvent *event);
    void mouseMoveEventFingerLeft(QMouseEvent *event);

    IconObject* getIconObjectAt(qlonglong id);
    void moveIconObjectAt(qlonglong id, QPoint newPos);
    void deleteIconObjectAt(qlonglong id);
    int getSubIndexOfIcon(QString type, QPoint relPos);

    bool eventFilterWire(QObject *obj, QEvent *event);
    bool eventFilterWireMouseMove(QObject *obj, QEvent *event);
    void eventFilterWireMouseMoveIconObject(QPoint meventPos, IconObject *io);

    void mouseReleaseEventSelect();
    void mouseReleaseEventFinger();

    QStringList parseMapFuncEdit(QString type);
    QSize parseMapFuncSize(QString type);
    QString getWireType(IconObject *io1, int subindex1,
                        IconObject *io2, int subindex2);
    void connectWirePairs(IconObject *io1, int subindex1,
                          IconObject *io2, int subindex2,
                          qlonglong wid = -1);
    QStringList intersectionOfAandB(QStringList a, QStringList b);
    bool areAandBequal(QStringList a, QStringList b);
    bool isAlreadyConnectedToOther(qlonglong index, int subindex);
    void updateWireConnectionTypes();

protected:
    void dragEnterEvent(QDragEnterEvent *event) override;
    void dragMoveEvent(QDragMoveEvent *event) override;
    void dropEvent(QDropEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *obj, QEvent *event) override;

    void paintEvent(QPaintEvent *event) override;
};

#endif // VIEWPORTAREAWIDGET_H
