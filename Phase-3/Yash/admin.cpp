#include <iostream>
#include <string>

#include "admin.h"

using namespace std;

class Admin
{
private:
    string username;
    string password;
    bool loggedIn;

public:
    Admin()
    {
        username = "admin";
        password = "admin123";
        loggedIn = false;
    }

    bool login()
    {
        string enteredUsername;
        string enteredPassword;

        if (loggedIn)
        {
            cout << "\nAdmin is already logged in.\n";
            return true;
        }

        cout << "\n========== ADMIN LOGIN ==========\n";

        cout << "Enter Username: ";
        cin >> enteredUsername;

        cout << "Enter Password: ";
        cin >> enteredPassword;

        if (enteredUsername == username &&
            enteredPassword == password)
        {
            loggedIn = true;

            cout << "\nAdmin login successful!\n";
            return true;
        }

        loggedIn = false;

        cout << "\nInvalid username or password!\n";
        return false;
    }

    void logout()
    {
        if (!loggedIn)
        {
            cout << "\nAdmin is not logged in.\n";
            return;
        }

        loggedIn = false;

        cout << "\nAdmin logged out successfully.\n";
    }

    bool isLoggedIn() const
    {
        return loggedIn;
    }
};

static Admin admin;

extern "C"
{

int adminLogin(void)
{
    return admin.login() ? 1 : 0;
}

void adminLogout(void)
{
    admin.logout();
}

int isAdminLoggedIn(void)
{
    return admin.isLoggedIn() ? 1 : 0;
}

}