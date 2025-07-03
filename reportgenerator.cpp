#include "ReportGenerator.h"
#include "MainWindow.h"
#include <QDebug>
#include <QTextTable>
#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QFont>

ReportGenerator::ReportGenerator(QObject *parent) : QObject(parent)
{
}

bool ReportGenerator::generateReport(const QList<Award>& awards, const QString& filePath)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(filePath);

    QTextDocument document;
    QTextCursor cursor(&document);

    // Header
    QTextCharFormat headerFormat;
    headerFormat.setFont(QFont("Arial", 20, QFont::Bold));

    QTextBlockFormat blockFormat;
    blockFormat.setAlignment(Qt::AlignCenter);

    cursor.insertBlock(blockFormat, headerFormat);
    cursor.insertText("Отчет о Наградах Спортсмена", headerFormat);

    // Table
    QTextTableFormat tableFormat;
    tableFormat.setBorder(2);
    tableFormat.setBorderStyle(QTextTableFormat::BorderStyle_Solid);
    tableFormat.setCellSpacing(2);
    tableFormat.setCellPadding(7);

    QTextTable *table = cursor.insertTable(awards.size() + 1, 10, tableFormat);

    // Table Header
    QTextCharFormat tableHeaderFormat;
    tableHeaderFormat.setFont(QFont("Arial", 14, QFont::Bold));

    QTextCursor cellCursor;
    QTextTableCell cell;

    // Headers
    QStringList headers = {"Название", "Дата", "Место", "Спорт", "Дисциплина", "Уровень", "Место", "Документ"};
    for (int i = 0; i < headers.size(); ++i) {
        cell = table->cellAt(0, i);
        cellCursor = table->cellAt(0, i).firstCursorPosition();
        cellCursor.insertText(headers[i], tableHeaderFormat);
    }

    // Table Data
    QTextCharFormat tableDataFormat;
    tableDataFormat.setFont(QFont("Arial", 10));

    for (int i = 0; i < awards.size(); ++i) {
        const Award& award = awards[i];

        cell = table->cellAt(i + 1, 0);
        cellCursor = table->cellAt(i + 1, 0).firstCursorPosition();
        cellCursor.insertText(award.getName(), tableDataFormat);

        cell = table->cellAt(i + 1, 1);
        cellCursor = table->cellAt(i + 1, 1).firstCursorPosition();
        cellCursor.insertText(award.getDate().toString("dd.MM.yyyy"), tableDataFormat);

        cell = table->cellAt(i + 1, 2);
        cellCursor = table->cellAt(i + 1, 2).firstCursorPosition();
        cellCursor.insertText(award.getLocation(), tableDataFormat);

        cell = table->cellAt(i + 1, 3);
        cellCursor = table->cellAt(i + 1, 3).firstCursorPosition();
        cellCursor.insertText(MainWindow::sportTypeToString(award.getSport()), tableDataFormat);

        cell = table->cellAt(i + 1, 4);
        cellCursor = table->cellAt(i + 1, 4).firstCursorPosition();
        cellCursor.insertText(award.getDiscipline(), tableDataFormat);

        cell = table->cellAt(i + 1, 5);
        cellCursor = table->cellAt(i + 1, 5).firstCursorPosition();
        cellCursor.insertText(MainWindow::competitionLevelToString(award.getLevel()), tableDataFormat);

        cell = table->cellAt(i + 1, 6);
        cellCursor = table->cellAt(i + 1, 6).firstCursorPosition();
        cellCursor.insertText(QString::number(award.getPlace()), tableDataFormat);

        cell = table->cellAt(i + 1, 7);
        cellCursor = table->cellAt(i + 1, 7).firstCursorPosition();
        cellCursor.insertText(award.getDocument(), tableDataFormat);
    }

    document.print(&printer);
    return true;
}
