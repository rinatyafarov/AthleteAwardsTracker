#ifndef AWARDTABLEMODEL_H
#define AWARDTABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include <QVariant>
#include "Award.h"

class AwardTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    enum AwardDataRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        DateRole,
        LocationRole,
        SportRole,
        DisciplineRole,
        LevelRole,
        PlaceRole,
        DocumentRole
    };
    explicit AwardTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    void setAwards(const QList<Award>& awards);
    void addAward(const Award& award);
    void updateAward(const Award& award);
    void removeAward(int row);
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    void searchAwards(const QString& searchText); // Add this line
    void filterAwards(const QString& sport, const QString& level);

private:
    QList<Award> m_awards;
    QList<Award> m_filteredAwards; // Add this line
    QString m_searchText;          // Add this line
    QString m_sportFilter;
    QString m_levelFilter;

};

#endif //
