#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include "OptionDialog.h"
#include "VRRenderThread.h"


MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->pushButton_1, &QPushButton::released, this, &MainWindow::handleButton1);
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
    ui->vtkWidget->setRenderWindow(renderWindow);

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
    cylinderActor->GetProperty()->SetColor(1., 0., 0.);
    cylinderActor->RotateX(30.0);
    cylinderActor->RotateY(-45.0);

    renderer->AddActor(cylinderActor);

    // Apply Lighting code here? (SW new 5)

    renderer->ResetCamera();
    renderer->GetActiveCamera()->Azimuth(30);
    renderer->GetActiveCamera()->Elevation(30);
    renderer->ResetCameraClippingRange();

    VRrenderer = new VRRenderThread();
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

    emit statusUpdateMessage(QString("The selected item is: ") + text, 0);

}

void MainWindow::on_actionOpen_File_triggered() {

    /*QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));

    emit statusUpdateMessage(fileName, 0);*/

    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("Open Files"), "C:\\", tr("STL Files(*.stl);;Text Files(*.txt)"));

    emit statusUpdateMessage(QString("Open File action triggered"), 0);

    if (fileNames.isEmpty())
        return; // User canceled the dialog or no file selected

    // Get the index of the selected item
    QModelIndex index = ui->treeView->currentIndex();

    // Get a pointer to the item from the index
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // To revert back to full filepath, change all filePath to fileName
    // and remove the 2 lines below
    for (const QString& filePath : fileNames) {
        QFileInfo fileInfo(filePath); //delete to revert
        QString fileName = fileInfo.fileName(); //delete to revert

        // Create a new ModelPart item with the filename as its name
        QModelIndex childPart = partList->appendChild(index, { fileInfo.fileName(), QString("true") });

        // Add the new part as a child of the selected part
        ModelPart* newPart = static_cast<ModelPart*>(childPart.internalPointer());

        // Call the loadSTL() function of the newly created item
        newPart->loadSTL(filePath); // change to fileName to revert


        VRrenderer->addActorToVR(newPart->getVRActor());
        // Update the name property of the selected item
     //selectedPart->set(0, QVariant(fileName));

        emit statusUpdateMessage(fileName, 0);
    }

    updateRender();
    renderer->ResetCamera();


}


void MainWindow::on_actionStart_VR_triggered() {
    updateVRRenderFromTree(partList->index(0, 0, QModelIndex()));
    VRrenderer->start();
}

void MainWindow::on_actionStop_VR_triggered() {
    emit statusUpdateMessage(QString("Stopping VR"), 0);
    VRrenderer->issueCommand(VRRenderThread::END_RENDER, 0.);
}

void MainWindow::on_actionItem_Options_triggered() {
    /* get selected item, update dialog UI based on selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    // Pass the selected part to the OptionDialog
    OptionDialog dialog(selectedPart, this);

    if (dialog.exec() == QDialog::Accepted) {
        /*update selected item with dialog UI info*/
       // dialog.updateModelPartFromDialog(selectedPart);
        emit statusUpdateMessage(QString("Dialog accepted "), 0);

    }
    else {
        emit statusUpdateMessage(QString("Dialog rejected "), 0);
    }

    updateRender();

}

/* Filters Start Here */

void MainWindow::on_actionClearFilters_triggered() {
  
    /* get selected item, update dialog UI based on selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    selectedPart->ClearFilters();

    // Update the render
    updateRender();
}

void MainWindow::on_actionClearALL_triggered()
{
    /* get the model from the tree view */
    QAbstractItemModel* model = ui->treeView->model();

    /* get the root item */
    QModelIndex rootIndex = model->index(0, 0);

    /* loop through all child items */
    for (int i = 0; i < model->rowCount(rootIndex); i++) {
        QModelIndex childIndex = model->index(i, 0, rootIndex);

        /* Get a pointer to the item from the index */
        ModelPart* selectedPart = static_cast<ModelPart*>(childIndex.internalPointer());

        selectedPart->ClearFilters();
    }

    // Update the render
    updateRender();
}

void MainWindow::on_actionShrinkFilter_triggered() {

/* get selected item, update dialog UI based on selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    selectedPart->applyShrinkFilter();

    // Update the render
    updateRender();
}

void MainWindow::on_actionShrinkALL_triggered()
{
    /* get the model from the tree view */
    QAbstractItemModel* model = ui->treeView->model();

    /* get the root item */
    QModelIndex rootIndex = model->index(0, 0);

    /* loop through all child items */
    for (int i = 0; i < model->rowCount(rootIndex); i++) {
        QModelIndex childIndex = model->index(i, 0, rootIndex);

        /* Get a pointer to the item from the index */
        ModelPart* selectedPart = static_cast<ModelPart*>(childIndex.internalPointer());

        selectedPart->applyShrinkFilter();
    }

    // Update the render
    updateRender();
}


void MainWindow::on_actionClipFilter_triggered() {
    // Iterate over all items
/*    for (int i = 0; i < partList->rowCount(QModelIndex()); ++i) {
        QModelIndex index = partList->index(i, 0, QModelIndex());
        ModelPart* part = static_cast<ModelPart*>(index.internalPointer());

        // Apply the shrink filter to the part
        part->applyShrinkFilter();
    }
*/
/* get selected item, update dialog UI based on selected item*/
    QModelIndex index = ui->treeView->currentIndex();

    /* Get a pointer to the item from the index */
    ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());

    selectedPart->applyClipFilter();


    // Update the render
    updateRender();
}

void MainWindow::on_actionClipALL_triggered()
{
    /* get the model from the tree view */
    QAbstractItemModel* model = ui->treeView->model();

    /* get the root item */
    QModelIndex rootIndex = model->index(0, 0);

    /* loop through all child items */
    for (int i = 0; i < model->rowCount(rootIndex); i++) {
        QModelIndex childIndex = model->index(i, 0, rootIndex);

        /* Get a pointer to the item from the index */
        ModelPart* selectedPart = static_cast<ModelPart*>(childIndex.internalPointer());

        selectedPart->applyClipFilter();
    }

    // Update the render
    updateRender();
}

/* Filters End Here*/

void MainWindow::updateRender() {
    renderer->RemoveAllViewProps();
    
    //updateRenderFromTree(partList->index(0, 0, QModelIndex()));
    
    int parentCount = partList->rowCount(QModelIndex());
    for (int i = 0; i < parentCount; i++) {
        updateRenderFromTree(partList->index(i, 0, QModelIndex()));
    }

    renderer->Render();
}

void MainWindow::updateRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        vtkSmartPointer<vtkActor> actor = selectedPart->getActor();
        if (actor != nullptr && selectedPart->visible())
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

void MainWindow::updateVRRenderFromTree(const QModelIndex& index) {
    if (index.isValid()) {
        ModelPart* selectedPart = static_cast<ModelPart*>(index.internalPointer());
        vtkSmartPointer<vtkActor> actor = selectedPart->getVRActor();
        if (actor != nullptr && selectedPart->visible())
        {
            if (actor != nullptr)
            {
                actor->GetProperty()->SetColor((double)selectedPart->getColourR() / 255., (double)selectedPart->getColourG() / 255., (double)selectedPart->getColourB() / 255.);
            }

            VRrenderer->addActorOffline(actor);

        }

    }
    if (!partList->hasChildren(index) || (index.flags() & Qt::ItemNeverHasChildren)) {
        return;
    }
    int rows = partList->rowCount(index);
    for (int i = 0; i < rows; i++) {
        updateVRRenderFromTree(partList->index(i, 0, index));
    }
}
