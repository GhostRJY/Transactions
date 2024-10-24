#include <iomanip>
#include <fstream>
#include <cstdlib>
#include <string>
#include <windows.h>

#include "Client.h"

// работа с бинарным файлом
// создание бинарного файла и записываю 100 пустых записей
void makeBinaryFile(const std::string &);
void writeToBinaryFile(const std::string &);
void readAllFile(const std::string &);

// бухгалтерский учет
void runTransaction();
int enterChoice();
void createTextFile(const std::string &);
void updateRecord(const std::string &);
void newRecord(const std::string &);
void deleteRecord(const std::string &);

// вспомогательные фун-ии
int getAccount(const std::string &);
void outputLine(std::ostream &, const Client &);

enum Choices
{
    PRINT = 1,
    UPDATE,
    NEWLINE,
    DELETELINE,
    READALL,
    END
};

void runTransaction()
{
    int choice;

    while ((choice = enterChoice()) != END)
    {
        switch (choice)
        {
        case PRINT:
            createTextFile("credit.dat");
            break;

        case UPDATE:
            updateRecord("credit.dat");
            break;

        case NEWLINE:
            newRecord("credit.dat");
            break;

        case DELETELINE:
            deleteRecord("credit.dat");
            break;

        case READALL:
            readAllFile("credit.dat");
            break;

        default:
            std::cerr << "Incorect choice!\n";
            break;
        }
    }
}

void makeBinaryFile(const std::string &path)
{
    std::ofstream outCredit(path, std::ios::binary);
    if (!outCredit)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    // формирую пустую запись
    Client blankClient;

    for (int i{0}; i < 100; ++i)
    {
        outCredit.write(reinterpret_cast<const char *>(&blankClient), sizeof(Client));
    }

    outCredit.close();
}

void writeToBinaryFile(const std::string &path)
{
    int account;

    // открыл файл для чтения и записи бинарно
    std::fstream outCredit(path, std::ios::in | std::ios::out | std::ios::binary);

    if (!outCredit)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    std::cout << "Enter account number(1-100, 0 - end input)\n?";

    Client client;
    std::cin >> account;

    // записываю инфо о клиентах
    while (account > 0 && account < 101)
    {
        // вводим информацию о клиенте
        client.enterAllValues(account);

        // ищу позицию в файле (номер счета)
        outCredit.seekp((client.getAccountNumber() - 1) * sizeof(Client));

        // привожу запись о клиенте к типу const char * и записываю структуру в файл
        outCredit.write(reinterpret_cast<const char *>(&client), sizeof(Client));

        // продолжаю записывать пока условие цикла истина
        std::cout << "Enter account number(1-100, 0 - end input)\n?";
        std::cin >> account;
    }

    outCredit.close();
}

void readAllFile(const std::string &path)
{
    std::ifstream file(path, std::ios::in);

    if (!file)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    std::cout << std::left << std::setw(10) << "Account"
              << std::setw(20) << "Last Name"
              << std::setw(20) << "First Name"
              << std::left << std::setw(15) << std::right << "Balance\n";

    Client client;

    // читаю первую запись в файле
    file.read(reinterpret_cast<char *>(&client), sizeof(Client));

    // вывожу записи в файле
    while (file && !file.eof())
    {
        // если запись не пустая тогда вывожу на экран
        if (client.getAccountNumber() != 0)
            outputLine(std::cout, client);

        file.read(reinterpret_cast<char *>(&client), sizeof(Client));
    }

    file.close();
}

int enterChoice()
{
    // менюшка
    std::cout << "\nEnter your choice\n"
              << "1 - save \"print.txt\" for printing\n"
              << "2 - update an account\n"
              << "3 - add new account\n"
              << "4 - delete account\n"
              << "5 - Show all accounts\n"
              << "6 - end program\n? ";

    int menuChoice;
    std::cin >> menuChoice;

    return menuChoice;
}

// формирую текстовый файл из бинарного
void createTextFile(const std::string &path)
{

    std::ofstream outTxt("print.txt", std::ios::out);

    if (!outTxt)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    outTxt << std::left << std::setw(10) << "Account"
           << std::setw(20) << "Last Name"
           << std::setw(20) << "First Name"
           << std::right << std::setw(15) << "Balance\n";

    std::ifstream file(path, std::ios::in);

    if (!file)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    // установил курсор в начало файла
    file.seekg(0);

    Client client;
    // читаю первую запись
    file.read(reinterpret_cast<char *>(&client), sizeof(Client));

    while (!file.eof())
    {

        // записываю запись в текстовый файл если номер счета !0
        if (client.getAccountNumber() != 0)
            outputLine(outTxt, client);

        // читаю следующую запись
        file.read(reinterpret_cast<char *>(&client), sizeof(Client));
    }
    outTxt.close();
    file.close();
}

void updateRecord(const std::string &path)
{
    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);

    if (!file)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    // получаю счет который нужно обновить
    int account = getAccount("Enter account to update");

    // перемещаю курсор в файле на нужную запись
    file.seekg((account - 1) * sizeof(Client));

    // считываю запись в бинарном
    Client client;
    file.read(reinterpret_cast<char *>(&client), sizeof(Client));

    // обновляю запись если она !0
    if (client.getAccountNumber() != 0)
    {
        // вывожу запись
        outputLine(std::cout, client);

        std::cout << "Enter payment: ";
        double transaction;
        std::cin >> transaction;

        // обновляю баланс
        double oldBalance = client.getBalance();
        client.setBalance(oldBalance + transaction);

        // показываю обновленую запись
        outputLine(std::cout, client);

        // вывожу обновленную записб в файл
        file.seekp((account - 1) * sizeof(Client));
        file.write(reinterpret_cast<const char *>(&client), sizeof(Client));
    }
    else
    {
        std::cerr << "Account #" << account << " has no information\n";
    }

    file.close();
}

// записываю новую запись в файл
void newRecord(const std::string &path)
{

    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);

    if (!file)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    int account = getAccount("Enter new account number");

    file.seekg((account - 1) * sizeof(Client));

    Client client;
    file.read(reinterpret_cast<char *>(&client), sizeof(Client));

    // если запись пустая записываю новую запись
    if (client.getAccountNumber() == 0)
    {
        client.enterAllValues(account);
        file.seekp((account - 1) * sizeof(Client));
        file.write(reinterpret_cast<const char *>(&client), sizeof(Client));
    }
    else
    {
        std::cerr << "Account #" << account << " contains information!\n";
    }

    file.close();
}

void deleteRecord(const std::string &path)
{

    std::fstream file(path, std::ios::in | std::ios::out | std::ios::binary);

    if (!file)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    int account = getAccount("Enter account to delete");

    file.seekg((account - 1) * sizeof(Client));

    Client client;
    file.read(reinterpret_cast<char *>(&client), sizeof(Client));

    if (client.getAccountNumber() != 0)
    {
        // пустой клиент
        Client blankClient;

        // ищу нужную запись
        file.seekp((account - 1) * sizeof(Client));

        // записываю пустой обьект
        file.write(reinterpret_cast<const char *>(&blankClient), sizeof(Client));

        std::cout << "Account #" << account << " deleted!\n";
    }
    else
    {
        std::cerr << "Account #" << account << "is empty !!!";
    }

    file.close();
}

// получаю номер счета
int getAccount(const std::string &line)
{
    int account;

    // получаю корректный номер счета
    do
    {
        std::cout << line << " (1-100): ";
        std::cin >> account;
    } while (account < 1 && account > 100);

    return account;
}

// вывожу одну запись в файл
void outputLine(std::ostream &output, const Client &record)
{
    output << std::left << std::setw(10) << record.getAccountNumber()
           << std::setw(20) << record.getLastName()
           << std::setw(20) << record.getFirstName()
           << std::setw(15) << std::setprecision(2) << std::right << std::fixed
           << std::showpoint << record.getBalance() << '\n';
}