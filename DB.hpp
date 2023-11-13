#ifndef DB_HPP
#define DB_HPP

#include <string>
#include <iostream>
#include <pqxx/pqxx>
#include <exception>
#include <iomanip>
#include <algorithm>

class DB {
private:
     std::string m_conninfo = "host=localhost port=5432 dbname=dbname user=username password=Password!";
public:
    DB (){}
    ~DB(){}

    const std::string &conninfo() const { return m_conninfo; }

    std::string replaceWS(std::string str) {
        std::replace(str.begin(), str.end(), ' ', '_');
        return str;
    }

    void catchSQLE(const pqxx::sql_error& e, std::string file, std::string func){
        std::cout << std::endl;
        std::cout << std::setw(30) << std::setfill('-') << "-" << std::endl;
        std::cerr << "File: " << file << std::endl;
        std::cerr << "Function: " << func << std::endl;
        std::cerr << "SQL error: " << e.what() << std::endl;
        std::cerr << "Query: " << e.query() << std::endl;
        std::cerr << "SQL state: " << e.sqlstate() << std::endl;
        std::cout << std::setw(30) << std::setfill('-') << "-" << std::endl;
    }

    void catchExc(const std::exception& e, std::string file, std::string func){
        std::cout << std::setw(30) << std::setfill('-') << "-" << std::endl;
        std::cerr << "File: " << file << std::endl;
        std::cerr << "Function: " << func << std::endl;
        std::cerr << "Stack trace: " << std::endl;
        std::cerr << "Exception: " << e.what() << std::endl;
        std::cout << std::setw(30) << std::setfill('-') << "-" << std::endl;
    }
};

#endif
