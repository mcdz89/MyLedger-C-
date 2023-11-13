#ifndef INSTITUTION_HPP
#define INSTITUTION_HPP

#include <pqxx/pqxx>
#include <iostream>
#include <exception>
#include <string>

class Institution {
private:
    std::string m_name;
public:
    Institution(std::string name, int num_acc);
    Institution() {}
    ~Institution() {}

    int countInst();

    // Get the number of Insitutions in the DB
    int numInstitutions();

    // Prompt the usr for an Institution name
    std::string setName();

    // Get a list of all Institutions in the DB
    int selectInst();

    std::string getName(int id);
    int getID(std::string name);

    // Helper Methods
    std::string filterStrInput(const std::string & input);
    std::string trim(const std::string& str);
};

#endif