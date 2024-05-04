#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "OptionDialog.h"
#include "TreeModel/ModelPart.h"
#include "TreeModel/ModelPartList.h"
#include "vtkGenericOpenGLRenderWindow.h"
#include <vtkActor.h>
#include <vtkCamera.h>
#include <vtkCylinderSource.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkOpenVRRenderWindow.h>
#include "VRRenderThread.h"
#include <qmutex.h>


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;

};
QT_END_NAMESPACE




class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    ModelPartList* partList;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkGenericOpenGLRenderWindow> renderWindow;
    VRRenderThread *VRrenderer;
    QMutex mutex;


public slots:
    void handleButton1();
    void handleButton2();
    void handleTreeClicked();
    void on_actionOpen_File_triggered();
    void on_actionItem_Options_triggered();
    
    void on_actionClearFilters_triggered();
    void on_actionClearALL_triggered();
    void on_actionShrinkFilter_triggered();
    void on_actionShrinkALL_triggered();
    void on_actionClipFilter_triggered();
    void on_actionClipALL_triggered();
    void on_actionWireframeFilter_triggered();
    void on_actionWireframeALL_triggered();

    void on_actionStart_VR_triggered();
    void on_actionStop_VR_triggered();

    void updateRender();
    void updateRenderFromTree(const QModelIndex& index);
    void updateVRRenderFromTree(const QModelIndex& index);


signals:
    void statusUpdateMessage(const QString& message, int timeout);
};


#endif // MAINWINDOW_H

