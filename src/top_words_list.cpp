#include "src/top_words_list.h"

#include <QDebug>

TopWordsList::TopWordsList(QObject *parent) : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> TopWordsList::roleNames() const
{
	return {{1, "data_model"}};
}

int TopWordsList::rowCount(const QModelIndex &) const
{
	return _list.size() > SIZE ? SIZE : _list.size();
}

QVariant TopWordsList::data(const QModelIndex &index, int) const
{
	auto stat = _list[index.row()];
	return QVariant::fromValue(TopWordsModel(stat));
}

void TopWordsList::read_file_begin(qint64 size)
{
	emit beginResetModel();
	_list.clear();
	emit endResetModel();

	_max_size = size;
	_read_size = 0;
	_curr_percent = 0;
	emit load_start();
}

void TopWordsList::read_chank(qint64 size)
{
	_read_size += size;
	auto prev = _curr_percent;
	_curr_percent = size / _max_size;

	if (_curr_percent > prev)
	{
		emit progress_updated(_curr_percent);
	}
}

void TopWordsList::update_stat(const word_stat *s)
{
	qDebug() << "update stat" << s->word << s->count << _list.size();
	// обновляем максимальное значение
	if (_max_value < s->count)
	{
		_max_value = s->count;
		emit max_value_changed(_max_value);
		qDebug() << "max_size" << _max_value;
	}

	// найти место для добавления нового word_stat
	int new_pos = _list.size();
	int old_pos = -1;
	int index = 0;
	for (auto& i : _list)
	{
		if (new_pos == _list.size() && s->count > i.count)
		{
			qDebug() << s->word << s->count << i.word;
			new_pos = index;
		}
		if (i.word == s->word)
		{
			old_pos = index;
			if (s->count == i.count) {
				return;
			}
		}
		index++;
	}
	qDebug() << "new" << new_pos << "old" << old_pos;

	// добавить новый в список
	if (old_pos < 0)
	{
		emit beginInsertRows(QModelIndex(), new_pos, new_pos);
		_list.insert(new_pos, *s);
		emit endInsertRows();
	}
	else
	{
		// Если позиция не изменилась
		if (old_pos == new_pos)
		{
			_list[old_pos] = *s;
			// обновили один элемент
			emit dataChanged(createIndex(old_pos, 0), createIndex(old_pos, 0));
		}
		else
		{
			// При обновлении статистики, позиция может изменится только в право
			// Так как число вхождений только растет
			// Вставляем элемент в новую позицию, сдвигая остальные вправо
			word_stat new_s = *s;
			word_stat tmp;
			qDebug() << "new pos " << new_pos << "size" << _list.size();
			for (int i = new_pos; i <= old_pos; i++)
			{
				tmp = _list[i];
				_list[i] = new_s;
				new_s = tmp;
			}
			// сдвинули все элементы начиная с new_pos
			emit dataChanged(createIndex(new_pos, 0), createIndex(_list.size() - 1, 0));
		}
	}

	if (_list.size() > SIZE)
	{
		emit beginRemoveRows(QModelIndex(), SIZE, _list.size() - 1);
		// _list.erase(_list.end() - 1);
		emit endRemoveRows();
	}
}

void TopWordsList::read_file_end()
{
	_curr_percent = 100;
	emit progress_updated(_curr_percent);
	emit load_end();
}
