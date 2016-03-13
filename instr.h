// @file instr.h
// @brief contains default class declarations for dialog box
// @author Alice Qin
// @date 2/25/16
//  contains the functionality of the box



#ifndef INSTR_H
#define INSTR_H

#include <QDialog>

namespace Ui {
class Instr;
}

class Instr : public QDialog
{
    Q_OBJECT

public:
    explicit Instr(QWidget *parent = 0);
    ~Instr();

private:
    Ui::Instr *ui;
};

#endif // INSTR_H
