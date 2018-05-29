//-----------------------------------------------------------------------------
// File: LinuxDeviceTreePlugin.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Copyright: Tampere University of Technology
// Author: Mikko Teuho
// Date: 08.05.2018
//
// Description:
// Generator plugin for Linux Device Tree.
//-----------------------------------------------------------------------------

#include "LinuxDeviceTreePlugin.h"

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/FileSet.h>
#include <IPXACTmodels/Design/Design.h>

#include <Plugins/PluginSystem/IPluginUtility.h>
#include <Plugins/LinuxDeviceTree/LinuxDeviceTreeDialog.h>
#include <Plugins/LinuxDeviceTree/LinuxDeviceTreeGenerator.h>

#include <library/LibraryInterface.h>

#include <QCoreApplication>
#include <QFileInfo>
#include <QMessageBox>

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::LinuxDeviceTreePlugin()
//-----------------------------------------------------------------------------
LinuxDeviceTreePlugin::LinuxDeviceTreePlugin()
{

}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::~LinuxDeviceTreePlugin()
//-----------------------------------------------------------------------------
LinuxDeviceTreePlugin::~LinuxDeviceTreePlugin()
{

}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getName()
//----------------------------------------------------------------------------
QString LinuxDeviceTreePlugin::getName() const
{
    return tr("Linux Device Tree Generator");
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getVersion()
//-----------------------------------------------------------------------------
QString LinuxDeviceTreePlugin::getVersion() const
{
    return "0.1";
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getDescription()
//-----------------------------------------------------------------------------
QString LinuxDeviceTreePlugin::getDescription() const
{
    return QString("Creates a Linux Device Tree from the selected document");
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getVendor()
//-----------------------------------------------------------------------------
QString LinuxDeviceTreePlugin::getVendor() const
{
    return tr("TUT");
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getLicence()
//-----------------------------------------------------------------------------
QString LinuxDeviceTreePlugin::getLicence() const
{
    return tr("GPL2");
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getLicenceHolder()
//-----------------------------------------------------------------------------
QString LinuxDeviceTreePlugin::getLicenceHolder() const
{
    return tr("Public");
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getSettingsWidget()
//-----------------------------------------------------------------------------
QWidget* LinuxDeviceTreePlugin::getSettingsWidget()
{
    return new QWidget();
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getSettingsModel()
//-----------------------------------------------------------------------------
PluginSettingsModel* LinuxDeviceTreePlugin::getSettingsModel()
{
    return NULL;
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getIcon()
//-----------------------------------------------------------------------------
QIcon LinuxDeviceTreePlugin::getIcon() const
{
    return QIcon(":icons/LinuxDeviceTreeGenerator.png");
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::checkGeneratorSupport()
//-----------------------------------------------------------------------------
bool LinuxDeviceTreePlugin::checkGeneratorSupport(QSharedPointer<Component const> component,
    QSharedPointer<Design const> design, QSharedPointer<DesignConfiguration const> designConfiguration) const
{
    return (design != 0 && designConfiguration != 0);
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::runGenerator()
//-----------------------------------------------------------------------------
void LinuxDeviceTreePlugin::runGenerator(IPluginUtility* utility, QSharedPointer<Component> component,
    QSharedPointer<Design> design, QSharedPointer<DesignConfiguration> designConfiguration)
{
    utility_ = utility;
    utility->printInfo("Running Linux Device Tree Generator " + getVersion());

    QFileInfo targetInfo(utility_->getLibraryInterface()->getPath(component->getVlnv()));

    QString suggestedPath = targetInfo.absolutePath();
    suggestedPath.append(QLatin1Char('/'));
    suggestedPath.append(component->getVlnv().getName());
    suggestedPath.append(QStringLiteral(".dts"));

    LinuxDeviceTreeDialog dialog(suggestedPath, component, design->getVlnv(), utility_->getParentWidget());

    if (dialog.exec() == QDialog::Accepted)
    {
        QString path = dialog.getOutputPath();
        QString activeView = dialog.getSelectedView();

        LinuxDeviceTreeGenerator generator(utility_->getLibraryInterface());
        generator.generate(component, activeView, path);

        utility_->printInfo("Generation successful.");

        if (dialog.saveFileToFileSet())
        {
            QString relativePath = General::getRelativePath(targetInfo.absoluteFilePath(), suggestedPath);
            QString targetFileSet = dialog.getTargetFileSet();

            saveFileToFileSet(component, targetFileSet, relativePath);

            utility->getLibraryInterface()->writeModelToFile(component);
            utility->printInfo("File " + relativePath + " stored in fileset " + targetFileSet +
                " within component " + component->getVlnv().toString() + ".");
        }
    }
    else
    {
        utility->printInfo("Generation aborted.");
    }
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getProgramRequirements()
//-----------------------------------------------------------------------------
QList<IPlugin::ExternalProgramRequirement> LinuxDeviceTreePlugin::getProgramRequirements()
{
    return QList<IPlugin::ExternalProgramRequirement>();
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin:::onErrorReport()
//-----------------------------------------------------------------------------
void LinuxDeviceTreePlugin::onErrorReport(const QString& report)
{
    utility_->printError(report);
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::saveFileToFileSet()
//-----------------------------------------------------------------------------
void LinuxDeviceTreePlugin::saveFileToFileSet(QSharedPointer<Component> component, QString const& fileSetName,
    QString const& filePath)
{
    QSharedPointer<FileSet> targetFileSet = getFileSet(component, fileSetName);
    if (targetFileSet)
    {
        QSettings settings;

        QSharedPointer<File> deviceTreeFile = targetFileSet->addFile(filePath, settings);
        deviceTreeFile->addFileType(QString("linuxDeviceTree"));
    }
}

//-----------------------------------------------------------------------------
// Function: LinuxDeviceTreePlugin::getFileSet()
//-----------------------------------------------------------------------------
QSharedPointer<FileSet> LinuxDeviceTreePlugin::getFileSet(QSharedPointer<Component> component,
    QString const& fileSetName)
{
    foreach(QSharedPointer<FileSet> fileSet, *component->getFileSets())
    {
        if (fileSet->name().compare(fileSetName) == 0)
        {
            return fileSet;
        }
    }

    QSharedPointer<FileSet> newFileSet(new FileSet(fileSetName, QStringLiteral("generatedFiles")));
    component->getFileSets()->append(newFileSet);

    return newFileSet;
}
