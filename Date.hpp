#ifndef DATE_HPP
#define DATE_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <memory>
#include <pqxx/pqxx>

class Date {
private:
    /* data */
public:
    Date(){}
    ~Date(){}

    // Convert string date into vector
    void stringToVec(std::vector<int> & date, std::string & dateStr);
    std::string vecToString(std::vector<int> & date);
    std::string hr_date(std::vector<int> & date);
    bool checkDate(std::vector<int> & date);
};

#endif