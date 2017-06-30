#ifndef FLOATINGMENUTOOLS_H
#define FLOATINGMENUTOOLS_H

#include <QFrame>
#include <QDebug>

namespace Ui {
class FloatingMenuTools;
}

class FloatingMenuTools : public QFrame
{
    Q_OBJECT
private slots:
    void on_pushButtonA_clicked();
    void on_pushButtonB_clicked();
    void on_pushButtonC_clicked();
    void on_pushButtonD_clicked();

public:
    explicit FloatingMenuTools(QWidget *parent = 0);
    ~FloatingMenuTools();

signals:
    void setTool(QString s);

private:
    Ui::FloatingMenuTools *ui;
};

#endif // FLOATINGMENUTOOLS_H
