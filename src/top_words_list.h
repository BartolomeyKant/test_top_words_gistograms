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
	Q_PROPERTY(float curr_progress READ curr_progress NOTIFY curr_progress_changed)

signals:
	void load_end();
	void load_start();
	void curr_progress_changed(float percent);
	void max_value_changed(uint64_t value);

public:
	TopWordsList(QObject *parent = nullptr);

	QHash<int, QByteArray> roleNames() const override;
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	uint max_value() const { return _max_value; }
	float curr_progress() const { return _curr_progress; }

public slots:
	void read_file_begin(qint64 size);
	void read_chank(qint64 size);
	void update_stat(const word_stat *s);
	void read_file_end();

private:
	void update_min();
	// Список указателей на слова
	QList<word_stat> _list;
	static constexpr uint SIZE = 15;
	uint64_t _max_size = 0;
	uint64_t _read_size = 0;
	float _curr_progress = 0;
	uint64_t _max_value = 0;
	uint64_t _min_value = 0;
};

#endif /*TOP_WORDS_LIST_H*/
