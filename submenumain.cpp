#include "submenumain.h"
#include "ui_submenumain.h"

qlonglong SubMenuMain::idCounter = 1;

SubMenuMain::SubMenuMain(QList<QList<QString> > menuList, QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SubMenuMain)
{
    ui->setupUi(this);
    this->currentTool = "SELECT";

    if(menuList.length() != 0){
        int maxrow;
        int maxcolumn;
        if(menuList.length() <= 3){
            maxrow = 1;
            maxcolumn = menuList.length();
        }else{
            maxrow = floor(menuList.length()/3)+((menuList.length()%3==0)?0:1);
            maxcolumn = 3;
        }

        this->setFixedSize(QSize(10 + maxcolumn*(FIXEDICONWIDTHSPACING+FIXEDICONWIDTH),
                                 10 + maxrow*(FIXEDICONWIDTHSPACING+FIXEDICONWIDTH)));
        for(int i = 0; i < menuList.length(); i++){
            int row = floor(i/3);
            int column = i%3;
            IconObject *tmp = new IconObject(this, Qt::WindowFlags(), menuList.at(i).at(4));
            //tmp->setValue(QByteArray()); //menuList.at(i).at(4).toLatin1()
            //tmp->setType(menuList.at(i).at(2));
            tmp->setPixmap(QPixmap(menuList.at(i).at(0)));
            tmp->setToolTip(menuList.at(i).at(1));
            tmp->move(10 + column*(FIXEDICONWIDTHSPACING+FIXEDICONWIDTH),
                      10 + row*(FIXEDICONWIDTHSPACING+FIXEDICONWIDTH));
            tmp->show();
            tmp->setAttribute(Qt::WA_DeleteOnClose);

        }
        this->setFrameShape(QFrame::StyledPanel);
        this->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
        this->show();
    }
}

SubMenuMain::~SubMenuMain(){
    delete ui;
}

void SubMenuMain::setMenuCurrentTool(QString s){
    this->currentTool = s;
    if(s == "SELECT"){
        setCursor(Qt::ArrowCursor);
    }else if(s == "WIRE"){
        setCursor(QCursor(QPixmap::fromImage(QImage(":/images/mouse_wire.png"))));
    }else if(s == "TEXT"){
        setCursor(Qt::IBeamCursor);
    }else if(s == "FINGER"){
        setCursor(Qt::OpenHandCursor);
    }
}

void SubMenuMain::mousePressEvent(QMouseEvent *event){
    if(!childAt(event->pos())){
        this->deleteLater();
        return;
    }
    QString oname = childAt(event->pos())->objectName();
    if(!oname.isEmpty()){
        this->deleteLater();
        return;
    }
    IconObject *child = static_cast<IconObject*>(childAt(event->pos()));
    if (!child)
        return;
    if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
        if(this->currentTool == "SELECT"){
            QPixmap pixmap = *child->pixmap();
            qlonglong index = SubMenuMain::idCounter;
            SubMenuMain::idCounter++;
            QString type = child->getType();
            QList<QByteArray> value = child->getValue();
            QList<bool> isInType = child->getIsInType();
            QStringList insOutsTypes = child->getInsOutsTypes();
            QStringList stowedTypes = child->getStowTypes();
            QList<QRect> insOutsRects = child->getInsOutsRects();
            QList<QPoint> connectionPoints = child->getConnectionPoints();
            QPixmap stowedPixmap = *child->pixmap();
            QSize pixmapSize = child->getPixmapSize();

            QByteArray itemData;
            QDataStream dataStream(&itemData, QIODevice::WriteOnly);
            dataStream << pixmap << QPoint(event->pos() - child->pos()) << index << type << value << isInType
                       << insOutsTypes << stowedTypes << insOutsRects << connectionPoints << stowedPixmap << pixmapSize;

            QMimeData *mimeData = new QMimeData;
            mimeData->setData(BLOCKDIAGMIME, itemData);

            QDrag *drag = new QDrag(this);
            drag->setMimeData(mimeData);
            drag->setPixmap(pixmap);
            drag->setHotSpot(event->pos() - child->pos());

            QPixmap tempPixmap = pixmap;
            QPainter painter;
            painter.begin(&tempPixmap);
            painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
            painter.end();

            if(drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction){
                child->setPixmap(pixmap);
                //        child->close();
            }else{
                child->show();
                child->setPixmap(pixmap);
            }
        }
    }
    event->accept();
}

