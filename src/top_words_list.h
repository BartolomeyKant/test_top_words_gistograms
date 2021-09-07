#ifndef TOP_WORDS_LIST_H
#define TOP_WORDS_LIST_H

#include <QObject>
#include <QAbstractListModel>

#include "src/top_words.h"

// модель для представления слова
class TopWordsModel
{
	Q_GADGET
	Q_PROPERTY(uint count MEMBER count)
	Q_PROPERTY(QString word MEMBER word)

public:
	TopWordsModel() = default;
	TopWordsModel(const word_stat &stat) : count(stat.count), word(stat.word) {}

	uint count;
	QString word;
};

Q_DECLARE_METATYPE(TopWordsModel);
Q_DECLARE_METATYPE(uint64_t);

/**
 * Список моделей для отображения в QML
 * хранит и выдает топ 15 результатов
*/
class TopWordsList : public QAbstractListModel
{
	Q_OBJECT
	Q_PROPERTY(uint max_value READ max_value NOTIFY max_value_changed)

signals:
	void load_end();
	void load_start();
	void progress_updated(uint percent);
	void max_value_changed(uint64_t value);

public:
	TopWordsList(QObject *parent = nullptr);

	QHash<int, QByteArray> roleNames() const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	uint max_value() const { return _max_value; }

public slots:
	void read_file_begin(qint64 size);
	void read_chank(qint64 size);
	void update_stat(const word_stat *s);
	void read_file_end();

private:
	// Список указателей на слова
	// NOTE! хранить сырые указатели не хорошо
	QList<word_stat> _list;
	static constexpr uint SIZE = 15;
	uint64_t _max_size;
	uint64_t _read_size;
	uint _curr_percent;
	uint64_t _max_value;
};

#endif /*TOP_WORDS_LIST_H*/
