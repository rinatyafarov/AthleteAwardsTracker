#include "AwardTableModel.h"
#include <QBrush>
#include "MainWindow.h"

AwardTableModel::AwardTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

int AwardTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_awards.size();
}

int AwardTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 8; // Number of columns
}

QVariant AwardTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();

    if (row < 0 || row >= m_awards.size())
        return QVariant();

    const Award& award = m_awards[row];

    switch (role) {
    case Qt::DisplayRole:
    case Qt::EditRole:
        switch (col) {
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

    case IdRole: return award.getId(); // Check this
    case NameRole: return award.getName(); // Check this
    case DateRole: return award.getDate(); // Check this
    case LocationRole: return award.getLocation(); // Check this
    case SportRole: return static_cast<int>(award.getSport()); // Check this
    case DisciplineRole: return award.getDiscipline(); // Check this
    case LevelRole: return static_cast<int>(award.getLevel()); // Check this
    case PlaceRole: return award.getPlace(); // Check this
    case DocumentRole: return award.getDocument(); // Check this
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
