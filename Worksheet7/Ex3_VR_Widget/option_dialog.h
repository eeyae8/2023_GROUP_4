#ifndef OPTION_DIALOG_H
#define OPTION_DIALOG_H

#include <QDialog>

namespace Ui {
class Option_Dialog;
}

class Option_Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Option_Dialog(QWidget *parent = nullptr);
    ~Option_Dialog();
    struct ActionMenuData getMenuData();
    void setMenuData(ActionMenuData data);
    void SliderUpdate();


private:
    Ui::Option_Dialog *ui;
};

struct ActionMenuData{
    QString name;
    bool isVisible;
    unsigned int r;
    unsigned int b;
    unsigned int g;
};

#endif // OPTION_DIALOG_H
