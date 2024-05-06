#ifndef FILTERDIALOG_H
#define FILTERDIALOG_H

#include <QDialog>
#include <QCheckBox>
#include "TreeModel/ModelPart.h"

namespace Ui {
class FilterDialog;
}

class FilterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FilterDialog(ModelPart* modelPart, QWidget* parent = nullptr);
/*    bool isShrinkFilterChecked() const;
    bool isClipFilterChecked() const;
    bool isWireframeFilterChecked() const; */
    ~FilterDialog();

    void updateFilterDialogFromModelPart(ModelPart* modelPart);

private slots:
    void updateModelPartFromFilterDialog();

private:
    Ui::FilterDialog *ui;
    QCheckBox *shrinkFilterCheckBox;
    QCheckBox *clipFilterCheckBox;
    QCheckBox *wireframeFilterCheckBox;
    ModelPart* part;
};

#endif // FILTERDIALOG_H
