#pragma once
#include <pqxx/pqxx>  // Подключение библиотеки pqxx
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

// Класс для построения SELECT-запросов
class SqlSelectQueryBuilder {
public:

    SqlSelectQueryBuilder& AddColumn(const std::string& column) {
        columns.push_back(column);
        return *this;
    }


    SqlSelectQueryBuilder& AddFrom(const std::string& table_name) {
        table = table_name;
        return *this;
    }


    SqlSelectQueryBuilder& AddWhere(const std::string& column, const std::string& value) {
        where_clauses.push_back(column + "=" + value);
        return *this;
    }


    std::string BuildQuery() const {
        std::ostringstream query;


        if (columns.empty()) {
            query << "SELECT *";
        } else {
            query << "SELECT ";
            for (size_t i = 0; i < columns.size(); ++i) {
                query << columns[i];
                if (i < columns.size() - 1) {
                    query << ", ";
                }
            }
        }


        query << " FROM " << table;


        if (!where_clauses.empty()) {
            query << " WHERE ";
            for (size_t i = 0; i < where_clauses.size(); ++i) {
                query << where_clauses[i];
                if (i < where_clauses.size() - 1) {
                    query << " AND ";
                }
            }
        }


        query << ";";

        return query.str();
    }

private:
    std::vector<std::string> columns;
    std::string table;
    std::vector<std::string> where_clauses;
};


void CreateTestTable(pqxx::connection& conn) {
    try {
        // Открываем транзакцию
        pqxx::work txn(conn);

        // SQL-запрос для создания таблицы
        std::string create_table_query = R"(
            CREATE TABLE IF NOT EXISTS students (
                id SERIAL PRIMARY KEY,
                name VARCHAR(100),
                phone VARCHAR(20)
            );
        )";

        // Выполняем запрос
        txn.exec(create_table_query);

        // Фиксируем транзакцию
        txn.commit();

        std::cout << "Таблица 'students' создана успешно." << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Ошибка при создании таблицы: " << e.what() << std::endl;
    }
}


void ExecuteSelectQuery(pqxx::connection& conn, const std::string& query) {
    try {
        // Открываем транзакцию
        pqxx::work txn(conn);

        // Выполняем SELECT-запрос
        pqxx::result res = txn.exec(query);

        // Выводим результаты запроса
        std::cout << "Результаты запроса:" << std::endl;
        for (const auto& row : res) {
            for (const auto& field : row) {
                std::cout << field.c_str() << " ";
            }
            std::cout << std::endl;
        }

        txn.commit();
    }
    catch (const std::exception &e) {
        std::cerr << "Ошибка выполнения запроса: " << e.what() << std::endl;
    }
}
