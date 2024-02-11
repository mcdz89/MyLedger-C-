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

// Check if Accounts exist
int Account::checkAcc() const {
    // Instantiate DB connection
    std::unique_ptr<DB> db(new DB());

    try {
        // Check if any Accounts exist by counting the rows
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT COUNT(*) FROM accounts");
        int count = r[0][0].as<int>();

        // Return 1 if any Accounts exist, 0 otherwise
        return (count > 0) ? 1 : 0;
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

// Define a function to filter out specific characters from the input string
std::string Account::filterStrInput(const std::string & input) {
    // Initialize an empty string to store the filtered characters
    std::string filteredString;
    // Define the characters to be filtered out
    const std::string charsToFilter = "@#$%^*()_+=[]{}|/<>?";

    // Iterate through each character in the input string
    for (char c : input) {
        // If the character is not found in the charsToFilter string, add it to the filteredString
        if (charsToFilter.find(c) == std::string::npos) {
            filteredString += c;
        }
    }

    // Return the filtered string
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

/**********************************************
* Print Table:  Function to print a formatted *
* table based on the provided header and data *
**********************************************/
void Account::printTable(std::vector<std::string> & header, std::vector<Account *> data) {
    // Calculate the maximum width for each column in the header
    std::vector<size_t> headerColumnWidths = {13, 8, 6, 8, 8, 10, 9}; // Default column widths for the header
    std::vector<size_t> dataColumnWidths(7, 0); // Initialize vector to store data column widths
    std::vector<size_t> maxColumnWidths(7, 0); // Initialize vector to store maximum column widths

    for (const auto& row : data) {
        std::cout << "Balance: " << sizeof(row->m_balance) << std::endl;
    }

    // Calculate the maximum width for each column in the data
    for (const auto& row : data) {
        // Update the maximum width for each column
        dataColumnWidths[0] = std::max(dataColumnWidths[0], row->m_instName.size()); // Institution
        dataColumnWidths[1] = std::max(dataColumnWidths[1], row->m_accName.size());  // Account
        dataColumnWidths[2] = 10; // Type
        dataColumnWidths[3] = 13; // Opened / rDate
        dataColumnWidths[4] = 10; // Interest
        dataColumnWidths[5] = 9; // Active
        dataColumnWidths[6] = std::max(dataColumnWidths[6], sizeof(row->m_balance)); // Balance
    }

    // Calculate the maximum width between dataColumnWidths and headerColumnWidths for each column
    for (size_t i = 0; i < 7; ++i) {
        maxColumnWidths[i] = std::max(dataColumnWidths[i], headerColumnWidths[i]);
    }
    // Print out a formatted Header with a box around it made of dashes and pluses at the intersections
    for (size_t i = 0; i < header.size(); i++) {
        std::cout << "+";
        for (size_t j = 0; j < maxColumnWidths[i]+1; j++) {
            std::cout << "-";
        }
    }
    std::cout << "+" << std::endl;

    // Print out the Header vector
    for (size_t i = 0; i < header.size(); i++) {
        std::cout << "| " << std::left << std::setw(static_cast<int>(maxColumnWidths[i])) << header[i];
    }
    std::cout << "|" << std::endl;

    // Print dashed line below the header
    for (size_t i = 0; i < header.size(); i++) {
        std::cout << "+";
        for (size_t j = 0; j < maxColumnWidths[i]+1; j++) {
            std::cout << "-";
        }
    }
    std::cout << "+" << std::endl;

    // Print out the data rows
    for (const auto& row : data) {
        //long double balance = stold(row->m_balanceStr);
        std::cout << "| ";
        std::cout << std::left << std::setw(static_cast<int>(maxColumnWidths[0])) << row->m_instName << "| ";
        std::cout << std::left << std::setw(static_cast<int>(maxColumnWidths[1])) << row->m_accName << "| ";
        std::cout << std::left << std::setw(static_cast<int>(maxColumnWidths[2])) << row->m_typeStr << "| ";
        std::cout << std::left << std::setw(static_cast<int>(maxColumnWidths[3])) << row->m_rDate << "| ";
        std::cout << std::left << std::setw(static_cast<int>(maxColumnWidths[4])) << row->m_interestStr << "| ";
        std::cout << std::left << std::setw(static_cast<int>(maxColumnWidths[5])) << row->m_activeStr << "| ";
        std::cout << std::left << "$";
        std::cout << std::right << std::setw(static_cast<int>(maxColumnWidths[6] - 2)) << row->m_balance << " |";
        std::cout << std::endl;
    }

    // Print dashed line below the data rows
    for (size_t i = 0; i < header.size(); i++) {
        std::cout << "+";
        for (size_t j = 0; j < maxColumnWidths[i]+1; j++) {
            std::cout << "-";
        }
    }
    std::cout << "+" << std::endl;
}
  
void Account::viewAcc() {
    // Define the header for the table
    std::vector<std::string> header = {"Institution", "Account", "Type", "Opened", "Interest", "Active", "Balance"};
    
    // Create a vector to store Account pointers
    std::vector<Account *> data;
    
    // Create a vector to store date components
    std::vector<int> dateVec;
  
    try {
        // Create smart pointers to class objects
        std::unique_ptr<DB> db = std::make_unique<DB>();
        std::unique_ptr<Date> date = std::make_unique<Date>();

        // Instantiate DB connection
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);

        // Formulate a query that retrieves account data along with related institution and account type information
        std::string query = R"(
            SELECT 
                accounts.institution, 
                accounts.type, 
                accounts.acc_id, 
                accounts.active, 
                accounts.balance, 
                accounts.interest, 
                accounts.apy, 
                accounts.opened,
                institutions.name,
                acc_type.name AS typeName
            FROM accounts
            LEFT JOIN institutions ON accounts.institution = institutions.id
            LEFT JOIN acc_type ON accounts.type = acc_type.id
        )";

        // Execute the query and store the result
        pqxx::result r = w.exec(query);

        // Loop through the result set and process each row
        for (const auto& row : r) {
            // Extract the opened date as a string
            std::string dateStr = row["Opened"].as<std::string>();

            // Convert the opened date to a vector of integers
            dateVec.clear();
            date->stringToVec(dateVec, dateStr);
            
            // Convert the vector of date components to a human-readable date string
            std::string r_Date = date->hr_date(dateVec);

            // Create an Account object and add it to the data vector
            Account * Acct = new Account(
                row["Name"].as<std::string>(),
                row["acc_id"].as<std::string>(),
                row["typeName"].as<std::string>(),
                r_Date,
                (row["Interest"].as<int>() == 1) ? "Yes" : "No",
                (row["Active"].as<int>() == 1) ? "Yes" : "No",
                row["Balance"].as<long double>()
            );
            data.push_back(Acct);
        }

        // Print the table using the header and data
        printTable(header, data);
    } catch (const pqxx::sql_error& e) {
        // Catch and handle any SQL errors
        std::unique_ptr<DB> db = std::make_unique<DB>();
        db->catchExc(e, __FILE__, __func__);
        throw;
    } catch (const std::exception& e) {
        // Catch and handle any other exceptions
        std::unique_ptr<DB> db = std::make_unique<DB>();
        db->catchExc(e, __FILE__, __func__);
        throw;
    }
}
