//-----------------------------------------------------------------------------
// File: FileSet.cpp
//-----------------------------------------------------------------------------
// Project: Kactus 2
// Author: Mikko Teuho
// Date: 18.09.2015
//
// Description:
// Describes the ipxact:fileSet element.
//-----------------------------------------------------------------------------

#include "FileSet.h"

#include <IPXACTmodels/VendorExtension.h>
#include <IPXACTmodels/common/FileTypes.h>
#include <IPXACTmodels/kactusExtensions/Kactus2Value.h>

/*
//-----------------------------------------------------------------------------
// Function: FileSet::FileSet()
//-----------------------------------------------------------------------------
FileSet::FileSet(QDomNode & fileSetNode):
NameGroup(),
	groups_(),
files_(), 
defaultFileBuilders_(),
dependencies_(),
functions_(),
fileSetId_(),
vendorExtensions_()
{
	for (int i = 0; i < fileSetNode.childNodes().count(); ++i) {
		QDomNode tempNode = fileSetNode.childNodes().at(i);

		if (tempNode.nodeName() == QString("spirit:group")) {
			QString groupName = tempNode.childNodes().at(0).nodeValue();
			groupName = XmlUtils::removeWhiteSpace(groupName);
			groups_.append(groupName);
		}

		else if (tempNode.nodeName() == QString("spirit:file")) {
			files_.append(QSharedPointer<File>(new File(tempNode, this)));
		}

		else if (tempNode.nodeName() == QString("spirit:defaultFileBuilder")) {
			defaultFileBuilders_.append(QSharedPointer<FileBuilder>(new FileBuilder(tempNode)));
		}

		else if (tempNode.nodeName() == QString("spirit:dependency") ) {
			dependencies_.append(tempNode.childNodes().at(0).nodeValue());
		}

		else if (tempNode.nodeName() == QString("spirit:function")) {
			functions_.append(QSharedPointer<Function>(new Function(tempNode)));
		}
		else if (tempNode.nodeName() == QString("spirit:vendorExtensions")) {
            parseVendorExtensions(tempNode);
		}
	}
}*/

//-----------------------------------------------------------------------------
// Function: FileSet::FileSet()
//-----------------------------------------------------------------------------
FileSet::FileSet(const QString& name /* = QString() */, const QString& group /* = QString() */) :
NameGroup(name),
Extendable(),
groups_(),
files_(new QList<QSharedPointer<File> > ()),
defaultFileBuilders_(new QList<QSharedPointer<FileBuilder> > ()),
dependencies_(),
functions_(new QList<QSharedPointer<Function> > ())
{
    if (!group.isEmpty())
    {
        groups_.append(group);
    }
}

//-----------------------------------------------------------------------------
// Function: FileSet::FileSet()
//-----------------------------------------------------------------------------
FileSet::FileSet(const FileSet &other) :
NameGroup(other),
Extendable(other),
groups_(other.groups_),
files_(new QList<QSharedPointer<File> > ()),
defaultFileBuilders_(new QList<QSharedPointer<FileBuilder> > ()),
dependencies_(other.dependencies_),
functions_(new QList<QSharedPointer<Function> > ())
{
    copyFiles(other);
    copyDefaultFileBuilders(other);
    copyFunctions(other);
}

//-----------------------------------------------------------------------------
// Function: FileSet::operator=()
//-----------------------------------------------------------------------------
FileSet& FileSet::operator=(const FileSet& other)
{
    if (this != &other)
    {
        NameGroup::operator=(other);
        Extendable::operator=(other);
        groups_ = other.groups_;
        dependencies_ = other.dependencies_;

        files_->clear();
        copyFiles(other);
        defaultFileBuilders_->clear();
        copyDefaultFileBuilders(other);
        functions_->clear();
        copyFunctions(other);
    }

    return *this;
}

//-----------------------------------------------------------------------------
// Function: FileSet::~FileSet()
//-----------------------------------------------------------------------------
FileSet::~FileSet()
{
    files_.clear();
    defaultFileBuilders_.clear();
    functions_->clear();
}
/*
//-----------------------------------------------------------------------------
// Function: fileset::isValid()
//-----------------------------------------------------------------------------
bool FileSet::isValid( QStringList& errorList, const QString& parentIdentifier, bool checkChildren )
    const
{
	bool valid = true;
	const QString thisIdentifier = QObject::tr("file set %1").arg(name());

	if (name().isEmpty())
    {
		errorList.append(QObject::tr("Name of the file set missing within %1").arg(
			parentIdentifier));
		valid = false;
	}

    if (dependencies_.contains(""))
    {
        errorList.append(QObject::tr("Empty dependency found in %1.").arg(thisIdentifier));

        valid = false;
    }

	if (checkChildren)
    {
		QStringList fileIDs;
		QStringList fileNames;
		foreach (QSharedPointer<File> file, files_)
        {
			if (fileNames.contains(file->name()))
            {
				errorList.append(QObject::tr("%1 contains several files with"
					" name %2").arg(thisIdentifier).arg(file->name()));
				valid = false;
			}
			else
            {
				fileNames.append(file->name());
			}

			if (!file->isValid(errorList, thisIdentifier))
            {
				valid = false;
			}

			// if file has file id specified then save it to list to be used later.
			if (!file->getFileId().isEmpty())
            {
				fileIDs.append(file->getFileId());
			}
		}

		foreach (QSharedPointer<FileBuilder> fileBuilder, defaultFileBuilders_)
        {
			if (!fileBuilder->isValid(errorList, thisIdentifier))
            {
				valid = false;
			}
		}

		foreach (QSharedPointer<Function> func, functions_)
        {
			if (!func->isValid(fileIDs, errorList, thisIdentifier))
            {
				valid = false;
			}
		}

	}

	return valid;
}
*//*
//-----------------------------------------------------------------------------
// Function: fileset::isValid()
//-----------------------------------------------------------------------------
bool FileSet::isValid( bool checkChildren ) const
{
	if (name().isEmpty())
    {
		return false;
	}

    foreach (QString dependecy, dependencies_)
    {
        if (dependecy.isEmpty())
        {
            return false;
        }
    }

	if (checkChildren)
    {
		QStringList fileIDs;
		QStringList fileNames;
		foreach (QSharedPointer<File> file, files_)
        {
			if (fileNames.contains(file->name()))
            {
				return false;
			}
			else
            {
				fileNames.append(file->name());
			}

			if (!file->isValid())
            {
				return false;
			}

			// if file has file id specified then save it to list to be used later.
			if (!file->getFileId().isEmpty())
            {
				fileIDs.append(file->getFileId());
			}
		}

		foreach (QSharedPointer<FileBuilder> fileBuilder, defaultFileBuilders_)
        {
			if (!fileBuilder->isValid())
            {
				return false;
			}
		}

		foreach (QSharedPointer<Function> func, functions_)
        {
			if (!func->isValid(fileIDs, true))
            {
				return false;
			}
		}
	}

	return true;
}*/

//-----------------------------------------------------------------------------
// Function: FileSet::getGroups()
//-----------------------------------------------------------------------------
QStringList FileSet::getGroups() const
{
    return groups_;
}

//-----------------------------------------------------------------------------
// Function: FileSet::setGroups()
//-----------------------------------------------------------------------------
void FileSet::setGroups(const QStringList& groups)
{
    groups_.clear();
    groups_ = groups;
}

//-----------------------------------------------------------------------------
// Function: FileSet::setGroups()
//-----------------------------------------------------------------------------
void FileSet::setGroups( const QString& groupName )
{
    groups_.clear();
    groups_.append(groupName);
}

//-----------------------------------------------------------------------------
// Function: FileSet::getFiles()
//-----------------------------------------------------------------------------
QSharedPointer<QList<QSharedPointer<File> > > FileSet::getFiles() const
{
    return files_;
}

//-----------------------------------------------------------------------------
// Function: FileSet::getFiles()
//-----------------------------------------------------------------------------
QStringList FileSet::getFileNames( const QStringList& fileTypes ) const
{
    QStringList files;

    foreach (QSharedPointer<File> file, *files_)
    {
        if (file->matchesFileType(fileTypes))
        {
            files.append(file->name());
        }
    }

    return files;
}

//-----------------------------------------------------------------------------
// Function: FileSet::setFiles()
//-----------------------------------------------------------------------------
void FileSet::setFiles(QSharedPointer<QList<QSharedPointer<File> > > newFiles)
{
    files_->clear();
    files_ = newFiles;
}

//-----------------------------------------------------------------------------
// Function: FileSet::getDefaultFileBuilders()
//-----------------------------------------------------------------------------
QSharedPointer<QList<QSharedPointer<FileBuilder> > > FileSet::getDefaultFileBuilders() const
{
    return defaultFileBuilders_;
}

//-----------------------------------------------------------------------------
// Function: FileSet::setDefaultFileBuilders()
//-----------------------------------------------------------------------------
void FileSet::setDefaultFileBuilders(QSharedPointer<QList<QSharedPointer<FileBuilder> > > newDefaultFileBuilders)
{
        defaultFileBuilders_->clear();
        defaultFileBuilders_ = newDefaultFileBuilders;
}

//-----------------------------------------------------------------------------
// Function: FileSet::getDependencies()
//-----------------------------------------------------------------------------
QStringList FileSet::getDependencies() const
{
    return dependencies_;
}

//-----------------------------------------------------------------------------
// Function: FileSet::setDependencies()
//-----------------------------------------------------------------------------
void FileSet::setDependencies(const QStringList &dependencies)
{
    dependencies_.clear();
    dependencies_ = dependencies;
}

//-----------------------------------------------------------------------------
// Function: FileSet::addDependency()
//-----------------------------------------------------------------------------
void FileSet::addDependency( const QString& path )
{
    dependencies_.append(path);
}

//-----------------------------------------------------------------------------
// Function: FileSet::getFunctions()
//-----------------------------------------------------------------------------
QSharedPointer<QList<QSharedPointer<Function> > > FileSet::getFunctions() const
{
	return functions_;
}


//-----------------------------------------------------------------------------
// Function: FileSet::setFunctions()
//-----------------------------------------------------------------------------
void FileSet::setFunctions(QSharedPointer<QList<QSharedPointer<Function> > > newFunctions)
{
	functions_->clear();
	functions_ = newFunctions;
}

//-----------------------------------------------------------------------------
// Function: FileSet::getVhdlLibraryNames()
//-----------------------------------------------------------------------------
QStringList FileSet::getVhdlLibraryNames() const
{
    QStringList libraries;
    foreach (QSharedPointer<File> file, *files_)
    {
        if (file->isVhdlFile() && !file->getLogicalName().isEmpty() && 
            !libraries.contains(file->getLogicalName()))
        {
            libraries.append(file->getLogicalName());
        }
    }
    return libraries;
}

//-----------------------------------------------------------------------------
// Function: FileSet::getFilePaths()
//-----------------------------------------------------------------------------
QStringList FileSet::getFilePaths() const
{
    QStringList filePaths;

    for (int i = 0; i < files_->size(); ++i)
    {
        filePaths.append(files_->at(i)->name());
    }
    return filePaths;
}

//-----------------------------------------------------------------------------
// Function: FileSet::addFile()
//-----------------------------------------------------------------------------
void FileSet::addFile(QSharedPointer<File> file)
{
    for (int i = 0; i < files_->size(); ++i)
    {
        if (files_->at(i)->name() == file->name())
        {
            files_->removeAt(i);
            files_->append(file);
            return;
        }
    }

    files_->append(file);
}

//-----------------------------------------------------------------------------
// Function: FileSet::addFile()
//-----------------------------------------------------------------------------
QSharedPointer<File> FileSet::addFile(const QString& filePath, QSettings& settings)
{
    QSharedPointer<File> file;
    foreach (QSharedPointer<File> fileSearch, *files_)
    {
        if (fileSearch->name() == filePath)
        {
            file = fileSearch;
        }
    }

    if (!file)
    {
        file = QSharedPointer<File> (new File(filePath));
        files_->append(file);
    }

    QFileInfo fileInfo(filePath);
    QStringList types = FileTypes::getFileTypes(settings, fileInfo);
    file->getFileTypes()->append(types);

    return file;
}

//-----------------------------------------------------------------------------
// Function: FileSet::getFile()
//-----------------------------------------------------------------------------
QSharedPointer<File> FileSet::getFile(const QString logicalName) const
{
    for (int i = 0; i < files_->size(); ++i)
    {
        if (files_->at(i)->getLogicalName() == logicalName)
        {
            return files_->at(i);
        }
    }

    return QSharedPointer<File>();
}

//-----------------------------------------------------------------------------
// Function: FileSet::getFileNames()
//-----------------------------------------------------------------------------
QStringList FileSet::getFileNames() const
{
    QStringList fileNames;
    for (int i = 0; i < files_->size(); ++i)
    {
        fileNames.append(files_->at(i)->name());
    }
    return fileNames;
}

//-----------------------------------------------------------------------------
// Function: FileSet::findFilesByFileType()
//-----------------------------------------------------------------------------
QStringList FileSet::findFilesByFileType( const QString& fileType ) const
{
    QStringList files;
    foreach (QSharedPointer<const File> file, *files_)
    {
        if (file->matchesFileType(fileType))
        {
            files.append(file->name());
        }
    }

    return files;
}

//-----------------------------------------------------------------------------
// Function: FileSet::removeFile()
//-----------------------------------------------------------------------------
void FileSet::removeFile( const QString& fileName )
{
    for (int i = 0; i < files_->size(); ++i)
    {
        if (files_->at(i)->name() == fileName)
        {
            files_->value(i).clear();
            files_->removeAt(i);
            return;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: FileSet::removeFile()
//-----------------------------------------------------------------------------
void FileSet::removeFile(QSharedPointer<File> file)
{
    for (int i = 0; i < files_->size(); ++i)
    {
        if (*files_->at(i) == *file)
        {
            files_->value(i).clear();
            files_->removeAt(i);
            break;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: FileSet::createFile()
//-----------------------------------------------------------------------------
QSharedPointer<File> FileSet::createFile()
{
    QSharedPointer<File> file(new File());
    files_->append(file);
    return file;
}

//-----------------------------------------------------------------------------
// Function: FileSet::getDefaultCommand()
//-----------------------------------------------------------------------------
QString FileSet::getDefaultCommand(QSharedPointer<File> file) const
{
    if (!file)
    {
        return QString();
    }

    QSharedPointer<QStringList> fileTypes = file->getFileTypes();
    foreach (QString fileType, *fileTypes)
    {
        foreach (QSharedPointer<FileBuilder> fileBuilder, *defaultFileBuilders_)
        {
            if (fileBuilder->hasFileType(fileType))
            {
                return fileBuilder->getCommand();
            }
        }
    }

    return QString();
}

//-----------------------------------------------------------------------------
// Function: FileSet::getDefaultFlags()
//-----------------------------------------------------------------------------
QString FileSet::getDefaultFlags(QSharedPointer<File> file) const
{
    if (!file)
    {
        return QString();
    }

    QSharedPointer<QStringList> fileTypes = file->getFileTypes();
    foreach (QString fileType, *fileTypes)
    {
        foreach (QSharedPointer<FileBuilder> fileBuilder, *defaultFileBuilders_)
        {
            if (fileBuilder->hasFileType(fileType))
            {
                return fileBuilder->getFlags();
            }
        }
    }
    return QString();
}

//-----------------------------------------------------------------------------
// Function: FileSet::getRTLFiles()
//-----------------------------------------------------------------------------
QList<QSharedPointer<File> > FileSet::getRTLFiles() const
{
    QList<QSharedPointer<File> > list;
    foreach (QSharedPointer<File> file, *files_)
    {
        if (file->isRTLFile())
        {
            list.append(file);
        }
    }
    return list;
}

//-----------------------------------------------------------------------------
// Function: FileSet::contains()
//-----------------------------------------------------------------------------
bool FileSet::contains( const QString& fileName ) const
{
    foreach (QSharedPointer<File> file, *files_)
    {
        if (file->name() == fileName)
        {
            return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Function: FileSet::changeFileName()
//-----------------------------------------------------------------------------
void FileSet::changeFileName( const QString& from, const QString& to )
{
    foreach (QSharedPointer<File> file, *files_)
    {
        if (file->name() == from)
        {
            file->setName(to);
            return;
        }
    }
}

//-----------------------------------------------------------------------------
// Function: FileSet::sortFiles()
//-----------------------------------------------------------------------------
void FileSet::sortFiles( const QStringList& fileNames )
{
    QSharedPointer<QList<QSharedPointer<File> > > tempList (new QList<QSharedPointer<File> > ());

    foreach (QString fileName, fileNames)
    {
        foreach (QSharedPointer<File> file, *files_)
        {
            if (file->name() == fileName)
            {
                tempList->append(file);
            }
        }
    }

    files_.clear();
    files_ = tempList;
}

//-----------------------------------------------------------------------------
// Function: FileSet::clearFiles()
//-----------------------------------------------------------------------------
void FileSet::clearFiles()
{
    files_->clear();
}

//-----------------------------------------------------------------------------
// Function: FileSet::getFileSetId()
//-----------------------------------------------------------------------------
QString FileSet::getFileSetId() const 
{
    foreach (QSharedPointer<VendorExtension> extension, *getVendorExtensions())
    {
        if (extension->type() == "kactus2:fileSetId")
        {
            QSharedPointer<Kactus2Value> idExtension = extension.dynamicCast<Kactus2Value>();
            return idExtension->value();
        }
    }

    return QString();
}

//-----------------------------------------------------------------------------
// Function: FileSet::setFileSetId()
//-----------------------------------------------------------------------------
void FileSet::setFileSetId( const QString& id ) 
{
    foreach (QSharedPointer<VendorExtension> extension, *getVendorExtensions())
    {
        if (extension->type() == "kactus2:fileSetId")
        {
            getVendorExtensions()->removeAll(extension);
        }
    }

    if (!id.isEmpty())
    {
        QSharedPointer<Kactus2Value> idExtension (new Kactus2Value("kactus2:fileSetId", id));
        getVendorExtensions()->append(idExtension);
    }
}

//-----------------------------------------------------------------------------
// Function: FileSet::copyFiles()
//-----------------------------------------------------------------------------
void FileSet::copyFiles(const FileSet& other)
{
    foreach (QSharedPointer<File> file, *other.files_)
    {
        if (file)
        {
            QSharedPointer<File> copy = QSharedPointer<File>(new File(*file.data()));
            files_->append(copy);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: FileSet::copyDefaultFileBuilders()
//-----------------------------------------------------------------------------
void FileSet::copyDefaultFileBuilders(const FileSet& other)
{
    foreach (QSharedPointer<FileBuilder> fileBuilder, *other.defaultFileBuilders_)
    {
        if (fileBuilder)
        {
            QSharedPointer<FileBuilder> copy = QSharedPointer<FileBuilder>(new FileBuilder(*fileBuilder.data()));
            defaultFileBuilders_->append(copy);
        }
    }
}

//-----------------------------------------------------------------------------
// Function: FileSet::copyFunctions()
//-----------------------------------------------------------------------------
void FileSet::copyFunctions(const FileSet& other)
{
    foreach (QSharedPointer<Function> funcion, *other.functions_)
    {
        if (funcion)
        {
            QSharedPointer<Function> copy = QSharedPointer<Function>(new Function(*funcion.data()));
            functions_->append(copy);
        }
    }
}