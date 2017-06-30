#include "iconobject.h"

IconObject::IconObject(QWidget *parent, Qt::WindowFlags f, QString iconType) : QLabel(parent, f){
    type = iconType;
    QStringList qsl = parseMapFuncEdit(iconType);
    QList<QByteArray> vals;
    for(int i = 0; i < qsl.length(); i++){
        QStringList lwd = qsl.at(i).split("_");
        QString widge = lwd.at(1);
        QString defaultValue = lwd.at(2);
        if(widge == "QCheckBox"){
            if(defaultValue == tr("true")){
                vals.append(QByteArray("true"));
            }else if(defaultValue == tr("false")){
                vals.append(QByteArray("false"));
            }
        }else if(widge == "QSpinBox"){
            bool ok;
            int intVal = defaultValue.toInt(&ok, 10);
            if(ok){
                vals.append(QByteArray::number(intVal));
            }
        }else if(widge == "QDoubleSpinBox"){
            bool ok;
            int dblVal = defaultValue.toDouble(&ok);
            if(ok){
                vals.append(QByteArray::number(dblVal));
            }
        }else if(widge == "QLineEdit"){
            if(defaultValue == tr("empty")){
                vals.append(QByteArray(""));
            }else{
                vals.append(defaultValue.toLatin1());
            }
        }
    }
    this->value = vals;
    setScaledContents(true);
    //setFixedHeight(FIXEDICONHEIGHT);
    //setFixedWidth(FIXEDICONWIDTH);
}

IconObject::~IconObject(){

}

QStringList IconObject::parseMapFuncEdit(QString type){
    QMap<QString, QString> mapFuncEdit;
    QMap<QString, QSize> mapFuncSize;
    QMap<QString, QMap<int, QMap<QString, QList<QString> > > > mapFuncVaryNode;
    for(int i = 0; i < icondatabase.length(); i++){
        for(int j = 0; j < icondatabase.at(i).at(1).length(); j++){
            if(icondatabase.at(i).at(1).at(j).length() >= 6){
                mapFuncEdit[icondatabase.at(i).at(1).at(j).at(4)] = icondatabase.at(i).at(1).at(j).at(5);
                mapFuncSize[icondatabase.at(i).at(1).at(j).at(4)] =
                        QSize(icondatabase.at(i).at(1).at(j).at(2).toInt(), icondatabase.at(i).at(1).at(j).at(3).toInt());
            }
            QMap<int, QMap<QString, QList<QString> > > list;
            if(icondatabase.at(i).at(1).at(j).length() >= 7){
                QStringList str1 = icondatabase.at(i).at(1).at(j).at(6).split(",");
                for(int k = 0; k < str1.length(); k++){
                    QStringList str2 = str1.at(k).split(":");
                    if(str2.length() != 2 || str2.at(0).toInt() != k)
                        continue;
                    QStringList str3 = str2.at(1).split("|");
                    QMap<QString, QList<QString> > map2;
                    for(int l = 0; l < str3.length(); l++){
                        QStringList str4 = str3.at(l).split("{");
                        if(str4.length() != 2 || str4.at(1)[str4.at(1).length()-1] != tr("}") )
                            continue;
                        QString str5 = str4.at(1).mid(0,str4.at(1).length()-1);
                        QStringList str6 = str5.split(";");
                        QList<QString> map1;
                        for(int m = 0; m < str6.length(); m++){
                            QStringList str7 = str6.at(m).split(">");
                            if(str7.length() != 2 || str7.at(0).toInt() != m )
                                continue;
                            map1.append(str7.at(1));
                        }
                        map2[str4.at(0)] = map1;
                    }
                    list[k] = map2;
                }
            }
            mapFuncVaryNode[icondatabase.at(i).at(1).at(j).at(4)] = list;
        }
    }
    QStringList editMenuWidgets = {};
    if(mapFuncEdit.contains(type)){
        QStringList insOutsAndEdits = mapFuncEdit[type].split(",");
        for(int i = 0; i < insOutsAndEdits.length(); i++){
            QStringList keyValue = insOutsAndEdits.at(i).split(":");
            if(keyValue.length() == 2 && keyValue.at(0) == tr("edit")){
                if(QRegExp("^\\(.*\\)$").indexIn(keyValue.at(1), QRegExp::CaretAtZero) != -1){
                    QString allWidgetsString = keyValue.at(1);
                    allWidgetsString.replace(QRegExp("^\\((.*)\\)$"), "\\1");
                    editMenuWidgets = allWidgetsString.split(";");
                }
            }
        }
        QList<bool> isInType = {};
        QStringList insOutsTypes = {};
        QList<QRect> insOutsRects = {};
        QList<QPoint> connectionPoints = {};
        for(int i = 0; i < insOutsAndEdits.length(); i++){
            QStringList keyValue = insOutsAndEdits.at(i).split(":");
            if(keyValue.length() == 2 && (keyValue.at(0) == tr("in") || keyValue.at(0) == tr("out"))){
                if(QRegExp("^\\(.*\\)\\{.*\\}$").indexIn(keyValue.at(1), QRegExp::CaretAtZero) != -1){
                    isInType.append((keyValue.at(0) == tr("in")));
                    QString allWidgetsString = keyValue.at(1);
                    allWidgetsString.replace(QRegExp("^\\((.*)\\)\\{.*\\}$"), "\\1");
                    insOutsTypes.append(allWidgetsString);
                    allWidgetsString = keyValue.at(1);
                    allWidgetsString.replace(QRegExp("^\\(.*\\)\\{(.*)\\}$"), "\\1");
                    QStringList rectString = allWidgetsString.split("|");
                    if(rectString.length() == 6){
                        insOutsRects.append(QRect(rectString.at(0).toInt(), rectString.at(1).toInt(), rectString.at(2).toInt(), rectString.at(3).toInt()));
                        connectionPoints.append(QPoint(rectString.at(4).toInt(), rectString.at(5).toInt()));
                    }
                }
            }
        }
        this->connectionPoints = connectionPoints;
        this->insOutsRects = insOutsRects;
        this->insOutsTypes = insOutsTypes;
        this->stowedTypes = insOutsTypes;
        this->isInType = isInType;
    }else{
        this->connectionPoints = {};
        this->insOutsRects = {};
        this->insOutsTypes = QList<QString>{};
        this->stowedTypes = QList<QString>{};
        this->isInType = {};
    }
    if(mapFuncSize.contains(type)){
        pixmapSize = mapFuncSize[type];
    }
    return editMenuWidgets;
}

void IconObject::setType(QString i){
    this->type = i;
}

QString IconObject::getType(){
    return this->type;
}

void IconObject::setStowedPixmap(QPixmap i){
    this->stowedPixmap = i;
}

QPixmap IconObject::getStowedPixmap(){
    return this->stowedPixmap;
}

void IconObject::setIndex(qlonglong i){
    this->index = i;
}

qlonglong IconObject::getIndex(){
    return this->index;
}

void IconObject::setValue(QList<QByteArray> i){
    this->value = i;
}

QList<QByteArray> IconObject::getValue(){
    return this->value;
}

void IconObject::setConnectionPoints(QList<QPoint> i){
    this->connectionPoints = i;
}

QList<QPoint> IconObject::getConnectionPoints(){
    return this->connectionPoints;
}

void IconObject::setInsOutsRects(QList<QRect> i){
    this->insOutsRects = i;
}

QList<QRect> IconObject::getInsOutsRects(){
    return this->insOutsRects;
}

void IconObject::setInsOutsTypes(QStringList i){
    this->insOutsTypes = i;
}

QStringList IconObject::getInsOutsTypes(){
    return this->insOutsTypes;
}

void IconObject::setStowTypes(QStringList i){
    this->stowedTypes = i;
}

QStringList IconObject::getStowTypes(){
    return this->stowedTypes;
}

void IconObject::setIsInType(QList<bool> i){
    this->isInType = i;
}

QList<bool> IconObject::getIsInType(){
    return this->isInType;
}

void IconObject::setPixmapSize(QSize i){
    this->pixmapSize = i;
}

QSize IconObject::getPixmapSize(){
    return this->pixmapSize;
}

void IconObject::print(){
    qDebug() << "type << value << index << isInType << insOutsTypes << insOutsRects << connectionPoints";
    qDebug() << type << value << index << isInType << insOutsTypes << insOutsRects << connectionPoints;
}
