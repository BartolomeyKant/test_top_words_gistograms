# Задание
Построить гистограмму на top-15 (по количество вхождений) слов в выбранном файле произвольного размера. После выбора файла должен быть виден прогресс обработки файла. Хорошим бонусом станет обновление гистограммы в условно реальном времени (без видимых продолжительных задержек). Топ 15 слов, должны быть, отсортированы по алфавиту. Подсчет слов и построение UI должны быть разделены минимум на два разных потока. Важно учитывать соразмерность столбцов гистограммы и возможность их вместить в видимой области окна приложения. В остальном, требований к визуальной части гистограммы нет (на усмотрение разработчика). Приложение должно быть реализовано на базе фреймворка Qt. Для UI использовать QML.

# Сборка
Разработка и тестирование выполнялись на машине с ОС Linux

Для сборки, перейти в директорию с проектом и выполнить конифгурацию `cmake`ом.

```
cd test_top_words_gistograms
mkdir build
cd build
cmake ..
make -j $(nproc)
```

# Запуск
Для выбора файла я не сделал файловый диалог, файл передается через аргументы командной строки.
В проекте лежит какой-то тестовый файл `test.file`, можно использовать его.

```
./top_words ../test.file
```

Запустится графическое приложение. В появившемся окне отобразится гистограмма топ 15 слов из файла, с сортировкой по алфавиту. Гистограмма обновляется в режиме рального времени, внизу экрана есть небольшой прогресс бар, обработки файла.
Все.