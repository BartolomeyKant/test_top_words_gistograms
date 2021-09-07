#include "src/top_words_list.h"

TopWordsList::TopWordsList(QObject *parent) : QAbstractListModel(parent)
{
}

int TopWordsList::rowCount(const QModelIndex &) const
{
	return _list.size() > SIZE ? SIZE : _list.size();
}

QVariant TopWordsList::data(const QModelIndex &index, int) const
{
	auto stat = _list[index.row()];
	return QVariant::fromValue(TopWordsModel(*stat));
}

void TopWordsList::read_file_begin(uint size)
{
	emit beginResetModel();
	_list.clear();
	emit endResetModel();

	max_size = size;
	read_size = 0;
	curr_percent = 0;
	emit load_start();
}

void TopWordsList::read_chank(uint size)
{
	read_size += size;
	auto prev = curr_percent;
	curr_percent = size / max_size;

	if (curr_percent > prev)
	{
		emit progress_updated(curr_percent);
	}
}

void TopWordsList::update_stat(const word_stat *s)
{
	// найти место для добавления нового word_stat
	int new_pos = _list.size();
	int old_pos = -1;
	int index = 0;
	for (auto i : _list)
	{
		if (i->count > s->count)
		{
			new_pos = index;
		}
		if (i->word == s->word)
		{
			old_pos = index;
		}
		index++;
	}

	// добавить новый в список
	if (old_pos < 0)
	{
		emit beginInsertRows(QModelIndex(), new_pos, new_pos);
		_list.insert(new_pos, s);
		emit endInsertRows();
	}
	else
	{
		if (old_pos == new_pos)
		{
			_list[old_pos] = s;
			emit dataChanged(createIndex(old_pos, 0), createIndex(old_pos, 0));
		}
		else {
			auto new_s = s;
			auto tmp = new_s;
			for (int i = new_pos; i < _list.size(); i++)
			{
				tmp = _list[i];
				_list[i] = new_s;
				new_s = tmp;
			}
			emit dataChanged(createIndex(0, new_pos), createIndex(0, _list.size() - 1));
		}
	}
	if (_list.size() > SIZE)
	{
		_list.erase(_list.end() - 1);
	}
}

void TopWordsList::read_file_end()
{
	curr_percent = 100;
	emit progress_updated(curr_percent);
	emit load_end();
}
