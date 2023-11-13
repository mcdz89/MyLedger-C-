#include "Account.hpp"
#include "DB.hpp"
#include "Date.hpp"

Account::Account(int inst, int type, std::string & name, int active, long double balance, int interest, long double apy, std::string & rDate, int day, int month, int year) {
    // Instantiate DB object
    std::unique_ptr<DB> db(new DB());

    // Instantiate DB connection
    try {
        // Insert Account into DB
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        w.exec("INSERT INTO accounts (institution,type,acc_id,active,balance,interest,apy,opened,day,month,year) VALUES (" + std::to_string(inst) + ", " + std::to_string(type) + ", '" + name + "', " + std::to_string(active) + ", " + std::to_string(balance) + ", " + std::to_string(interest) + ", " + std::to_string(apy) + ", '"+ rDate + "', " + std::to_string(day) + ", " + std::to_string(month) + ", " + std::to_string(year) + ")");
        w.commit();
    } catch (const pqxx::sql_error& e) {
        // Handle SQL error
        db->catchSQLE(e, __FILE__, __func__);
        throw;
    } catch (const std::exception& e) {
        // Handle other exceptions
        db->catchExc(e, __FILE__, __func__);
        throw;
    }
    
}

std::string Account::setAccName() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Prompt the user for an Account name
    std::cout << std::endl << "Enter the Account name: ";
    std::getline(std::cin, m_accName);

    // Strip all leading and trailing whitespace
    m_accName = trim(m_accName);

    // Filter out special chars
    m_accName = filterStrInput(m_accName);

    // Return the entered Account name
    return m_accName;
}

int Account::setAccType() {
    std::map<int, std::string> accountTypes;
    
    // Instantiate DB object
    std::unique_ptr<DB> db(new DB());

    // Instantiate DB connection
    pqxx::connection c(db->conninfo().c_str());

    std::cout << std::endl << "Select an Account type: ";

    try {
        // Get the Account types from the DB
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT * FROM acc_type");

        // Store the account type values in a map
        for (const auto& row : r) {
            accountTypes.insert({row[0].as<int>(), row[1].as<std::string>()});
        }
    } catch (const pqxx::sql_error& e) {
        // Handle SQL error
        db->catchSQLE(e, __FILE__, __func__);
        throw;
    } catch (const std::exception& e) {
        // Handle other exceptions
        db->catchExc(e, __FILE__, __func__);
        throw;
    }

    // Iterate over the map and print the account types with an index
    int index = 0;
    std::cout << "\n";
    for (const auto& accountType : accountTypes) {
        std::cout << ++index << ". " << accountType.second << std::endl;
    }

    // Prompt the user for an Account type
    std::cin >> m_accType;

    // Iterate over the map using the user selection to get the accountTypes->first value
    int selectedID = 0;
    if (m_accType != 0) {
        auto it = std::next(accountTypes.begin(), m_accType - 1); // Adjust the index to match the map's iterator
        selectedID = it->first; // Get the ID from the map
    }

    // Return the selected account type
    return selectedID;
}

int Account::setAccInterest() {
    // Prompt the user to see if the Account accrues interest
    std::cout << std::endl  << "Does the Account accrue interest? (Y/N): ";
    char interest;
    std::cin >> interest;

    // Process the user's input
    m_interest = (interest == 'Y' || interest == 'y') ? 1 : 0;

    return m_interest;
}

int Account::setAccStatus() {
    // Prompt the user to see if the Account is active
    std::cout << "Is the Account active? (Y/N): ";
    char active;
    std::cin >> active;

    // Process the user's input
    m_active = (active == 'Y' || active == 'y') ? 1 : 0;

    return m_active;
}

long double Account::setAccBal(){
    // Prompt the user for an Account balance
    std::cout << "Enter the Account balance: ";
    std::cin >> m_balance;

    // Return the entered Account balance
    return m_balance;
}

void Account::setOpenDate(std::string & date){
    bool valid = false;
    std::vector<int> dateVec;

    date = "";

    // Create a smart pointer to a Date object
    std::unique_ptr<Date> dateObj(new Date());

    while (!valid) {
        // Prompt the user for an Account open date
        std::cout << "Enter the Account open date (YYYY-MM-DD): ";
        std::cin >> date;

        // Convert the date string to a vector
        dateVec.clear();
        dateVec.erase(dateVec.begin(), dateVec.end());
        dateObj->stringToVec(dateVec, date);

        // Check if the date is valid
        valid = dateObj->checkDate(dateVec);
    }
}

int Account::getId() const {
    // Create a smart pointer to a DB object
    std::unique_ptr<DB> db(new DB());

    try {
        // Instantiate DB connection
        pqxx::connection c(db->conninfo().c_str());

        // Get the ID from the DB
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT id FROM accounts ORDER BY id DESC LIMIT 1");

        // Return the ID
        return r[0][0].as<int>();
    } catch (const pqxx::sql_error& e) {
        // Handle SQL error
        db->catchSQLE(e, __FILE__, __func__);
        throw;
    } catch (const std::exception& e) {
        // Handle other exceptions
        db->catchExc(e, __FILE__, __func__);
        throw;
    }
}

std::string Account::getAccName(int id) const {
    std::string name;

    // Create a smart pointer to a DB object
    std::unique_ptr<DB> db(new DB());

    try {
        // Instantiate DB connection
        pqxx::connection c(db->conninfo().c_str());

        // Get the Account name from the DB
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT name FROM accounts WHERE id = " + std::to_string(id) + " ORDER BY name ASC");

        if (!r.empty()) {
            name = r[0][0].as<std::string>();
        } else {
            throw std::runtime_error("Account not found");
        }
    } catch (const pqxx::sql_error& e) {
        // Handle SQL error
        db->catchSQLE(e, __FILE__, __func__);
        throw;
    } catch (const std::exception& e) {
        // Handle other exceptions
        db->catchExc(e, __FILE__, __func__);
        throw;
    }

    return name;
}

std::string Account::getTypeStr(int id) const {
    std::string typeStr;
    // Create a smart pointer to a DB object
    std::unique_ptr<DB> db(new DB());

    try {
        // Instantiate DB connection
        pqxx::connection c(db->conninfo().c_str());

        // Get the Account type from the DB
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT name FROM acc_type WHERE id = " + std::to_string(id));

        // Return the Account type
        typeStr = r[0][0].as<std::string>();
    } catch (const pqxx::sql_error& e) {
        // Handle SQL error
        db->catchSQLE(e, __FILE__, __func__);
        throw;
    } catch (const std::exception& e) {
        // Handle other exceptions
        db->catchExc(e, __FILE__, __func__);
        throw;
    }

    return typeStr;
}

// Convert Long Double to Int
int Account::convertLDtoInt(long double ld) {
    // Convert a monetary value to a penny value
    m_balanceInt = static_cast<int>(ld * 100);
    return m_balanceInt;
}

// Convert Int to Long Double
long double Account::convertIntToLD(int intValue) {
    // Convert a penny value to a monetary value
    long double ldValue = static_cast<long double>(intValue) / 100;
    return ldValue;
}

std::string Account::filterStrInput(const std::string & input) {
    std::string filteredString;
    const std::string charsToFilter = "@#$%^*()_+=[]{}|/<>?";

    for (char c : input) {
        if (charsToFilter.find(c) == std::string::npos) {
            filteredString += c;
        }
    }

    return filteredString;
}

// trim leading and trailing whitespace
std::string Account::trim(const std::string& str) {
    std::string trimmedStr = str;
    // Trim leading whitespace
    trimmedStr.erase(trimmedStr.begin(), std::find_if(trimmedStr.begin(), trimmedStr.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    // Trim trailing whitespace
    trimmedStr.erase(std::find_if(trimmedStr.rbegin(), trimmedStr.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), trimmedStr.end());
    return trimmedStr;
}