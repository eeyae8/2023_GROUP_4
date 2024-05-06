#ifndef MAINWINDOW_H
#define MAINWINDOW_H
/** @file
*This file contains the main window class for the application.

*/
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

#include <vtkLight.h>


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
	/** Handle tree clicked function.
	* This function is called when the user clicks on an item in the tree. It calls the updateRenderFromTree function.
 
    */
    void handleTreeClicked();
/** Open file function.
* This function is called when the user clicks the "Open File" button in the menu bar. It opens a file dialog and loads the selected file.
* It uses ModelPart to add the actor to VR.
*/
    void on_actionOpen_File_triggered();
    /** Opens item options and allows user to change the colour of the object in both GUI and VR. 
    */
    void on_actionItem_Options_triggered();
    
    void on_actionClearFilters_triggered();
    void on_actionClearALL_triggered();
    void on_actionShrinkFilter_triggered();
    void on_actionShrinkALL_triggered();
    void on_actionClipFilter_triggered();
    void on_actionClipALL_triggered();
    void on_actionWireframeFilter_triggered();
    void on_actionWireframeALL_triggered();

    /** Start VR function.
* This function is called when the user clicks the "Start VR" button in the menu bar. It creates a new VRRenderThread object and starts it.
* It calls the VRRenderThread constructor, which initialises the actor list and rotation variables.


*/

    void on_actionStart_VR_triggered();
	/** Stop VR function.
	* This function is called when the user clicks the "Stop VR" button in the menu bar. It stops the VRRenderThread object.
	* It calls the VRRenderThread destructor, which stops the thread and deletes the object.
    */
    void on_actionStop_VR_triggered();

	/** Update VR function.
	*This function updates the VR render window. It calls the updateRender function of the VRRenderThread object.
    
    */
    void updateRender();
	/** Update VR render from tree function.
	*This function updates the VR render window from the tree. It calls the updateRenderFromTree function of the VRRenderThread object.
	* @param index QModelIndex object.
	* Cycles through actors in the tree and updates the VR render window.
    */
    void updateRenderFromTree(const QModelIndex& index);
	/** Update VR render from tree function.
	*This function updates the VR render window from the tree. It calls the updateRenderFromTree function of the VRRenderThread object.
	* @param index QModelIndex object.
	* Cycles through actors in the tree and updates the VR render window.
    */
    void updateVRRenderFromTree(const QModelIndex& index);


signals:
	/** Status update message signal.
	*This signal is emitted when a status update message is required.
    */
    void statusUpdateMessage(const QString& message, int timeout);
};


#endif // MAINWINDOW_H

