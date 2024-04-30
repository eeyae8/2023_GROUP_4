#ifndef OPTIONDIALOG_H
#define OPTIONDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QColorDialog>
#include "TreeModel/ModelPart.h"

namespace Ui {
class OptionDialog;
}

class OptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionDialog(ModelPart *modelPart, QWidget *parent = nullptr);
    ~OptionDialog();

    void updateDialogFromModelPart(ModelPart *modelPart);


private slots:
    //void acceptDialog();
    void updateModelPartFromDialog();
    void handleRGBButton();

private:
    Ui::OptionDialog *ui;
    ModelPart *modelPart;
    unsigned char r_char, g_char, b_char;
    unsigned char r_load, g_load, b_load;
    int r_load_int, g_load_int, b_load_int;
    ModelPart* part;
};

#endif // OPTIONDIALOG_H
