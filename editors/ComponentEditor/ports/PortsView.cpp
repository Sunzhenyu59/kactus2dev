//-----------------------------------------------------------------------------
// File: PortsView.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Esko Pekkarinen
// Date: 2.12.2013
//
// Description:
// View for port editor.
//-----------------------------------------------------------------------------

#include "PortsView.h"

#include <QMenu>
#include <QHeaderView>

//-----------------------------------------------------------------------------
// Function: PortsView::PortsView()
//-----------------------------------------------------------------------------
PortsView::PortsView(int const& nameColumn, QWidget *parent):
EditableTableView(parent),
createBus_(tr("Create new bus definition"), this),
createExistingBus_(tr("Use existing bus definition"), this),
nameColumn_(nameColumn)
{
    verticalHeader()->show();
    verticalHeader()->setMaximumWidth(300);
    verticalHeader()->setMinimumWidth(horizontalHeader()->fontMetrics().width(tr("Name"))*2);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    connect(&createBus_, SIGNAL(triggered()), this, SLOT(onCreateNewBus()), Qt::UniqueConnection);

    connect(&createExistingBus_, SIGNAL(triggered()), this, SLOT(onCreateExistingBus()), Qt::UniqueConnection);
}

//-----------------------------------------------------------------------------
// Function: PortsView::~PortsView()
//-----------------------------------------------------------------------------
PortsView::~PortsView() 
{
}

//-----------------------------------------------------------------------------
// Function: PortsView::onCreateBus()
//-----------------------------------------------------------------------------
void PortsView::onCreateNewBus()
{
    QStringList selectedPorts;
    foreach(QModelIndex const& portIndex, selectedIndexes())
    {   
        QString portName = portIndex.sibling(portIndex.row(), nameColumn_).data().toString();
        if (!selectedPorts.contains(portName))
        {
            selectedPorts.append(portName);
        }
    }

    emit createBus(selectedPorts);
}

//-----------------------------------------------------------------------------
// Function: PortsView::onCreateExistingBus()
//-----------------------------------------------------------------------------
void PortsView::onCreateExistingBus()
{
    QStringList selectedPorts;
    foreach(QModelIndex const& portIndex, selectedIndexes())
    {   
        QString portName = portIndex.sibling(portIndex.row(), nameColumn_).data().toString();
        if (!selectedPorts.contains(portName))
        {
            selectedPorts.append(portName);
        }
    }

    emit createExistingBus(selectedPorts);
}

//-----------------------------------------------------------------------------
// Function: PortsView::contextMenuEvent()
//-----------------------------------------------------------------------------
void PortsView::contextMenuEvent(QContextMenuEvent* event)
{
    pressedPoint_ = event->pos();

    QModelIndex index = indexAt(pressedPoint_);

    QMenu menu(this);
    menu.addAction(&addAction_);

    // if at least one valid item is selected
    if (index.isValid())
    {
        menu.addAction(&removeAction_);
        menu.addAction(&clearAction_);
        menu.addAction(&cutAction_);
        menu.addAction(&copyAction_);
        menu.addAction(&pasteAction_);
    }
    
    if (index.isValid())
    {
        menu.addSeparator();
        QMenu* createMenu = menu.addMenu(tr("New bus interface"));
        createMenu->addAction(&createBus_);    
        createMenu->addAction(&createExistingBus_);           
    }

    if (importExportAllowed())
    {
        menu.addSeparator();
        menu.addAction(&importAction_);
        menu.addAction(&exportAction_);
    }

    menu.exec(event->globalPos());

    event->accept();
}

//-----------------------------------------------------------------------------
// Function: PortsView::mousePressEvent()
//-----------------------------------------------------------------------------
void PortsView::mousePressEvent(QMouseEvent* event)
{
    EditableTableView::mousePressEvent(event);

    QModelIndex currentIndex = indexAt(pressedPoint_);
    if (!currentIndex.isValid())
    {
        emit(changeExtensionsEditorItem(currentIndex));
    }
}

//-----------------------------------------------------------------------------
// Function: PortsView::currentChanged()
//-----------------------------------------------------------------------------
void PortsView::currentChanged(const QModelIndex &current, const QModelIndex &previous)
{
    emit(changeExtensionsEditorItem(current));

    EditableTableView::currentChanged(current, previous);
}
