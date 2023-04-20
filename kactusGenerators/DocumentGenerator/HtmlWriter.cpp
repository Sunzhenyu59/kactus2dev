#include "HtmlWriter.h"

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/MemoryMap.h>
#include <IPXACTmodels/Component/AddressBlock.h>
#include <IPXACTmodels/Component/Register.h>
#include <IPXACTmodels/Component/Field.h>
#include <IPXACTmodels/Component/BusInterface.h>
#include <IPXACTmodels/Component/FileSet.h>

#include <KactusAPI/include/ExpressionFormatter.h>
#include <KactusAPI/include/LibraryInterface.h>

#include <QDateTime>
#include <QFileInfo>
#include <QSettings>
#include <QStringLiteral>

namespace HTML
{
    const QString SPACE("&nbsp;");

    const QString INDENT("&nbsp;&nbsp;&nbsp;");

    const QString TABLE("<table frame=\"box\" rules=\"all\" border=\"1\" cellPadding=\"3\" title=\"");

    const QString DOCTYPE("<!DOCTYPE html PUBLIC \"-//W3C//DTD HTML 4.01//EN\" "
        "\"http://www.w3.org/TR/html4/strict.dtd\">");

    const QString ENCODING("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\">");

    const QString VALID_W3C_STRICT("\t\t<p>\n"
        "\t\t\t<a href=\"https://validator.w3.org/#validate_by_upload\">\n"
        "\t\t\t<img src=\"http://www.w3.org/Icons/valid-html401\""
        "alt=\"Valid HTML 4.01 Strict\" height=\"31\"""width=\"88\">\n"
        "\t\t\t</a>\n"
        "\t\t</p>");
}

HtmlWriter::HtmlWriter(QSharedPointer<Component> component, ExpressionFormatter* formatter,
    LibraryInterface* libraryHandler) :
    component_(component),
    componentNumber_(0),
    expressionFormatter_(formatter),
    libraryHandler_(libraryHandler),
    DocumentationWriter(formatter)
{
    vlnvString_ = component_->getVlnv().toString();
}

HtmlWriter::~HtmlWriter()
{
}

void HtmlWriter::writeHeader(QTextStream& stream)
{
    QSettings settings;
    
    // write the top of the html document
    stream << HTML::DOCTYPE << Qt::endl;
    stream << "<html>" << Qt::endl;
    stream << "\t<head>" << Qt::endl;
    stream << "\t" << HTML::ENCODING << Qt::endl;
    stream << "\t\t<title>Kactus2 generated documentation for component " <<
        component_->getVlnv().getName() << " " << component_->getVlnv().getVersion() << "</title>" << Qt::endl;
    stream << "\t</head>" << Qt::endl;
    stream << "\t<body>" << Qt::endl;
    stream << "\t\t<h6>This document was generated by Kactus2 on " <<
        QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") << " by user " <<
        settings.value("General/Username").toString() << "</h6>" << Qt::endl;
}

void HtmlWriter::writeKactusAttributes(QTextStream& stream, int subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "Kactus2 attributes", "attributes");

    stream << "\t\t<p>" << Qt::endl;
    stream << "\t\t\t<strong>" << HTML::INDENT << "Product hierarchy: </strong>" <<
        KactusAttribute::hierarchyToString(component_->getHierarchy()) << "<br>" << Qt::endl;

    stream << "\t\t\t<strong>" << HTML::INDENT << "Component implementation: </strong>" <<
        KactusAttribute::implementationToString(component_->getImplementation()) << "<br>" << Qt::endl;

    stream << "\t\t\t<strong>" << HTML::INDENT << "Component firmness: </strong>" <<
        KactusAttribute::firmnessToString(component_->getFirmness()) << "<br>" << Qt::endl;

    stream << "\t\t</p>" << Qt::endl;
}

void HtmlWriter::writeTableOfContents(QTextStream& stream)
{
    QString vlnvHeader = "\t\t" + HTML::INDENT + "<a href=\"#" + component_->getVlnv().toString();

    stream << "\t\t<a href=\"#" << component_->getVlnv().toString() << "\">" << componentNumber_ << ". Component" <<
        HTML::SPACE << component_->getVlnv().toString(" - ") << "</a><br>" << Qt::endl;

    // subHeader is running number that counts the number of sub headers for component
    int subHeader = 1;

    // component always has kactus parameters
    stream << vlnvHeader << ".kts_params\">" << componentNumber_ << "." << subHeader << ". Kactus2 attributes</a><br>" <<
        Qt::endl;
    ++subHeader;

    if (component_->hasParameters())
    {
        stream << vlnvHeader << ".parameters\">" << componentNumber_ << "." << subHeader <<
            ". General parameters</a><br>" << Qt::endl;
        ++subHeader;
    }

    if (!component_->getMemoryMaps()->isEmpty())
    {
        stream << vlnvHeader << ".memoryMaps\">" << componentNumber_ << "." << subHeader << ". Memory maps</a><br>" <<
            Qt::endl;
        ++subHeader;
    }

    if (component_->hasPorts())
    {
        stream << vlnvHeader << ".ports\">" << componentNumber_ << "." << subHeader << ". Ports</a><br>" << Qt::endl;
        ++subHeader;
    }

    if (component_->hasInterfaces())
    {
        stream << vlnvHeader << ".interfaces\">" << componentNumber_ << "." << subHeader << ". Bus interfaces</a><br>" <<
            Qt::endl;
        ++subHeader;
    }

    if (component_->hasFileSets())
    {
        stream << vlnvHeader << ".fileSets\">" << componentNumber_ << "." << subHeader << ". File sets</a><br>" << Qt::endl;
        ++subHeader;
    }

    if (component_->hasViews())
    {
        stream << vlnvHeader << ".views\">" << componentNumber_ << "." << subHeader << ". Views</a><br>" << Qt::endl;
    }
}

void HtmlWriter::writeParameters(QTextStream& stream, int subHeaderNumber)
{
    QStringList headers({
        QStringLiteral("Name"),
        QStringLiteral("Type"),
        QStringLiteral("Value"),
        QStringLiteral("Resolve"),
        QStringLiteral("Bit vector left"),
        QStringLiteral("Bit vector right"),
        QStringLiteral("Array left"),
        QStringLiteral("Array right"),
        QStringLiteral("Description")
    });

    writeSubHeader(stream, subHeaderNumber, "General parameters", "parameters");

    // Write table element
    stream << indent(3) << HTML::TABLE << "List of parameters defined for the component\">" << Qt::endl;

    // Write header row
    writeTableHeader(stream, headers, 4);

    // Write parameters
    for (auto const& parameter : *component_->getParameters())
    {
        QStringList paramFields(QStringList()
            << parameter->name()
            << parameter->getType()
            << expressionFormatter_->formatReferringExpression(parameter->getValue())
            << parameter->getValueResolve()
            << expressionFormatter_->formatReferringExpression(parameter->getVectorLeft())
            << expressionFormatter_->formatReferringExpression(parameter->getVectorRight())
            << expressionFormatter_->formatReferringExpression(parameter->getArrayLeft())
            << expressionFormatter_->formatReferringExpression(parameter->getArrayRight())
            << parameter->description()
        );

        writeTableRow(stream, paramFields, 4);
    }

    stream << indent(3) << "</table>" << Qt::endl;
}

void HtmlWriter::writeMemoryMaps(QTextStream& stream, int subHeaderNumber)
{
    if (component_->getMemoryMaps()->isEmpty())
    {
        return;
    }

    writeSubHeader(stream, subHeaderNumber, "Memory maps", "memoryMaps");

    const QList<QSharedPointer<MemoryMap> > componentMemoryMaps = *component_->getMemoryMaps().data();
    int memoryMapNumber = 1;

    for (auto const& memoryMap : componentMemoryMaps)
    {
        QList subHeaderNumbers({
            componentNumber_,
            subHeaderNumber,
            memoryMapNumber,
        });

        writeSubHeader(stream, subHeaderNumbers, memoryMap->name(), 3);

        stream << indent(3) << "<p>" << Qt::endl;

        if (!memoryMap->description().isEmpty())
        {
            stream << indent(3) << HTML::INDENT << "<strong>Description:</strong> " <<
                memoryMap->description() << "<br>" << Qt::endl;
        }
        
        stream << indent(3) << HTML::INDENT << "<strong>Address unit bits (AUB):</strong> " <<
            memoryMap->getAddressUnitBits() << "<br>" << Qt::endl;

        stream << indent(3) << "</p>" << Qt::endl;

        QList <QSharedPointer <AddressBlock> > addressBlocks = getMemoryMapAddressBlocks(memoryMap);
        writeAddressBlocks(stream, addressBlocks, subHeaderNumber, memoryMapNumber);

        ++memoryMapNumber;
    }
}

void HtmlWriter::writeAddressBlocks(QTextStream& stream, QList<QSharedPointer<AddressBlock>> addressBlocks,
    int subHeaderNumber, int memoryMapNumber)
{
    if (addressBlocks.isEmpty())
    {
        return;
    }

    int addressBlockNumber = 1;

    for (auto const& addressBlock : addressBlocks)
    {        
        QList subHeaderNumbers({
            componentNumber_,
            subHeaderNumber,
            memoryMapNumber,
            addressBlockNumber
        });
        
        writeSubHeader(stream, subHeaderNumbers, addressBlock->name(), 3);

        if (!addressBlock->description().isEmpty())
        {
            stream << indent(3) << "<p>" << Qt::endl;
            stream << indent(3) << HTML::INDENT << "<strong>Description:</strong> " <<
                addressBlock->description() << "<br>" << Qt::endl;
            stream << indent(3) << "</p>" << Qt::endl;
        }

        QStringList headers(QStringList()
            << QStringLiteral("Usage")
            << QStringLiteral("Base address [AUB]")
            << QStringLiteral("Range [AUB]")
            << QStringLiteral("Width [AUB]")
            << QStringLiteral("Access")
            << QStringLiteral("Volatile")
        );

        QStringList addressBlockTableCells(QStringList()
            << General::usage2Str(addressBlock->getUsage())
            << expressionFormatter_->formatReferringExpression(addressBlock->getBaseAddress())
            << expressionFormatter_->formatReferringExpression(addressBlock->getRange())
            << expressionFormatter_->formatReferringExpression(addressBlock->getWidth())
            << AccessTypes::access2Str(addressBlock->getAccess())
            << addressBlock->getVolatile()
        );

        QString title = "List of values in " + addressBlock->name() + ".";

        // Table with title
        stream << indent (3) << HTML::TABLE << title << "\">" << Qt::endl;
        
        // Address block table headers
        writeTableHeader(stream, headers, 4);

        // Address block table body
        writeTableRow(stream, addressBlockTableCells, 4);

        stream << indent(3) << "</table>" << Qt::endl;

        QList <QSharedPointer <Register> > registers = getAddressBlockRegisters(addressBlock);
        writeRegisters(stream, registers, subHeaderNumber, memoryMapNumber, addressBlockNumber);

        ++addressBlockNumber;
    }
}

void HtmlWriter::writeRegisters(QTextStream& stream, QList<QSharedPointer<Register>> registers,
    int subHeaderNumber, int memoryMapNumber, int addressBlockNumber)
{
    if (registers.isEmpty())
    {
        return;
    }

    int registerNumber = 1;

    for (auto const& currentRegister : registers)
    {
        QList subHeaderNumbers({
            componentNumber_,
            subHeaderNumber,
            memoryMapNumber,
            addressBlockNumber,
            registerNumber
        });

        writeSubHeader(stream, subHeaderNumbers, currentRegister->name(), 3);

        if (!currentRegister->description().isEmpty())
        {
            stream << indent(3) << "<p>" << Qt::endl;
            stream << indent(3) << HTML::INDENT << "<strong>Description:</strong> " <<
                currentRegister->description() << "<br>" << Qt::endl;
            stream << indent(3) << "</p>" << Qt::endl;
        }

        QStringList registerTableHeaders(QStringList()
            << QStringLiteral("Offset [AUB]")
            << QStringLiteral("Size [bits]")
            << QStringLiteral("Dimension")
            << QStringLiteral("Volatile")
            << QStringLiteral("Access")
        );

        QString tableTitle = "List of values in " + currentRegister->name() + ".";
        stream << indent(3) << HTML::TABLE << tableTitle << "\">" << Qt::endl;

        writeTableHeader(stream, registerTableHeaders, 4);

        QStringList registerInfoTableCells(QStringList()
            << expressionFormatter_->formatReferringExpression(currentRegister->getAddressOffset())
            << expressionFormatter_->formatReferringExpression(currentRegister->getSize())
            << expressionFormatter_->formatReferringExpression(currentRegister->getDimension())
            << currentRegister->getVolatile()
            << AccessTypes::access2Str(currentRegister->getAccess())
        );
        
        writeTableRow(stream, registerInfoTableCells, 4);
        
        stream << indent(3) << "</table>" << Qt::endl;

        writeFields(stream, currentRegister);

        ++registerNumber;
    }
}

void HtmlWriter::writeFields(QTextStream& stream, QSharedPointer<Register> currentRegister)
{
    if (currentRegister->getFields()->isEmpty())
    {
        return;
    }

    stream << indent(3) << "<h4>Register " << currentRegister->name() << " contains the following fields:</h4>" <<
        Qt::endl;

    QString tableTitle = "List of fields contained within register " + currentRegister->name() + ".";
    stream << indent(3) << HTML::TABLE << tableTitle << "\">" << Qt::endl;

    QStringList fieldTableHeaders(QStringList()
        << QStringLiteral("Field name")
        << QStringLiteral("Offset [bits]")
        << QStringLiteral("Width [bits]")
        << QStringLiteral("Volatile")
        << QStringLiteral("Access")
        << QStringLiteral("Resets")
        << QStringLiteral("Description")
    );

    writeTableHeader(stream, fieldTableHeaders, 4);

    for (auto const& field : *currentRegister->getFields())
    {
        QStringList fieldTableCells(QStringList()
            << "<a id=\"" + vlnvString_ + ".field." + field->name() + "\">" + field->name() + "</a>"
            << expressionFormatter_->formatReferringExpression(field->getBitOffset())
            << expressionFormatter_->formatReferringExpression(field->getBitWidth())
            << field->getVolatile().toString()
            << AccessTypes::access2Str(field->getAccess())
            << getFieldResetInfo(field)
            << field->description()
        );

        writeTableRow(stream, fieldTableCells, 4);
    }

    stream << indent(3) << "</table>" << Qt::endl;
}

void HtmlWriter::writePorts(QTextStream& stream, int subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "Ports", "ports");

    const QList<QSharedPointer<Port> > ports = *component_->getPorts().data();
    QString tableTitle = "List of all ports the component has.";
    
    writePortTable(stream, tableTitle, ports);
}

void HtmlWriter::writeInterfaces(QTextStream& stream, int& subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "Bus interfaces", "interfaces");

    int interfaceNumber = 1;

    for (auto const& interface : *component_->getBusInterfaces())
    {
        QList subHeaderNumbers({
            componentNumber_,
            subHeaderNumber,
            interfaceNumber,
        });

        writeSubHeader(stream, subHeaderNumbers, interface->name(), 3);

        stream << indent(3) << "<p>" << Qt::endl;

        if (!interface->description().isEmpty())
        {
            stream << indent(3) << HTML::INDENT << "<strong>Description:</strong> " <<
                interface->description() << "<br>" << Qt::endl;
        }

        stream << indent(3) << HTML::INDENT << "<strong>Interface mode:</strong> " <<
            General::interfaceMode2Str(interface->getInterfaceMode()) << "<br>" << Qt::endl;

        stream << indent(3) << HTML::INDENT <<
            "<strong>Ports used in this interface:</strong>";

        if (auto const& ports = component_->getPortsMappedInInterface(interface->name()); ports.isEmpty())
        {
            stream << " None" << Qt::endl;
        }
        else
        {
            stream << Qt::endl << indent(3) << "</p>" << Qt::endl;

            QString tableTitle("List of ports contained in interface " + (interface->name()) + ".");
            writePortTable(stream, tableTitle, ports);
        }

        ++interfaceNumber;
    }
}

void HtmlWriter::writeFileSets(QTextStream& stream, int& subHeaderNumber)
{
    writeSubHeader(stream, subHeaderNumber, "File sets", "fileSets");

    const QList<QSharedPointer<FileSet> > fileSets = *component_->getFileSets();

    int fileSetNumber = 1;

    for (auto const& fileSet : fileSets)
    {
        writeSubHeader(stream, QList({ componentNumber_, subHeaderNumber, fileSetNumber }), fileSet->name(), 3);

        stream << indent(3) << "<p>" << Qt::endl;

        // description
        if (!fileSet->description().isEmpty())
        {
            stream << indent(3) << HTML::INDENT << "<strong>Description:</strong> " <<
                fileSet->description() << "<br>" << Qt::endl;
        }

        // identifiers
        writeFileSetGroupdIdentifiers(stream, fileSet);

        // Default file builders table
        writeDefaultFileBuilders(stream, fileSet);

        writeFiles(stream, fileSet, subHeaderNumber, fileSetNumber);

        ++fileSetNumber;
    }
}

void HtmlWriter::setComponentNumber(int componentNumber)
{
    componentNumber_ = componentNumber;
}

QString HtmlWriter::indent(int n) const
{
    auto tab = QStringLiteral("\t");
    return tab.repeated(n);
}

void HtmlWriter::writeSubHeader(QTextStream& stream, QList<int> const& subHeaderNumbers, QString const& title, int level) const
{
    // Writes the header level
    QString headerTag = QStringLiteral("<h") + QString::number(level) + QStringLiteral(">");
    QString headerClosingTag = QStringLiteral("</h") + QString::number(level) + QStringLiteral(">");

    QStringList subHeaderNumberParts;
    for (int number : subHeaderNumbers)
    {
        subHeaderNumberParts << QString::number(number);
    }

    QString headerTitle = title;

    if (!subHeaderNumberParts.isEmpty())
    {
        headerTitle = subHeaderNumberParts.join(".") + " " + title;
    }

    stream << indent(3) << headerTag << headerTitle << headerClosingTag << Qt::endl;
}

void HtmlWriter::writeSubHeader(QTextStream& stream, int subHeaderNumber,
    QString const& headerText, QString const& headerId) const
{
    stream << "\t\t<h2><a id=\"" << component_->getVlnv().toString() << "." << headerId << "\">" <<
        componentNumber_ << "." << subHeaderNumber << " " << headerText << "</a></h2>" << Qt::endl;
}

void HtmlWriter::writeTableRow(QTextStream& stream, QStringList const& fields, int indentation)
{
    stream << indent(indentation) << "<tr>" << Qt::endl;
    
    for (auto const& field : fields)
    {
        stream << indent(indentation + 1) << "<td>" << field << "</td>" << Qt::endl;
    }

    stream << indent(indentation) << "</tr>" << Qt::endl;
}

void HtmlWriter::writeTableHeader(QTextStream& stream, QStringList const& headerFields, int indentation)
{
    stream << indent(indentation) << "<tr>" << Qt::endl;

    for (auto const& fields : headerFields)
    {
        stream << indent(indentation + 1) << "<th>" << fields << "</th>" << Qt::endl;
    }

    stream << indent(indentation) << "</tr>" << Qt::endl;
}

void HtmlWriter::writePortTable(QTextStream& stream, QString const& tableTitle, QList<QSharedPointer<Port>> ports)
{
    QStringList portTableHeaders(QStringList()
        << QStringLiteral("Name")
        << QStringLiteral("Direction")
        << QStringLiteral("Left bound")
        << QStringLiteral("Right bound")
        << QStringLiteral("Port type")
        << QStringLiteral("Type definition")
        << QStringLiteral("Default value")
        << QStringLiteral("Array left")
        << QStringLiteral("Array right")
        << QStringLiteral("Description")
    );

    
    stream << indent(3) << HTML::TABLE << tableTitle << "\">" << Qt::endl;

    writeTableHeader(stream, portTableHeaders, 4);

    for (auto const& port : ports)
    {
        QStringList portTableCells(QStringList()
            << "<a id=\"" + vlnvString_ + ".port." + port->name() + "\">" + port->name() + "</a>"
            << DirectionTypes::direction2Str(port->getDirection())
            << expressionFormatter_->formatReferringExpression(port->getLeftBound())
            << expressionFormatter_->formatReferringExpression(port->getRightBound())
            << port->getTypeName()
            << port->getTypeDefinition(port->getTypeName())
            << expressionFormatter_->formatReferringExpression(port->getDefaultValue())
            << expressionFormatter_->formatReferringExpression(port->getArrayLeft())
            << expressionFormatter_->formatReferringExpression(port->getArrayRight())
            << port->description()
        );

        writeTableRow(stream, portTableCells, 4);
    }

    stream << indent(3) << "</table>" << Qt::endl;
}

void HtmlWriter::writeFileSetGroupdIdentifiers(QTextStream& stream, QSharedPointer<FileSet> fileSet) const
{
    stream << indent(3) << HTML::INDENT << "<strong>Identifiers:</strong> ";
    
    QStringList groups = *fileSet->getGroups();
    
    stream << groups.join(", ") << "<br>" << Qt::endl;
}

void HtmlWriter::writeDefaultFileBuilders(QTextStream& stream, QSharedPointer<FileSet> fileSet)
{
    const auto defaultFileBuilders = fileSet->getDefaultFileBuilders();

    if (defaultFileBuilders->isEmpty())
    {
        stream << indent(3) << "</p>" << Qt::endl;
        return;
    }

    stream << indent(3) << HTML::INDENT << "<strong>Default file builders:</strong>" << Qt::endl;
    stream << indent(3) << "</p>" << Qt::endl;

    QStringList defaultBuilderHeaders(QStringList()
        << QStringLiteral("File type")
        << QStringLiteral("Command")
        << QStringLiteral("Flags")
        << QStringLiteral("Replace default flags")
    );

    QString tableTitle = QStringLiteral("Default file build commands");
    stream << indent(3) << HTML::TABLE << tableTitle << "\">" << Qt::endl;

    writeTableHeader(stream, defaultBuilderHeaders, 4);

    for (auto const& defaultBuilder : *defaultFileBuilders)
    {
        QStringList builderCells(QStringList()
            << defaultBuilder->getFileType()
            << defaultBuilder->getCommand()
            << defaultBuilder->getFlags()
            << expressionFormatter_->formatReferringExpression(defaultBuilder->getReplaceDefaultFlags())
        );

        writeTableRow(stream, builderCells, 4);
    }

    stream << indent(3) << "</table>" << Qt::endl;
}

void HtmlWriter::writeFiles(QTextStream& stream, QSharedPointer<FileSet> fileSet, int subHeaderNumber, int fileSetNumber)
{
    QList<QSharedPointer<File> > files = *fileSet->getFiles();

    if (files.isEmpty())
    {
        return;
    }

    int filesSubHeaderNumber = 1;

    QList filesSubHeaderNumbers({
        componentNumber_,
        subHeaderNumber,
        fileSetNumber,
        filesSubHeaderNumber
    });

    writeSubHeader(stream, filesSubHeaderNumbers, "Files", 4);

    QStringList fileHeaders(QStringList()
        << QStringLiteral("File name")
        << QStringLiteral("Logical name")
        << QStringLiteral("Build command")
        << QStringLiteral("Build flags")
        << QStringLiteral("Specified file types")
        << QStringLiteral("Description")
    );

    QString tableTitle = QStringLiteral("List of files contained in this file set.");
    stream << indent(3) << HTML::TABLE << tableTitle << "\">" << Qt::endl;

    writeTableHeader(stream, fileHeaders, 4);

    for (auto const& file : files)
    {
        writeSingleFile(stream, file);
    }

    stream << indent(3) << "</table>" << Qt::endl;
}

void HtmlWriter::writeSingleFile(QTextStream& stream, QSharedPointer<File> file)
{
    QString relativeFilePath = file->name();
    QString absFilePath = General::getAbsolutePath(libraryHandler_->getPath(component_->getVlnv()), relativeFilePath);

    QFileInfo fileInfo(absFilePath);

    // get relative path from html file to the file
    QFileInfo htmlInfo(getTargetPath());
    QString pathFromDocToFile = General::getRelativePath(getTargetPath(), absFilePath);

    QSharedPointer<BuildCommand> buildCommand = file->getBuildCommand();

    QStringList fileTypes = *file->getFileTypes();

    QStringList fileTableCells(QStringList()
        << "<a href=\"" + pathFromDocToFile + "\">" + file->name() + "</a>"
        << file->getLogicalName()
        << (buildCommand ? buildCommand->getCommand() : QStringLiteral(""))
        << (buildCommand ? buildCommand->getFlags() : QStringLiteral(""))
        << fileTypes.join(",<br>")
        << file->getDescription()
    );

    writeTableRow(stream, fileTableCells, 4);
}
