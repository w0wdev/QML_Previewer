#pragma once

#include <QWidget>
#include "ui_scripteditor.h"

class ScriptEditor : public QWidget
{
	Q_OBJECT

public:
	ScriptEditor(QWidget *parent = nullptr);
	~ScriptEditor();
	QString GetCurFileName();
	void setCurFileName(QString fileName);

	QCodeEditor* GetCodeEditor();
	bool checkHasUnsavedContent();

signals:
	void fileNameChanged(QString strNewName);

private slots:
	void on_saveButton_clicked();
	void on_saveAsButton_clicked();
	void on_openFileButton_clicked();
	void on_refreshButton_clicked();

private:
	Ui::ScriptEditorClass* ui;
};

