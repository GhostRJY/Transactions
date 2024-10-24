#ifndef CLIENT_H
#define CLIENT_H
#include <iostream>
#include <string>

// класс для формирование структуры записи в файл
class Client
{
private:
    int m_account;
    std::string m_lname;
    std::string m_fname;
    double m_balance;

public:
    Client(int account = 0, std::string lname = "", std::string fname = "", double balance = 0.0);

    void setAccountNumber(const int);
    int getAccountNumber() const;

    void setLastName(const std::string &);
    std::string getLastName() const;

    void setFirstName(const std::string &);
    std::string getFirstName() const;

    void setBalance(const double);
    double getBalance() const;

    void enterAllValues(int);
};

Client::Client(int account, std::string lname, std::string fname, double balance)
{
    setAccountNumber(account);
    setLastName(lname);
    setFirstName(fname);
    setBalance(balance);
}

void Client::setAccountNumber(const int account)
{
    this->m_account = account;
}

int Client::getAccountNumber() const
{
    return m_account;
}

void Client::setLastName(const std::string &lname)
{
    this->m_lname = lname;
}

std::string Client::getLastName() const
{
    return m_lname;
}

void Client::setFirstName(const std::string &fname)
{
    this->m_fname = fname;
}

std::string Client::getFirstName() const
{
    return m_fname;
}

void Client::setBalance(const double balance)
{
    this->m_balance = balance;
}

double Client::getBalance() const
{
    return m_balance;
}

void Client::enterAllValues(int account)
{
    // вводим информацию о клиенте
    std::cout << "Enter lastname, firstname, balance\n?";
    std::cin >> m_lname;
    std::cin >> m_fname;
    std::cin >> m_balance;
    m_account = account;
}

#endif