#include "HtmlWriter.h"

#include <IPXACTmodels/Component/Component.h>

#include <KactusAPI/include/ExpressionFormatter.h>

#include <QDateTime>
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

HtmlWriter::HtmlWriter(QSharedPointer<Component> component) :
    component_(component),
    componentNumber_(0)
{
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
    writeSubHeader(subHeaderNumber, stream, "Kactus2 attributes", "attributes");

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

void HtmlWriter::writeParameters(QTextStream& stream, ExpressionFormatter* formatter,
    int subHeaderNumber)
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

    writeSubHeader(subHeaderNumber, stream, "General parameters", "parameters");

    // Write table element
    stream << indent(3) << HTML::TABLE << "List of parameters defined for the component\">" << Qt::endl;

    // Write header row
    stream << indent(4) << "<tr>" << Qt::endl;
    
    for (auto const& header : headers)
    {
        stream << indent(5) << "<th>" << header << "</th>" << Qt::endl;
    }

    stream << indent(4) << "</tr>" << Qt::endl;

    // Write parameters
    for (auto const& parameter : *component_->getParameters())
    {
        stream << indent(4) << "<tr>" << Qt::endl;
        
        stream << indent(5) << "<td>" << parameter->name() << "</td>" << Qt::endl;
        stream << indent(5) << "<td>" << parameter->getType() << "</td>" << Qt::endl;
        stream << indent(5) << "<td>" <<
            formatter->formatReferringExpression(parameter->getValue()) << "</td>" << Qt::endl;
        
        stream << indent(5) << "<td>" << parameter->getValueResolve() << "</td>" << Qt::endl;
        stream << indent(5) << "<td>" <<
            formatter->formatReferringExpression(parameter->getVectorLeft()) << "</td>" << Qt::endl;
        
        stream << indent(5) << "<td>" <<
            formatter->formatReferringExpression(parameter->getVectorRight()) << "</td>" << Qt::endl;
        
        stream << indent(5) << "<td>" <<
            formatter->formatReferringExpression(parameter->getArrayLeft()) << "</td>" << Qt::endl;
        
        stream << indent(5) << "<td>" <<
            formatter->formatReferringExpression(parameter->getArrayRight()) << "</td>" << Qt::endl;
        
        stream << indent(5) << "<td>" << parameter->description() << "</td>" << Qt::endl;

        stream << indent(4) << "</tr>" << Qt::endl;
    }

    stream << indent(3) << "</table>" << Qt::endl;
}

void HtmlWriter::writeSubHeader(unsigned int subHeaderNumber, QTextStream& stream, 
    QString const& headerText, QString const& headerId)
{
    stream << "\t\t<h2><a id=\"" << component_->getVlnv().toString() << "." << headerId << "\">" <<
        componentNumber_ << "." << subHeaderNumber << " " << headerText << "</a></h2>" << Qt::endl;
}

void HtmlWriter::setComponentNumber(unsigned int componentNumber)
{
    componentNumber_ = componentNumber;
}

QString HtmlWriter::indent(int n) const
{
    auto tab = QStringLiteral("\t");
    return tab.repeated(n);
}
