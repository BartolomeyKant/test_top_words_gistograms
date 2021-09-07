#include "src/top_words.h"

#include <QFile>
#include <QRegularExpression>
#include <stdexcept>

TopWords::TopWords(const QString &file, QObject *parent) : QObject(parent)
{
	_file = file;
}

void TopWords::solve()
{
	QFile f(_file);

	f.open(QFile::ReadOnly);
	if (!f.isOpen())
	{
		throw std::runtime_error("unable to open file for reading " +
								 f.errorString().toStdString());
	}
	emit file_loaded(f.size());

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
			auto &s = _words[w];
			if (s.word.isEmpty())
			{
				s.word = w;
			}
			s.count++;
			emit stat_updated(&s);
		}
		emit chank_readed(buf.size());
	}
	f.close();
	emit read_end();
}
