#pragma once

#include <QWidget>
#include "ui_pagescripting.h"
class PageScripting : public QWidget
{
	Q_OBJECT

public:
	PageScripting(QWidget *parent = nullptr);
	~PageScripting();

private:
	Ui::PageScriptingClass *ui;

	void createEditorTab(QString fileName, QString content);
	void removeEditor(ScriptEditor* editor);
	QString qmlToRun();

signals:
	void reloadQml(QString str);

private slots:
	void on_runButton_clicked();
	void on_stopButton_clicked();
};

