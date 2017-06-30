#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    minimizeAction = new QAction(tr("&Minimize"), this);
    connect(minimizeAction, SIGNAL(triggered()), this, SLOT(allHide()));
    restoreAction = new QAction(tr("&Show"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(allShow()));
    quitAction = new QAction(tr("&Exit"), this);
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(minimizeAction);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addAction(quitAction);
    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    QIcon icon = QIcon(":/images/systray.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    trayIcon->show();
    connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setMovable(false);
    ui->actionStop->setDisabled(true);

    sa = new QScrollArea();
    vpaw = new ViewPortAreaWidget();
    vpaw->setObjectName(tr("vpaw"));
    //vpaw->setStyleSheet(tr("QWidget {border-style: solid;}"));
    vpaw->setStyleSheet(tr("border-style: solid; background-image: url(:/images/grid.png); background-repeat: repeat-xy;"));

    sa->setWidget(vpaw);
    sa->acceptDrops();
    vpaw->acceptDrops();
    ui->gridLayout->addWidget(sa);
    vpaw->setFixedSize(size());

    sa->setHorizontalScrollBar(ui->horizontalScrollBar);
    sa->setVerticalScrollBar(ui->verticalScrollBar);
    sa->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    sa->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    qda = new QDialog(this);
    QGridLayout *qglo = new QGridLayout();
    qda->setLayout(qglo);
    sda = new QScrollArea();
    ee = new ExecutionEngine();
    qglo->addWidget(sda);
    sda->setWidget(ee);

    qhsbr = new QScrollBar(Qt::Horizontal);
    qvsbr = new QScrollBar(Qt::Vertical);

    sda->setHorizontalScrollBar(qhsbr);
    sda->setVerticalScrollBar(qvsbr);
    sda->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    sda->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);

    qda->setFixedSize(size());
    ee->setFixedSize(size());

    connect(this, &MainWindow::continue_actionDump_iconMap_triggered, vpaw, &ViewPortAreaWidget::continue_actionDump_iconMap_triggered);
    connect(this, &MainWindow::sendShowAll, vpaw, &ViewPortAreaWidget::showAll);
    connect(this, &MainWindow::sendHideAll, vpaw, &ViewPortAreaWidget::hideAll);
    connect(this, &MainWindow::resizedMainWindow, vpaw, &ViewPortAreaWidget::resizedMainWindow);
    connect(vpaw, &ViewPortAreaWidget::putPositionOfScrollArea, this, &MainWindow::setPositionOfScrollArea);

    qApp->installEventFilter(vpaw);
    qApp->installEventFilter(this);

    setFocus();
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::iconActivated(QSystemTrayIcon::ActivationReason reason){
    switch (reason) {
    case QSystemTrayIcon::Trigger:
    case QSystemTrayIcon::DoubleClick:
        if(isVisible()){
            allHide();
        }else{
            allShow();
        }
        break;
    default:
        ;
    }
}

void MainWindow::closeEvent(QCloseEvent *event){
    event->accept();
    QTimer::singleShot(250, qApp, SLOT(quit()));
}

void MainWindow::resizeEvent(QResizeEvent* event){
    resizedMainWindow(event->size());
}

void MainWindow::setPositionOfScrollArea(QPointF delta){
    if(sa->horizontalScrollBar()->value() == 0 && delta.rx() > 0){
    }else if(sa->horizontalScrollBar()->value() == sa->horizontalScrollBar()->maximum() && delta.rx() < 0){
        vpaw->setFixedWidth(vpaw->width()-delta.rx());
    }else{
        sa->horizontalScrollBar()->setValue(
                sa->horizontalScrollBar()->value()-delta.rx()
            );
    }

    if(sa->verticalScrollBar()->value() == 0 && delta.ry() > 0){
    }else if(sa->verticalScrollBar()->value() == sa->verticalScrollBar()->maximum() && delta.ry() < 0){
        vpaw->setFixedHeight(vpaw->height()-delta.ry());
    }else{
        sa->verticalScrollBar()->setValue(
                sa->verticalScrollBar()->value()-delta.ry()
            );
    }
}

void MainWindow::allHide(){
    setVisible(false);
    sendHideAll();
}

void MainWindow::allShow(){
    setVisible(true);
    sendShowAll();
}

void MainWindow::on_actionNew_triggered(){
    int result = QMessageBox::question(this, tr("Labpatch"), tr("Create New?"), QMessageBox::Ok, QMessageBox::Cancel);
    if(result != QMessageBox::Ok)
        return;
    vpaw->deleteAllIconObjects();
}

void MainWindow::on_actionOpen_triggered(){
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Labpatch Files (*.lph)"));
    if(!fileName.isEmpty()){
        QFile f(fileName);
        f.open(QIODevice::ReadOnly);
        vpaw->stateFromXML(f.readAll());
        f.close();
    }
}

void MainWindow::on_actionSave_as_triggered(){
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "", tr("Labpatch Files (*.lph)"), Q_NULLPTR, QFileDialog::DontConfirmOverwrite);
    if(!fileName.isEmpty()){
        if(fileName.mid(fileName.length()-4, 4) != tr(".lph"))
            fileName += ".lph";
        QFileInfo check_file(fileName);
        if(check_file.exists() && check_file.isFile()){
            int result = QMessageBox::question(this, tr("Labpatch"), tr("File Exists. Replace it?"), QMessageBox::Ok, QMessageBox::Cancel);
            if(result != QMessageBox::Ok)
                return;
        }
        QFile f(fileName);
        f.open(QIODevice::WriteOnly|QIODevice::Truncate);
        f.write(vpaw->stateToXML().toLatin1());
        f.close();
    }
}

void MainWindow::on_actionExit_triggered(){
    QTimer::singleShot(250, qApp, SLOT(quit()));
}

void MainWindow::on_actionRun_triggered(){
    ee->setFixedSize(vpaw->size());
    qda->setFixedSize(size());
    qda->show();
    sda->horizontalScrollBar()->setValue(sa->horizontalScrollBar()->value());
    sda->verticalScrollBar()->setValue(sa->verticalScrollBar()->value());
}

void MainWindow::on_actionStop_triggered(){

}

void MainWindow::on_actionAbout_triggered(){
    QMessageBox::information(this, tr("Labpatch"), tr("Labpatch v0.0.4\n\tCreated By Fugu\n")+
                             tr("A basic block diagram framework for the Qt programming environment\n"), QMessageBox::Ok);
}

void MainWindow::on_actionToolbar_triggered(){
    QMessageBox::information(this, tr("Labpatch"), tr("Triggered\n"), QMessageBox::Ok);
}

void MainWindow::on_actionDump_iconMap_triggered(){
    continue_actionDump_iconMap_triggered();
    //QList<QList<QByteArray> > i = {
    //    {QByteArray::number(2), QByteArray::number(4)},
    //    {QByteArray::number(3), QByteArray::number(-1)},
    //    {QByteArray(), QByteArray()},
    //};
    //iconfuncdatabase["plusFunc"](tr("LPH_COMPLEXTYPE"), i);
    //qDebug() << i[2];
}
