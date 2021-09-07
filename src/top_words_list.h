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
	TopWordsModel(const word_stat &stat) : count(stat.count), word(stat.word) {}

	uint count;
	QString word;
};

/**
 * Список моделей для отображения в QML
 * хранит и выдает топ 15 результатов
*/
class TopWordsList : public QAbstractListModel
{
	Q_OBJECT
signals:
	void load_end();
	void load_start();
	void progress_updated(uint percent);

public:
	TopWordsList(QObject *parent = nullptr);

	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

public slots:
	void read_file_begin(uint size);
	void read_chank(uint size);
	void update_stat(const word_stat *s);
	void read_file_end();

private:
	// list of pointers to word_stats
	QList<const word_stat *> _list;
	static constexpr uint SIZE = 15;
	uint max_size;
	uint read_size;
	uint curr_percent;
};

#endif /*TOP_WORDS_LIST_H*/
