#include "AwardTableModel.h"
#include <QBrush>
#include "MainWindow.h"

AwardTableModel::AwardTableModel(QObject *parent)
    : QAbstractTableModel(parent), m_searchText(""), m_sportFilter(""), m_levelFilter("")
{
}

void AwardTableModel::filterAwards(const QString& sport, const QString& level)
{
    beginResetModel();
    m_sportFilter = sport;
    m_levelFilter = level;

    m_filteredAwards.clear();

    for (const Award& award : m_awards) {
        bool sportMatch = m_sportFilter.isEmpty() || MainWindow::sportTypeToString(award.getSport()) == m_sportFilter;
        bool levelMatch = m_levelFilter.isEmpty() || MainWindow::competitionLevelToString(award.getLevel()) == m_levelFilter;

        if (sportMatch && levelMatch) {
            m_filteredAwards.append(award);
        }
    }
    endResetModel();
}

int AwardTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    bool isFiltered = !(m_sportFilter.isEmpty() && m_levelFilter.isEmpty());
    return isFiltered ? m_filteredAwards.size() : m_awards.size();
}

QVariant AwardTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();

    const QList<Award>& currentAwards = (m_sportFilter.isEmpty() && m_levelFilter.isEmpty()) ? m_awards : m_filteredAwards;

    if (row < 0 || row >= currentAwards.size())
        return QVariant();

    const Award& award = currentAwards[row];
    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (index.column()) {
        case 0: return award.getName();
        case 1: return award.getDate().toString("dd.MM.yyyy");
        case 2: return award.getLocation();
        case 3: return MainWindow::sportTypeToString(award.getSport());
        case 4: return award.getDiscipline();
        case 5: return MainWindow::competitionLevelToString(award.getLevel());
        case 6: return award.getPlace();
        case 7: return award.getDocument();
        default: return QVariant();
        }
    case IdRole: return award.getId();
    case NameRole: return award.getName();
    case DateRole: return award.getDate();
    case LocationRole: return award.getLocation();
    case SportRole: return static_cast<int>(award.getSport());
    case DisciplineRole: return award.getDiscipline();
    case LevelRole: return static_cast<int>(award.getLevel());
    case PlaceRole: return award.getPlace();
    case DocumentRole: return award.getDocument();
    default: return QVariant();
    }
}

QVariant AwardTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case 0: return "Название";
        case 1: return "Дата";
        case 2: return "Место проведения";
        case 3: return "Вид спорта";
        case 4: return "Дисциплина";
        case 5: return "Уровень";
        case 6: return "Занятое место";
        case 7: return "Документ";
        default: return QVariant();
        }
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

void AwardTableModel::setAwards(const QList<Award> &awards)
{
    beginResetModel();
    m_awards = awards;
    m_filteredAwards = awards;
    m_searchText = "";
    endResetModel();
}

void AwardTableModel::addAward(const Award &award)
{
    beginInsertRows(QModelIndex(), m_awards.size(), m_awards.size());
    m_awards.append(award);
    endInsertRows();
}

void AwardTableModel::updateAward(const Award &award)
{
    for (int i = 0; i < m_awards.size(); ++i) {
        if (m_awards[i].getId() == award.getId()) {
            m_awards[i] = award;
            QModelIndex index = createIndex(i, 0);
            emit dataChanged(index, index);
            return;
        }
    }
}

void AwardTableModel::removeAward(int row)
{
    beginRemoveRows(QModelIndex(), row, row);
    m_awards.removeAt(row);
    endRemoveRows();
}

Qt::ItemFlags AwardTableModel::flags(const QModelIndex &index) const {
    if (!index.isValid()) {
        return Qt::ItemIsEnabled;
    }

    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

void AwardTableModel::searchAwards(const QString &searchText)
{
    beginResetModel();
    m_searchText = searchText;
    m_filteredAwards.clear();

    if (searchText.isEmpty()) {
        m_filteredAwards = m_awards; // Restore the filtered list to the original list
        endResetModel();
        return;
    }

    for (const Award &award : m_awards) {
        if (award.getName().contains(searchText, Qt::CaseInsensitive) ||
            award.getLocation().contains(searchText, Qt::CaseInsensitive) ||
            award.getDiscipline().contains(searchText, Qt::CaseInsensitive)) {
            m_filteredAwards.append(award);
        }
    }

    endResetModel();
}
int AwardTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8; // Number of columns
}
