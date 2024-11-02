#include "header.h"

int main() {
    try {
        pqxx::connection conn("dbname=testdb user=lana password=123 hostaddr=127.0.0.1 port=5432");

        if (conn.is_open()) {
            std::cout << "Подключение к базе данных успешно!" << std::endl;

            CreateTestTable(conn);
            AddStudent(conn, "John", "1234567890");
            AddStudent(conn, "Jane", "0987654321");

            SqlSelectQueryBuilder query_builder;
            query_builder.AddColumns({"name", "phone"});
            query_builder.AddFrom("students");

            std::map<std::string, std::string> conditions = {{"id", "42"}, {"name", "John"}};
            query_builder.AddWhere(conditions);

            std::string query = query_builder.BuildQuery();
            std::cout << "Сформированный запрос: " << query << std::endl;

            ExecuteSelectQuery(conn, query);
        } else {
            std::cerr << "Не удалось подключиться к базе данных." << std::endl;
        }
    }
    catch (const std::exception &e) {
        std::cerr << "Ошибка подключения: " << e.what() << std::endl;
    }

    return 0;
}