#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>
#include <QPushButton>
#include <QCheckBox>
#include <QLineEdit>
#include <QSystemTrayIcon>
#include <QTimer>
#include <QFrame>
#include <QLabel>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDrag>
#include <QPainter>
#include <QDebug>
#include <QBitmap>
#include <QCloseEvent>
#include <QVBoxLayout>
#include <QScrollArea>
#include <QSpinBox>
#include <QPair>

#include <float.h>

#include "viewportareawidget.h"
#include "executionengine.h"

class QDragEnterEvent;
class QDropEvent;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
private slots:
    void iconActivated(QSystemTrayIcon::ActivationReason reason);
    void on_actionNew_triggered();
    void on_actionOpen_triggered();
    void on_actionSave_as_triggered();
    void on_actionExit_triggered();
    void on_actionRun_triggered();
    void on_actionStop_triggered();
    void on_actionAbout_triggered();
    void on_actionToolbar_triggered();
    void on_actionDump_iconMap_triggered();
    void allHide();
    void allShow();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
public slots:
    void setPositionOfScrollArea(QPointF delta);

signals:
    void sendShowAll();
    void sendHideAll();
    void resizedMainWindow(QSize sz);
    void continue_actionDump_iconMap_triggered();

private:
    Ui::MainWindow *ui;

    QAction *minimizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;

    QScrollArea *sa;
    ViewPortAreaWidget *vpaw;

    QDialog *qda;
    QScrollArea *sda;
    QScrollBar *qhsbr;
    QScrollBar *qvsbr;
    ExecutionEngine *ee;

protected:
    //bool eventFilter(QObject *obj, QEvent *event) override;
    void resizeEvent(QResizeEvent* event) override;
    void closeEvent(QCloseEvent *event) override;
};

#endif // MAINWINDOW_H
