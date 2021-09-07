#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QThread>

#include "src/top_words.h"
#include "src/top_words_list.h"

int main(int argc, char **argv)
{
	QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication app(argc, argv);
	QQmlApplicationEngine engine;

	QString file;
	if (argc == 2) {
		file = argv[1];
	}
	else {
		qDebug() << "Usage: " << argv[0] << "<file name>";
		return 1;
	}

	// отдельный поток для расчета слов
	QThread words_thread;
	// объект для посчета количества слов
	TopWords words_solver(file);
	words_solver.moveToThread(&words_thread);

	TopWordsList word_list;
	qmlRegisterSingletonInstance(
		"org.bulsandhoofs.topwordlist", 0, 1, "TopWordList", &word_list);

	// connect signal slots
	QObject::connect(&words_solver, &TopWords::file_loaded, &word_list, &TopWordsList::read_file_begin, Qt::QueuedConnection);
	QObject::connect(&words_solver, &TopWords::chank_readed, &word_list, &TopWordsList::read_chank, Qt::QueuedConnection);
	QObject::connect(&words_solver, &TopWords::read_end, &word_list, &TopWordsList::read_file_end, Qt::QueuedConnection);
	QObject::connect(&words_solver, &TopWords::stat_updated, &word_list, &TopWordsList::update_stat, Qt::QueuedConnection);

	QObject::connect(&words_thread, &QThread::started, &words_solver, &TopWords::solve);

	words_thread.start();

	const QUrl url(QStringLiteral("qrc:/main.qml"));
	QObject::connect(
		&engine,
		&QQmlApplicationEngine::objectCreated,
		&app,
		[url](QObject *obj, const QUrl &objUrl)
		{
			if (!obj && url == objUrl)
				QCoreApplication::exit(-1);
		},
		Qt::QueuedConnection);

	engine.load(url);
	return app.exec();
}
