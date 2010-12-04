/*
    dialog.h

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

#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QString>
#include <QStringList>

namespace Ui {

    class Dialog;
}

class Dialog : public QDialog {

    Q_OBJECT

public:

    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:

    Ui::Dialog *ui;

    QStringList fileExtensions;
    QStringList *files;
    ptrdiff_t lines;

    bool itsOurFile(QString);
    void scanDirectory(QString);
    void getSourceCodeStringsNumber();

private slots:

    void on_pushButton_quit_clicked();
    void on_pushButton_start_clicked();
    void on_pushButton_selectDirectory_clicked();

};

#endif // DIALOG_H
