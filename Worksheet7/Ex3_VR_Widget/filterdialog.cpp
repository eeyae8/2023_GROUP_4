#include "filterdialog.h"
#include "ui_filterdialog.h"

FilterDialog::FilterDialog(ModelPart* modelPart, QWidget* parent)
    : QDialog(parent)
    , ui(new Ui::FilterDialog)
{
    ui->setupUi(this);
    part = modelPart;

    updateFilterDialogFromModelPart(modelPart);
    connect(this, &QDialog::accepted, this, &FilterDialog::updateModelPartFromFilterDialog);
}

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::updateFilterDialogFromModelPart(ModelPart* modelPart)
{
    ui->clipFilterCheckBox->setChecked(modelPart->filterClip());
    ui->shrinkFilterCheckBox->setChecked(modelPart->filterShrink());
    ui->wireframeFilterCheckBox->setChecked(modelPart->filterWireframe());
}

void FilterDialog::updateModelPartFromFilterDialog()
{
    part->setFilterClip(ui->clipFilterCheckBox->isChecked());
    part->setFilterShrink(ui->shrinkFilterCheckBox->isChecked());
    part->setFilterWireframe(ui->wireframeFilterCheckBox->isChecked());
}