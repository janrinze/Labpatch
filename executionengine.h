#ifndef EXECUTIONENGINE_H
#define EXECUTIONENGINE_H

#include <QWidget>

class ExecutionEngine : public QWidget
{
    Q_OBJECT

public:
    explicit ExecutionEngine(QWidget *parent = 0);
    virtual ~ExecutionEngine();

};

#endif // EXECUTIONENGINE_H
