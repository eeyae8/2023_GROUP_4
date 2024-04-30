#include "optiondialog.h"
#include "ui_optiondialog.h"

OptionDialog::OptionDialog(ModelPart *modelPart, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::OptionDialog)

{
    ui->setupUi(this);
    part = modelPart;

    //r_char = part->getColourR();
    //g_char = part->getColourG();
    //b_char = part->getColourB();
    unsigned char r_load, g_load, b_load;
    int r_load_int = 221, g_load_int = 221, b_load_int = 221;
    connect(ui->pushButton_RGB, &QPushButton::clicked, this, &OptionDialog::handleRGBButton);
    updateDialogFromModelPart(modelPart);
    connect(this, &QDialog::accepted, this, &OptionDialog::updateModelPartFromDialog/*acceptDialog*/);
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::updateDialogFromModelPart(ModelPart *modelPart)
{
    ui->nameEdit->setText(modelPart->data(0).toString());
    ui->isVisibleCheckbox->setChecked(modelPart->visible());
    r_char = r_load = part->getColourR();
    g_char = g_load = part->getColourG();
    b_char = b_load = part->getColourB();
    r_load_int = static_cast<int>(r_load);
    g_load_int = static_cast<int>(g_load);
    b_load_int = static_cast<int>(b_load);
}

void OptionDialog::updateModelPartFromDialog()
{
    part->set(0, ui->nameEdit->text());
    part->setVisible(ui->isVisibleCheckbox->isChecked());
    part->setColour(r_char, g_char, b_char);
}

void OptionDialog::handleRGBButton() {
    QColor initialColour(r_load_int, g_load_int, b_load_int);

    QColor color = QColorDialog::getColor(initialColour);
    if (color.isValid()){
        int r = color.red();
        int g = color.green();
        int b = color.blue();

        r_char = static_cast<unsigned char>(r);
        g_char = static_cast<unsigned char>(g);
        b_char = static_cast<unsigned char>(b);
    }
}