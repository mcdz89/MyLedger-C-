#include "Date.hpp"
#include "DB.hpp"

#include <ctime>

void Date::stringToVec(std::vector<int> & date, std::string & dateStr) {
    // Convert string date into vector
    std::stringstream ss(dateStr);
    std::string token;
    while (std::getline(ss, token, '-')) {
        int value = std::stoi(token);
        date.push_back(value);
    }
}

std::string Date::vecToString(std::vector<int> & date) {
    std::stringstream ss;
    for (unsigned int i = 0; i < sizeof(date); i++) {
        ss << date[i];
        if (i < sizeof(date) - 1) {
            ss << "-";
        }
    }
    return ss.str();
}

std::string Date::hr_date(std::vector<int> & date) {
    std::string name = "";

    // Instantiate DB connection
    std::unique_ptr<DB> db(new DB());

    try {
        // Get the Institution name from the DB
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT name FROM months WHERE id = " + std::to_string(date[1]));

        if (!r.empty()) {
            name = r[0][0].as<std::string>();
        }
    } catch (const pqxx::sql_error& e) {
        // Handle SQL error
        std::cerr << "SQL error: " << e.what() << std::endl;
        throw;
    } catch (const std::exception& e) {
        // Handle other exceptions
        std::cerr << "Exception: " << e.what() << std::endl;
        throw;
    }
 
    std::stringstream ss;
    ss << date[0] << "-" << name << "-" << date[2];
    return ss.str();
}

bool Date::checkDate(std::vector<int> &dateVec) {
    // Get current year, month, and day
    std::time_t now = std::time(nullptr);
    std::tm *current_time = std::localtime(&now);
    int current_year = current_time->tm_year + 1900;
    int current_month = current_time->tm_mon + 1;
    int current_day = current_time->tm_mday;

    // Verify the user input year is between 1900 and the current year
    if (dateVec[0] < 1900 || dateVec[0] > current_year) {
        std::cout << "Invalid year" << std::endl;
        return false;
    }

    // Verify the user input month is between 1 and 12
    if (dateVec[1] < 1 || dateVec[1] > 12) {
        std::cout << "Invalid month" << std::endl;
        return false;
    }

    // If the user input year is the current year, verify the user input month is not greater than the current month
    if (dateVec[0] == current_year && dateVec[1] > current_month) {
        std::cout << "Invalid month" << std::endl;
        return false;
    }

    // Verify the user input day based on the month of the year taking leap year into account
    int daysInMonth = 0;
    switch (dateVec[1]) {
        case 1: // January
        case 3: // March
        case 5: // May
        case 7: // July
        case 8: // August
        case 10: // October
        case 12: // December
            daysInMonth = 31;
            break;
        case 4: // April
        case 6: // June
        case 9: // September
        case 11: // November
            daysInMonth = 30;
            break;
        case 2: // February
            if ((dateVec[0] % 4 == 0 && dateVec[0] % 100 != 0) || dateVec[0] % 400 == 0) {
                // Leap year
                daysInMonth = 29;
            } else {
                // Not a leap year
                daysInMonth = 28;
            }
            break;
        default:
            return false;
    }

    // Check if the user input day is between 1 and the number of days in the month
    if ((dateVec[2] < 1 || dateVec[2] > daysInMonth) && dateVec[0] == current_year && dateVec[1] == current_month) {
        std::cout << "Invalid day!" << std::endl;
        return false;
    }

    // If the user input year is the current year, and the user input month is the current month,
    // verify the user input day is not greater than the current day
    if (dateVec[0] == current_year && dateVec[1] == current_month && dateVec[2] > current_day) {
        std::cout << "Invalid day!" << std::endl;
        return false;
    }

    return true;
}