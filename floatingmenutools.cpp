#include "floatingmenutools.h"
#include "ui_floatingmenutools.h"

FloatingMenuTools::FloatingMenuTools(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::FloatingMenuTools)
{
    ui->setupUi(this);

    ui->pushButtonA->setIcon(QIcon(":/images/mouse1.png"));
    ui->pushButtonA->setIconSize(QSize(25,25));
    ui->pushButtonA->setFixedSize(QSize(25,25));

    ui->pushButtonB->setIcon(QIcon(":/images/mouse2.png"));
    ui->pushButtonB->setIconSize(QSize(25,25));
    ui->pushButtonB->setFixedSize(QSize(25,25));

    ui->pushButtonC->setIcon(QIcon(":/images/mouse3.png"));
    ui->pushButtonC->setIconSize(QSize(25,25));
    ui->pushButtonC->setFixedSize(QSize(25,25));

    ui->pushButtonD->setIcon(QIcon(":/images/mouse4.png"));
    ui->pushButtonD->setIconSize(QSize(25,25));
    ui->pushButtonD->setFixedSize(QSize(25,25));

    ui->pushButtonE->setIcon(QIcon(":/images/blank_tool.png"));
    ui->pushButtonE->setIconSize(QSize(25,25));
    ui->pushButtonE->setFixedSize(QSize(25,25));
    ui->pushButtonE->setEnabled(false);

    ui->pushButtonF->setIcon(QIcon(":/images/blank_tool.png"));
    ui->pushButtonF->setIconSize(QSize(25,25));
    ui->pushButtonF->setFixedSize(QSize(25,25));
    ui->pushButtonF->setEnabled(false);

    ui->pushButtonG->setIcon(QIcon(":/images/blank_tool.png"));
    ui->pushButtonG->setIconSize(QSize(25,25));
    ui->pushButtonG->setFixedSize(QSize(25,25));
    ui->pushButtonG->setEnabled(false);

    ui->pushButtonH->setIcon(QIcon(":/images/blank_tool.png"));
    ui->pushButtonH->setIconSize(QSize(25,25));
    ui->pushButtonH->setFixedSize(QSize(25,25));
    ui->pushButtonH->setEnabled(false);

    ui->pushButtonI->setIcon(QIcon(":/images/blank_tool.png"));
    ui->pushButtonI->setIconSize(QSize(25,25));
    ui->pushButtonI->setFixedSize(QSize(25,25));
    ui->pushButtonI->setEnabled(false);

    this->move(10,470);
    this->setFrameStyle(QFrame::Sunken | QFrame::StyledPanel);
    this->setFixedSize(QSize(128, 128));
    this->show();

    this->setTool("SELECT");
}

FloatingMenuTools::~FloatingMenuTools(){
    delete ui;
}


void FloatingMenuTools::on_pushButtonA_clicked(){
    setTool(QString("SELECT"));
    //QPixmap pxmp = QPixmap::fromImage(QImage(":/images/mouse_select.png"));
    //QCursor custom(pxmp);
    //this->setCursor(custom);
    setCursor(Qt::ArrowCursor);
}

void FloatingMenuTools::on_pushButtonB_clicked(){
    setTool(QString("WIRE"));
    //QPixmap pxmp = QPixmap::fromImage(QImage(":/images/mouse_wire.png"));
    //QCursor custom(pxmp);
    setCursor(QCursor(QPixmap::fromImage(QImage(":/images/mouse_wire.png"))));
}

void FloatingMenuTools::on_pushButtonC_clicked(){
    setTool(QString("TEXT"));
    //QPixmap pxmp = QPixmap::fromImage(QImage(":/images/mouse_text.png"));
    //QCursor custom(pxmp);
    //this->setCursor(custom);
    setCursor(Qt::IBeamCursor);
}

void FloatingMenuTools::on_pushButtonD_clicked(){
    setTool(QString("FINGER"));
    //QPixmap pxmp = QPixmap::fromImage(QImage(":/images/mouse_finger.png"));
    //QCursor custom(pxmp);
    //this->setCursor(custom);
    setCursor(Qt::OpenHandCursor);
}
