#include "floatingmenumain.h"
#include "ui_floatingmenumain.h"

FloatingMenuMain::FloatingMenuMain(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FloatingMenuMain)
{
    ui->setupUi(this);

    int maxrow;
    int maxcolumn;
    if(icondatabase.length() <= 2){
        maxrow = 1;
        maxcolumn = icondatabase.length();
    }else{
        maxrow = floor(icondatabase.length()/2)+((icondatabase.length()%2==0)?0:1);
        maxcolumn = 2;
    }

    this->setFixedSize(QSize(10 + maxcolumn*(FIXEDICONWIDTHSPACING+FIXEDICONWIDTH),
                             10 + maxrow*(FIXEDICONWIDTHSPACING+FIXEDICONWIDTH)));

    for(int i = 0; i < icondatabase.length(); i++){
        int row = floor(i/2);
        int column = i%2;
        IconObject *tmp = new IconObject(this);
        tmp->setIndex(i);
        tmp->setFixedSize(FIXEDICONWIDTH, FIXEDICONHEIGHT);
        tmp->setPixmap(QPixmap(icondatabase.at(i).at(0).at(0).at(0)));
        tmp->setToolTip(icondatabase.at(i).at(0).at(0).at(1));
        tmp->move(10 + column*(FIXEDICONWIDTHSPACING+FIXEDICONWIDTH),
                  10 + row*(FIXEDICONHEIGHTSPACING+FIXEDICONHEIGHT));
        tmp->show();
    }

    this->move(10,40);
    this->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    this->show();
    this->currentTool = "SELECT";
}

FloatingMenuMain::~FloatingMenuMain()
{
    delete ui;
}

void FloatingMenuMain::mousePressEvent(QMouseEvent *event){
    if(!childAt(event->pos()))
        return;
    QString oname = childAt(event->pos())->objectName();
    if(oname == "centralWidget" || oname == "mainToolBar")
        return;
    if(!oname.isEmpty()){
        return;
    }
    IconObject *child = static_cast<IconObject*>(childAt(event->pos()));
    if (!child)
        return;
    if((event->buttons() & Qt::LeftButton) == Qt::LeftButton){
        if(this->currentTool == "SELECT"){
            SubMenuMain *smm = new SubMenuMain(icondatabase.at(child->getIndex()).at(1), this);
            smm->move(pos()+event->pos());
        }
    }
}

void FloatingMenuMain::setMenuCurrentTool(QString s){
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
