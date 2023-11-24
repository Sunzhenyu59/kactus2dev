//-----------------------------------------------------------------------------
// File: WirePortsEditorConstructor.cpp
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 29.04.2019
//
// Description:
// Constructs wire ports editor items.
//-----------------------------------------------------------------------------

#include "WirePortsEditorConstructor.h"

#include <editors/ComponentEditor/ports/PortsView.h>
#include <editors/ComponentEditor/ports/WirePortsModel.h>
#include <editors/ComponentEditor/ports/WirePortColumns.h>
#include <editors/ComponentEditor/ports/WirePortsFilter.h>
#include <editors/ComponentEditor/ports/WirePortsDelegate.h>

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/validators/PortValidator.h>

#include <QCompleter>
#include <QVBoxLayout>

//-----------------------------------------------------------------------------
// Function: WirePortsEditorConstructor::constructModel()
//-----------------------------------------------------------------------------
PortsModel* WirePortsEditorConstructor::constructModel(QObject* parent) const
{
    return new WirePortsModel(parameterFinder_, portsInterface_, signalInterface_, parent);
}

//-----------------------------------------------------------------------------
// Function: WirePortsEditorConstructor::constructFilter()
//-----------------------------------------------------------------------------
PortsFilter* WirePortsEditorConstructor::createFilter(QObject* parent) const
{
    return new WirePortsFilter(portsInterface_, parent);
}

//-----------------------------------------------------------------------------
// Function: WirePortsEditorConstructor::constructView()
//-----------------------------------------------------------------------------
PortsView* WirePortsEditorConstructor::createView(QWidget* parent) const
{
    auto view = new PortsView(WirePortColumns::NAME, busInterface_, parent);

    view->setDefaultImportExportPath(defaultPath_);
    view->setAllowImportExport(true);
    view->setAlternatingRowColors(false);
    view->setSortingEnabled(true);
    view->setItemsDraggable(false);
    view->setFitColumnsToContent(false);

    return view;
}

//-----------------------------------------------------------------------------
// Function: WirePortsEditorConstructor::constructDelegate()
//-----------------------------------------------------------------------------
PortsDelegate* WirePortsEditorConstructor::constructDelegate(QObject* parent) const
{
    return new WirePortsDelegate(component_, completionModel_, parameterFinder_, portValidator_->getTypeValidator(), 
        parent);
}
