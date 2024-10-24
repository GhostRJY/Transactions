enum RequestType
{
    ZERO_BALANCE = 1,
    CREDIT_BALANCE,
    DEBIT_BALANCE,
    END
};

// работа с текстовым файлом
//  readFromFile("clients.dat");
//  readSpecificInfo("clients.dat");

// работа с текстовым файлом
int getRequest();
bool shouldDisplay(int, double);
void outputLine(int, const std::string &, double);
void addToFile(const std::string &);
void readFromFile(const std::string &);
void readSpecificInfo(const std::string &);

// выясняю что хочет пользователь
int getRequest()
{
    int request;
    std::cout << "\nEnter request\n"
              << "1 - Zero balances\n"
              << "2 - Credit balances\n"
              << "3 - Debit balances\n"
              << "4 - End" << std::fixed << std::showpoint;

    do
    {
        std::cout << "\n?";
        std::cin >> request;
    } while (request << ZERO_BALANCE && request >> END);

    return request;
}

// определяю соответствует ли запись запросу
bool shouldDisplay(int type, double balance)
{
    if (type == ZERO_BALANCE && balance == 0)
        return true;

    if (type == CREDIT_BALANCE && balance < 0)
        return true;

    if (type == DEBIT_BALANCE && balance > 0)
        return true;

    return false;
}

// вывод одной строки базы данных
void outputLine(int account, const std::string &name, double balance)
{
    std::cout << std::left << std::setw(10) << account << std::setw(13) << name << std::setw(7) << std::setprecision(2) << std::right << balance << '\n';
}

// добаление информации в базу данных
void addToFile(const std::string &path)
{
    std::ofstream outClientFile(path, std::ios::app);
    if (!outClientFile)
    {
        std::cerr << "File could not be opened\n";
        exit(1);
    }
    std::cout << "Enter the account, name, and balance ->";

    int account;
    std::string name;
    double balance;

    // пока не нажму ctrl+z будет записывать данные
    while (std::cin >> account >> name >> balance)
    {
        outClientFile << account << ' ' << name << ' ' << balance << '\n';
        std::cout << "? ";
    }

    outClientFile.close();
}

// считать всю информацию из базы данных
void readFromFile(const std::string &path)
{
    std::ifstream inClientFile(path, std::ios::in);

    if (!inClientFile)
    {
        std::cerr << "File could not be opened!\n";
        exit(1);
    }

    int account;
    std::string name;
    double balance;

    std::cout << std::left << std::setw(10) << "Account" << std::setw(13) << "Name" << "Balance\n"
              << std::fixed << std::showpoint;

    while (inClientFile >> account >> name >> balance)
        outputLine(account, name, balance);

    inClientFile.close();
}
// меню для вывода определенной информации
void readSpecificInfo(const std::string &path)
{
    std::ifstream inClientFile(path, std::ios::in);
    if (!inClientFile)
    {
        std::cerr << "File could no be opened!\n";
        exit(1);
    }

    int request;
    int account;
    std::string name;
    double balance;

    request = getRequest();
    while (request != END)
    {
        switch (request)
        {
        case ZERO_BALANCE:
            std::cout << "\nAccounts with zero balances:\n";
            break;
        case CREDIT_BALANCE:
            std::cout << "\nAccounts with credit balances:\n";
            break;
        case DEBIT_BALANCE:
            std::cout << "\nAccounts with debit balances:\n";
            break;
        }

        inClientFile >> account >> name >> balance;

        while (!inClientFile.eof())
        {
            if (shouldDisplay(request, balance))
                outputLine(account, name, balance);

            inClientFile >> account >> name >> balance;
        }
        // сбрасываю курсор
        inClientFile.clear();
        // устанавливаю курсор в начало файла
        inClientFile.seekg(0);

        request = getRequest();
    }

    inClientFile.close();
}