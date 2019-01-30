#include "mapmodel.h"

MapModel::MapModel(QObject* parent) : QAbstractTableModel(parent) {}

int MapModel::rowCount(const QModelIndex& parent) const {
	if (map != nullptr)
		return map->count();
	else
		return 0;
}

int MapModel::columnCount(const QModelIndex& parent) const {
	return 2;
}

QVariant MapModel::data(const QModelIndex& index, int role) const {
	// If map not defined, return nothing
	if (map == nullptr)
		return QVariant();
	
	// If index out of bounds, return nothing
	if (index.row() < 0 || index.row() >= map->count() || role != Qt::DisplayRole)
		return QVariant();
	
	if (index.column() == 0) {
		QDateTime timestamp;
		timestamp.setTime_t(map->keys().at(index.row()));
		return timestamp.toString(Qt::SystemLocaleShortDate);
	}
	
	if (index.column() == 1) {
		return map->values().at(index.row());
	}
	
	return QVariant();
}

void MapModel::setMap(QMap<uint, QVariant>* map) {
	this->map = map;
}
