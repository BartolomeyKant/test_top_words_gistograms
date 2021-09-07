#include "src/top_words.h"

#include <QFile>
#include <QRegularExpression>
#include <stdexcept>
#include <QThread>
#include <QDebug>
#include <math.h>

TopWords::TopWords(const QString &file, QObject *parent) : QObject(parent)
{
	_file = file;
}

/**
 * Object for slow solving
*/
struct slower {
	slower(qint64 size) {
		time = 15 + log(size);
	}

	void slow() {
		QThread::msleep(time);
	}

	qint64 time;
};

void TopWords::solve()
{
	qDebug() << "start solve";
	QFile f(_file);

	f.open(QFile::ReadOnly);
	if (!f.isOpen())
	{
		throw std::runtime_error("unable to open file for reading " +
								 f.errorString().toStdString());
	}
	emit file_loaded(f.size());
	slower s(f.size());

	QRegularExpression re(R"([^a-zA-zа-яА-яёЁ]+)");
	QString sbuf;
	// читаем файл до конца
	while (!f.atEnd())
	{
		// читаем файл по одной строчке 
		auto buf = f.readLine();
		sbuf = QString::fromUtf8(buf);
		// для сплита используем регулярку, соответствующую любым символам не принадлежащих слову
		auto words = sbuf.split(re);
		if (words.empty())
		{
			continue;
		}
		for (auto &w : words)
		{
			if (w.isEmpty()) {
				continue;
			}
			auto &s = _words[w];
			if (s.word.isEmpty())
			{
				s.word = w;
			}
			s.count++;
			emit stat_updated(&s);
		}
		emit chank_readed(buf.size());
		s.slow();
	}
	f.close();
	emit read_end();
}
