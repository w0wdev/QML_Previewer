#include "scripteditor.h"
#include "../util.h"
#include <QFileDialog>
#include <QFilesystemWatcher>
#include <QMessageBox>
#include <QmlHighlighter>
#include "qsettings.h"

ScriptEditor::ScriptEditor(QWidget *parent)
	: QWidget(parent),
    ui(new Ui::ScriptEditorClass)
{
	ui->setupUi(this);
    ui->openFileButton->setIcon(Util::getIcon("icons/Open Folder-96.png"));
    ui->saveButton->setIcon(Util::getIcon("icons/Save-96.png"));
    ui->saveAsButton->setIcon(Util::getIcon("icons/Save as-96.png"));
    ui->refreshButton->setIcon(Util::getIcon("icons/Refresh-96.png"));

    this->setStyleSheet(R"(
        QWidget {
            background-color: #202225;
            border: none;
        }
        )");

    ui->codeEdit->setAutoFillBackground(true);

    connect(ui->codeEdit, &QCodeEditor::saveTriggered, [this]() {
        on_saveButton_clicked();
        });

    ui->codeEdit->setHighlighter(new QmlHighlighter);
    ui->codeEdit->setHighlightBlocks(false);
    ui->codeEdit->setAutoFillBackground(true);
}

void ScriptEditor::on_saveButton_clicked()
{
    qDebug() << "save button clicked.";
    QString fileName = GetCurFileName();
    QFileInfo fileInfo(fileName);
    if (!fileInfo.exists())
    {
        on_saveAsButton_clicked();
        return;
    }
    
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::critical(this, "Save Qml File", "Could not open\n" + fileName + "\nfor writing");
        return;
    }

    file.write(ui->codeEdit->toPlainText().toUtf8());
    file.close();
    setCurFileName(fileName);
    emit fileNameChanged(fileName);
}

void ScriptEditor::on_saveAsButton_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Save qml", GetCurFileName(), tr("QML files (*.qml)"));
    if (!fileName.isEmpty())
    {
        qDebug() << "typed name.";
        QString strExtension = ".qml";
        bool bHasExtension = fileName.endsWith(".qml", Qt::CaseInsensitive);
        if (!bHasExtension) fileName.append(strExtension);
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::critical(this, "Save Qml File", "Could not open\n" + fileName + "\nfor writing");
            return;
        }
        file.write(ui->codeEdit->toPlainText().toUtf8());
        file.close();
        setCurFileName(fileName);
        emit fileNameChanged(fileName);
    }
}

void ScriptEditor::on_openFileButton_clicked()
{
    QString filePathName = GetCurFileName();
    if (filePathName.isEmpty())
    {
        filePathName = QSettings().value("scripting/lastPath", "").toString();
    }
    QString fileName = QFileDialog::getOpenFileName(this, "Open Qml File", filePathName, "QML files (*.qml)");
    if (!fileName.isEmpty())
    {
        QFile file(fileName);
        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, "Open Qml File", "Could not open\n" + fileName + "\nfor reading");
            return;
        }
        QSettings().setValue("scripting/lastPath", QFileInfo(file).canonicalFilePath());

        ui->codeEdit->setPlainText(QString::fromUtf8(file.readAll()));
        setCurFileName(fileName);
        emit fileNameChanged(fileName);
    }
}

void ScriptEditor::on_refreshButton_clicked()
{
    QString filePathName = GetCurFileName();
    QFileInfo fileInfo(filePathName);
    if (!fileInfo.exists()) {
        QMessageBox::warning(this, "Refresh File", "No file is opened.");
        return;
    }
    QFile file(filePathName);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(this, "Refresh File", "Could not open\n" + filePathName + "\nfor reading");
        return;
    }
    ui->codeEdit->setPlainText(QString::fromUtf8(file.readAll()));
    file.close();
}

QString ScriptEditor::GetCurFileName()
{
    return ui->fileNowLabel->text();
}

void ScriptEditor::setCurFileName(QString fileName)
{
    ui->fileNowLabel->setText(fileName);
}

QCodeEditor* ScriptEditor::GetCodeEditor()
{
    return ui->codeEdit;
}


bool ScriptEditor::checkHasUnsavedContent()
{
    bool result = false;
    QString strFileName = GetCurFileName();
    QFileInfo fileInfo(strFileName);
    if (!fileInfo.exists())
    {
        QString str = ui->codeEdit->toPlainText();
        int size = str.size();
        if (ui->codeEdit->toPlainText().size() > 0)
        {
            result = true;
        }
    }
    else
    {
        QFile file(strFileName);
        if (file.open(QIODevice::ReadOnly))
        {
            if (QString::fromUtf8(file.readAll()) != ui->codeEdit->toPlainText().toUtf8()) result = true;
        }
    }
    return result;
}


ScriptEditor::~ScriptEditor()
{
}
