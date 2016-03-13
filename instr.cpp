// @name Alice Qin
// @date 02/25/2016
// @file instr.cpp
// @brief This program contains default implementation for instruction box



#include "instr.h"
#include "ui_instr.h"

Instr::Instr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Instr)
{
    ui->setupUi(this);
}

Instr::~Instr()
{
    delete ui;
}
