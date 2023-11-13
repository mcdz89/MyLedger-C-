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
	long double m_proj_bal;
	std::string m_openDate;
	std::string m_rDate;
	int m_day;
	int m_month;
	int m_year;
	long double m_apy;
public:
	Account(int inst, int type, std::string & name, int active, long double balance, int interest, long double apy, std::string & rDate, int day, int month, int year);
    Account() {}
	~Account() {}

    // Setter Methods
	std::string setAccName();
	int setAccType();
	int setAccInterest();
	int setAccStatus();
	long double setAccBal();
	void setOpenDate(std::string & date);

	// Helper Methods
	std::string filterStrInput(const std::string & input);
	std::string trim(const std::string& str);

	// Getter Methods
	int getId() const;
	std::string getAccName(int id) const;
	std::string getTypeStr(int) const;
	int getInterest() const { return m_interest; }
	std::string getInterestStr() const { return m_interestStr; }
	int getActive() const { return m_active; }
	std::string getActiveStr() const { return m_activeStr; }
	long double getBalance() const { return m_balance; }
	long double getProjectedBalance() const { return m_proj_bal; }
	std::string getOpenDate() const { return m_openDate; }
	std::string getRDate() const { return m_rDate; }
	int getDay() const { return m_day; }
	int getMonth() const { return m_month; }
	int getYear() const { return m_year; }
	long double getAPY() const { return m_apy; }

	// Helper Methods
	int convertLDtoInt(long double);
	long double convertIntToLD(int);
};

#endif