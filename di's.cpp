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

// Функция  создания таблиц
void createTables(sql::Connection *conn) {
    string createTasksTable = "CREATE TABLE IF NOT EXISTS tasks ("
                              "id INT AUTO_INCREMENT PRIMARY KEY, "
                              "title VARCHAR(255) NOT NULL, "
                              "priority ENUM('Высокий', 'Средний', 'Низкий') NOT NULL, "
                              "due_date DATE NOT NULL, "
                              "status ENUM('Выполнена', 'В процессе', 'Отложена') NOT NULL)";
    
    string createUsersTable = "CREATE TABLE IF NOT EXISTS users ("
                              "id INT AUTO_INCREMENT PRIMARY KEY, "
                              "name VARCHAR(255) NOT NULL)";
    
    string createTaskAssignmentsTable = "CREATE TABLE IF NOT EXISTS task_assignments ("
                                       "assignment_id INT AUTO_INCREMENT PRIMARY KEY, "
                                       "task_id INT, "
                                       "user_id INT, "
                                       "FOREIGN KEY(task_id) REFERENCES tasks(id), "
                                       "FOREIGN KEY(user_id) REFERENCES users(id))";
    
    executeSQL(conn, createTasksTable);
    executeSQL(conn, createUsersTable);
    executeSQL(conn, createTaskAssignmentsTable);
}

// Функция  добавления данных в таблицу пользователей
void addUser(sql::Connection *conn, const string &name) {
    try {
        sql::PreparedStatement *pstmt = conn->prepareStatement("INSERT INTO users (name) VALUES (?)");
        pstmt->setString(1, name);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException &e) {
        cout << "Ошибка добавления пользователя: " << e.what() << endl;
    }
}

// Функция  добавления данных в таблицу задач
void addTask(sql::Connection *conn, const string &title, const string &priority, const string &due_date, const string &status) {
    try {
        sql::PreparedStatement *pstmt = conn->prepareStatement("INSERT INTO tasks (title, priority, due_date, status) VALUES (?, ?, ?, ?)");
        pstmt->setString(1, title);
        pstmt->setString(2, priority);
        pstmt->setString(3, due_date);
        pstmt->setString(4, status);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException &e) {
        cout << "Ошибка добавления задачи: " << e.what() << endl;
    }
}

// Функция  назначения задач пользователям
void assignTask(sql::Connection *conn, int task_id, int user_id) {
    try {
        sql::PreparedStatement *pstmt = conn->prepareStatement("INSERT INTO task_assignments (task_id, user_id) VALUES (?, ?)");
        pstmt->setInt(1, task_id);
        pstmt->setInt(2, user_id);
        pstmt->executeUpdate();
        delete pstmt;
    } catch (sql::SQLException &e) {
        cout << "Ошибка назначения задачи: " << e.what() << endl;
    }
}

// Функция  получения всех задач с их приоритетами и сроками
void getTasks(sql::Connection *conn) {
    try {
        sql::Statement *stmt = conn->createStatement();
        sql::ResultSet *res = stmt->executeQuery("SELECT title, priority, due_date FROM tasks");
        
        while (res->next()) {
            cout << "Название: " << res->getString("title") << ", "
                 << "Приоритет: " << res->getString("priority") << ", "
                 << "Срок выполнения: " << res->getString("due_date") << endl;
        }
        
        delete res;
        delete stmt;
    } catch (sql::SQLException &e) {
        cout << "Ошибка получения задач: " << e.what() << endl;
    }
}

// Функция  получения задач конкретного пользователя
void getUserTasks(sql::Connection *conn, const string &userName) {
    try {
        sql::PreparedStatement *pstmt = conn->prepareStatement("SELECT t.title, t.priority, t.due_date, t.status FROM tasks t "
                                                               "JOIN task_assignments ta ON t.id = ta.task_id "
                                                               "JOIN users u ON ta.user_id = u.id "
                                                               "WHERE u.name = ?");
        pstmt->setString(1, userName);
        
        sql::ResultSet *res = pstmt->executeQuery();
        
        while (res->next()) {
            cout << "Название: " << res->getString("title") << ", "
                 << "Приоритет: " << res->getString("priority") << ", "
                 << "Срок выполнения: " << res->getString("due_date") << ", "
                 << "Статус: " << res->getString("status") << endl;
        }
        
        delete res;
        delete pstmt;
    } catch (sql::SQLException &e) {
        cout << "Ошибка получения задач пользователя: " << e.what() << endl;
    }
}

// Функция  поиска задач с высоким приоритетом, срок которых истекает в ближайшие 7 дней
void getHighPriorityTasks(sql::Connection *conn) {
    try {
        sql::ResultSet *res = conn->createStatement()->executeQuery("SELECT title, priority, due_date FROM tasks "
                                                                   "WHERE priority = 'Высокий' AND due_date BETWEEN CURDATE() AND CURDATE() + INTERVAL 7 DAY");
        
        while (res->next()) {
            cout << "Название: " << res->getString("title") << ", "
                 << "Приоритет: " << res->getString("priority") << ", "
                 << "Срок выполнения: " << res->getString("due_date") << endl;
        }
        
        delete res;
    } catch (sql::SQLException &e) {
        cout << "Ошибка поиска задач: " << e.what() << endl;
    }
}

// Функция  подсчета задач по статусам
void countTasksByStatus(sql::Connection *conn) {
    try {
        sql::ResultSet *res = conn->createStatement()->executeQuery("SELECT status, COUNT(*) AS task_count FROM tasks GROUP BY status");
        
        while (res->next()) {
            cout << "Статус: " << res->getString("status") << ", "
                 << "Количество задач: " << res->getInt("task_count") << endl;
        }
        
        delete res;
    } catch (sql::SQLException &e) {
        cout << "Ошибка подсчета задач: " << e.what() << endl;
    }
}

int main() {
    try {
        sql::mysql::MySQL_Driver *driver = sql::mysql::get_mysql_driver_instance();
        sql::Connection *conn = driver->connect("tcp://127.0.0.1:3306", "root", "password");  // представим что реальную задачу делаем
        conn->setSchema("task_management"); 

        // Создаем таблицы
        createTables(conn);

        // Добавляем пользователей
        addUser(conn, "Иван Иванов");
        addUser(conn, "Мария Петрова");
        addUser(conn, "Сергей Сидоров");

        // Добавляем задачи
        addTask(conn, "Сдать лабу", "высочайший", "2024-30-11", "В процессе");
        addTask(conn, "Добить статью", "как выше", "2024-12-10", "В процессе");
        addTask(conn, "Начерта", "высокий", "2024-12-01", "Выполнена");

        // Назначаем задачи пользователям
        assignTask(conn, 1, 1); 
