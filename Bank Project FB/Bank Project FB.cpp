
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;


struct sUser {
    string UserName;
    string Password;
    short Permissions;
    bool MarkForDelete = false;
};

struct sClient
{
    string AccountNumber;
    string PinCode;
    string Name;
    string Phone;
    double AccountBalance;
    bool MarkForDelete = false;
};

enum enMainMenueOptions
{
    eListClients = 1, eAddNewClient = 2,
    eDeleteClient = 3, eUpdateClient = 4,
    eFindClient = 5, eTransactions = 6, eManageUsers = 7, eLogout = 8
};

enum enMainMenuePermissions {
    eAll = -1, pListClients = 1, pAddNewClient = 2, pDeleteClient = 4, pUpdateClients = 8, pFindClient = 16, pTransactions = 32, pManageUsers = 64
};

const string ClientsFileName = "Clients.txt";
const string UsersFileName = "Users.txt";

sUser CurrentUser;

void Login();
void ShowMainMenue();
void ShowTransactionMenueScreen();
void ShowManageUsersMenue();

enum enTransactionsMenueOptions {
    eDeposit = 1,
    eWithdraw = 2,
    eTotalBalances = 3,
    eMainMenue = 4
};

enum enManageUsersOptions {
    eListUsers = 1, eAddNewUser = 2, eDeleteUser = 3, eUpdateUser = 4, eFindUser = 5, eMainMenue2 = 6
};

vector<string> SplitString(string S1, string Delim)
{
    vector<string> vString;
    short pos = 0;
    string sWord; // define a string variable  

    // use find() function to get the position of the delimiters  
    while ((pos = S1.find(Delim)) != std::string::npos)
    {
        sWord = S1.substr(0, pos); // store the word   
        if (sWord != "")
        {
            vString.push_back(sWord);
        }

        S1.erase(0, pos + Delim.length());  /* erase() until positon and move to next word. */
    }

    if (S1 != "")
    {
        vString.push_back(S1); // it adds last word of the string.
    }

    return vString;

}

sClient ConvertLinetoRecord(string Line, string Seperator = "#//#")
{
    sClient Client;
    vector<string> vClientData;
    vClientData = SplitString(Line, Seperator);

    Client.AccountNumber = vClientData[0];
    Client.PinCode = vClientData[1];
    Client.Name = vClientData[2];
    Client.Phone = vClientData[3];
    Client.AccountBalance = stod(vClientData[4]);//cast string to double
    return Client;
}


string ConvertRecordToLine(sClient Client, string Seperator = "#//#")
{

    string stClientRecord = "";
    stClientRecord += Client.AccountNumber + Seperator;
    stClientRecord += Client.PinCode + Seperator;
    stClientRecord += Client.Name + Seperator;
    stClientRecord += Client.Phone + Seperator;
    stClientRecord += to_string(Client.AccountBalance);
    return stClientRecord;
}

bool ClientExistsByAccountNumber(string AccountNumber, string FileName)
{

    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            if (Client.AccountNumber == AccountNumber)
            {
                MyFile.close();
                return true;
            }
            vClients.push_back(Client);
        }

        MyFile.close();

    }
    return false;
}

sClient ReadNewClient()
{
    sClient Client;
    cout << "Enter Account Number? ";

    // Usage of std::ws will extract allthe whitespace character
    getline(cin >> ws, Client.AccountNumber);

    while (ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName))
    {
        cout << "\nClient with [" << Client.AccountNumber << "] already exists, Enter another Account Number? ";
        getline(cin >> ws, Client.AccountNumber);
    }

    cout << "Enter PinCode? ";
    getline(cin, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;

    return Client;
}

vector <sClient> LoadCleintsDataFromFile(string FileName)
{
    vector <sClient> vClients;
    fstream MyFile;
    MyFile.open(FileName, ios::in);//read Mode

    if (MyFile.is_open())
    {
        string Line;
        sClient Client;

        while (getline(MyFile, Line))
        {
            Client = ConvertLinetoRecord(Line);
            vClients.push_back(Client);
        }
        MyFile.close();
    }
    return vClients;
}

void PrintClientRecordLine(sClient Client)
{
    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(10) << left << Client.PinCode;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.Phone;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

bool CheckAccessPermission(enMainMenuePermissions Peermission) {

    if (CurrentUser.Permissions == enMainMenuePermissions::eAll) {
        return true;
    }

    if ((Peermission & CurrentUser.Permissions) == Peermission) {
        return true;
    }
    else {
        return false;
    }
}

void ShowAccessDeniedMessage() {
    cout << "\n------------------------------------\n";
    cout << "Access Denied, \nYou dont Have Permission To Do this,\nPlease Conact Your Admin.";
    cout << "\n------------------------------------\n";
}

void ShowAllClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pListClients)) {
        ShowAccessDeniedMessage();
    }

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(10) << "Pin Code";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Phone";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else {
        for (sClient Client : vClients)
        {

            PrintClientRecordLine(Client);
            cout << endl;
        }
    }


    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

}

void PrintClientCard(sClient Client)
{
    cout << "\nThe following are the client details:\n";
    cout << "-----------------------------------";
    cout << "\nAccout Number: " << Client.AccountNumber;
    cout << "\nPin Code     : " << Client.PinCode;
    cout << "\nName         : " << Client.Name;
    cout << "\nPhone        : " << Client.Phone;
    cout << "\nAccount Balance: " << Client.AccountBalance;
    cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber, vector <sClient> vClients, sClient& Client)
{
    for (sClient C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            Client = C;
            return true;
        }

    }
    return false;
}

sClient ChangeClientRecord(string AccountNumber)
{
    sClient Client;

    Client.AccountNumber = AccountNumber;

    cout << "\n\nEnter PinCode? ";
    getline(cin >> ws, Client.PinCode);

    cout << "Enter Name? ";
    getline(cin, Client.Name);

    cout << "Enter Phone? ";
    getline(cin, Client.Phone);

    cout << "Enter AccountBalance? ";
    cin >> Client.AccountBalance;
    return Client;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    for (sClient& C : vClients)
    {

        if (C.AccountNumber == AccountNumber)
        {
            C.MarkForDelete = true;
            return true;
        }

    }

    return false;
}

vector <sClient> SaveCleintsDataToFile(string FileName, vector <sClient> vClients)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out);//overwrite

    string DataLine;

    if (MyFile.is_open())
    {
        for (sClient C : vClients)
        {

            if (C.MarkForDelete == false)
            {
                //we only write records that are not marked for delete.  
                DataLine = ConvertRecordToLine(C);
                MyFile << DataLine << endl;
            }

        }

        MyFile.close();
    }

    return vClients;
}

void AddDataLineToFile(string FileName, string  stDataLine)
{
    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open())
    {

        MyFile << stDataLine << endl;

        MyFile.close();
    }
}

void AddNewClient()
{
    sClient Client;
    Client = ReadNewClient();
    AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
    char AddMore = 'Y';
    do
    {
        //system("cls");
        cout << "Adding New Client:\n\n";

        AddNewClient();
        cout << "\nClient Added Successfully, do you want to add more clients? Y/N? ";
        cin >> AddMore;

    } while (toupper(AddMore) == 'Y');

}

bool DeleteClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{
    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);

        cout << "\n\nAre you sure you want delete this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
            SaveCleintsDataToFile(ClientsFileName, vClients);

            //Refresh Clients 
            vClients = LoadCleintsDataFromFile(ClientsFileName);

            cout << "\n\nClient Deleted Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

bool UpdateClientByAccountNumber(string AccountNumber, vector <sClient>& vClients)
{

    sClient Client;
    char Answer = 'n';

    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
    {

        PrintClientCard(Client);
        cout << "\n\nAre you sure you want update this client? y/n ? ";
        cin >> Answer;
        if (Answer == 'y' || Answer == 'Y')
        {
            for (sClient& C : vClients)
            {
                if (C.AccountNumber == AccountNumber)
                {
                    C = ChangeClientRecord(AccountNumber);
                    break;
                }
            }

            SaveCleintsDataToFile(ClientsFileName, vClients);

            cout << "\n\nClient Updated Successfully.";
            return true;
        }

    }
    else
    {
        cout << "\nClient with Account Number (" << AccountNumber << ") is Not Found!";
        return false;
    }
}

string ReadClientAccountNumber()
{
    string AccountNumber = "";

    cout << "\nPlease enter AccountNumber? ";
    cin >> AccountNumber;
    return AccountNumber;

}

void ShowDeleteClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pDeleteClient)) {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n-----------------------------------\n";
    cout << "\tDelete Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    DeleteClientByAccountNumber(AccountNumber, vClients);

}

void ShowUpdateClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pUpdateClients)) {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tUpdate Client Info Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();
    UpdateClientByAccountNumber(AccountNumber, vClients);
}

void ShowAddNewClientsScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pAddNewClient)) {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tAdd New Clients Screen";
    cout << "\n-----------------------------------\n";
    AddNewClients();
}

void ShowFindClientScreen()
{
    if (!CheckAccessPermission(enMainMenuePermissions::pFindClient)) {
        ShowAccessDeniedMessage();
        return;
    }
    cout << "\n-----------------------------------\n";
    cout << "\tFind Client Screen";
    cout << "\n-----------------------------------\n";

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    sClient Client;
    string AccountNumber = ReadClientAccountNumber();
    if (FindClientByAccountNumber(AccountNumber, vClients, Client))
        PrintClientCard(Client);
    else
        cout << "\nClient with Account Number[" << AccountNumber << "] is not found!";


}

void GoBackToMainMenue()
{
    cout << "\n\nPress any key to go back to Main Menue...";
    system("pause>0");
    ShowMainMenue();

}

enTransactionsMenueOptions ReadTransactionsMenueOption() {
    short Choice = 0;

    do {
        cout << "Choose what do you want to do? [1 to 4]?";
        cin >> Choice;
    } while (Choice < 1 || Choice > 4);

    return (enTransactionsMenueOptions)Choice;
}

short ReadMainMenueOption()
{

    short Choice = 0;

    do {
        cout << "Choose what do you want to do? [1 to 8]? ";
        cin >> Choice;
    } while (Choice < 1 || Choice > 8);


    return Choice;
}

void GoBackToTransactionMenue() {
    cout << "\n\nPress any key to go back to transactions Menue...";
    system("pause>0");
    system("cls");
    ShowTransactionMenueScreen();
}

bool DepositInBalanceByAccountNumber(string AccountNumber, vector <sClient>& vCLients, double DepositAmount) {

    char Answer = 'n';

    cout << "\n\nAre you sure you want perform this transaction? y/n ?";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {

        for (sClient& C : vCLients) {
            if (C.AccountNumber == AccountNumber) {

                C.AccountBalance += DepositAmount;
                SaveCleintsDataToFile(ClientsFileName, vCLients);
                cout << "\n\nDone, secssusfully, new Balance is: " << C.AccountBalance << endl;

                return true;
            }
        }
        return false;
    }

}

void ShowDepositScreen() {

    cout << "\n-------------------------------\n";
    cout << "\tDeposit Screen";
    cout << "\n-------------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        cout << "\nClient with [" << AccountNumber << "\ does not exist.";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter deposit amount? ";
    cin >> Amount;

    DepositInBalanceByAccountNumber(AccountNumber, vClients, Amount);

}

void PrintClientTotalBalanceRecordLine(sClient Client) {

    cout << "| " << setw(15) << left << Client.AccountNumber;
    cout << "| " << setw(40) << left << Client.Name;
    cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalancesScreen() {
    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);

    cout << "\n\t\t\t\t\tClient List (" << vClients.size() << ") Client(s).";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "| " << left << setw(15) << "Accout Number";
    cout << "| " << left << setw(40) << "Client Name";
    cout << "| " << left << setw(12) << "Balance";
    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    double TotalBalances = 0;

    if (vClients.size() == 0)
        cout << "\t\t\t\tNo Clients Available In the System!";
    else

        for (sClient Client : vClients)
        {
            PrintClientTotalBalanceRecordLine(Client);
            TotalBalances += Client.AccountBalance;

            cout << endl;
        }

    cout << "\n_______________________________________________________";
    cout << "_________________________________________\n" << endl;

    cout << "\t\t\t\t Total Balances = " << TotalBalances << endl;
}

void ShowWithdrawScreen() {

    cout << "\n---------------------------\n";
    cout << "\tWithdraw Screen";
    cout << "\n---------------------------\n";

    sClient Client;

    vector <sClient> vClients = LoadCleintsDataFromFile(ClientsFileName);
    string AccountNumber = ReadClientAccountNumber();

    while (!FindClientByAccountNumber(AccountNumber, vClients, Client)) {
        cout << "\nClient with [" << AccountNumber << "\ does not exist.";
        AccountNumber = ReadClientAccountNumber();
    }

    PrintClientCard(Client);

    double Amount = 0;
    cout << "\nPlease enter withdraw amount? ";
    cin >> Amount;

    while (Amount > Client.AccountBalance) {
        cout << "\nAmount Exceeds the balance, you can withdraw up to : " << Client.AccountBalance << endl;
        cout << "please enter another account? ";
        cin >> Amount;
    }

    DepositInBalanceByAccountNumber(AccountNumber, vClients, (-1) * Amount);


}

void PerformTransactionsMenueOption(enTransactionsMenueOptions TransactionsMenueOption, sUser CurrentUser) {

    switch (TransactionsMenueOption) {

    case enTransactionsMenueOptions::eDeposit:
        system("cls");
        ShowDepositScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionsMenueOptions::eWithdraw:
        system("cls");
        ShowWithdrawScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionsMenueOptions::eTotalBalances:
        system("cls");
        ShowTotalBalancesScreen();
        GoBackToTransactionMenue();
        break;

    case enTransactionsMenueOptions::eMainMenue:
        ShowMainMenue();
    }
}

void ShowTransactionMenueScreen() {
    if (!CheckAccessPermission(enMainMenuePermissions::pTransactions)) {
        ShowAccessDeniedMessage();
        return;
    }

    cout << "\n======================================\n";
    cout << "\t\tTransactions Menue Screen";
    cout << "\n======================================\n";

    cout << "\t[1] Deposit.\n";
    cout << "\t[2] Withdraw.\n";
    cout << "\t[3] Total Balance.\n";
    cout << "\t[4] Main Menue.";
    cout << "\n======================================\n";

    PerformTransactionsMenueOption(ReadTransactionsMenueOption(), CurrentUser);


}

sUser ConvertUserLineToRecord(string Line, string Seperator = "#//#") {

    sUser User;
    vector <string> vUserData = SplitString(Line, Seperator);

    User.UserName = vUserData[0];
    User.Password = vUserData[1];
    User.Permissions = stoi(vUserData[2]);

    return User;
}

string ConvertUserRecordToLine(sUser LineRecord, string Seperator = "#//#") {

    string Line = "";

    Line += LineRecord.UserName + Seperator;
    Line += LineRecord.Password + Seperator;
    Line += to_string(LineRecord.Permissions);

    return Line;
}

vector <sUser> LoadUsersFromFileToVector(string FileName) {

    vector <sUser> vUsers;

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        sUser User;

        while (getline(MyFile, Line)) {
            User = ConvertUserLineToRecord(Line);
            vUsers.push_back(User);
        }

        MyFile.close();
    }

    return vUsers;
}

short ReadManageUsersOption() {

    short Choice = 0;

    do {
        cout << "Choose what do you want to do [1 to 6]?";
        cin >> Choice;
    } while (Choice < 0 || Choice > 6);

    return Choice;
}

void PrintUsersData(vector <sUser> vUsers) {

    for (sUser User : vUsers) {
        cout << "| " << setw(15) << left << User.UserName;
        cout << "| " << setw(10) << left << User.Password;
        cout << "| " << setw(12) << left << User.Permissions;
        cout << endl;
    }
}

void ShowListUsersScreen() {

    vector <sUser> vUsers = LoadUsersFromFileToVector(UsersFileName);

    cout << "\n\t\t\t\tUsers list (" << vUsers.size() << ") User(s).\n";

    cout << "\n-----------------------------------------------------------\n";
    cout << "| " << setw(15) << left << "User Name";
    cout << "| " << setw(10) << left << "Password";
    cout << "| " << setw(12) << left << "Permissions";
    cout << "\n-----------------------------------------------------------\n";

    if (vUsers.size() == 0) {
        cout << "\n\t\t\t\tNo User(s) available in the system" << endl;
    }
    else {
        PrintUsersData(vUsers);

    }
}

bool CheckUserByUserNameAndPassword(string UserName, string Password, sUser& CurrentUser) {

    fstream MyFile;
    MyFile.open(UsersFileName, ios::in);

    if (MyFile.is_open()) {

        string Line;
        sUser User;

        while (getline(MyFile, Line)) {
            User = ConvertUserLineToRecord(Line);

            if (User.UserName == UserName && User.Password == Password) {
                CurrentUser = User;
                MyFile.close();
                return true;
            }
        }
        MyFile.close();
    }

    return false;
}

void GoBackToManageUsersMenue() {
    cout << "\n\nPress any key to go back to Manage users Menue...";
    system("pause>0");
    system("cls");
    ShowManageUsersMenue();
}

bool UserExistsByUserName(string UserName, string FileName) {

    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {
        string Line;
        sUser User;

        while (getline(MyFile, Line)) {
            User = ConvertUserLineToRecord(Line);
            if (User.UserName == UserName) {
                MyFile.close();
                return true;
            }
        }
    }
    return false;
}

short ReadPermissionsToSet() {

    char Answer = 'n';
    short Permissions = 0;

    cout << "Do you want to give full access? y/n? ";
    cin >> Answer;

    if (Answer == 'y' || Answer == 'Y') {
        return -1;
    }

    cout << "\nDo you want to give access to :\n";

    cout << "\nShow Client List? y/n";
    cin >> Answer;
    Permissions += (Answer == 'y' || Answer == 'Y') ? enMainMenuePermissions::pListClients : 0;

    cout << "\nAdd New Client? y\n?";
    cin >> Answer;
    Permissions += (Answer == 'y' || Answer == 'Y') ? enMainMenuePermissions::pAddNewClient : 0;

    cout << "\nDelete Client? y/n?";
    cin >> Answer;
    Permissions += (Answer == 'y' || Answer == 'Y') ? enMainMenuePermissions::pDeleteClient : 0;

    cout << "\nUpdate Client? y/n?";
    cin >> Answer;
    Permissions += (Answer == 'y' || Answer == 'Y') ? enMainMenuePermissions::pUpdateClients : 0;

    cout << "\nFind Client? y/n?";
    cin >> Answer;
    Permissions += (Answer == 'y' || Answer == 'Y') ? enMainMenuePermissions::pFindClient : 0;

    cout << "\nTransactions? y/n?";
    cin >> Answer;
    Permissions += (Answer == 'y' || Answer == 'Y') ? enMainMenuePermissions::pTransactions : 0;

    cout << "\nManage Users? y/n?";
    cin >> Answer;
    Permissions += (Answer == 'y' || Answer == 'Y') ? enMainMenuePermissions::pManageUsers : 0;

    return Permissions;


}

sUser ReadNewUser() {

    sUser User;

    cout << "Enter UserName?";
    getline(cin >> ws, User.UserName);

    while (UserExistsByUserName(User.UserName, UsersFileName)) {
        cout << "\nUser with [" << User.UserName << "] already exists, Enter another UserName?";
        getline(cin >> ws, User.UserName);
    }

    cout << "Enter Password? ";
    getline(cin, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

string ReadUserName() {
    string UserName;
    cout << "\nPlease enter UserName? ";
    getline(cin >> ws, UserName);

    return UserName;
}

void SaveUserDataToFile(string FileName, string UserRecord) {

    fstream MyFile;
    MyFile.open(FileName, ios::out | ios::app);

    if (MyFile.is_open()) {
        MyFile << UserRecord << endl;
        MyFile.close();
    }

}

void AddNewUser() {
    sUser User = ReadNewUser();
    SaveUserDataToFile(UsersFileName, ConvertUserRecordToLine(User));
}

void AddNewUsers() {

    char AddMore = 'Y';

    do {
        cout << "\nAdding New User:\n";

        AddNewUser();
        cout << "\nUser Added Successfully, do you want to add more Users? Y/N?";
        cin >> AddMore;

    } while (AddMore == 'y' || AddMore == 'Y');
}

void ShowAddNewUserScreen() {

    cout << "\n----------------------------\n";
    cout << "\tAdd New User Screen";
    cout << "\n----------------------------\n";

    AddNewUsers();
}

vector <sUser> LoadUsersDateFromFile(string FileName) {

    vector <sUser> vUsers;
    fstream MyFile;
    MyFile.open(FileName, ios::in);

    if (MyFile.is_open()) {

        string Line;
        sUser User;

        while (getline(MyFile, Line)) {
            User = ConvertUserLineToRecord(Line);
            vUsers.push_back(User);
        }
        MyFile.close();
    }
    return vUsers;
}

void PrintUserCard(sUser User) {
    cout << "\nThe following are the user details:";

    cout << "\n-----------------------------------\n";
    cout << "UserName    :" << User.UserName << endl;
    cout << "Password    :" << User.Password << endl;
    cout << "Permissions :" << User.Permissions << endl;
    cout << "\n-----------------------------------\n";
}

bool FindUserByUserName(string UserName, sUser& User, vector <sUser> vUsers) {

    for (sUser U : vUsers) {
        if (U.UserName == UserName) {
            User = U;
            return true;
        }
    }
    return false;
}

bool MarkUserForDeleteByUserName(string UserName, vector <sUser>& vUsers) {

    for (sUser& U : vUsers) {
        if (U.UserName == UserName) {
            U.MarkForDelete = true;
            return true;
        }
    }
    return false;
}

void SaveUserDateToFile(string FileName, vector <sUser> vUsers) {

    fstream MyFile;
    MyFile.open(FileName, ios::out);

    if (MyFile.is_open()) {

        for (sUser U : vUsers) {

            string DataLine;
            if (U.MarkForDelete == false) {
                DataLine = ConvertUserRecordToLine(U);
                MyFile << DataLine << endl;
            }
        }
        MyFile.close();
    }
}

bool DeleteUserByUserName(string UserName, vector <sUser>& vUsers) {


    if (UserName == "Admin") {
        cout << "\n\nYo connot delete this user\n";
        return false;
    }

    sUser User;
    char Answer = 'y';

    if (FindUserByUserName(UserName, User, vUsers)) {
        PrintUserCard(User);

        cout << "\nAre you sure you want delete this user? y/n ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {
            MarkUserForDeleteByUserName(UserName, vUsers);
            SaveUserDateToFile(UsersFileName, vUsers);

            vUsers = LoadUsersDateFromFile(UsersFileName);
            cout << "\n\nUser Deleted Successfully.\n";

            return true;
        }

    }
    else {
        cout << "\nUser with userName (" << UserName << ") is not found!";
        return false;
    }
}

void ShowDeleteUserScreen() {

    cout << "\n-------------------------------\n";
    cout << "\tDelete Users Screen";
    cout << "\n-------------------------------\n";

    vector <sUser> vUsers = LoadUsersDateFromFile(UsersFileName);
    string UserName = ReadUserName();
    DeleteUserByUserName(UserName, vUsers);

}

sUser ChangeUserRecord(string UserName) {

    sUser User;

    User.UserName = UserName;

    cout << "\nEnter Password? ";
    getline(cin >> ws, User.Password);

    User.Permissions = ReadPermissionsToSet();

    return User;
}

bool UpdateUserByUserName(string UserName, vector <sUser>& vUsers) {

    char Answer = 'y';
    sUser User;

    if (FindUserByUserName(UserName, User, vUsers)) {
        PrintUserCard(User);

        cout << "\nAre you sure you want update this user? y/n ?";
        cin >> Answer;

        if (Answer == 'y' || Answer == 'Y') {

            for (sUser& U : vUsers) {
                if (U.UserName == UserName) {
                    U = ChangeUserRecord(UserName);
                    break;
                }
            }
            SaveUserDateToFile(UsersFileName, vUsers);
            cout << "\n\nUser Updated Successfully.";
            return true;
        }
    }
    else {
        cout << "User with userName (" << UserName << ") is not found.";
        return false;
    }
}

void ShowUpdateUserScreen() {

    cout << "\n---------------------------\n";
    cout << "\tUpdate Users Screen";
    cout << "\n---------------------------\n";

    vector <sUser> vUsers = LoadUsersDateFromFile(UsersFileName);
    string UserName = ReadUserName();
    UpdateUserByUserName(UserName, vUsers);

}

bool FindUserByUserName(string UserName) {

    vector <sUser> vUsers = LoadUsersDateFromFile(UsersFileName);

    for (sUser U : vUsers) {
        if (U.UserName == UserName) {
            PrintUserCard(U);
            return true;
        }
    }
    cout << "\nUser with username (" << UserName << ") is not found\n";
    return false;

}

void ShowFindUserScreen() {

    cout << "\n----------------------------\n";
    cout << "\tFind User Screen";
    cout << "\n----------------------------\n";

    string UserName = ReadUserName();
    FindUserByUserName(UserName);
}

void PerformManageUsersMenueOption(enManageUsersOptions ManageUsersOption) {

    switch (ManageUsersOption) {

    case enManageUsersOptions::eListUsers:
        system("cls");
        ShowListUsersScreen();
        GoBackToManageUsersMenue();
        break;


    case enManageUsersOptions::eAddNewUser:
        system("cls");
        ShowAddNewUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eDeleteUser:
        system("cls");
        ShowDeleteUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eUpdateUser:
        system("cls");
        ShowUpdateUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eFindUser:
        system("cls");
        ShowFindUserScreen();
        GoBackToManageUsersMenue();
        break;

    case enManageUsersOptions::eMainMenue2:
        system("cls");
        ShowMainMenue();

    }




}

void ShowManageUsersMenue() {

    if (!CheckAccessPermission(enMainMenuePermissions::pManageUsers)) {
        ShowAccessDeniedMessage();
        GoBackToMainMenue();
        return;
    }
    system("cls");
    cout << "\n======================================\n";
    cout << "\tMaage users menue screen";
    cout << "\n======================================\n";

    cout << "\t[1] List Users.\n";
    cout << "\t[2] Add New User.\n";
    cout << "\t[3] Delete User.\n";
    cout << "\t[4] Update user.\n";
    cout << "\t[5] Find User.\n";
    cout << "\t[6] Main Menue.";
    cout << "\n======================================\n";

    PerformManageUsersMenueOption((enManageUsersOptions)ReadManageUsersOption());
}

void PerfromMainMenueOption(enMainMenueOptions MainMenueOption)
{
    switch (MainMenueOption)
    {
    case enMainMenueOptions::eListClients:
    {
        system("cls");
        ShowAllClientsScreen();
        GoBackToMainMenue();
        break;
    }
    case enMainMenueOptions::eAddNewClient:
        system("cls");
        ShowAddNewClientsScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eDeleteClient:
        system("cls");
        ShowDeleteClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eUpdateClient:
        system("cls");
        ShowUpdateClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eFindClient:
        system("cls");
        ShowFindClientScreen();
        GoBackToMainMenue();
        break;

    case enMainMenueOptions::eTransactions:
        system("cls");
        ShowTransactionMenueScreen();

    case enMainMenueOptions::eManageUsers:
        system("cls");
        ShowManageUsersMenue();
        break;

    case enMainMenueOptions::eLogout:
        system("cls");
        Login();
    }
}

void ShowMainMenue()
{
    system("cls");
    cout << "===========================================\n";
    cout << "\t\tMain Menue Screen\n";
    cout << "===========================================\n";
    cout << "\t[1] Show Client List.\n";
    cout << "\t[2] Add New Client.\n";
    cout << "\t[3] Delete Client.\n";
    cout << "\t[4] Update Client Info.\n";
    cout << "\t[5] Find Client.\n";
    cout << "\t[6] Transactions.\n";
    cout << "\t[7] Manage Users.\n";
    cout << "\t[8] Logout.\n";
    cout << "===========================================\n";
    PerfromMainMenueOption((enMainMenueOptions)ReadMainMenueOption());
}

bool FindUserByUserNameAndPassword(string UserName, string Password, sUser& User) {

    vector <sUser> vUsers = LoadUsersFromFileToVector(UsersFileName);

    for (sUser U : vUsers) {
        if (U.UserName == UserName && U.Password == Password) {
            User = U;
            return true;
        }
    }
    return false;
}

bool LoadUserInfo(string UserName, string Password) {

    if (FindUserByUserNameAndPassword(UserName, Password, CurrentUser))
        return true;
    else
        return false;

}

void Login() {

    bool LoginFailed = false;
    string Username, Password;

    do {
        system("cls");

        cout << "\n-----------------------------\n";
        cout << "\tLogin Screen";
        cout << "\n-----------------------------\n";

        if (LoginFailed) {
            cout << "Invalid Username/Password!\n";
        }

        cout << "Enter Username? ";
        getline(cin >> ws, Username);

        cout << "Enter Password? ";
        getline(cin, Password);

        LoginFailed = !LoadUserInfo(Username, Password);

    } while (LoginFailed);

    ShowMainMenue();
}


int main()

{
    Login();
    system("pause>0");
    return 0;
}
