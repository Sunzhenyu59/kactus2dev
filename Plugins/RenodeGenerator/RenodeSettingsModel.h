//-----------------------------------------------------------------------------
// File: RenodeSettingsModel.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 05.06.2023
//
// Description:
// Interface for renode generator settings model.
//-----------------------------------------------------------------------------

#ifndef RENODESETTINGSMODEL_H
#define RENODESETTINGSMODEL_H

#include <KactusAPI/include/PluginSettingsModel.h>

#include <QSettings>

//-----------------------------------------------------------------------------
//! Interface for renode generator settings model.
//-----------------------------------------------------------------------------
class RenodeSettingsModel : public PluginSettingsModel
{
public:

    /*!
     *  Constructor.
     */
    RenodeSettingsModel();

    /*!
     *  Destructor.
     */
    virtual ~RenodeSettingsModel() = default;
    
    /*!
     *  Loads plugin settings from the given settings store. Is called when Kactus2 loads the plugin.
     *
     *      @param [in] settings The settings store.
     */
    virtual void loadSettings(QSettings& settings) override final;

    /*!
     *  Saves plugin settings to the given settings store. Is called when Kactus2 applies changes to settings.
     *
     *      @param [in,out] settings The settings store.
     */
    virtual void saveSettings(QSettings& settings) override final;

    /*!
     *  Validates the settings of the plugin.
     */
    virtual bool validateSettings() const override final;

    /*!
     *  Get the path to the current peripheral template configuration.
     *	
     *      @return Path to the current peripheral template configuration.
     */
    QString getFolderPath() const;

    /*!
     *  Set a new path for peripheral template configuration file.
     *	
     *      @param [in] newFolderPath   The new path to peripheral template configuration file.
     */
    void setFolderPath(QString const& newFolderPath);

private:

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Path to the peripheral template configuration file.
    QString templateFolderPath_ = QString("");
};

//-----------------------------------------------------------------------------

#endif // RENODESETTINGSMODEL_H
