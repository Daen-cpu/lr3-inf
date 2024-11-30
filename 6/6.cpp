#include <iostream>
#include <mysql_driver.h>
#include <mysql_connection.h>
#include <cppconn/prepared_statement.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <string>

using namespace std;

// Функция  выполнения SQL-запросов
void executeSQL(sql::Connection *conn, const string &sql) {
    try {
        sql::Statement *stmt = conn->createStatement();
        stmt->execute(sql);
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "Ошибка SQL: " << e.what() << endl;
    }
}

// Функция  получения отчета о просроченных задачах
void getOverdueTasksReport(sql::Connection *conn) {
    try {
        sql::ResultSet *res = conn->createStatement()->executeQuery(
            "SELECT t.title, t.due_date, u.name AS user_name, t.status "
            "FROM tasks t "
            "JOIN task_assignments ta ON t.id = ta.task_id "
            "JOIN users u ON ta.user_id = u.id "
            "WHERE t.due_date < CURDATE() AND t.status != 'Выполнена'"
        );

        cout << "Отчет о просроченных задачах:\n";
        while (res->next()) {
            cout << "Задача: " << res->getString("title") << ", "
                 << "Срок выполнения: " << res->getString("due_date") << ", "
                 << "Ответственный: " << res->getString("user_name") << ", "
                 << "Статус: " << res->getString("status") << endl;
        }

        delete res;
    } catch (sql::SQLException &e) {
        cout << "Ошибка получения отчета о просроченных задачах: " << e.what() << endl;
    }
}

// Функция  обновления статуса задачи
void updateTaskStatus(sql::Connection *conn, int taskId, const string &newStatus) {
    try {
        sql::PreparedStatement *pstmt = conn->prepareStatement(
            "UPDATE tasks SET status = ? WHERE id = ?"
        );
        pstmt->setString(1, newStatus);
        pstmt->setInt(2, taskId);
        pstmt->executeUpdate();
        delete pstmt;
        cout << "Статус задачи с ID " << taskId << " обновлен на " << newStatus << endl;
    } catch (sql::SQLException &e) {
        cout << "Ошибка обновления статуса задачи: " << e.what() << endl;
    }
}

// Функция  удаления задачи
void deleteTask(sql::Connection *conn, int taskId) {
    try {
        // Сначала удаляем назначения назначения задачи
        sql::PreparedStatement *pstmt1 = conn->prepareStatement("DELETE FROM task_assignments WHERE task_id = ?");
        pstmt1->setInt(1, taskId);
        pstmt1->executeUpdate();
        delete pstmt1;

        // Удаляем саму задачу
        sql::PreparedStatement *pstmt2 = conn->prepareStatement("DELETE FROM tasks WHERE id = ?");
        pstmt2->setInt(1, taskId);
        pstmt2->executeUpdate();
        delete pstmt2;

        cout << "Задача с ID " << taskId << " удалена." << endl;
    } catch (sql::SQLException &e) {
        cout << "Ошибка удаления задачи: " << e.what() << endl;
    }
}

int main() {
    try {
        // Подключение к базе данных MySQL
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *conn = driver->connect("tcp://127.0.0.1:3306", "root", "password");  // представляем что задача реальная и у нас в самом деле что-то есть
        conn->setSchema("task_management");  // та, что по заданию

        // Получаем отчет о просроченных задачах
        getOverdueTasksReport(conn);

        // Обновление статуса задачи
        updateTaskStatus(conn, 2, "Выполнена");

        // Удаление задачи
        deleteTask(conn, 3);

        delete conn;  // заканчиваем сеанс
    } catch (sql::SQLException &e) {
        cout << "Ошибка подключения к базе данных: " << e.what() << endl;
    }

    return 0;
}
