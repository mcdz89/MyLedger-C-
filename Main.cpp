#include <iostream>
#include <string>
#include <exception>
#include <vector>
#include <map>
#include <memory>
#include <iomanip>

#include "Menu.hpp"
#include "MenuCommand.hpp"
#include "DB.hpp"
#include "Institution.hpp"
#include "Account.hpp"
#include "Date.hpp"

std::string printMenu(std::string& complete);
void printTable();
void addAcc();

int main() {
    system("clear");
    std::string complete = "";

    std::cout << std::setprecision(2) << std::showpoint << std::fixed;
    
    try {
        while (complete != "q") {
            complete = printMenu(complete);
        }
    } catch (const std::exception& e) {
        std::cout << "Exception caught: " << e.what() << std::endl;
        return -1;
    }

}

std::string printMenu(std::string& complete) {
    /*********************************
    * Main Menu
    * 1 Accounts
	* - 1 Add Bank Account
    ********************************/
   // Menu object calls
    Menu* mainMenu;   // Displays the Main Menu
    Menu* accMenu;    // Displays the Account Menu

    mainMenu = new Menu("0", "Main Menu", 1, "q");
    accMenu = new Menu("1", "Accounts", 1, "q");
    mainMenu->addMenuItem(accMenu);
    accMenu->addMenuItem(new MenuCommand("1", "Add Bank Account", 1, addAcc));

    complete = mainMenu->execute();

    return complete;
}

/****************************
* Display a formatted table *
****************************/
/****************************
* Display a formatted table *
****************************/
void printTable(const std::vector<std::vector<std::string>>& data) {
    // Make a unique pointer to a DB object
    std::unique_ptr<DB> db(new DB());
    
    // Find the maximum width for each column
    std::vector<size_t> maxColumnWidths(data[0].size(), 0);
    for (const auto& row : data) {
        for (size_t i = 0; i < row.size(); ++i) {
            maxColumnWidths[i] = std::max(maxColumnWidths[i], row[i].size());
        }
    }
    
    // Print the first row with dashes and plus signs
    for (size_t i = 0; i < data[0].size(); ++i) {
        std::cout << " " << std::left << std::setw(static_cast<int>(maxColumnWidths[i]) + 1) << data[0][i];
        if (i < data[0].size() - 1) {
            std::cout << " | ";
        }
    }
    std::cout << "\n";
    
    // Print the dashes below the first row
    for (size_t i = 0; i < data[0].size(); ++i) {
        std::cout << std::left << std::setw(static_cast<int>(maxColumnWidths[i]) + 2) << std::setfill('-') << "-" << std::setfill(' ');
        if (i < data[0].size() - 1) {
            std::cout << "-+-";
        }
    }
    std::cout << "\n";
    
    // Print the rest of the data
    try {
        for (size_t rowIdx = 1; rowIdx < data.size(); ++rowIdx) {
            for (size_t i = 0; i < data[rowIdx].size(); ++i) {
                if (i == 6){
                    // Convert data[rowIdx][i] to a long double and only have two decimal places
                    std::cout << " " << std::left << std::setw(static_cast<int>(maxColumnWidths[i]) + 1) << std::fixed << std::setprecision(2) << std::stold(data[rowIdx][i]);
                } else {
                    std::cout << " " << std::left << std::setw(static_cast<int>(maxColumnWidths[i]) + 1) << std::fixed << std::setprecision(2) << data[rowIdx][i];
                }
                
                if (i < data[rowIdx].size() - 1) {
                    std::cout << " | ";
                }
            }
            std::cout << "\n";
        }
    } catch (const std::exception& e) {
        db->catchExc(e, __FILE__, __func__);
    }
    

}


/********************************************************************
* addAcc(): Starting point for adding a new Institution and Account *
********************************************************************/
void addAcc() {
    char verify = 'x';
    std::vector<int> dateVec;
    bool instExists = false;
    bool complete = false;
    std::vector<std::vector<std::string>> table;

    // Instantiate pointers to class objects
    std::unique_ptr<Institution> inst = std::make_unique<Institution>();
    std::unique_ptr<Account> acc = std::make_unique<Account>();
    std::unique_ptr<Date> date = std::make_unique<Date>();
    std::unique_ptr<DB> db = std::make_unique<DB>();

    while (!complete) {
        std::string dateStr = "";
        std::string instName = "";
        int instSel = 0;

        system("clear");

        // Print a message to indicate that we are adding an account
        std::cout << "Add Account\n";

        /* This sections covers adding a new Insitutitom */

        if (inst->countInst() == 0) {
            // If an institution does not exist, add one
            instName = inst->setName();
            instExists = true;
        } else if (instExists) {
            // Else if an institution does exist, select one
            instSel = inst->selectInst();
            instName = inst->getName(instSel);
            instExists = false;
        } else {
            // Else if an institution exists, and user wants to add another
            instName = inst->setName();
            instExists = true;
        }

        /* This section covers adding a new Account */

        // Call the Account's set methods
        std::string accName = acc->setAccName();
        int accType = acc->setAccType();
        int accInterest = acc->setAccInterest();
        int accStatus = acc->setAccStatus();
        long double accBal = acc->setAccBal();
        acc->setOpenDate(dateStr);
        std::cout << std::endl;
        
        // Convert string date into vector
        dateVec.clear();
        dateVec.erase(dateVec.begin(), dateVec.end());
        date->stringToVec(dateVec, dateStr);

        // Format a human readable date
        std::string hr_date = date->hr_date(dateVec);

        // Set human readable data for Account info
        std::string accTypeStr = (accType == 1) ? "Checking" : "Savings";

        std::string accInterestStr = (accInterest == 1) ? "Yes" : "No";

        std::string accStatusStr = (accStatus == 1) ? "Active" : "Inactive";

        table = {{"Institution","Account","Open Date","Type","Interest","Status","Balance"}, {instName, accName, hr_date, accTypeStr, accInterestStr, accStatusStr, std::to_string(accBal)}};

        // Display table
        printTable(table);

        /* Print out the Account information
        std::cout << "| Institution |";
        std::cout << " Account |";
        std::cout << " Open Date |";
        std::cout << " Type |";
        std::cout << " Interest |";
        std::cout << " Status |";
        std::cout << " Balance |\n";

        std::cout << "|";
        std::cout << std::setfill('-') << std::setw(0) << "|";
        std::cout << std::setfill('-') << std::setw(0) << "|";
        std::cout << std::setfill('-') << std::setw(0) << "|";
        std::cout << std::setfill('-') << std::setw(0) << "|";
        std::cout << std::setfill('-') << std::setw(0) << "|";
        std::cout << std::setfill('-') << std::setw(0) << "|";
        std::cout << std::setfill('-') << std::setw(0) << "|\n";

        std::cout << "| " << std::left << std::setw(0) << instName << " | ";
        std::cout << std::left << std::setw(0) << accName << " | ";
        std::cout << std::left << std::setw(0) << hr_date << " | ";
        std::cout << std::left << std::setw(0) << accTypeStr << " | ";
        std::cout << std::left << std::setw(0) << accInterestStr << " | ";
        std::cout << std::left << std::setw(0) << accStatusStr << " | $";
        std::cout << std::left << std::setw(0) << accBal << "|\n";
        */

        // Prompt the user to verify if the data is correct, if not then continue, else, add the account
        // Prompt the user to verify if the data is correct
        verify = 'x';
        std::cout << "Is the data correct? (Y/N): ";
        std::cin >> verify;

        // If user input is correct
        if (verify == 'y' || verify == 'Y' || verify == 'n' || verify == 'N') {
            // If account data is correct
            if (verify == 'Y' || verify == 'y') {
                if (!instExists) {
                    // Create a unique pointer to an Institution object and call the 
                    // Institution's constructor to add a new institution
                    std::unique_ptr<Institution> newInstitution = std::make_unique<Institution>(instName, instSel);
                }

                // The id of the new institution
                    instSel = inst->getID(instName);
                
                // Replace whitespaces with underscores
                accName = db->replaceWS(accName);

                // Create a unique pointer to an Account object and call the 
                // Account's constructor
                std::unique_ptr<Account> account = std::make_unique<Account>(instSel, accType, accName, accStatus, accBal, accInterest, 0, dateStr, dateVec[2], dateVec[1], dateVec[0]);
                std::cout << "Another? (Y/N): ";
                std::cin >> verify;

                // If user input is correct
                if (verify == 'y' || verify == 'Y' || verify == 'n' || verify == 'N') {
                    // If user wants to add another account
                    if (verify == 'Y' || verify == 'y') {
                        continue;
                    } else { // If user does not want to add another account
                        complete = true;
                    }
                } else {
                    std::cout << "Invalid input. Please try again." << std::endl;
                    continue;
                }
            } else { // If account data is not correct
                system("clear");
                continue;
            }
        } else {
            std::cout << "Invalid input. Please try again." << std::endl;
            continue;
        }
    }
}