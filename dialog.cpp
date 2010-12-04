/*
    dialog.cpp

    Copyright (C) 2010 Artem Petrov <pa2311@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.
    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "dialog.h"
#include "ui_dialog.h"

#include <QString>
#include <QStringList>
#include <QFileDialog>
#include <QDir>
#include <QFileInfo>
#include <QFile>
#include <QIODevice>

#include <QDebug>

Dialog::Dialog(QWidget *parent) : QDialog(parent),
                                  ui(new Ui::Dialog),
                                  lines(0) {

    ui->setupUi(this);

    files = new QStringList();
}

Dialog::~Dialog() {

    delete ui;

    delete files;
}

bool Dialog::itsOurFile(QString extension) {

    for (ptrdiff_t i=0; i<fileExtensions.count(); i++) {

        if (extension == fileExtensions.at(i)) {

            return true;
        }
    }

    return false;
}

void Dialog::scanDirectory(QString dir) {

    QDir projectDirectory(dir);
    QString fullName;
    QFileInfo fileInfo;

    QStringList fileNamesList(projectDirectory.entryList(QDir::Dirs | QDir::Files, QDir::Name));

    for (ptrdiff_t i=0; i<fileNamesList.count(); i++) {

        fullName = dir + QDir::separator() + fileNamesList.at(i);
        fileInfo.setFile(fullName);

        if ( fileInfo.isDir() && ( ( fileNamesList.at(i) != "." ) && ( fileNamesList.at(i) != ".." ) ) ) {

            scanDirectory(fullName);
        }
        else if ( fileInfo.isFile() && itsOurFile(fileInfo.suffix()) ) {

            files->push_back(fullName);
        }
    }
}

void Dialog::getSourceCodeStringsNumber() {

    QString str;
    QFile srcFile;

    for (ptrdiff_t i=0; i<files->count(); i++) {

        srcFile.setFileName(files->at(i));
        srcFile.open(QIODevice::ReadOnly);

        while (!srcFile.atEnd()) {

            str = srcFile.readLine().trimmed();

            if ( (!str.isEmpty()) && (!str.isNull()) ) {

                lines++;
            }
        }

        srcFile.close();
    }
}

void Dialog::on_pushButton_selectDirectory_clicked() {

    QString projectDirectory(QFileDialog::getExistingDirectory(
            this,
            tr("Select project directory..."),
            QDir::currentPath()
    ));

    if (!projectDirectory.isEmpty()) {

        ui->lineEdit_path->setText(projectDirectory);
    }
}

void Dialog::on_pushButton_start_clicked() {

    ui->pushButton_selectDirectory->setEnabled(false);
    ui->pushButton_start->setEnabled(false);
    ui->pushButton_quit->setEnabled(false);

    //

    fileExtensions = ui->lineEdit_fileExtensions->text().split(";", QString::SkipEmptyParts);

    scanDirectory(ui->lineEdit_path->text());
    getSourceCodeStringsNumber();

    ui->label_stringsNumber->setText(QString::number(lines));

    files->clear();
    lines = 0;

    //

    ui->pushButton_selectDirectory->setEnabled(true);
    ui->pushButton_start->setEnabled(true);
    ui->pushButton_quit->setEnabled(true);
}

void Dialog::on_pushButton_quit_clicked() {

    close();
}
