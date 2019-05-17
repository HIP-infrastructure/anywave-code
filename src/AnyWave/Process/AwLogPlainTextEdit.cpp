#include "AwLogPlainTextEdit.h"
#include <QScrollBar>

AwLogPlainTextEdit::AwLogPlainTextEdit(QWidget *parent)
	: QPlainTextEdit(parent)
{
	setReadOnly(true);
}

AwLogPlainTextEdit::~AwLogPlainTextEdit()
{
}


void AwLogPlainTextEdit::appendMessage(const QString& text)
{
	this->appendPlainText(text); // Adds the message to the widget
	this->verticalScrollBar()->setValue(this->verticalScrollBar()->maximum()); 
}