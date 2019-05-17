#pragma once

#include <QPlainTextEdit>

class AwLogPlainTextEdit : public QPlainTextEdit
{
	Q_OBJECT

public:
	AwLogPlainTextEdit(QWidget *parent);
	~AwLogPlainTextEdit();

	void appendMessage(const QString& text);
};
