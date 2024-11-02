#include "header.h"

int main() {
    try {

        pqxx::connection conn("dbname=testdb user=lana password=123 hostaddr=127.0.0.1 port=5432");

        if (conn.is_open()) {
            std::cout << "Подключение к базе данных успешно!" << std::endl;

            CreateTestTable(conn);

            SqlSelectQueryBuilder query_builder;
            query_builder.AddColumn("name").AddColumn("phone");
            query_builder.AddFrom("students");
            query_builder.AddWhere("id","42").AddWhere("name","John");

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