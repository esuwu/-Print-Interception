-Print-Interception
Код на ветке alexandr_dex.

Usage: собранный бинарник запускать через sudo! (Иначе программа не сможет менять права доступа на файлы, на которые у нее нет прав). В Parser.cpp захардкожен путь к директории, в которую программа положит распарсенные данные (в define) #define FILE_TO_WRITE "/var/spool/DataFromPrinters"

Если не хочется запариваться, просто создайте такую же директорию в /var/spool

Описание программы:
Классы watcher и parser работают с общей потокобезопасной очередью thread_save_queue. Watcher мониторит изменения и появившиеся метафайлы добавляет в очередь thread_save_queue, в parser чекает появилось ли что-то в очереди и если да, забирает путь к метайфайлу оттуда. Парсит метафайл, копирует соответствующий ему d*** и отправляет в /var/spool/DataFromPrinters
