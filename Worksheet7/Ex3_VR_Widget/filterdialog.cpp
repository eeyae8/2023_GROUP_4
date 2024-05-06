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

/*bool FilterDialog::isShrinkFilterChecked() const
{
    return ui->shrinkFilterCheckBox->isChecked();
}

bool FilterDialog::isClipFilterChecked() const
{
    return ui->clipFilterCheckBox->isChecked();
}

bool FilterDialog::isWireframeFilterChecked() const
{
    return ui->wireframeFilterCheckBox->isChecked();
} */

FilterDialog::~FilterDialog()
{
    delete ui;
}

void FilterDialog::updateFilterDialogFromModelPart(ModelPart* modelPart)
{
    ui->shrinkFilterCheckBox->setChecked(modelPart->filterShrink());
    ui->clipFilterCheckBox->setChecked(modelPart->filterClip());
    ui->wireframeFilterCheckBox->setChecked(modelPart->filterWireframe());
}

void FilterDialog::updateModelPartFromFilterDialog()
{
    part->setFilterShrink(ui->shrinkFilterCheckBox->isChecked());
    part->setFilterClip(ui->clipFilterCheckBox->isChecked());
    part->setFilterWireframe(ui->wireframeFilterCheckBox->isChecked());
}