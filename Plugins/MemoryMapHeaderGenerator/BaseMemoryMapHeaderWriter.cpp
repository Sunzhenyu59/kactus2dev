//-----------------------------------------------------------------------------
// File: BaseMemoryMapHeaderWriter.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 07.05.2015
//
// Description:
// The base implementation for memory map header writer.
//-----------------------------------------------------------------------------

#include "BaseMemoryMapHeaderWriter.h"

#include <library/LibraryInterface.h>

#include <KactusAPI/include/IPluginUtility.h>

#include <editors/ComponentEditor/common/ComponentParameterFinder.h>
#include <editors/ComponentEditor/common/ParameterFinder.h>
#include <editors/ComponentEditor/common/IPXactSystemVerilogParser.h>
#include <editors/ComponentEditor/common/ExpressionFormatter.h>

#include <IPXACTmodels/common/AccessTypes.h>

#include <IPXACTmodels/Component/FileSet.h>
#include <IPXACTmodels/Component/File.h>
#include <IPXACTmodels/Component/MemoryMapBase.h>

#include <QCoreApplication>
#include <QDate>
#include <QSettings>
#include <QDir>
#include <QDesktopServices>
#include <QUrl>
#include <QMessageBox>

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::BaseMemoryMapHeaderWriter()
//-----------------------------------------------------------------------------
BaseMemoryMapHeaderWriter::BaseMemoryMapHeaderWriter(IPluginUtility* utility):
utility_(utility)
{

}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::BaseMemoryMapHeaderWriter()
//-----------------------------------------------------------------------------
BaseMemoryMapHeaderWriter::~BaseMemoryMapHeaderWriter()
{

}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::checkDirectoryStructure()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::checkDirectoryStructure(QDir dir)
{
    if (!dir.exists())
    {
        QString dirName(dir.dirName());
        dir.cdUp();
        dir.mkpath(dirName);
    }
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::openFileErrorMessage()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::openFileErrorMessage(QString const& absoluteFilePath)
{
    QString message(QObject::tr("File: %1 could not be opened for writing.").arg(absoluteFilePath));
    QMessageBox::critical(utility_->getParentWidget(), QCoreApplication::applicationName(), message);
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::writeTopOfHeaderFile()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::writeTopOfHeaderFile(QTextStream& stream, QString const& fileName,
    QString const& header, QString const& description)
{
    stream << "/*" << endl;
    stream << " * File: " << fileName << endl;
    stream << " * Created on: " << QDate::currentDate().toString("dd.MM.yyyy") << endl;

    QSettings settings;
    QString userName = settings.value("General/Username").toString();

    stream << " * Generated by: " << userName << endl;
    stream << " *" << endl;
    stream << " * Description:" << endl;
    stream << description << endl;

    stream << "#ifndef " << header << endl;
    stream << "#define " << header << endl << endl;
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::addHeaderFile()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::addHeaderFile(QSharedPointer<Component> component, const QFileInfo& fileInfo,
    QString const& filesetName, QStringList const& swViewNames, QString const& instanceId) const
{
    QString xmlDir = utility_->getLibraryInterface()->getDirectoryPath(component->getVlnv());

    // if the directory does not exist
    QDir ipXactDir(xmlDir);
    Q_ASSERT(ipXactDir.exists());
    Q_ASSERT(fileInfo.exists());

    // calculate the relative path 
    QString relPath =  ipXactDir.relativeFilePath(fileInfo.absoluteFilePath());

    // file set where the file is added to
    QSharedPointer<FileSet> fileSet = component->getFileSet(filesetName);
    if (fileSet.isNull())
    {
        fileSet = QSharedPointer<FileSet>(new FileSet(filesetName));
        component->getFileSets()->append(fileSet);
    }

    fileSet->setGroups("generatedFiles");
    fileSet->setDescription(QObject::tr("Contains header files generated for the component.\n"
        "Do not rename this file set, name is used to find the generated headers."));

    // if instance id was specified
    if (!instanceId.isEmpty())
    {
        // use the same id as the instance the file set is created for.
        fileSet->setFileSetId(instanceId);
    }

    QSettings settings;
    QSharedPointer<File> file = fileSet->addFile(relPath, settings);
    Q_ASSERT(file);

    file->setDescription(QObject::tr("A header file generated by Kactus2.\n"
        "This file contains the register and memory addresses defined in the memory map(s)"));
    file->setIncludeFile(true);

    foreach (QSharedPointer<View> swView, *component->getViews())
    {
        if (swViewNames.contains(swView->name()))
        {
            QSharedPointer<ComponentInstantiation> insta = component->getModel()->findComponentInstantiation(swView->getComponentInstantiationRef());

            if (insta)
            {
                insta->getFileSetReferences()->append(filesetName);
            }
        }
    }

    displayMemoryMapHeader(fileInfo.absoluteFilePath());
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::displayMemoryMapHeader()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::displayMemoryMapHeader(QString const& filePath) const
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(filePath));
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::writeRegistersFromMemoryMap()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::writeRegisterFromMemoryMap(QSharedPointer<ParameterFinder> finder,
    QSharedPointer<MemoryMapBase> containingMemoryMap, QTextStream& stream, bool useAddressBlockID, quint64 offset,
    QString const& idString)
{
    QSharedPointer<IPXactSystemVerilogParser> expressionParser (new IPXactSystemVerilogParser(finder));
    QSharedPointer<ExpressionFormatter> formatter (new ExpressionFormatter(finder));

    foreach (QSharedPointer<MemoryBlockBase> memoryItem, *containingMemoryMap->getMemoryBlocks())
    {
        QSharedPointer<AddressBlock> addressBlock = memoryItem.dynamicCast<AddressBlock>();

        if (addressBlock && (addressBlock->getUsage() == General::REGISTER || (General::usage2Str(
            addressBlock->getUsage()).isEmpty() && !addressBlock->getRegisterData()->isEmpty())))
        {
            writeRegistersFromAddressBlock(expressionParser, formatter, addressBlock, stream, useAddressBlockID,
                offset, idString);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::writeRegistersFromAddressBlock()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::writeRegistersFromAddressBlock(QSharedPointer<ExpressionParser> expressionParser,
    QSharedPointer<ExpressionFormatter> formatter, QSharedPointer<AddressBlock> currentAddressBlock,
    QTextStream& stream, bool useAddressBlockID, quint64 offset, QString const& idString)
{
    QString addressBlockOffsetInt = expressionParser->parseExpression(currentAddressBlock->getBaseAddress());
    quint64 addressBlockOffset = addressBlockOffsetInt.toUInt() + offset;
    
    QString id;
    if (!idString.isEmpty())
    {
        id = idString;
        if (useAddressBlockID)
        {
            id.append(currentAddressBlock->name().toUpper());
        }
    }
    
    if (useAddressBlockID)
    {
        id.append(currentAddressBlock->name().toUpper());
    }

    stream << "/*" << endl;
    stream << " * Address block: " << currentAddressBlock->name() << endl;
    if (!currentAddressBlock->description().isEmpty())
    {
        stream << " * Description:" << endl;
        stream << " * " << currentAddressBlock->description() << endl;
    }
    stream << "*/" << endl;

    foreach (QSharedPointer<RegisterBase> registerModelItem, *currentAddressBlock->getRegisterData())
    {
        QSharedPointer<Register> registerItem = registerModelItem.dynamicCast<Register>();
        if (registerItem)
        {
            writeRegister(expressionParser, formatter, registerItem, stream, addressBlockOffset, id);
        }
    }
    stream << endl;
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::writeRegister()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::writeRegister(QSharedPointer<ExpressionParser> expressionParser,
    QSharedPointer<ExpressionFormatter> formatter, QSharedPointer<Register> currentRegister, QTextStream& stream,
    quint64 addressBlockOffset, QString const& idString /* = QString() */)
{
    QString parsedOffset = expressionParser->parseExpression(currentRegister->getAddressOffset());
    quint64 registerOffsetInt = parsedOffset.toUInt() + addressBlockOffset;
    QString registerOffsetString = QString::number(registerOffsetInt, 16);
    registerOffsetString.prepend("0x");

    stream << "/*" << endl;
    stream << " * Register name: " << currentRegister->name() << endl;

    if (!currentRegister->description().isEmpty())
    {
        stream << " * Description:" << endl;
        stream << " * " << currentRegister->description() << endl;
    }

    stream << " * Offset: " << formatter->formatReferringExpression(currentRegister->getAddressOffset());
    if (!expressionParser->isPlainValue(currentRegister->getAddressOffset()))
    {
        quint64 parsedOffsetInt = parsedOffset.toUInt();
        QString parsedOffsetString = QString::number(parsedOffsetInt, 16);
        stream << " = 0x" << parsedOffsetString;
    }

    stream << endl << "*/" << endl;
    stream << "#define ";

    if (idString.isEmpty())
    {
        stream << currentRegister->name().toUpper() << " " << registerOffsetString;
    }
    else
    {
        stream << idString.toUpper() << "_" << currentRegister->name().toUpper() << " " <<
            registerOffsetString;
    }

    stream << endl;
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::writeMemoryAddresses()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::writeMemoryAddresses(QSharedPointer<ParameterFinder> finder,
    QSharedPointer<MemoryMapBase> targetMemoryMap, QTextStream& stream, quint64 offset,
    QString const& idString) const
{
    QSharedPointer<IPXactSystemVerilogParser> expressionParser (new IPXactSystemVerilogParser(finder));
    QSharedPointer<ExpressionFormatter> formatter (new ExpressionFormatter(finder));

    foreach (QSharedPointer<MemoryBlockBase> memoryItem, *targetMemoryMap->getMemoryBlocks())
    {
        QSharedPointer<AddressBlock> currentAddressBlock = memoryItem.dynamicCast<AddressBlock>();
        if (currentAddressBlock && (currentAddressBlock->getUsage() == General::MEMORY ||
            currentAddressBlock->getUsage() == General::RESERVED))
        {
            QString addressOffsetString = expressionParser->parseExpression(currentAddressBlock->getBaseAddress());
            quint64 addressOffset = addressOffsetString.toUInt() + offset;
            QString addressStart = QString::number(addressOffset, 16);
            addressStart.prepend("0x");

            QString endAddress = getAddressBlockLastAddress(currentAddressBlock, expressionParser);
            
            stream << "/*" << endl;
            if (currentAddressBlock->getUsage() == General::MEMORY)
            {
                stream << " * Memory block name: " << currentAddressBlock->name() << endl;
            }
            else
            {
                stream << " * Reserved block name: " << currentAddressBlock->name() << endl;
            }

            stream << " * Width: " << expressionParser->parseExpression(currentAddressBlock->getWidth());
            if (!expressionParser->isPlainValue(currentAddressBlock->getWidth()))
            {
                stream << " = " << formatter->formatReferringExpression(currentAddressBlock->getWidth());
            }
            stream << endl;

            QString parseRange = expressionParser->parseExpression(currentAddressBlock->getRange());

            stream << " * Range: " << expressionParser->parseExpression(currentAddressBlock->getRange());
            if (!expressionParser->isPlainValue(currentAddressBlock->getRange()))
            {
                stream << " = " << formatter->formatReferringExpression(currentAddressBlock->getRange());
            }
            stream << endl;

            QString accessString = AccessTypes::access2Str(currentAddressBlock->getAccess());
            if (!accessString.isEmpty())
            {
                stream << " * Access: " << accessString << endl;
            }
            stream << "*/" << endl;

            QString blockName = currentAddressBlock->name().toUpper();
            if (!idString.isEmpty())
            {
                blockName = idString.toUpper() + "_" + blockName;
            }
            stream << "#define " << blockName << "_START " << addressStart;
            if (!expressionParser->isPlainValue(currentAddressBlock->getBaseAddress()))
            {
                stream << " // " << formatter->formatReferringExpression(currentAddressBlock->getBaseAddress());
            }
            stream << endl;
            stream << "#define " << blockName << "_END " << endAddress << endl;
            stream << endl;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::getAddressBlockLastAddress()
//-----------------------------------------------------------------------------
QString BaseMemoryMapHeaderWriter::getAddressBlockLastAddress(QSharedPointer<AddressBlock> targetAddressBlock,
    QSharedPointer<ExpressionParser> expressionParser) const
{
    QString parsedBaseAddress = expressionParser->parseExpression(targetAddressBlock->getBaseAddress());
    QString parsedRange = expressionParser->parseExpression(targetAddressBlock->getRange());

    quint64 lastAddress = parsedBaseAddress.toUInt() + parsedRange.toUInt();
    QString lastAddressString = "0x";
    if (lastAddress != 0)
    {
        lastAddress -= 1;
    }

    lastAddressString.append(QString::number(lastAddress, 16));
    return lastAddressString;
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::formattedValueFor()
//-----------------------------------------------------------------------------
QString BaseMemoryMapHeaderWriter::parsedValueFor(QString const& expression,
    QSharedPointer<ParameterFinder> finder) const
{
    QSharedPointer<IPXactSystemVerilogParser> expressionParser (new IPXactSystemVerilogParser(finder));

    return expressionParser->parseExpression(expression);
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::informStartOfGeneration()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::informStartOfGeneration() const
{
    utility_->printInfo(QObject::tr("Generation started %1").arg(QDateTime::currentDateTime().
        toString(Qt::LocalDate)));
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::informWritingFinished()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::informWritingFinished(QString const& outputFile) const
{
    utility_->printInfo(QObject::tr("Finished writing file %1.").arg(outputFile));
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::informGenerationComplete()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::informGenerationComplete() const
{
    utility_->printInfo(QObject::tr("Generation complete."));
}

//-----------------------------------------------------------------------------
// Function: BaseMemoryMapHeaderWriter::informGenerationAbort()
//-----------------------------------------------------------------------------
void BaseMemoryMapHeaderWriter::informGenerationAbort() const
{
    utility_->printInfo(QObject::tr("Generation aborted."));
}
