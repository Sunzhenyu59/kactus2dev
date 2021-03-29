//-----------------------------------------------------------------------------
// File: PythonAPI.h
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 12.02.2020
//
// Description:
// Interface for accessing Kactus2 data using Python.
//-----------------------------------------------------------------------------

// #pragma once

#include "pythonapi_global.h"

#include <QString>

#include <common/KactusAPI.h>

#include <string>
#include <vector>
#include <QSharedPointer>

class LibraryInterface;
class MessageMediator;

class Component;
class Field;
class Register;
class RegisterBase;
class AddressBlock;
class MemoryMap;

class PortsInterface;
class ParametersInterface;
class MemoryMapInterface;

class ComponentParameterFinder;
class ExpressionParser;
class PortValidator;
class ExpressionFormatter;

class ParameterValidator;
class MemoryMapValidator;


//-----------------------------------------------------------------------------
//! Interface for accessing Kactus2 data using Python.
//-----------------------------------------------------------------------------
class PythonAPI
{
public:
    
    /*!
     *  The constructor.
     */
    PythonAPI();

    /*!
     *  The destructor.
     */
    ~PythonAPI() = default;
   
    /*!
     *  Get Kactus2 version.
     *
     *      @return The current Kactus2 version.
     */
    std::string getVersion() const;

    /*!
     * Gets the paths where IP-XACT files are stored.
     *
     *     @return The paths where IP-XACT files are stored.
     */
    std::vector<std::string> getLibraryPaths() const;

    /*!
     * Set the paths where IP-XACT files are stored.
     *
     *     @param [in] paths  The locations that are currently available to store the files.
     */
    void setLibraryPaths(std::vector<std::string> paths) const;

    /*!
     * Get the default library path for IP-XACT files.
     *
     *     @return The default path for IP-XACT files.
     */
    std::string getDefaultLibraryPath() const;

    /*!
     * Set the default library path for IP-XACT files.
     *
     *     @param [in] path  The default path to set for IP-XACT files.
     */
    void setDefaultLibraryPath(std::string const& path) const;
    
    /*!
     * Import a source file (RTL) into the library as a component.
     *
     *     @param [in] filePath     The path to the file to import.
     *     @param [in] targetVLNV   The VLNV to use for the imported component.
     *     @param [in] overwrite    Overwrite the component in the library, if it already exists.
     *
     *     @return The number of imported IP-XACT components.
     */
    int importFile(std::string const& path, std::string vlnv, bool overwrite = false) const;

    /*!
     * Run a file generation for a component.
     *
     *     @param [in] format           The requested output format.
     *     @param [in] vlnv             The component VLNV to run the generation for.
     *     @param [in] viewName         The component view to run the generation for.
     *     @param [in] outputDirectory  The output directory for the generation results.
     */
     void generate(std::string const& format, std::string const& vlnv, std::string const& viewName,
        std::string const& outputDirectory) const;

    /*!
     *  Get the number of files in the library.
     *
     *      @return The number of files in the library.
     */
    int getFileCount() const;

    /*!
     *  List all the VLNVs in the library.
     *
     *      @param[in] vendor   The desired vendor.
     */
    void listVLNVs(QString const& vendor = QString("")) const;

    /*!
     *  List all the component VLNVs in the library.
     */
    void listComponentVLNVs() const;

    /*!
     *  Set the selected component as active component.
     *
     *      @param [in] componentVLNV   VLNV of the selected component.
     *
     *      @return True, if the component exists, false otherwise.
     */
    bool openComponent(QString const& componentVLNV);

    /*!
     *  Remove the active component.
     */
    void closeOpenComponent();

    /*!
     *  Get the name of the active component.
     */
    QString getComponentName();

    /*!
     *  Get the description of the active component.
     */
    QString getComponentDescription();

    /*!
     *  Save the component to the library.
     */
    void saveComponent();

    /*!
     *  Get the interface for accessing the component ports.
     *
     *      @return Interface for accessing the component ports.
     */
    PortsInterface* getPortsInterface() const;

    /*!
     *  Get the interface for accessing the component parameters.
     *
     *      @return Interface for accessing the component parameters.
     */
    ParametersInterface* getComponentParameterInterface() const;

    /*!
     *  Get the interface for accessing component memory maps.
     *
     *      @return Interface for accessing component memory maps.
     */
    MemoryMapInterface* getMapInterface();

    /*!
     *  Set the available address blocks for the address block interface.
     *
     *      @param [in] mapName     Name of the memory map containing the desired address blocks.
     */
    void setBlocksForInterface(std::string const& mapName);

    /*!
     *  Set the available registers for the register interface.
     *
     *      @param [in] mapName     Name of the memory map containing the desired address blocks.
     *      @param [in] blockName   Name of the address block containing the desired registers.
     */
    void setRegistersForInterface(std::string const& mapName, std::string const& blockName);

    /*!
     *  Set the available fields for the field interface.
     *
     *      @param [in] mapName         Name of the memory map containing the desired address blocks.
     *      @param [in] blockName       Name of the address block containing the desired registers.
     *      @param [in] registerName    Name of the register containing the desired fields.
     */
    void setFieldsForInterface(std::string const& mapName, std::string const& blockName,
        std::string const& registerName);

    /*!
     *  Set the available resets for the reset interface.
     *
     *      @param [in] mapName         Name of the memory map containing the desired address blocks.
     *      @param [in] blockName       Name of the address block containing the desired registers.
     *      @param [in] registerName    Name of the register containing the desired fields.
     *      @param [in] fieldName       Name of the field containing the desired resets.
     */
    void setResetsForInterface(std::string const& mapName, std::string const& blockName,
        std::string const& registerName, std::string const& fieldName);

private:

    /*!
     *  Construct validator for memory items.
     */
    void constructMemoryValidators();

    /*!
     *  Construct interfaces for memory items.
     */
    void constructMemoryInterface();

    /*!
     *  Get the selected memory map.
     *
     *      @param [in] mapName     Name of the selected memory map.
     *
     *      @return The selected memory map.
     */
    QSharedPointer<MemoryMap> getMemoryMap(QString const& mapName) const;

    /*!
     *  Get the selected address block.
     *
     *      @param [in] containingMap   Memory map containing the address block.
     *      @param [in] blockName       Name of the selected address block.
     *
     *      @return The selected address block.
     */
    QSharedPointer<AddressBlock> getAddressBock(QSharedPointer<MemoryMap> containingMap, QString const& blockName)
        const;

    /*!
     *  Get the selected register.
     *
     *      @param [in] containingBlock     Address block containing the registers.
     *      @param [in] registerName        Name of the selected register.
     *
     *      @return The selected register.
     */
    QSharedPointer<Register> getRegister(QSharedPointer<AddressBlock> containingBlock, QString const& registerName)
        const;

    /*!
     *  Get the selected field.
     *
     *      @param [in] containingRegister  Register containing the fields.
     *      @param [in] fieldName           Name of the selected field.
     *
     *      @return The selected field.
     */
    QSharedPointer<Field> getField(QSharedPointer<Register> containingRegister, QString const& fieldName) const;

    /*!
     *  Send an error text for a non-existing memory map.
     *
     *      @param [in] mapName     Name of the memory map.
     */
    void sendMemoryMapNotFoundError(QString const& mapName) const;

    /*!
     *  Send an error text for a non-existing address block.
     *
     *      @param [in] mapName     Name of the memory map containing the address block.
     *      @param [in] blockName   Name of the address block.
     */
    void sendAddressBlockNotFoundError(QString const& mapName, QString const& blockName) const;

    /*!
     *  Send an error text for a non-existing register.
     *
     *      @param [in] mapName         Name of the memory map containing the address block.
     *      @param [in] blockName       Name of the address block containing the register.
     *      @param [in] registerName    Name of the register.
     */
    void sendRegisterNotFoundError(QString const& mapName, QString const& blockName, QString const& registerName)
        const;

    /*!
     *  Send an error text for a non-existing field.
     *
     *      @param [in] mapName         Name of the memory map containing the address block.
     *      @param [in] blockName       Name of the address block containing the register.
     *      @param [in] registerName    Name of the register containing the field.
     *      @param [in] fieldName       Name of the field.
     */
    void sendFieldNotFoundError(QString const& mapName, QString const& blockName, QString const& registerName,
        QString const& fieldName) const;

    //-----------------------------------------------------------------------------
    // Data.
    //-----------------------------------------------------------------------------

    //! Interface to the library.
    LibraryInterface* library_;

    //! Message handler.
    MessageMediator* messager_;

    //! Currently active component.
    QSharedPointer<Component> activeComponent_;

    //! Interface for accessing the component ports.
    PortsInterface* portsInterface_;

    //! Interface for accessing the component parameters.
    ParametersInterface* componentParameterInterface_;

    //! Interface for accessing memory maps.
    MemoryMapInterface* mapInterface_;

    //! Component parameter finder.
    QSharedPointer<ComponentParameterFinder> parameterFinder_;

    //! Parser for expressions.
    QSharedPointer<ExpressionParser> expressionParser_;

    //! Formatter for expressions.
    QSharedPointer<ExpressionFormatter>expressionFormatter_;

    //! Validator for ports.
    QSharedPointer<PortValidator> portValidator_;

    //! Validator for parameters.
    QSharedPointer<ParameterValidator> parameterValidator_;

    //! Validator for memory maps.
    QSharedPointer<MemoryMapValidator> mapValidator_;
};
