//-----------------------------------------------------------------------------
// File: TransactionalPortsEditorConstructor.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 29.04.2019
//
// Description:
// Constructs transactional ports editor items.
//-----------------------------------------------------------------------------

#ifndef TRANSACTIONALPORTSEDITORCONSTRUCTOR_H
#define TRANSACTIONALPORTSEDITORCONSTRUCTOR_H

#include <editors/ComponentEditor/ports/PortsEditorConstructor.h>

#include <QSortFilterProxyModel>
#include <QSharedPointer>
#include <QWidget>

class Component;
class PortsModel;
class PortsView;
class PortValidator;
class PortsDelegate;
class ExpressionParser;
class ParameterCompleter;

//-----------------------------------------------------------------------------
//! Constructs transactional ports editor items.
//-----------------------------------------------------------------------------
class TransactionalPortsEditorConstructor : public PortsEditorConstructor
{
public:

    /*!
     *  The constructor.
     */
    TransactionalPortsEditorConstructor() = default;

    /*!
     *  The destructor.
     */
    virtual ~TransactionalPortsEditorConstructor() = default;

    /*!
     *  Construct a ports model.
     *
     *      @param [in] component               The component being edited.
     *      @param [in] expressionParser        Parser for expressions.
     *      @param [in] parameterFinder         Locates the different parameters of the containing component.
     *      @param [in] expressionFormatter     The expression formatter.
     *      @param [in] portValidator           Validator used for ports.
     *      @param [in] filter                  Filter for ports model.
     *      @param [in] parent                  The owner of the constructed model.
     *
     *      @return The created ports model.
     */
    virtual PortsModel* constructModel(QSharedPointer<Component> component,
        QSharedPointer<ExpressionParser> expressionParser, QSharedPointer<ParameterFinder> parameterFinder,
        QSharedPointer<ExpressionFormatter> expressionFormatter, QSharedPointer<PortValidator> portValidator,
        QSortFilterProxyModel* filter, QObject* parent = 0) const;

    /*!
     *  Construct a filter.
     *
     *      @param [in] parent  Owner of the filter.
     *
     *      @return The created ports filter.
     */
    virtual PortsFilter* constructFilter(QObject* parent = 0) const;

    /*!
     *  Construct a view.
     *
     *      @param [in] defaultPath     Default import / export path.
     *      @param [in] parent          Owner of the view.
     *
     *      @return The created view.
     */
    virtual PortsView* constructView(QString const& defaultPath, QWidget* parent) const;

    /*!
     *  Construct a delegate.
     *
     *      @param [in] component           The component being edited.
     *      @param [in] parameterCompleter  Completer for expressions.
     *      @param [in] parameterFinder     The parameter finder.
     *      @param [in] portValidator       Validator used for ports.
     *      @param [in] parent              The owner of the delegate.
     *
     *      @return The created delegate.
     */
    virtual PortsDelegate* constructDelegate(QSharedPointer<Component> component,
        ParameterCompleter* parameterCompleter, QSharedPointer<ParameterFinder> parameterFinder,
        QSharedPointer<PortValidator> portValidator, QObject* parent = 0) const;

    //! No copying.
    TransactionalPortsEditorConstructor(const TransactionalPortsEditorConstructor& other) = delete;
    TransactionalPortsEditorConstructor& operator=(const TransactionalPortsEditorConstructor& other) = delete;
};

#endif // TRANSACTIONALPORTSEDITORCONSTRUCTOR_H
