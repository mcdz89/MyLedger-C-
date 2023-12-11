#include "Institution.hpp"
#include "DB.hpp"

#include <stdio.h>

Institution::Institution(std::string name, int num_acc) {
    // Instantiate DB object
    std::unique_ptr<DB> db(new DB());

    // Increment the number of institutions
    num_acc += 1;

    try { 
        // Insert Institution into DB
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        w.exec("INSERT INTO institutions (name, num_acc) VALUES ('" + name + "', " + std::to_string(num_acc) + ")");
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

int Institution::countInst() {
    // Instantiate DB object
    std::unique_ptr<DB> db(new DB());

    try {
        // Check if any Institutions exist by counting the rows
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT COUNT(*) FROM institutions");
        int count = r[0][0].as<int>();

        // Return number of institutions
        return count;
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

std::string Institution::setName() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Prompt the user for an Institution name
    std::cout << std::endl;
    std::cout << "Enter the Institution name: ";
    std::getline(std::cin, m_name);

    // Strip all leading and trailing whitespace
    m_name = trim(m_name);

    // Filter string input
    m_name = filterStrInput(m_name);

    // Return the entered Institution name
    return m_name;
}

int Institution::selectInst() {
    std::map<int, std::string> inst;
    int sel = 0;

    // Add a record to the map: 0, Add new Institution
    inst.insert({0, "Add new Institution"});
    
    // Instantiate DB object
    std::unique_ptr<DB> db(new DB());

    try {
        // Check if any Institutions exist by counting the rows
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT * FROM institutions ORDER BY name ASC");
        
        for (unsigned int i = 0; i < r.size(); i++) {
            inst.insert({r[i][0].as<int>(), r[i][1].as<std::string>()});
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

    int index = 0;
    for (auto& [key, value] : inst) {
        std::cout << ++index << " - " << value << "\n";
    }
    
    std::cout << "Select an Institution: ";
    std::cin >> sel;

    // If the selection is not 0, use the selected index to get the ID of the selected Institution from the map
    int selectedID = 0;
    if (sel > 1) {
        auto it = std::next(inst.begin(), sel - 1); // Adjust the index to match the map's iterator
        selectedID = it->first; // Get the ID from the map
    } else {
        selectedID = 0;
    }

    return selectedID;
}

std::string Institution::getName(int id) {
    // Instantiate DB object
    std::unique_ptr<DB> db(new DB());

    try {
        // Get the Institution name from the DB
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT name FROM institutions WHERE id = " + std::to_string(id));

        if (!r.empty()) {
            return r[0][0].as<std::string>();
        } else {
            throw std::runtime_error("Institution not found");
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
}

int Institution::getID(std::string name) {
    int id = 0;

    // Instantiate DB object
    std::unique_ptr<DB> db(new DB());

    try {
        // Get the ID from the DB
        pqxx::connection c(db->conninfo().c_str());
        pqxx::work w(c);
        pqxx::result r = w.exec("SELECT id FROM institutions WHERE name = '" + name + "'");

        if (!r.empty()) {
            id = r[0][0].as<int>();
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

    return id;
}

std::string Institution::filterStrInput(const std::string & input) {
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
std::string Institution::trim(const std::string& str) {
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