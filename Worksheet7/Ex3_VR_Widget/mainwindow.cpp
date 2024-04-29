#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "option_dialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton, &QPushButton::released, this, &MainWindow::handleButton1);
    connect(ui->pushButton_2, &QPushButton::released, this, &MainWindow::handleButton2);
    connect(this, &MainWindow::statusUpdateMessage, ui->statusbar, &QStatusBar::showMessage);
    connect(ui->treeView, &QTreeView::clicked, this, &MainWindow::handleTreeClicked);
    ui->treeView->addAction(ui->actionItem_Options);
    /* Create /allocate the ModelList*/
    this->partList = new ModelPartList("PartsList");

    /* Link it to te treeview in the GUI*/
    ui->treeView->setModel(this->partList);

    /*Manually create a model tree*/
    ModelPart* rootItem = this->partList->getRootItem();
    /*Add 3 top level items*/
    for (int i = 0; i < 3; i++) {
        /*Create strings fro both data columns*/
        QString name = QString("TopLevel %1").arg(i);
        QString visible("true");

            /* Create child item */

            ModelPart* childItem = new ModelPart({ name, visible });



        /* Append to tree top-level */

        rootItem->appendChild(childItem);



        /* Add 5 sub-items */

        /*for (int j = 0; j < 5; j++) {

            QString name = QString("Item %1,%2").arg(i).arg(j);

            QString visible("true");



            ModelPart* childChildItem = new ModelPart({ name, visible });



            /* Append to parent */

            //childItem->appendChild(childChildItem);

        //}

    }
    //Link render window with QT widget
    renderWindow = vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New();
    ui->widget->setRenderWindow(renderWindow);

    //added a renderer
    renderer = vtkSmartPointer<vtkRenderer>::New();
    renderWindow->AddRenderer(renderer);

    //create object and add renderer
      // This creates a polygonal cylinder model with eight circumferential facets
  // (i.e, in practice an octagonal prism).
    vtkNew<vtkCylinderSource> cylinder;
    cylinder->SetResolution(8);

    // The mapper is responsible for pushing the geometry into the graphics
    // library. It may also do color mapping, if scalars or other attributes are
    // defined.
    vtkNew<vtkPolyDataMapper> cylinderMapper;
    cylinderMapper->SetInputConnection(cylinder->GetOutputPort());

    // The actor is a grouping mechanism: besides the geometry (mapper), it
    // also has a property, transformation matrix, and/or texture map.
    // Here we set its color and rotate it around the X and Y axes.
    vtkNew<vtkActor> cylinderActor;
    cylinderActor->SetMapper(cylinderMapper);
    cylinderActor->GetProperty()->SetColor( 1., 0., 0. );
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleButton1() {
    emit statusUpdateMessage(QString("Add button was clicked"), 0);

}

void MainWindow::handleButton2() {
    emit statusUpdateMessage(QString("Minus button was clicked"), 0);


}

void MainWindow::handleTreeClicked() {
    /* Get the index of the selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    /* In this case, we will retrieve the name string from the internal QVariant data array*/
    QString text = selectedPart->data(0).toString();

    emit statusUpdateMessage(QString("The selected item is: ")+text, 0);

}

void MainWindow::on_actionOpen_File_triggered() {
    
    /*QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));
    
    emit statusUpdateMessage(fileName, 0);*/

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));

    emit statusUpdateMessage("Opened file: " + fileName, 0);

    if (fileName.isEmpty())
        return; // User canceled the dialog or no file selected

    // Get the index of the selected item in the tree view

    QFileInfo fileInfo(fileName);

    QModelIndex selectedIndex = ui->treeView->currentIndex();

    //ModelPart* parentPart = selectedIndex.isValid() ? static_cast<ModelPart*>(selectedIndex.internalPointer()) : partList->getRootItem();


    // Create a new item as a child of the currently selected item or the root item if no item is selected
    //ModelPart* newItem = new ModelPart({ fileName, QVariant(true) }, parentPart);

    //parentPart->appendChild(newItem);

    QModelIndex childPart = partList->appendChild(selectedIndex, { fileInfo.fileName(), QString("true") });
    ModelPart* newItem = static_cast<ModelPart*>(childPart.internalPointer());

    // Load the STL file
    newItem->loadSTL(fileName);

    updateRender();
    // Emit status update message
  

    



}

void MainWindow::on_actionItem_Options_triggered() {
   // OptionDialog dialog(this);

    QModelIndex index = ui ->treeView->currentIndex();
   ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

    if(part==nullptr)
    {
       return;
    }

    Option_Dialog dialog(this);

    struct ActionMenuData menuData;

    menuData.r= part->getColourR();
    menuData.g= part->getColourG();
    menuData.b= part->getColourB();

    menuData.name= part->data(0).toString();
    menuData.isVisible= part->data(1).toBool();

    dialog.setMenuData(menuData);

    if (dialog.exec() == QDialog::Accepted){

        struct ActionMenuData colour = dialog.getMenuData();

        part->setColour(colour.r,colour.g,colour.b);
        part->set(0,colour.name);
        part->set(1, QVariant(colour.isVisible).toString());
        part->setVisible(colour.isVisible);
        emit statusUpdateMessage(QString("Dialog accepted"),0);
        updateRender();
    } else {
        emit statusUpdateMessage(QString("Dialog rejected"),0);
    }

}

void MainWindow::updateRender() {
    renderer->RemoveAllViewProps();
    updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    renderer->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor != nullptr && selectedPart -> visible())
        {
            if (actor != nullptr)
            {
                actor->GetProperty()->SetColor((double)selectedPart->getColourR() / 255., (double)selectedPart->getColourG() / 255., (double)selectedPart->getColourB() / 255.);
            }

            renderer->AddActor(actor);
        }

    }
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateRenderFromTree(partList->index(i, 0, index));
    }
}
