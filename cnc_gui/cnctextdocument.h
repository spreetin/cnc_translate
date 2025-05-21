#ifndef CNCTEXTDOCUMENT_H
#define CNCTEXTDOCUMENT_H

#include <QPlainTextEdit>
#include <QWidget>
#include "../brands/parameters.h"

class CNCTextDocument : public QPlainTextEdit
{
public:
    CNCTextDocument(QWidget * parent = nullptr);

    void setParameters(NCParser::MachineParameters &parameters){
        param = parameters;
    }

protected:
    bool event(QEvent *e) override;

    NCParser::MachineParameters param;
};

#endif // CNCTEXTDOCUMENT_H
