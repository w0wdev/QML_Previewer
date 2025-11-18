#include "pagescripting.h"
#include "../util.h"
#include <QQuickWidget>
#include <QPushButton>
#include <QFileInfo>
#include <QTabBar>
#include <QMessageBox>
#include <QQmlEngine>
#include <QQmlContext>
#include "scripteditor.h"
#include <QQuickItem>

PageScripting::PageScripting(QWidget *parent)
	: QWidget(parent)
	, ui(new Ui::PageScriptingClass)
{
	ui->setupUi(this);
	ui->qmlWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
	ui->qmlWidget->setClearColor(Util::getAppQColor("normalBackground"));
	
	ui->runButton->setText("");
	ui->runButton->setIcon(Util::getIcon("icons/Circled Play-96.png"));
	ui->runButton->setIconSize(QSize(30, 30));
	ui->runButton->setFlat(true);
	ui->runButton->setAttribute(Qt::WA_LayoutOnEntireRect);

	ui->stopButton->setText("");
	ui->stopButton->setIcon(Util::getIcon("icons/Shutdown-96.png"));
	ui->stopButton->setIconSize(QSize(30, 30));
	ui->stopButton->setFlat(true);
	ui->stopButton->setAttribute(Qt::WA_LayoutOnEntireRect);

	QPushButton* plusButton = new QPushButton();
	plusButton->setIcon(Util::getIcon("icons/Plus Math-96.png"));
	plusButton->setFlat(true);
	plusButton->setText("New Tab");
	plusButton->setStyleSheet(R"(
		QPushButton {
			background-color: #2c2c2c;
			color: #cccccc;
			border: 1px solid #3a3a3a;
			border-radius: 6px;
			padding: 4px 10px;
		}
		QPushButton:hover {
			background-color: #3d3d3d;
			color: white;
			border: 1px solid #0078d7;
		}
		QPushButton:pressed {
			background-color: #555555;
		}
		)");
	ui->fileTabs->setCornerWidget(plusButton);
	connect(plusButton, &QPushButton::clicked, [this]() {
		createEditorTab("", "");
		});

	ui->fileTabs->setStyleSheet(R"(
		QTabWidget::pane {
			border: 1px solid #2f2f2f;
			background: #1e1e1e;
			border-radius: 6px;
			top: -1px;
		}

		QTabBar::tab {
			background: #2c2c2c;
			color: #bfbfbf;
			padding: 6px 14px;
			border-top-left-radius: 6px;
			border-top-right-radius: 6px;
			margin-right: 2px;
		}

		QTabBar::tab:selected {
			background: #3f3f3f;
			color: #ffffff;
			font-weight: bold;
			border-bottom: 2px solid #0078d7;
		}

		QTabBar::tab:!selected {
			background: #2a2a2a;
			color: #999999;
		}

		QTabBar::tab:hover {
			background: #404040;
		}

		QTabWidget::tab-bar {
			alignment: left;
		}
		)");

	ui->qmlWidget->engine()->rootContext()->setContextProperty("QmlUi", this);

	connect(ui->mainEdit, &ScriptEditor::fileNameChanged, [this](QString fileName) {
		QFileInfo fileInfo(fileName);
		QString strTabName = "main";

		if (fileInfo.exists())
		{
			strTabName = fileInfo.fileName() + " (main)";
		}
		ui->fileTabs->setTabText(0, strTabName);
	});
}


PageScripting::~PageScripting()
{}

void PageScripting::createEditorTab(QString fileName, QString content)
{
	ScriptEditor* editor = new ScriptEditor();
	int nTabIndex = ui->fileTabs->addTab(editor, "Untitled");
	ui->fileTabs->setCurrentIndex(nTabIndex);
	editor->setCurFileName(fileName);
	editor->GetCodeEditor()->setPlainText(content);
	QPushButton* closeButton = new QPushButton();
	closeButton->setIcon(Util::getIcon("icons/Cancel-96.png"));
	closeButton->setFlat(true);
	ui->fileTabs->tabBar()->setTabButton(nTabIndex, QTabBar::RightSide, closeButton);

	connect(closeButton, &QPushButton::clicked, [this, editor]() {
		removeEditor(editor);
		});
}

void PageScripting::removeEditor(ScriptEditor* editor)
{
	bool bCloseTab = false;
	if (editor->checkHasUnsavedContent())
	{
		QMessageBox::StandardButton answer = QMessageBox::question(this, "Delete tab",
			"Do you want to delete the tab contents?", QMessageBox::Yes | QMessageBox::No);
		if (answer == QMessageBox::Yes) bCloseTab = true;
	}
	else bCloseTab = true;
	if (bCloseTab)
	{
		int nTabIndex = ui->fileTabs->indexOf(editor);
		if (nTabIndex > 0) ui->fileTabs->removeTab(nTabIndex);
	}
}

QString PageScripting::qmlToRun()
{
	ScriptEditor* editor = (ScriptEditor*)ui->fileTabs->currentWidget();
	return editor->GetCodeEditor()->toPlainText();
}

void PageScripting::on_stopButton_clicked()
{
	ui->qmlWidget->engine()->clearComponentCache();
	ui->qmlWidget->setSource(QUrl("qrc:/res/qml/Empty.qml"));
}

void PageScripting::on_runButton_clicked()
{
	qDebug() << "CHECK:" << QFile(":/res/qml/DynamicLoader.qml").exists();
	ui->qmlWidget->engine()->clearComponentCache();
	ui->qmlWidget->setSource(QUrl());
	ui->qmlWidget->setSource(QUrl(QStringLiteral("qrc:/res/qml/DynamicLoader.qml")));
	emit reloadQml(qmlToRun());
}


