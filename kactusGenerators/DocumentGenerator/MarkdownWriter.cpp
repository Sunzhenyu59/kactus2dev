#include "MarkdownWriter.h"

#include <IPXACTmodels/Component/Component.h>
#include <IPXACTmodels/Component/MemoryMap.h>
#include <IPXACTmodels/Component/AddressBlock.h>
#include <IPXACTmodels/Component/Register.h>

#include <KactusAPI/include/ExpressionFormatter.h>

#include <QDateTime>
#include <QSettings>
#include <QString>

MarkdownWriter::MarkdownWriter(QSharedPointer<Component> component, ExpressionFormatter* formatter) :
    component_(component),
    componentNumber_(0),
    expressionFormatter_(formatter)
{
}

MarkdownWriter::~MarkdownWriter()
{
}

void MarkdownWriter::writeHeader(QTextStream& stream)
{
    QSettings settings;
    
    // Write markdown header
    stream << "###### This document was generated by Kactus2 on " <<
        QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm:ss") <<
        " by user " << settings.value("General/Username").toString() <<
        Qt::endl;
}

void MarkdownWriter::writeKactusAttributes(QTextStream& stream, int subHeaderNumber)
{
    writeSubHeader(subHeaderNumber, stream, "Kactus2 attributes", "attributes");

    stream << "**Product hierarchy:** " <<
        KactusAttribute::hierarchyToString(component_->getHierarchy()) << "  " << Qt::endl
        << "**Component implementation:** " <<
        KactusAttribute::implementationToString(component_->getImplementation()) << "  " << Qt::endl
        << "**Component firmness:** " <<
        KactusAttribute::firmnessToString(component_->getFirmness()) << "  " << Qt::endl;
}

void MarkdownWriter::writeTableOfContents(QTextStream& stream)
{
    QString vlnv(component_->getVlnv().toString());

    // Write component header
    stream << componentNumber_ << ". [" << "Component " << component_->getVlnv().toString(" - ") << "]" <<
        "(#" << vlnv << ")  " << Qt::endl;

    // subHeader is running number that counts the number of sub headers for component
    int subHeader = 1;

    // Write component subheaders. Component has at least kactus attributes.
    stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Kactus2 attributes" <<
        "](#" << vlnv << ".attributes)  " << Qt::endl;

    ++subHeader;

    if (component_->hasParameters())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[General parameters]" <<
            "(#" << vlnv << ".parameters)  " << Qt::endl;
        ++subHeader;
    }

    if (!component_->getMemoryMaps()->isEmpty())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Memory maps]" <<
            "(#" << vlnv << ".memoryMaps)  " << Qt::endl;
        ++subHeader;
    }

    if (component_->hasPorts())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Ports]" <<
            "(#" << vlnv << ".ports)  " << Qt::endl;
        ++subHeader;
    }
    
    if (component_->hasInterfaces())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Bus interfaces]" <<
            "(#" << vlnv << ".interfaces)  " << Qt::endl;
        ++subHeader;
    }
    
    if (component_->hasFileSets())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[File sets]" <<
            "(#" << vlnv << ".fileSets)  " << Qt::endl;
        ++subHeader;
    }

    if (component_->hasViews())
    {
        stream << "\t" << componentNumber_ << "." << subHeader << ". " << "[Views]" <<
            "(#" << vlnv << ".views)  " << Qt::endl;
        ++subHeader;
    }
}

void MarkdownWriter::writeParameters(QTextStream& stream, int subHeaderNumber)
{
    writeSubHeader(subHeaderNumber, stream, "General parameters", "parameters");

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
    
    QString tableSeparator(":---- ");
    QStringList tableSeparators = tableSeparator.repeated(headers.length()).split(" ", Qt::SkipEmptyParts);

    writeTableLine(stream, headers);
    writeTableSeparator(stream, headers.length());

    for (auto const& parameter : *component_->getParameters())
    {
        QStringList paramCells(QStringList()
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

        writeTableLine(stream, paramCells);
    }
}

void MarkdownWriter::writeSubHeader(unsigned int subHeaderNumber, QTextStream& stream,
    QString const& headerText, QString const& headerId)
{
    stream << "## " << componentNumber_ << "." << subHeaderNumber << " " << headerText << " <a id=\"" <<
        component_->getVlnv().toString() << "." << headerId << "\">  " << Qt::endl << Qt::endl;
}

void MarkdownWriter::writeMemoryMaps(QTextStream& stream, int subHeaderNumber)
{
    if (component_->getMemoryMaps()->isEmpty())
    {
        return;
    }
    
    writeSubHeader(subHeaderNumber, stream, "Memory maps", "memoryMaps");

    const QList<QSharedPointer<MemoryMap> > componentMemoryMaps = *component_->getMemoryMaps().data();
    int memoryMapNumber = 1;

    for (auto const& memoryMap : componentMemoryMaps)
    {
        // Memory map header
        stream << "### " << componentNumber_ << "." << subHeaderNumber
            << "." << memoryMapNumber << " " << memoryMap->name()
            << " <a id=\"" << component_->getVlnv().toString()
            << ".memoryMap." << memoryMap->name() << "\">  " << Qt::endl;

        // Memory map description and address unit bits
        if (!memoryMap->description().isEmpty())
        {
            stream << "**Description:** " << memoryMap->description() << "  " << Qt::endl;
        }
        
        stream << "**Address unit bits (AUB):** " << memoryMap->getAddressUnitBits() << "  " << Qt::endl;

        QList<QSharedPointer <AddressBlock> > addressBlocks = getMemoryMapAddressBlocks(memoryMap);
        writeAddressBlocks(stream, addressBlocks, subHeaderNumber, memoryMapNumber);

        ++memoryMapNumber;
    }
}

void MarkdownWriter::writeAddressBlocks(QTextStream& stream, QList<QSharedPointer<AddressBlock>> addressBlocks,
    int subHeaderNumber, int memoryMapNumber)
{
    if (addressBlocks.isEmpty())
    {
        return;
    }

    int addressBlockNumber = 1;

    for (auto const& addressBlock : addressBlocks)
    {
        // header
        stream << "### " << componentNumber_ << "." << subHeaderNumber << "." << memoryMapNumber
            << "." << addressBlockNumber << " " << addressBlock->name() << " <a id=\""
            << component_->getVlnv().toString() << ".addressBlock." << addressBlock->name()
            << "\">  " << Qt::endl << Qt::endl;
        
        // description
        if (!addressBlock->description().isEmpty())
        {
            stream << "**Description:** " << addressBlock->description() << "  " << Qt::endl;
        }

        QStringList headers(QStringList()
            << QStringLiteral("Usage")
            << QStringLiteral("Base address [AUB]")
            << QStringLiteral("Range [AUB]")
            << QStringLiteral("Width [AUB]")
            << QStringLiteral("Access")
            << QStringLiteral("Volatile")
        );

        writeTableLine(stream, headers);
        writeTableSeparator(stream, headers.length());

        QStringList addressBlockTableCells(QStringList()
            << General::usage2Str(addressBlock->getUsage())
            << expressionFormatter_->formatReferringExpression(addressBlock->getBaseAddress())
            << expressionFormatter_->formatReferringExpression(addressBlock->getRange())
            << expressionFormatter_->formatReferringExpression(addressBlock->getWidth())
            << AccessTypes::access2Str(addressBlock->getAccess())
            << addressBlock->getVolatile()
        );

        writeTableLine(stream, addressBlockTableCells);

        QList <QSharedPointer <Register> > registers = getAddressBlockRegisters(addressBlock);
        // writeRegisters();

        ++addressBlockNumber;
    }
}

void MarkdownWriter::writeRegisters(QTextStream& stream, QList<QSharedPointer<Register>> registers, int subHeaderNumber, int memoryMapNumber, int addressBlockNumber)
{
    if (registers.isEmpty())
    {
        return;
    }

    int registerNumber = 1;

    for (auto const& currentRegister : registers)
    {
        stream << "### " << componentNumber_ << "." << subHeaderNumber << "." << memoryMapNumber
            << "." << addressBlockNumber << "." << registerNumber << " " << currentRegister->name()
            << " <a id=\"" << component_->getVlnv().toString() << ".register." << currentRegister->name()
            << "\">  " << Qt::endl << Qt::endl;

        if (!currentRegister->description().isEmpty())
        {
            stream << "**Description:** " << currentRegister->description() << "  " << Qt::endl;
        }

        QStringList registerHeaders(QStringList()
            << QStringLiteral("Offset [AUB]")
            << QStringLiteral("Size [bits]")
            << QStringLiteral("Dimension")
            << QStringLiteral("Volatile")
            << QStringLiteral("Access")
        );

        writeTableLine(stream, registerHeaders);

        QStringList registerInfo;
        registerInfo << expressionFormatter_->formatReferringExpression(currentRegister->getAddressOffset());

        if (currentRegister->getSize().isEmpty())
        {
            registerInfo << currentRegister->getSize();
        }
        else
        {
            registerInfo << expressionFormatter_->formatReferringExpression(currentRegister->getSize());
        }

        if (currentRegister->getDimension().isEmpty())
        {
            registerInfo << currentRegister->getDimension();
        }
        else
        {
            registerInfo << expressionFormatter_->formatReferringExpression(currentRegister->getDimension());
        }

        registerInfo << currentRegister->getVolatile()
            << AccessTypes::access2Str(currentRegister->getAccess());

        writeTableLine(stream, registerInfo);

        //writeFields(currentRegister);

        ++registerNumber;
    }
}

void MarkdownWriter::setComponentNumber(unsigned int componentNumber)
{
    componentNumber_ = componentNumber;
}

QList<QSharedPointer<AddressBlock>> MarkdownWriter::getMemoryMapAddressBlocks(QSharedPointer<MemoryMap> memoryMap) const
{
    QList<QSharedPointer <AddressBlock> > addressBlocks;
    for (auto const& memoryMapItem : *memoryMap->getMemoryBlocks())
    {
        QSharedPointer<AddressBlock> addressItem = memoryMapItem.dynamicCast<AddressBlock>();

        if (addressItem)
        {
            addressBlocks.append(addressItem);
        }
    }

    return addressBlocks;
}

QList<QSharedPointer<Register>> MarkdownWriter::getAddressBlockRegisters(QSharedPointer<AddressBlock> addressBlock) const
{
    QList <QSharedPointer <Register> > registers;
    for (auto const& registerModelItem : *addressBlock->getRegisterData())
    {
        QSharedPointer <Register> registerItem = registerModelItem.dynamicCast<Register>();

        if (registerItem)
        {
            registers.append(registerItem);
        }
    }

    return registers;
}

void MarkdownWriter::writeTableLine(QTextStream& stream, QStringList const& cells) const
{
    for (auto const& cell : cells)
    {
        stream << "|" << cell;
    }

    stream << "|" << Qt::endl;
}

void MarkdownWriter::writeTableSeparator(QTextStream& stream, int columns)
{
    QString tableSeparator(":---- ");   // :--- aligns text in cells to the left
    QStringList tableSeparators = tableSeparator.repeated(columns).split(" ", Qt::SkipEmptyParts);
    writeTableLine(stream, tableSeparators);
}