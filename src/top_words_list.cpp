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
	_curr_progress = 0;
	emit load_start();
}

void TopWordsList::read_chank(qint64 size)
{
	_read_size += size;
	auto prev = _curr_progress;
	_curr_progress = static_cast<float>(_read_size) /static_cast<float>( _max_size);

	if (_curr_progress > prev)
	{
		emit curr_progress_changed(_curr_progress);
	}
}

void TopWordsList::update_stat(const word_stat *s)
{
	// обновляем максимальное значение
	if (_max_value < s->count)
	{
		_max_value = s->count;
		emit max_value_changed(_max_value);
	}

	if (s->count < _min_value && _list.size() >= SIZE) {
		return;
	}

	// найти место для добавления нового word_stat
	int new_pos = _list.size();
	int old_pos = -1;
	int index = 0;
	for (auto& i : _list)
	{
		if (i.word.compare(s->word, Qt::CaseInsensitive) > 0)
		{
			new_pos = index;
			break;
		}
		if (i.word == s->word)
		{
			if (s->count == i.count) {
				return;
			}
			i = *s;
			// обновили один элемент
			emit dataChanged(createIndex(index, 0), createIndex(index, 0));
			update_min();
			return;
		}
		index++;
	}

	// добавить новый в список
	if (old_pos < 0)
	{
		emit beginInsertRows(QModelIndex(), new_pos, new_pos);
		_list.insert(new_pos, *s);
		emit endInsertRows();
	}

	if (_list.size() > SIZE)
	{
		// remove min element
		int min_index = 0;
		uint64_t min_val = _max_value;

		for (int i = 0; i < _list.size(); i++)
		{
			if (_list[i].count < min_val)
			{
				min_index = i;
				min_val = _list[i].count;
			}
		}

		emit beginRemoveRows(QModelIndex(), min_index, min_index);
		_list.removeAt(min_index);
		emit endRemoveRows();
	}
	update_min();
}


void TopWordsList::read_file_end()
{
	_curr_progress = 1;
	emit curr_progress_changed(_curr_progress);
	emit load_end();
}

void TopWordsList::update_min() {
	uint64_t min = _max_value;
	for (auto& s : _list) {
		if (s.count < min) {
			min = s.count;
		}
	}
	_min_value = min;
}
