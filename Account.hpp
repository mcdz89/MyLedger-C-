#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP

#include <string>
#include <iostream>
#include <vector>
#include <pqxx/pqxx>
#include <exception>
#include <algorithm>

// std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Add this line before the prompt

class Account {
private:
    int m_id;
	std::string m_instName;
	std::string m_accName;
	int m_accType;
	std::string m_typeStr;
	int m_interest;
	std::string m_interestStr;
	int m_active;
	std::string m_activeStr;
	long double m_balance;
	int m_balanceInt;
	std::string m_balanceStr;
	long double m_proj_bal;
	std::string m_openDate;
	std::string m_rDate;
	int m_day;
	int m_month;
	int m_year;
	long double m_apy;
public:
	// Constructor for adding a new account
	Account(int inst, int type, std::string & name, int active, long double balance, int interest, long double apy, std::string & rDate, int day, int month, int year);
	
	// Constructor for viewing account
	Account(std::string instName, std::string accName, std::string accTypeStr, std::string hr_date, std::string accInterestStr, std::string accStatusStr, std::string accBal){
		m_instName = instName;
		m_accName = accName;
		m_typeStr = accTypeStr;
		m_rDate = hr_date;
		m_interestStr = accInterestStr;
		m_activeStr = accStatusStr;
		m_balanceStr =accBal;
	}
    Account() {}
	~Account() {}

    // Setter Methods
	std::string setAccName(); // This function sets the account name and returns it as a string
	int setAccType(); // This function sets the account type and returns it as an integer
	int setAccInterest(); // This function sets the account interest and returns it as an integer
	int setAccStatus(); // This function sets the account status and returns it as an integer
	long double setAccBal(); // This function sets the account balance and returns it as a long double
	void setOpenDate(std::string & date); // This function sets the open date of the account using the provided date string

	// Getter Methods
	int getId() const; // Get the account ID
	std::string getAccName(int id) const; // Get the account name based on the given ID
	std::string getTypeStr(int) const; // Get the account type based on the given ID
	int getInterest() const { return m_interest; } // Get the interest rate
	std::string getInterestStr() const { return m_interestStr; } // Get the interest rate as a string
	int getActive() const { return m_active; } // Get the active status
	std::string getActiveStr() const { return m_activeStr; } // Get the active status as a string
	long double getBalance() const { return m_balance; } // Get the current balance
	long double getProjectedBalance() const { return m_proj_bal; } // Get the projected balance
	std::string getOpenDate() const { return m_openDate; } // Get the account opening date
	std::string getRDate() const { return m_rDate; } // Get the last transaction date
	int getDay() const { return m_day; } // Get the day of the last transaction
	int getMonth() const { return m_month; } // Get the month of the last transaction
	int getYear() const { return m_year; } // Get the year of the last transaction
	long double getAPY() const { return m_apy; } // Get the annual percentage yield (APY)
	int checkAcc() const; // Check the account status

	// Helper Methods
	int convertLDtoInt(long double); // Function to convert a long double to an integer
	long double convertIntToLD(int); // Function to convert an integer to a long double
	std::string filterStrInput(const std::string & input); // Function to filter string input
	std::string trim(const std::string& str); // Function to trim whitespace from a string
	
	// Print Methods
	void printTable(std::vector<std::string> &, std::vector<Account*>); // This function takes a reference to a vector of strings and a vector of pointers to Account objects
	void viewAcc(); // This function does not take any parameters and returns void
};

#endif