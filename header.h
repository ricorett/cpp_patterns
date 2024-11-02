#pragma once
#include <pqxx/pqxx>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

class SqlSelectQueryBuilder {
public:

    SqlSelectQueryBuilder& AddColumns(const std::vector<std::string>& columnsToAdd) noexcept {
        for (const auto& column : columnsToAdd) {
            columns.push_back(column);
        }
        return *this;
    }


    SqlSelectQueryBuilder& AddFrom(const std::string& table_name) {
        table = table_name;
        return *this;
    }

    SqlSelectQueryBuilder& AddWhere(const std::map<std::string, std::string>& kv) noexcept {
        for (const auto& pair : kv) {
            where_clauses.push_back(pair.first + "='" + pair.second + "'");
        }
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

void AddStudent(pqxx::connection& conn, const std::string& name, const std::string& phone) {
    try {
        pqxx::work txn(conn);
        std::string insert_query = "INSERT INTO students (name, phone) VALUES (" + txn.quote(name) + ", " + txn.quote(phone) + ");";
        txn.exec(insert_query);
        txn.commit();
        std::cout << "Студент " << name << " добавлен успешно." << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Ошибка при добавлении студента: " << e.what() << std::endl;
    }
}

void CreateTestTable(pqxx::connection& conn) {
    try {
        pqxx::work txn(conn);

        std::string create_table_query = R"(
            CREATE TABLE IF NOT EXISTS students (
                id SERIAL PRIMARY KEY,
                name VARCHAR(100),
                phone VARCHAR(20)
            );
        )";

        txn.exec(create_table_query);
        txn.commit();
        std::cout << "Таблица 'students' создана успешно." << std::endl;
    }
    catch (const std::exception &e) {
        std::cerr << "Ошибка при создании таблицы: " << e.what() << std::endl;
    }
}


void ExecuteSelectQuery(pqxx::connection& conn, const std::string& query) {
    try {
        pqxx::work txn(conn);
        pqxx::result res = txn.exec(query);
        std::cout << "Результаты запроса:" << std::endl;
        for (const auto& row : res) {
            for (const auto& field : row) {
                std::cout << field.c_str() << " ";
            }
            std::cout << std::endl;
        }
        // DEBUG
        std::cout << "Отладка: результат запроса - " << res.size() << " строк." << std::endl;
        txn.commit();
    }
    catch (const std::exception &e) {
        std::cerr << "Ошибка выполнения запроса: " << e.what() << std::endl;
    }
}
