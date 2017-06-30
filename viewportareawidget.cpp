#include "viewportareawidget.h"
#include "defines.h"

ViewPortAreaWidget::ViewPortAreaWidget(){
    setAcceptDrops(true);
    fmenu = new FloatingMenuMain();
    ftools = new FloatingMenuTools();
    connect(ftools, &FloatingMenuTools::setTool, this, &ViewPortAreaWidget::setMenuCurrentTool);
    connect(ftools, &FloatingMenuTools::setTool, fmenu, &FloatingMenuMain::setMenuCurrentTool);
    ftools->setTool("SELECT");

    wireColor = Qt::black;
    wireColorType = tr("LPH_UNKNOWN");
    wireStyle = Qt::SolidLine;
    widCounter = 1;

    startWire.first = -1; // icon index
    startWire.second = -1; // icon's node subindex
    startWireChildType = Q_NULLPTR;

    for(int i = 0; i < icondatabase.length(); i++){
        for(int j = 0; j < icondatabase.at(i).at(1).length(); j++){
            if(icondatabase.at(i).at(1).at(j).length() >= 6){
                mapFuncEdit[icondatabase.at(i).at(1).at(j).at(4)] = icondatabase.at(i).at(1).at(j).at(5);
                mapFuncPixmap[icondatabase.at(i).at(1).at(j).at(4)] = icondatabase.at(i).at(1).at(j).at(0);
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
    startFinger = QPoint(-1,-1);
    refSelectDrop = QPoint(-1,-1);

    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);

    QTimer::singleShot(250, this, SLOT(arrangeToolbars()));
}

ViewPortAreaWidget::~ViewPortAreaWidget(){

}

QString ViewPortAreaWidget::stateToXML(){
    QString ret;
    QXmlStreamWriter stream(&ret);
    stream.writeStartDocument();
    QListIterator<QObject*> i(children());
    stream.writeStartElement("lph");
    stream.writeStartElement("main");
    stream.writeAttribute("h", QString::number(size().width()));
    stream.writeAttribute("v", QString::number(size().height()));
    stream.writeEndElement(); // main
    while (i.hasNext()) {
        QObject *tmp = i.next();
        if(tr(tmp->metaObject()->className()) == tr("IconObject")){
            IconObject *objAt = static_cast<IconObject*>(tmp);
            stream.writeStartElement("iconobject");
            stream.writeAttribute("index", QString::number(objAt->getIndex()));
            stream.writeAttribute("type", objAt->getType());
            stream.writeAttribute("pos", QString::number(objAt->pos().rx())
                 + "," + QString::number(objAt->pos().ry()));
            QStringList val;
            foreach(QByteArray j, objAt->getValue()){
                val.append(j.toHex());
            }
            stream.writeAttribute("value", val.join(","));
            stream.writeEndElement(); // iconobject

        }
    }
    QMapIterator<qlonglong, QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > > wireIter(this->wires);
    while(wireIter.hasNext()){
        wireIter.next();
        qlonglong wid = wireIter.key();
        QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > a2 = wireIter.value();
        stream.writeStartElement("wire");
        stream.writeAttribute("wid", QString::number(wid));
        stream.writeAttribute("p1", QString::number(a2.first.first));
        stream.writeAttribute("s1", QString::number(a2.first.second));
        stream.writeAttribute("p2", QString::number(a2.second.first));
        stream.writeAttribute("s2", QString::number(a2.second.second));
        stream.writeEndElement(); // wire
    }
    stream.writeEndElement(); // lph
    stream.writeEndDocument();
    return ret;
}

void ViewPortAreaWidget::stateFromXML(QByteArray in){
    deleteAllIconObjects();
    updateWireConnectionTypes();
    QXmlStreamReader xml(in);
    while (!xml.atEnd()) {
        xml.readNext();
        if (xml.isStartElement()){
            if(xml.name() == "main"){
                foreach(QXmlStreamAttribute i, xml.attributes()){
                    if(i.name() == "h"){
                        bool ok;
                        int h = i.value().toString().toInt(&ok);
                        if(!ok){
                            qDebug() << "h not ok" << h << i.value();
                            qApp->quit();
                        }
                        setFixedWidth(h);
                    }else if(i.name() == "v"){
                        bool ok;
                        int v = i.value().toString().toInt(&ok);
                        if(!ok){
                            qDebug() << "v not ok " << v << i.value();
                            qApp->quit();
                        }
                        setFixedHeight(v);
                    }
                }
            }else if(xml.name() == "iconobject"){
                qlonglong index = -1;
                QString type = "";
                QString posComma = "";
                QString valueHexJoined = "";
                foreach(QXmlStreamAttribute i, xml.attributes()){
                    if(i.name() == "index"){
                        bool ok;
                        qlonglong v = i.value().toString().toLongLong(&ok);
                        if(!ok){
                            qDebug() << "v not ok " << v << i.value();
                            qApp->quit();
                        }
                        index = v;
                    }else if(i.name() == "type"){
                        type = i.value().toString();
                    }else if(i.name() == "pos"){
                        posComma = i.value().toString();
                    }else if(i.name() == "value"){
                        valueHexJoined = i.value().toString();
                    }
                }
                if(index > 0 && !type.isEmpty() && !posComma.isEmpty()){
                    if(index>SubMenuMain::idCounter){
                       SubMenuMain::idCounter = index;
                       SubMenuMain::idCounter++;
                    }
                    if(!posComma.contains(QChar(',')) || posComma.split(',').length() != 2){
                        qDebug() << "pos malformed " << posComma;
                        qApp->quit();
                    }
                    bool ok;
                    int posX = posComma.split(',')[0].toInt(&ok);
                    if(!ok){
                        qDebug() << "posX not ok " << posComma;
                        qApp->quit();
                    }
                    int posY = posComma.split(',')[1].toInt(&ok);
                    if(!ok){
                        qDebug() << "posY not ok " << posComma;
                        qApp->quit();
                    }
                    if(!mapFuncPixmap.contains(type)){
                        qDebug() << "type not found " << type;
                        qApp->quit();
                    }
                    IconObject *tmp = new IconObject(this, Qt::WindowFlags(), type);
                    QSize icoSize = parseMapFuncSize(type);
                    //tmp->setFixedHeight(FIXEDICONHEIGHT);
                    //tmp->setFixedWidth(FIXEDICONWIDTH);
                    tmp->setFixedHeight(icoSize.height());
                    tmp->setFixedWidth(icoSize.width());
                    tmp->setIndex(index);
                    QPixmap tmpPixmap(mapFuncPixmap[type]);
                    tmp->setPixmap(tmpPixmap);
                    tmp->setStowedPixmap(tmpPixmap);
                    tmp->move(QPoint(posX, posY));
                    if(!valueHexJoined.isEmpty()){
                        QList<QByteArray> ret;
                        foreach(QString n,valueHexJoined.split(',')){
                            ret.append(QByteArray::fromHex(n.toLatin1()));
                        }
                        tmp->setValue(ret);
                    }
                    tmp->show();
                    tmp->setAttribute(Qt::WA_DeleteOnClose);
                }else{
                    qDebug() << "iconobject parsing error ";
                    qApp->quit();
                }
            }
        }
    }
    if (xml.hasError()) {
        qDebug() << xml.errorString();
        qApp->quit();
    }
    QXmlStreamReader xml2(in);
    while (!xml2.atEnd()) {
        xml2.readNext();
        if (xml2.isStartElement()){
            if(xml2.name() == "wire"){
                qlonglong index1 = -1, index2 = -1, wid = -1;
                int subindex1 = -1, subindex2 = -1;
                foreach(QXmlStreamAttribute i, xml2.attributes()){
                    bool ok;
                    if(i.name() == "p1"){
                        index1 = i.value().toString().toLongLong(&ok);
                    }else if(i.name() == "s1"){
                        subindex1 = i.value().toString().toInt(&ok);
                    }else if(i.name() == "p2"){
                        index2 = i.value().toString().toLongLong(&ok);
                    }else if(i.name() == "s2"){
                        subindex2 = i.value().toString().toInt(&ok);
                    }else if(i.name() == "wid"){
                        wid = i.value().toString().toLongLong(&ok);
                    }else{
                        qDebug() << "i.name() not found: " << i.name();
                        qApp->quit();
                    }
                    if(!ok){
                        qDebug() << "int parsing not ok";
                        qApp->quit();
                    }
                }
                IconObject *io1 = getIconObjectAt(index1);
                IconObject *io2 = getIconObjectAt(index2);
                wireColorType = getWireType(io1, subindex1, io2, subindex2);
                if(wireColorType.isEmpty()){
                    qDebug() << "disjoint wire in file.";
                    qApp->quit();
                }
                if(!wireColorType.isEmpty() && iconcolordatabase.contains(wireColorType)){
                    wireColor = iconcolordatabase[wireColorType];
                }else if(!wireColorType.isEmpty()){
                    wireColor = iconcolordatabase[tr("LPH_UNKNOWN")];
                }
                connectWirePairs(io1, subindex1, io2, subindex2, wid);
            }
        }
    }
    if (xml2.hasError()) {
        qDebug() << xml2.errorString();
        qApp->quit();
    }
}

void ViewPortAreaWidget::setMenuCurrentTool(QString s){
    currentTool = s;
    if(s == "SELECT"){
        setCursor(Qt::ArrowCursor);
    }else if(s == "WIRE"){
        setCursor(QCursor(QPixmap::fromImage(QImage(":/images/mouse_wire.png"))));
    }else if(s == "TEXT"){
        setCursor(Qt::IBeamCursor);
    }else if(s == "FINGER"){
        setCursor(Qt::OpenHandCursor);
    }
    startWire.first = -1;
    startWire.second = -1;
    startWireChildType = Q_NULLPTR;
    endWire = QPoint();
    startFinger = QPoint(-1,-1);
    if(selectedIndecies.length() != 0){
        foreach(int i, selectedIndecies){
            IconObject *tmpChild = getIconObjectAt(i);
            tmpChild->show();
            tmpChild->setPixmap(tmpChild->getStowedPixmap());
        }
        selectedIndecies = {};
        update();
    }
    update();
}

void ViewPortAreaWidget::showAll(){
    this->fmenu->setVisible(true);
    this->ftools->setVisible(true);
}

void ViewPortAreaWidget::hideAll(){
    this->fmenu->setVisible(false);
    this->ftools->setVisible(false);
}

void ViewPortAreaWidget::arrangeToolbars(){
    QPoint pt = this->mapToGlobal(QPoint(0,0));
    this->fmenu->move(pt-QPoint(this->fmenu->width()+10,0));
    this->ftools->move(pt-QPoint(this->ftools->width()+10,-this->fmenu->height()-30));
}

void ViewPortAreaWidget::resizedMainWindow(QSize sz){
    if(sz.width() > this->width()){
        this->setFixedWidth(sz.width());
    }
    if(sz.height() > this->height()){
        this->setFixedHeight(sz.height());
    }
}

void ViewPortAreaWidget::paintEvent(QPaintEvent *event){
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
    if(this->currentTool == "WIRE" && this->startWire.first >= 0 && !this->endWire.isNull()){
        QPoint sLine = QPoint(0,0);
        IconObject *obj = getIconObjectAt(this->startWire.first);
        if(obj != Q_NULLPTR){
            sLine = obj->getConnectionPoints().at(this->startWire.second);
        }
        QPen pen(this->wireColor, 3, this->wireStyle, Qt::RoundCap, Qt::RoundJoin);
        QPainter *paint = new QPainter(this);
        paint->setPen(pen);
        paint->setRenderHint(QPainter::Antialiasing,true);
        paint->drawLine(sLine+obj->pos(), this->endWire);
        paint->end();
    }
    QMapIterator<qlonglong, QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > > wireIter(wires);
    while(wireIter.hasNext()) {
        wireIter.next();
        qlonglong wid = wireIter.key();
        QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > onePair = wireIter.value();
        QPoint sLine = QPoint(0,0);
        QPoint eLine = QPoint(0,0);
        IconObject *obj1 = getIconObjectAt(onePair.first.first);
        if(obj1 != Q_NULLPTR){
            sLine = obj1->getConnectionPoints().at(onePair.first.second);
        }
        IconObject *obj2 = getIconObjectAt(onePair.second.first);
        if(obj2 != Q_NULLPTR){
            eLine = obj2->getConnectionPoints().at(onePair.second.second);
        }
        QBrush thiswirecolor = Qt::black;
        if(this->wireColors.contains(wid)){
            thiswirecolor = this->wireColors[wid].first;
        }
        QPen pen(thiswirecolor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
        QPainter *paint = new QPainter(this);
        paint->setPen(pen);
        paint->setRenderHint(QPainter::Antialiasing,true);
        IconObject *onePairObj = getIconObjectAt(onePair.first.first);
        IconObject *twoPairObj = getIconObjectAt(onePair.second.first);
        //paint->drawLine(sLine+this->mapIconPos[onePair.first.first], eLine+this->mapIconPos[onePair.second.first]);
        paint->drawLine(sLine+onePairObj->pos(), eLine+twoPairObj->pos());
        paint->end();
    }
    event->accept();
}

void ViewPortAreaWidget::dragEnterEvent(QDragEnterEvent *event){
    if (event->mimeData()->hasFormat(BLOCKDIAGMIME)) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void ViewPortAreaWidget::dragMoveEvent(QDragMoveEvent *event){
    if (event->mimeData()->hasFormat(BLOCKDIAGMIME)) {
        if (event->source() == this) {
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    } else {
        event->ignore();
    }
}

void ViewPortAreaWidget::dropEvent(QDropEvent *event){
    update();
    if(event->mimeData()->hasFormat(BLOCKDIAGMIME) && this->currentTool == "SELECT") {
        QByteArray itemData = event->mimeData()->data(BLOCKDIAGMIME);
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        qlonglong id;
        QString type;
        QList<QByteArray> value;
        QList<bool> isInType;
        QStringList insOutsTypes;
        QStringList stowedTypes;
        QList<QRect> insOutsRects;
        QList<QPoint> connectionPoints;
        QPixmap stowedPixmap;
        QSize pixmapSize;

        dataStream >> pixmap >> offset >> id >> type >> value >> isInType >> insOutsTypes >>
                stowedTypes >>  insOutsRects >> connectionPoints >> stowedPixmap >> pixmapSize;

        IconObject *newIcon = new IconObject(this);
        newIcon->setPixmap(pixmap);
        //newIcon->setFixedHeight(FIXEDICONHEIGHT);
        //newIcon->setFixedWidth(FIXEDICONWIDTH);
        newIcon->setFixedHeight(pixmapSize.height());
        newIcon->setFixedWidth(pixmapSize.width());
        newIcon->move(event->pos() - offset);

        newIcon->setIndex(id);
        newIcon->setType(type);
        newIcon->setValue(value);
        newIcon->setIsInType(isInType);
        newIcon->setInsOutsTypes(insOutsTypes);
        newIcon->setStowTypes(stowedTypes);
        newIcon->setInsOutsRects(insOutsRects);
        newIcon->setConnectionPoints(connectionPoints);
        newIcon->setStowedPixmap(stowedPixmap);
        newIcon->setPixmapSize(pixmapSize);

        newIcon->show();
        newIcon->setAttribute(Qt::WA_DeleteOnClose);

        refSelectDrop = newIcon->pos();

        if (event->source() == this){
            event->setDropAction(Qt::MoveAction);
            event->accept();
        } else {
            event->acceptProposedAction();
        }
    }else{
        event->ignore();
    }
}

bool ViewPortAreaWidget::eventFilter(QObject *obj, QEvent *event){
    QString cname = QString(obj->metaObject()->className());
    if(event->type() == QEvent::FocusIn && cname == tr("QWidgetWindow")){
        fmenu->raise();
        ftools->raise();
    }else if(currentTool == "WIRE"){
        eventFilterWire(obj, event);
    }
    return QObject::eventFilter(obj, event);
}

bool ViewPortAreaWidget::eventFilterWire(QObject *obj, QEvent *event){
    if(event->type() == QEvent::MouseMove){
        eventFilterWireMouseMove(obj, event);
    }
    return true;
}

bool ViewPortAreaWidget::eventFilterWireMouseMove(QObject *obj, QEvent *event){
    QString cname = QString(obj->metaObject()->className());
    if(startWire.first >= 0 && ( cname == tr("IconObject") || cname == tr("QWidgetWindow") )){
        QMouseEvent *mevent = static_cast<QMouseEvent*>(event);
        //mapFromGlobal(mevent->globalPos()))
        if(cname == tr("IconObject")){
            IconObject *io = static_cast<IconObject*>(obj);
            if(io->getIndex() != startWire.first){ // && getSubIndexOfIcon(io->getType())
                eventFilterWireMouseMoveIconObject(mevent->pos(), io);
            }else{
                wireStyle = Qt::DashLine;
            }
        }else{
            wireColor = Qt::black;
            wireColorType = tr("LPH_UNKNOWN");
            wireStyle = Qt::SolidLine;
        }
        endWire = mapTo(this, mapFromGlobal(mevent->globalPos()));
        update();
    }
    return true;
}

QString ViewPortAreaWidget::getWireType(IconObject *io1, int subindex1,
                                        IconObject *io2, int subindex2){
    QStringList startTypesAry = io1->getInsOutsTypes().at(subindex1).split("|");
    QStringList endTypesAry = io2->getInsOutsTypes().at(subindex2).split("|");
    QStringList intersect = {};
    bool a = io1->getIsInType().at(subindex1);
    bool b = io2->getIsInType().at(subindex2);
    if(!a && !b){ //both out
        return "";
    }else if(a && b){ //both in should be allowed at some point?
        return "";
    }else if(a && !b){//a is in, b is out
        intersect = intersectionOfAandB(startTypesAry, endTypesAry);
        if(intersect.isEmpty()){
            return "";
        }
        if(isAlreadyConnectedToOther(io1->getIndex(), subindex1)){
            return "";
        }
    }else{//b is in, a is out
        intersect = intersectionOfAandB(startTypesAry, endTypesAry);
        if(intersect.isEmpty()){
            return "";
        }
        if(isAlreadyConnectedToOther(io2->getIndex(), subindex2)){
            return "";
        }
    }
    return intersect.join("|");
}

QStringList ViewPortAreaWidget::intersectionOfAandB(QStringList a, QStringList b){
    QStringList results = {};
    for(int i = 0; i < a.length(); i++){
        for(int j = 0; j < b.length(); j++){
            if(a.at(i) == b.at(j) && !results.contains(a.at(i))){
                results.append(a.at(i));
            }
        }
    }
    return results;
}

bool ViewPortAreaWidget::areAandBequal(QStringList a, QStringList b){
    if(a.length() != b.length())
        return false;
    bool result = true;
    for(int i = 0; i < a.length(); i++){
        if(!b.contains(a.at(i))){
            result = false;
            break;
        }
    }
    return result;
}

bool ViewPortAreaWidget::isAlreadyConnectedToOther(qlonglong index, int subindex){
    bool isAlreadyConnectedToOther = false;
    QMapIterator<qlonglong, QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > > wireIter(wires);
    while(wireIter.hasNext()) {
        wireIter.next();
        QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > thispair = wireIter.value();
        QPair<qlonglong,int> onePair = thispair.first;
        QPair<qlonglong,int> twoPair = thispair.second;
        IconObject *onePairIco = getIconObjectAt(onePair.first);
        IconObject *twoPairIco = getIconObjectAt(twoPair.first);
        bool onePairIsInType = onePairIco->getIsInType().at(onePair.second);
        bool twoPairIsInType = twoPairIco->getIsInType().at(twoPair.second);
        if((onePair.first == index && onePair.second == subindex && onePairIsInType) ||
                (twoPair.first == index && twoPair.second == subindex && twoPairIsInType)){
            isAlreadyConnectedToOther = true;
            break;
        }
    }
    return isAlreadyConnectedToOther;
}

void ViewPortAreaWidget::eventFilterWireMouseMoveIconObject(QPoint meventPos, IconObject *io){
    if(io == Q_NULLPTR)
        return;
    int subindex = getSubIndexOfIcon(io->getType(), meventPos);
    if(io->getInsOutsTypes().length() > subindex && subindex >= 0){
        IconObject *io2 = getIconObjectAt(startWire.first);
        wireColorType = getWireType(io2, startWire.second, io, subindex);
        if(wireColorType.isEmpty())
            wireStyle = Qt::DashLine;
        if(!wireColorType.isEmpty() && iconcolordatabase.contains(wireColorType)){
            wireColor = iconcolordatabase[wireColorType];
        }else if(!wireColorType.isEmpty()){
            wireColor = iconcolordatabase[tr("LPH_UNKNOWN")];
        }
    }
}

QStringList ViewPortAreaWidget::parseMapFuncEdit(QString type){
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
    }
    return editMenuWidgets;
}

QSize ViewPortAreaWidget::parseMapFuncSize(QString type){
    QMap<QString, QSize> mapFuncSize;
    for(int i = 0; i < icondatabase.length(); i++){
        for(int j = 0; j < icondatabase.at(i).at(1).length(); j++){
            if(icondatabase.at(i).at(1).at(j).length() >= 5){
                mapFuncSize[icondatabase.at(i).at(1).at(j).at(4)] =
                        QSize(icondatabase.at(i).at(1).at(j).at(2).toInt(), icondatabase.at(i).at(1).at(j).at(3).toInt());
            }
        }
    }
    if(mapFuncSize.contains(type))
        return mapFuncSize[type];
    else
        return QSize();
}

void ViewPortAreaWidget::mouseReleaseEvent(QMouseEvent *event){
    (void)(event);
    rubberBand->hide();
    if(currentTool == "SELECT"){
        mouseReleaseEventSelect(); //event);
    }
    rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    if(currentTool == "FINGER"){
        mouseReleaseEventFinger(); //event);
    }
    update();
}

void ViewPortAreaWidget::mouseReleaseEventSelect(){ //QMouseEvent *event){
    if(rubberBand->geometry().height() != 0 || rubberBand->geometry().width() != 0){
        for(int i = 0; i < children().length(); i++){
            if(tr(children().at(i)->metaObject()->className()) == tr("IconObject")){
                IconObject *child = static_cast<IconObject*>(children().at(i));
                if(rubberBand->geometry().contains(child->pos())){
                    QPixmap pixmap = *child->pixmap();
                    QPixmap tempPixmap = pixmap;
                    QPainter painter;
                    painter.begin(&tempPixmap);
                    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
                    painter.end();
                    child->setPixmap(tempPixmap);
                    selectedIndecies.append(child->getIndex());
                }
            }
        }
    }
}

void ViewPortAreaWidget::mouseReleaseEventFinger(){ //QMouseEvent *event){
    if(startFinger != QPoint(-1,-1)){
        qApp->restoreOverrideCursor();
        setMouseTracking(false);
        deltaFinger = QPoint(0,0);
    }
}

void ViewPortAreaWidget::mouseMoveEvent(QMouseEvent *event){
    if(this->currentTool == "SELECT"){
        if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
            mouseMoveEventSelectLeft(event);
        }else if((event->buttons() & Qt::RightButton) == Qt::RightButton){
            //mouseMoveEventSelectRight(event);
        }
    }else if(this->currentTool == "WIRE"){
        if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
            //mouseMoveEventWireLeft(event);
        }else if((event->buttons() & Qt::RightButton) == Qt::RightButton){
            //mouseMoveEventWireRight(event);
        }
    }else if(this->currentTool == "FINGER"){
        if(this->startFinger != QPoint(-1,-1) && (event->buttons() & Qt::LeftButton) != Qt::LeftButton){
            this->startFinger = QPoint(-1,-1);
        }
        if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
            mouseMoveEventFingerLeft(event);
        }else if((event->buttons() & Qt::RightButton) == Qt::RightButton){
            //mouseMoveEventFingerRight(event);
        }
    }
}

void ViewPortAreaWidget::mouseMoveEventSelectLeft(QMouseEvent *event){
    rubberBand->setGeometry(QRect(origin, event->pos()).normalized());
}

void ViewPortAreaWidget::mouseMoveEventFingerLeft(QMouseEvent *event){
    if(startFinger != QPoint(-1,-1)){
        //QPoint endFinger = mapFromGlobal(event->globalPos());
        //deltaScrollArea(endFinger, this->startFinger);
        deltaFinger += (event->pos() - startFinger);
        startFinger = event->pos();
        putPositionOfScrollArea(deltaFinger);
    }
}

void ViewPortAreaWidget::mousePressEvent(QMouseEvent *event){
    if(this->currentTool == "SELECT"){
        if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
            mousePressEventSelectLeft(event);
        }else if((event->buttons() & Qt::RightButton) == Qt::RightButton){
            mousePressEventSelectRight(event);
        }
    }else if(this->currentTool == "WIRE"){
        if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
            mousePressEventWireLeft(event);
        }else if((event->buttons() & Qt::RightButton) == Qt::RightButton){
            mousePressEventWireRight(event);
        }
    }else if(this->currentTool == "FINGER"){
        if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
            mousePressEventFingerLeft(event);
        }else if((event->buttons() & Qt::RightButton) == Qt::RightButton){
            //mousePressEventFingerRight(event);
        }
    }
}

IconObject* ViewPortAreaWidget::mousePressEventGetChildAt(QMouseEvent *event){
    if(!childAt(event->pos()))
        return Q_NULLPTR;
    QString oname = childAt(event->pos())->objectName();
    if(oname == "centralWidget" || oname == "mainToolBar"
            || oname == "statusBar" || oname == "qt_scrollarea_viewport")
        return Q_NULLPTR;
    IconObject *child = static_cast<IconObject*>(childAt(event->pos()));
    if (!child)
        return Q_NULLPTR;
    return child;

}


void ViewPortAreaWidget::mousePressEventSelectLeft(QMouseEvent *event){
    //QRubberBand
    origin = event->pos();
    if (!rubberBand)
        rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
    rubberBand->setGeometry(QRect(origin, QSize()));
    rubberBand->show();

    IconObject *child = mousePressEventGetChildAt(event);
    if (child == Q_NULLPTR){
        mousePressEventSelectLeftChildNull();
    }else if(selectedIndecies.length() != 0){
        mousePressEventSelectLeftSelectedIndeciesNotEmpty(event, child);
    }else{
        mousePressEventSelectLeftSelectedIndeciesEmpty(event, child);
    }
}

void ViewPortAreaWidget::mousePressEventSelectLeftChildNull(){
    if(selectedIndecies.length() != 0){
        foreach(int i, selectedIndecies){
            IconObject *tmpChild = getIconObjectAt(i);
            tmpChild->show();
            tmpChild->setPixmap(tmpChild->getStowedPixmap());
        }
        selectedIndecies = {};
        update();
    }
}

void ViewPortAreaWidget::mousePressEventSelectLeftSelectedIndeciesNotEmpty(QMouseEvent *event, IconObject *child){
    QPixmap pixmap = *child->pixmap();
    qlonglong id = child->getIndex();
    QString type = child->getType();
    QList<QByteArray> value = child->getValue();
    QList<bool> isInType = child->getIsInType();
    QStringList insOutsTypes = child->getInsOutsTypes();
    QStringList stowedTypes = child->getStowTypes();
    QList<QRect> insOutsRects = child->getInsOutsRects();
    QList<QPoint> connectionPoints = child->getConnectionPoints();
    QPixmap stowedPixmap = child->getStowedPixmap();
    QSize pixmapSize = child->getPixmapSize();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos()) << id << type << value << isInType
               << insOutsTypes << stowedTypes << insOutsRects << connectionPoints << stowedPixmap << pixmapSize;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(BLOCKDIAGMIME, itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    QPixmap scaledPixmap(pixmap);
    scaledPixmap = scaledPixmap.scaled(pixmapSize);
    //drag->setPixmap(pixmap);
    drag->setPixmap(scaledPixmap);
    drag->setHotSpot(event->pos() - child->pos() );

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);
    refSelectDrop =child->pos();
    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        foreach(int i, selectedIndecies){
            if(i != child->getIndex()){
                IconObject *busyChild = getIconObjectAt(i);
                moveIconObjectAt(i, busyChild->pos() + refSelectDrop - child->pos());
            }
        }
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
    }
}

void ViewPortAreaWidget::mousePressEventSelectLeftSelectedIndeciesEmpty(QMouseEvent *event, IconObject *child){
    QPixmap pixmap = *child->pixmap();
    qlonglong id = child->getIndex();
    QString type = child->getType();
    QList<QByteArray> value = child->getValue();
    QList<bool> isInType = child->getIsInType();
    QStringList insOutsTypes = child->getInsOutsTypes();
    QStringList stowedTypes = child->getStowTypes();
    QList<QRect> insOutsRects = child->getInsOutsRects();
    QList<QPoint> connectionPoints = child->getConnectionPoints();
    QPixmap stowedPixmap = child->getStowedPixmap();
    QSize pixmapSize = child->getPixmapSize();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - child->pos()) << id << type << value << isInType
               << insOutsTypes << stowedTypes << insOutsRects << connectionPoints << stowedPixmap << pixmapSize;

    QMimeData *mimeData = new QMimeData;
    mimeData->setData(BLOCKDIAGMIME, itemData);

    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    QPixmap scaledPixmap(pixmap);
    scaledPixmap = scaledPixmap.scaled(pixmapSize);
    //drag->setPixmap(pixmap);
    drag->setPixmap(scaledPixmap);
    drag->setHotSpot(event->pos() - child->pos() );

    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    child->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction) {
        child->close();
    } else {
        child->show();
        child->setPixmap(pixmap);
    }
}

void ViewPortAreaWidget::mousePressEventSelectRight(QMouseEvent *event){
    if(selectedIndecies.length() != 0){
        QMenu myMenu;
        myMenu.addAction("Delete");
        myMenu.popup(event->globalPos());
        QAction* selectedItem = myMenu.exec();
        if(selectedItem){
            if(selectedItem->text() == "Delete"){
                foreach(int i, selectedIndecies){
                    deleteIconObjectAt(i);
                }
                updateWireConnectionTypes();
            }
        }
        foreach(int i, selectedIndecies){
            IconObject *tmpChild = getIconObjectAt(i);
            tmpChild->show();
            tmpChild->setPixmap(tmpChild->getStowedPixmap());
        }
        origin = event->pos();
        selectedIndecies = {};
        update();
        return;
    }

    IconObject *child = mousePressEventGetChildAt(event);
    if (child == Q_NULLPTR)
        return;

    QStringList editMenuWidgets = parseMapFuncEdit(child->getType());
    QMenu myMenu;
    if(!editMenuWidgets.isEmpty())
        myMenu.addAction("Edit");
    bool hasUnlinkableWiresAttached = false;
    QMapIterator<qlonglong, QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > > wireIter(this->wires);
    while(wireIter.hasNext()) {
        wireIter.next();
        QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > thispair = wireIter.value();
        if(thispair.first.first == child->getIndex()  || thispair.second.first == child->getIndex()){
            hasUnlinkableWiresAttached = true;
            break;
        }
    }
    if(hasUnlinkableWiresAttached)
        myMenu.addAction("Unlink Attached Wires");
    myMenu.addAction("Delete");
    myMenu.addAction("Properties");
    myMenu.popup(event->globalPos());

    QAction* selectedItem = myMenu.exec();
    if(selectedItem){
        if(selectedItem->text() == "Edit"){
            QDialog *frame = new QDialog(this);
            frame->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
            frame->setWindowState(Qt::WindowActive);
            frame->move(event->globalPos());
            frame->show();
            QVBoxLayout *vblo = new QVBoxLayout(frame);
            QList<QByteArray> previousNamedType = {};
            previousNamedType = child->getValue();
            for(int i = 0; i < editMenuWidgets.length(); i++){
                if(editMenuWidgets.at(i).contains("_")){
                    QStringList labelWidgetAndDefaultValue = editMenuWidgets.at(i).split("_");
                    if(labelWidgetAndDefaultValue.length() == 3){
                        QString label = labelWidgetAndDefaultValue.at(0);
                        QString widge = labelWidgetAndDefaultValue.at(1);
                        QString defaultValue = labelWidgetAndDefaultValue.at(2);
                        if(widge == "QCheckBox"){
                            QHBoxLayout *hblo = new QHBoxLayout();
                            QLabel *lab1 = new QLabel(label);
                            QCheckBox *cb1 = new QCheckBox(frame);
                            hblo->addWidget(lab1);
                            hblo->addWidget(cb1);
                            vblo->addLayout(hblo);
                            if(previousNamedType.length() >= i + 1){
                                QString val = previousNamedType.at(i);
                                if(val == tr("true")){
                                    cb1->setChecked(true);
                                }else if(val == tr("false")){
                                    cb1->setChecked(false);
                                }
                            }else{
                                if(defaultValue == tr("true")){
                                    cb1->setChecked(true);
                                    child->setValue(QList<QByteArray>{QByteArray("true")});
                                }else if(defaultValue == tr("false")){
                                    cb1->setChecked(false);
                                    child->setValue(QList<QByteArray>{QByteArray("false")});
                                }
                            }
                        }else if(widge == "QSpinBox"){
                            QHBoxLayout *hblo = new QHBoxLayout();
                            QLabel *lab1 = new QLabel(label);
                            QSpinBox *sb1 = new QSpinBox(frame);
                            sb1->setMaximum(INT32_MAX);
                            sb1->setMinimum(INT32_MIN);
                            hblo->addWidget(lab1);
                            hblo->addWidget(sb1);
                            vblo->addLayout(hblo);
                            if(previousNamedType.length() >= i + 1){
                                QString val = previousNamedType.at(i);
                                bool ok;
                                int intVal = val.toInt(&ok, 10);
                                if(ok){
                                    sb1->setValue(intVal);
                                }
                            }
                        }else if(widge == "QDoubleSpinBox"){
                            QHBoxLayout *hblo = new QHBoxLayout();
                            QLabel *lab1 = new QLabel(label);
                            QDoubleSpinBox *dsb1 = new QDoubleSpinBox(frame);
                            dsb1->setMaximum(DBL_MAX);
                            dsb1->setMinimum(-DBL_MAX);
                            hblo->addWidget(lab1);
                            hblo->addWidget(dsb1);
                            vblo->addLayout(hblo);
                            if(previousNamedType.length() >= i + 1){
                                QString val = previousNamedType.at(i);
                                bool ok;
                                double dubVal = val.toDouble(&ok);
                                if(ok){
                                    dsb1->setValue(dubVal);
                                }
                            }
                        }else if(widge == "QLineEdit"){
                            QHBoxLayout *hblo = new QHBoxLayout();
                            QLabel *lab1 = new QLabel(label);
                            QLineEdit *le1 = new QLineEdit(frame);
                            hblo->addWidget(lab1);
                            hblo->addWidget(le1);
                            vblo->addLayout(hblo);
                            if(previousNamedType.length() >= i + 1){
                                QString val = previousNamedType.at(i);
                                le1->setText(val);
                            }
                        }
                    }
                }
            }
            QHBoxLayout *hblo2 = new QHBoxLayout();
            QPushButton *pb1 = new QPushButton(frame);
            pb1->setText(tr("Ok"));
            pb1->setDefault(true);
            pb1->autoDefault();
            hblo2->addWidget(pb1);
            QPushButton *pb2 = new QPushButton(frame);
            pb2->setText(tr("Cancel"));
            hblo2->addWidget(pb2);
            vblo->addLayout(hblo2);
            connect(pb1, &QPushButton::clicked, frame, &QDialog::accept);
            connect(pb2, &QPushButton::clicked, frame, &QDialog::reject);
            frame->setStyleSheet("background: #D6D6D6;");
            if(frame->exec()){
                QList<QByteArray> namedType = {};
                for(int i = 0; i < frame->children().length(); i++){
                    if(frame->children().at(i)->metaObject()->className() == tr("QCheckBox")){
                        QCheckBox *t = qobject_cast<QCheckBox*>(frame->children().at(i));
                        if(t->isChecked())
                            namedType.append(tr("true").toLatin1());
                        else
                            namedType.append(tr("false").toLatin1());
                    }else if(frame->children().at(i)->metaObject()->className() == tr("QSpinBox")){
                        QSpinBox *t = qobject_cast<QSpinBox*>(frame->children().at(i));
                        namedType.append(QString::number(t->value()).toLatin1());
                    }else if(frame->children().at(i)->metaObject()->className() == tr("QDoubleSpinBox")){
                        QDoubleSpinBox *t = qobject_cast<QDoubleSpinBox*>(frame->children().at(i));
                        namedType.append(QString::number(t->value()).toLatin1());
                    }else if(frame->children().at(i)->metaObject()->className() == tr("QLineEdit")){
                        QLineEdit *t = qobject_cast<QLineEdit*>(frame->children().at(i));
                        namedType.append(t->text().toLatin1());
                    }
                }
                child->setValue(namedType);
            }
            frame->close();
        }else if(selectedItem->text() == "Unlink Attached Wires"){
            QMapIterator<qlonglong, QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > > wireIter(wires);
            while(wireIter.hasNext()) {
                wireIter.next();
                QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > thispair = wireIter.value();
                if(thispair.first.first == child->getIndex()  || thispair.second.first == child->getIndex()){
                    if(wireColors.contains(wireIter.key())){
                        wireColors.remove(wireIter.key());
                    }
                    wires.remove(wireIter.key());
                }
            }
            child->setInsOutsTypes(child->getStowTypes());
            updateWireConnectionTypes();
            update();
        }else if(selectedItem->text() == "Delete"){
            deleteIconObjectAt(child->getIndex());
            updateWireConnectionTypes();
        }else if(selectedItem->text() == "Properties"){
            QFrame *frame = new QFrame(this);
            frame->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
            //frame->move(pos()+child->pos());
            frame->move(event->globalPos());
            frame->show();
            QVBoxLayout *vblo = new QVBoxLayout(frame);
            QLabel *name1 = new QLabel(tr("id = ")+QString::number(child->getIndex()));
            vblo->addWidget(name1);
            QLabel *name2 = new QLabel(tr("type = ")+QString(child->getType()));
            vblo->addWidget(name2);
            QLabel *name3 = new QLabel(tr("value = ")+QString(child->getValue().join("\n")));
            vblo->addWidget(name3);
        }
    }
}

int ViewPortAreaWidget::getSubIndexOfIcon(QString type, QPoint relPos){
    int ret = -1;
    QList<QRect> insOutsRects = {};
    if(mapFuncEdit.contains(type)){
        QStringList insOutsAndEdits = mapFuncEdit[type].split(",");
        for(int i = 0; i < insOutsAndEdits.length(); i++){
            QStringList keyValue = insOutsAndEdits.at(i).split(":");
            if(keyValue.length() == 2 && (keyValue.at(0) == tr("in") || keyValue.at(0) == tr("out"))){
                if(QRegExp("^\\(.*\\)\\{.*\\}$").indexIn(keyValue.at(1), QRegExp::CaretAtZero) != -1){
                    QString allWidgetsString = keyValue.at(1);
                    allWidgetsString.replace(QRegExp("^\\((.*)\\)\\{.*\\}$"), "\\1");
                    allWidgetsString = keyValue.at(1);
                    allWidgetsString.replace(QRegExp("^\\(.*\\)\\{(.*)\\}$"), "\\1");
                    QStringList rectString = allWidgetsString.split("|");
                    if(rectString.length() == 6){
                        insOutsRects.append(QRect(rectString.at(0).toInt(), rectString.at(1).toInt(), rectString.at(2).toInt(), rectString.at(3).toInt()));
                    }
                }
            }
        }
    }
    QSize icoSize = parseMapFuncSize(type);
    for(int i = 0; i < insOutsRects.length(); i++){
        //if(relPos.rx() == FIXEDICONWIDTH - 1)
        if(relPos.rx() == icoSize.width() - 1)
            relPos += QPoint(-1,0);
        //if(relPos.ry() == FIXEDICONHEIGHT - 1)
        if(relPos.ry() == icoSize.height() - 1)
            relPos += QPoint(0,-1);
        if(insOutsRects.at(i).contains(relPos)){
            ret = i;
            break;
        }
    }
    return ret;
}

void ViewPortAreaWidget::updateWireConnectionTypes(){
    for(int i = 0; i < children().length(); i++){
        if(tr(children().at(i)->metaObject()->className()) == tr("IconObject")){
            IconObject *child = static_cast<IconObject*>(children().at(i));
            child->setInsOutsTypes(child->getStowTypes());
        }
    }
    QMapIterator<qlonglong, QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > > wireIter(wires);
    while(wireIter.hasNext()){
        wireIter.next();
        qlonglong wid = wireIter.key();
        QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > a = wireIter.value();
        IconObject *onePairIco = getIconObjectAt(a.first.first);
        IconObject *twoPairIco = getIconObjectAt(a.second.first);
        QStringList isect = intersectionOfAandB(
                onePairIco->getInsOutsTypes()[a.first.second].split("|"),
                twoPairIco->getInsOutsTypes()[a.second.second].split("|")
            );
        if(isect.isEmpty()){
            qDebug() << "a wire exixts thaht has no intersect? Fault";
            qApp->quit();
        }else if(isect.length() == 1){
            wireColors[wid].first = iconcolordatabase[isect.at(0)];
            wireColors[wid].second = isect.at(0);
        }else{
            wireColors[wid].first = iconcolordatabase[tr("LPH_UNKNOWN")];
            wireColors[wid].second = tr("LPH_UNKNOWN");
        }
        if(
                !areAandBequal(isect, onePairIco->getInsOutsTypes()[a.first.second].split("|"))
                ||
                !areAandBequal(isect, twoPairIco->getInsOutsTypes()[a.second.second].split("|"))
            ){
            wireIter.toFront();
            if(!areAandBequal(isect, onePairIco->getInsOutsTypes()[a.first.second].split("|"))){
                onePairIco->setInsOutsTypes(isect);
                if(isect.length() == 1 &&
                        mapFuncVaryNode.contains(onePairIco->getType()) &&
                        mapFuncVaryNode[onePairIco->getType()].contains(a.first.second) &&
                        mapFuncVaryNode[onePairIco->getType()][a.first.second].contains(isect.at(0))){
                    onePairIco->setInsOutsTypes(mapFuncVaryNode[onePairIco->getType()][a.first.second][isect.at(0)]);
                }
            }
            if(!areAandBequal(isect, twoPairIco->getInsOutsTypes()[a.second.second].split("|"))){
                twoPairIco->setInsOutsTypes(isect);
                if(isect.length() == 1 &&
                        mapFuncVaryNode.contains(twoPairIco->getType()) &&
                        mapFuncVaryNode[twoPairIco->getType()].contains(a.second.second) &&
                        mapFuncVaryNode[twoPairIco->getType()][a.second.second].contains(isect.at(0))){
                    twoPairIco->setInsOutsTypes(mapFuncVaryNode[twoPairIco->getType()][a.second.second][isect.at(0)]);
                }
            }
            //fix setInsOutsTypes on node(s)
            //run mapFuncVaryNode updates
            //wireIter.toFront();
            //continue;
        }
    }
}

void ViewPortAreaWidget::connectWirePairs(IconObject *io1, int subindex1, IconObject *io2, int subindex2, qlonglong wid){
    if(wid == -1 && !wires.contains(widCounter) && !wireColors.contains(widCounter)){
        wires[widCounter] = QPair<QPair<qlonglong,int>,QPair<qlonglong,int> >(
                    QPair<qlonglong,int>(io1->getIndex(), subindex1),
                    QPair<qlonglong,int>(io2->getIndex(),subindex2)
                );
        widCounter++;
    }else if(!wires.contains(wid) && !wireColors.contains(wid)){
        wires[wid] = QPair<QPair<qlonglong,int>,QPair<qlonglong,int> >(
                    QPair<qlonglong,int>(io1->getIndex(),subindex1),
                    QPair<qlonglong,int>(io2->getIndex(),subindex2)
                );
        if(wid >= widCounter){
            widCounter = wid;
            widCounter++;
        }
    }else{
        qDebug() << "invalid wid, exiting: wid == " << wid << ", widCounter == " << widCounter;
        qApp->quit();
    }
    updateWireConnectionTypes();
    wireColor = Qt::black;
    wireColorType = tr("LPH_UNKNOWN");
}

void ViewPortAreaWidget::mousePressEventWireLeft(QMouseEvent *event){
    IconObject *child = mousePressEventGetChildAt(event);
    if (child == Q_NULLPTR)
        return;
    int subindex = getSubIndexOfIcon(child->getType(), event->pos() - child->pos());
    if(startWire.first < 0 && subindex >= 0){
        startWire.first = child->getIndex();
        startWireChildType = child->getType();
        startWire.second = subindex;
    }else{
        if(wireStyle != Qt::DashLine){
            connectWirePairs(getIconObjectAt(startWire.first), startWire.second, child, subindex);
        }
        startWire.first = -1;
        startWire.second = -1;
        startWireChildType = Q_NULLPTR;
        endWire = QPoint();
        update();
    }
}

void ViewPortAreaWidget::mousePressEventWireRight(QMouseEvent *event){
    this->startWire.first = -1;
    this->startWire.second = -1;
    this->startWireChildType = Q_NULLPTR;
    this->endWire = QPoint();
    update();
    event->accept();
}

void ViewPortAreaWidget::mousePressEventFingerLeft(QMouseEvent *event){
    startFinger = event->pos();
    qApp->setOverrideCursor(Qt::ClosedHandCursor);
    setMouseTracking(true);
}

//void ViewPortAreaWidget::mousePressEventFingerRight(QMouseEvent *event){
//
//}

void ViewPortAreaWidget::moveIconObjectAt(qlonglong id, QPoint newPos){
    IconObject *tmpChild = getIconObjectAt(id);
    tmpChild->move(newPos.rx(), newPos.ry());
}

void ViewPortAreaWidget::deleteAllIconObjects(){
    for(int i = 0; i < children().length(); i++){
        if(tr(children().at(i)->metaObject()->className()) == tr("IconObject")){
            IconObject *io = static_cast<IconObject*>(children().at(i));
            deleteIconObjectAt(io->getIndex());
        }
    }
    updateWireConnectionTypes();
}

void ViewPortAreaWidget::deleteIconObjectAt(qlonglong id){
    IconObject *tmpChild = getIconObjectAt(id);
    QMapIterator<qlonglong, QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > > wireIter(wires);
    while(wireIter.hasNext()) {
        wireIter.next();
        QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > thispair = wireIter.value();
        if(thispair.first.first == id  || thispair.second.first == id){
            if(wireColors.contains(wireIter.key())){
                wireColors.remove(wireIter.key());
            }
            wires.remove(wireIter.key());
        }
        if(thispair.first.first == id){
            bool hasAnyConnectedWires = false;
            QMapIterator<qlonglong, QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > > innerWireIter(wires);
            while(innerWireIter.hasNext()){
                innerWireIter.next();
                QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > innerPair = innerWireIter.value();
                if(innerPair.first.first == thispair.second.first){
                    hasAnyConnectedWires = true;
                    break;
                }
                if(innerPair.second.first == thispair.second.first){
                    hasAnyConnectedWires = true;
                    break;
                }
            }
            if(!hasAnyConnectedWires){
                IconObject *busyChild = getIconObjectAt(thispair.second.first);
                busyChild->setInsOutsTypes(busyChild->getStowTypes());
            }
        }else if(thispair.second.first == id){
            bool hasAnyConnectedWires = false;
            QMapIterator<qlonglong, QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > > innerWireIter(wires);
            while(innerWireIter.hasNext()){
                innerWireIter.next();
                QPair<QPair<qlonglong, int>, QPair<qlonglong, int> > innerPair = innerWireIter.value();
                if(innerPair.first.first == thispair.first.first){
                    hasAnyConnectedWires = true;
                    break;
                }
                if(innerPair.second.first == thispair.first.first){
                    hasAnyConnectedWires = true;
                    break;
                }
            }
            if(!hasAnyConnectedWires){
                IconObject *busyChild = getIconObjectAt(thispair.first.first);
                busyChild->setInsOutsTypes(busyChild->getStowTypes());
            }
        }
    }
    tmpChild->close();
    update();
}

IconObject* ViewPortAreaWidget::getIconObjectAt(qlonglong id){
    for(int i = 0; i < children().length(); i++){
        if(tr(children().at(i)->metaObject()->className()) == tr("IconObject")){
            IconObject *io = static_cast<IconObject*>(children().at(i));
            if(io->getIndex() == id){
                return io;
            }
        }
    }
    return Q_NULLPTR;
}

void ViewPortAreaWidget::continue_actionDump_iconMap_triggered(){
    qDebug() << "##################################################################";
    QListIterator<QObject*> i(this->children());
    while (i.hasNext()) {
        QObject *tmp = i.next();
        if(tr(tmp->metaObject()->className()) == tr("IconObject")){
            IconObject *objAt = static_cast<IconObject*>(tmp);
            QString ioarray = "";
            foreach(bool j, objAt->getIsInType()){
                ioarray += (j)?"i":"o";
            }
            qDebug() << objAt->getIndex() << ": " << objAt->getType() << objAt->getValue() << " > " << objAt->getInsOutsTypes() << "\\" << objAt->pos() << ioarray;
            //objAt->print();
        }
    }
    qDebug() << "!!!";
    QMapIterator<qlonglong, QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > > wireIter(this->wires);
    while(wireIter.hasNext()){
        wireIter.next();
        qlonglong wid = wireIter.key();
        QPair<QPair<qlonglong,int>, QPair<qlonglong,int> > a2 = wireIter.value();
        qDebug() << "[" << wid << "] " << a2.first.first << ":" << a2.first.second << " <=> " << a2.second.first << ":" << a2.second.second << " - " << wireColors[wid];
    }
    qDebug() << "!!!";
    foreach(qlonglong i, selectedIndecies){
        qDebug() << i;
    }
    qDebug() << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%";

}
