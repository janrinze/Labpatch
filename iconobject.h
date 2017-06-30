#ifndef ICONOBJECT_H
#define ICONOBJECT_H

#include <QObject>
#include <QWidget>
#include <QLabel>
#include <QMap>
#include <QDebug>

#include "defines.h"

class IconObject : public QLabel
{
    Q_OBJECT
public:
    explicit IconObject(QWidget *parent = Q_NULLPTR, Qt::WindowFlags f = Qt::WindowFlags(), QString iconType = QString());
    virtual ~IconObject();
    void setType(QString i);
    QString getType();
    void setStowedPixmap(QPixmap i);
    QPixmap getStowedPixmap();
    void setValue(QList<QByteArray> i);
    QList<QByteArray> getValue();
    void setIndex(qlonglong i);
    qlonglong getIndex();
    void setConnectionPoints(QList<QPoint> i);
    QList<QPoint> getConnectionPoints();
    void setInsOutsRects(QList<QRect> i);
    QList<QRect> getInsOutsRects();
    void setInsOutsTypes(QStringList i);
    QStringList getInsOutsTypes();
    void setStowTypes(QStringList i);
    QStringList getStowTypes();
    void setIsInType(QList<bool> i);
    QList<bool> getIsInType();
    void setPixmapSize(QSize i);
    QSize getPixmapSize();
    void print();

private:
    QString type;
    QList<QByteArray> value;
    qlonglong index;
    QList<bool> isInType;
    QStringList insOutsTypes;
    QStringList stowedTypes;
    QList<QRect> insOutsRects;
    QList<QPoint> connectionPoints;
    QPixmap stowedPixmap;
    QSize pixmapSize;

    QStringList parseMapFuncEdit(QString type);
};

#endif // ICONOBJECT_H
