#include "option_dialog.h"
#include "ui_option_dialog.h"
#include "QSlider"

Option_Dialog::Option_Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Option_Dialog)
{
    ui->setupUi(this);
    connect(ui->horizontalSlider, &QSlider::sliderMoved, this , &Option_Dialog::SliderUpdate);
    connect(ui->horizontalSlider_2, &QSlider::sliderMoved, this , &Option_Dialog::SliderUpdate);
    connect(ui->horizontalSlider_3, &QSlider::sliderMoved, this , &Option_Dialog::SliderUpdate);
    connect(ui->horizontalSlider, &QSlider::sliderReleased, this , &Option_Dialog::SliderUpdate);
    connect(ui->horizontalSlider_2, &QSlider::sliderReleased, this , &Option_Dialog::SliderUpdate);
    connect(ui->horizontalSlider_3, &QSlider::sliderReleased, this , &Option_Dialog::SliderUpdate);
}

Option_Dialog::~Option_Dialog()
{
    delete ui;
}

struct ActionMenuData Option_Dialog::getMenuData()
{
    struct ActionMenuData menuData;
    menuData.r = ui->horizontalSlider->value();
    menuData.g = ui->horizontalSlider_2->value();
    menuData.b = ui->horizontalSlider_3->value();
    menuData.isVisible = ui->checkBox->isChecked();
    menuData.name = ui->plainTextEdit->toPlainText();

    return menuData;
}

void Option_Dialog::setMenuData(ActionMenuData data)
{
    ui->horizontalSlider->setValue(data.r);
    ui->horizontalSlider_2->setValue(data.g);
    ui->horizontalSlider_3->setValue(data.b);
    ui->plainTextEdit->setPlainText(data.name);
    ui->checkBox->setChecked(data.isVisible);
    ui->label_3->setText(QString("R: %1, G: %2, B: %3").arg(data.r).arg(data.g).arg(data.b));

}

void Option_Dialog::SliderUpdate()
{
    struct ActionMenuData colorData = getMenuData();
    ui->label_3->setText(QString("R: %1, G: %2, B: %3").arg(colorData.r).arg(colorData.g).arg(colorData.b));
}

