#ifndef MAPMODEL_H
#define MAPMODEL_H

#include <QAbstractTableModel>
#include <QMap>
#include <QDateTime>

// Source code inspired from https://stackoverflow.com/questions/23484511/how-to-display-a-simple-qmap-in-a-qtableview-in-qt
class MapModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	
	enum MapRoles {
		KeyRole = Qt::UserRole + 1,
		ValueRole
	};
	
	explicit MapModel(QObject* parent = nullptr);
	
	int rowCount(const QModelIndex& parent = QModelIndex()) const;
	int columnCount(const QModelIndex& parent = QModelIndex()) const;
	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
	void setMap(QMap<uint, QVariant>* map);
	
private:
	QMap<uint, QVariant>* map;
};

#endif // MAPMODEL_H
