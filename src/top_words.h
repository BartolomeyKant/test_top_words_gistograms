#ifndef TOP_WORDS_H
#define TOP_WORDS_H

#include <QObject>
#include <QString>
#include <QHash>

/**
 * Структура для хранения статистики по слову
*/
struct word_stat
{
	QString word;
	uint count;
};

/**
 * Читаем файл и считаем количество слов
*/
class TopWords : public QObject
{
	Q_OBJECT

signals:
	void file_loaded(uint size);
	void chank_readed(uint size);
	void read_end();
	void stat_updated(const word_stat *word);

public:
	/**
	 * Создаем объект для расчета топ файлов из файла
	*/
	TopWords(const QString &file, QObject *parent = nullptr);

public slots:
	/**
	 * Запуск решения
	*/
	void solve();

private:
	QHash<QString, word_stat> _words;
	QString _file;
};

#endif /*TOP_WORDS_H*/
