# lr3-inf
Реализую на MySQL Connector/C++.
Вариант 5
Цель: Разработать базу данных для учета задач, их приоритета и сроков
выполнения, а также для управления состоянием задач.
Задание
1. Создание базы данных
Создайте базу данных с именем task_management.
2. Создание таблиц
Создайте таблицу tasks для хранения информации о задачах:
 Поля: id (уникальный идентификатор задачи), title (название задачи),
priority (приоритет задачи, например, «Высокий», «Средний»,
«Низкий»), due_date (срок выполнения задачи), status (статус задачи,
например, «Выполнена», «В процессе», «Отложена»).
Создайте таблицу users для хранения информации о пользователях,
назначенных на выполнение задач:
 Поля: id (уникальный идентификатор пользователя), name (имя
пользователя).
Создайте таблицу task_assignments для учета назначений задач
пользователям:
 Поля: assignment_id (уникальный идентификатор назначения), task_id
(идентификатор задачи), user_id (идентификатор пользователя).
3. Связи между таблицами
Установите связь между таблицами task_assignments и tasks через внешний
ключ task_id, чтобы каждое назначение задачи было связано с конкретной
задачей.
Установите связь между таблицами task_assignments и users через внешний
ключ user_id, чтобы каждое назначение задачи было связано с конкретным
пользователем.
4. Добавление данных
Заполните таблицу users данными о нескольких пользователях, включая их
имена.
Заполните таблицу tasks данными о нескольких задачах, указав их приоритет,
срок выполнения и статус.
Заполните таблицу task_assignments данными о назначениях задач
пользователям.
5. Запросы
Напишите SQL-запрос для получения списка всех задач с их приоритетом и
сроками выполнения.
Напишите SQL-запрос для получения всех задач определенного
пользователя.
Напишите SQL-запрос для поиска задач с высоким приоритетом, срок
выполнения которых истекает в ближайшие 7 дней.
Напишите SQL-запрос для получения количества задач в каждом статусе.
6. Дополнительное задание (по желанию)
Реализуйте отчет о задачах, которые просрочены, и пользователей,
ответственных за их выполнение.
Напишите запрос для обновления статуса задачи по ее идентификатору.
Создайте запрос для удаления задачи из списка задач, при этом проверьте,
что удаление не затрагивает другие данные.
