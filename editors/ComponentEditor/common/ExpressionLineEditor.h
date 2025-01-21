//-----------------------------------------------------------------------------
// File: ExpressionLineEditor.h
//-----------------------------------------------------------------------------
// Project: Kactus2
// Author: Mikko Teuho
// Date: 18.10.2022
//
// Description:
// Line editor for expressions with parameter name completion.
//-----------------------------------------------------------------------------

#ifndef EXPRESSIONLINEEDITOR_H
#define EXPRESSIONLINEEDITOR_H

#include <editors/ComponentEditor/common/MasterExpressionEditor.h>

#include <QCompleter>
#include <QLineEdit>

class ParameterFinder;

//-----------------------------------------------------------------------------
//! Line editor for expressions with parameter name completion.
//-----------------------------------------------------------------------------
class ExpressionLineEditor : public QLineEdit, public MasterExpressionEditor
{
    Q_OBJECT
public:

	/*!
     *  The constructor.
     * 
     *    @param [in] parameterFinder     The parameter finder.
     *    @param [in] parent              The parent widget.
     */
    ExpressionLineEditor(QSharedPointer<ParameterFinder> parameterFinder, QWidget* parent = 0);

	/*!
     *  The destructor.
     */
	virtual ~ExpressionLineEditor() = default;

    // Disable copying.
    ExpressionLineEditor(ExpressionLineEditor const& rhs) = delete;
    ExpressionLineEditor& operator=(ExpressionLineEditor const& rhs) = delete;

    /*!
     *  Sets a completer whose selection will be appended to the text.
     *
     *    @param [in] completer   The completer to set.
     */
    void setAppendingCompleter(QCompleter* completer) final;
    
    /*!
     *  Sets the expression in the editor.
     *
     *    @param [in] expression   The expression to set.
     */
    void setExpression(QString const& expression) final;

protected:

    /*!
     *  Captures the context menu event and accepts it.
     *
     *    @param [in] menuEvent   The event for context menu.
     */
    void contextMenuEvent(QContextMenuEvent* menuEvent) final;

    /*!
     *  Captures the mouse press event and finishes editing the current word.
     *
     *    @param [in] mouseEvent  The event representing user input.
     */
    void mousePressEvent(QMouseEvent* mouseEvent) final;

    /*!
     *  Captures the user input and edits the underlying expression accordingly.
     *
     *    @param [in] keyEvent   The event representing the user input.
     */
    void keyPressEvent(QKeyEvent* keyEvent) final;

    /*!
     *  Selects the text when the editor is opened.
     *
     *    @param [in] focusEvent   The event representing the focus change.
     */
    void focusInEvent(QFocusEvent* focusEvent) final;

    /*!
     *  Captures the mouse move and sets/clears notSelectingText flag according to mouse press.
     *
     *    @param [in] mouseEvent   The mouse event representing the mouse movement and button press.
     */
    void mouseMoveEvent(QMouseEvent* mouseEvent) final;

    /*!
     *  Clears notSelectingText flag when the mouse button is released.
     *
     *    @param [in] mouseEvent   The event representing the mouse button release.
     */
    void mouseReleaseEvent(QMouseEvent* mouseEvent) final;

    /*!
     *  Emits a signal informing of the ending of editing the expression.
     *
     *    @param [in] event   The event representing the changing of focus.
     */
    void focusOutEvent(QFocusEvent *event) final;

signals:

    /*!
     *  Increase the amount of references to a parameter corresponding to the id.
     *
     *    @param [in] parameterId     The id of the parameter being referenced.
     */
    void increaseReference(QString const& parameterId);

    /*!
     *  Decrease the amount of references to a parameter corresponding to the id.
     *
     *    @param [in] parameterId     The id of the parameter being referenced.
     */
    void decreaseReference(QString const& parameterId);

private slots:

    /*!
     *  Called when a word completion has been selected. Replaces the word under the cursor with the selection.
     *
     *    @param [in] word   The selected word.
     */
    void complete(QModelIndex const& index);

    /*!
     *  Called when the cursor position changes. If a different word is under the new position, it is suggested
     *  for new completion.
     *
     *    @param [in] oldPosition     Old position of the cursor.
     *    @param [in] newPosition     New position of the cursor.
     */
    void updateCompletions(int oldPosition, int newPosition);

private:

    /*!
     *  Changes the color of the font for the current word to red.
     */
    void colorCurrentWordRed() final;

    /*!
     *  Changes the color of the font for the current word to black.
     */
    void colorCurrentWordBlack() final;

    /*!
     *  Check if a selection exists.
     *
     *    @return True, if a selection exists, false otherwise.
     */
    bool hasSelection() final;

    /*!
     *  Let the parent widget handle the key press event.
     *
     *    @param [in] keyEvent    The selected key press event.
     */
    void handleParentKeyPressEvent(QKeyEvent* keyEvent) final;

    /*!
     *  Decrease references made to the selected ID.
     *
     *    @param [in] referenceItem   ID of the selected item.
     */
    void handleReferenceDecrease(QString const& referenceItem) final;

    /*!
     *  Increase references made to the selected ID.
     *
     *    @param [in] referenceItem   ID of the selected item.
     */
    void handleReferenceIncrease(QString const& referenceItem) final;

    /*!
     *  Set the completed parameter name for the editor.
     *
     *    @param [in] parameterName   The completed parameter name.
     */
    void setCompletedParameterName(QString const& parameterName) final;

    /*!
     *  Get the current cursor position.
     *
     *    @return The position of the cursor.
     */
    int getCursorPosition() const final;

    /*!
     *  Get the current text of the editor.
     *
     *    @return The current text.
     */
    QString getCurrentText() const final;

    /*!
     *  Get the selected text.
     *
     *    @return The selected text.
     */
    QString getSelectedText() const final;

    /*!
     *  Count the length of the first word in the current selection.
     *
     *    @return Length of the first word in the selection.
     */
    int getSelectionFirstWord() const final;

    /*!
     *  Count the length of the last word in the current selection.
     *
     *    @return Length of the last word in the selection.
     */
    int getSelectionLastWord() const final;

    /*!
     *	Get the start index of the selection.
     *
     * 	    @return The start index of the current selection.
     */
    int getSelectionStartIndex() const final;
};

#endif // EXPRESSIONLINEEDITOR_H