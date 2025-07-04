#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QObject>
#include <QList>
#include <QString>
#include <QPainter>
#include <QtPrintSupport/QPrinter>
#include <QTextDocument>
#include "Award.h"

class ReportGenerator : public QObject
{
    Q_OBJECT

public:
    explicit ReportGenerator(QObject *parent = nullptr);
    bool generateReport(const QList<Award>& awards, const QString& filePath);

private:
    void drawHeader(QPainter *painter);
    void drawAwardTable(QPainter *painter, const QList<Award>& awards);
    void drawFooter(QPainter *painter);
};

#endif
