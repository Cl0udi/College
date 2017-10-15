/*
file: Trivia_Game.cpp
date: 10-14-17


GAME QUIZ

    FIRST PART:
    Menu with options to log in, register or exit.

    SECOND PART:
    Menu with options to play, change difficulty, display score and return to
    main menu. If play is selected it gives the user 3 categories to chose from.
    difficulty changes the number of multiple choices per question.

    LONGEST RUN TIME < O(N)

*/

#include <iostream>
#include <vector>

using namespace std;
void inputInteger(int & test, int selections);
void inputUsername(string & test, vector<string> test2);
void inputPassword(string & test);
void searchUserName(vector<string> test2, int & userNumber, bool& exit);
void testPassword(string userTemp, vector<string> test, int userNumber, bool& exit);
void scienceTrivia(int& playerScore);


int main()
{
    int choice, userNum, difficulty = 1, playerScore = 0;
    bool exitStatus = false;
    string user, password;
    vector<string> allUsers;
    vector<string> allPasswords;

    Menu:
    cout << "\n\n***********\n"
         << "MAIN MENU\n\n"
         << "***********\n"
         << "Options: \n\n"
         << "1) Log in\n"
         << "2) Register\n"
         << "3) Exit\n";

    inputInteger(choice, 3);

    if(choice == 3){goto Exit;}

    else if(choice == 2)
    {
        inputUsername(user, allUsers);
        inputPassword(password);

        cout << "\nCreating User Name '" << user << "' with password '" << password
             << " '\n";
        allUsers.push_back(user);
        allPasswords.push_back(password);
        cout << "\nSuccess. Returning to main menu.\n\n";
        goto Menu;
    }

    else if(choice == 1)
    {
        searchUserName(allUsers, userNum, exitStatus);
        if(exitStatus){cout << "\nReturning to main menu.\n\n"; goto Menu;}

        testPassword(user, allPasswords, userNum, exitStatus);
        if(exitStatus){cout << "\nReturning to main menu.\n\n"; goto Menu;}

        goto Game;

    }

    Game:
        cout << "\n\n\n############################"
             << "\nWelcome to Trivia Game Menu!\n"
             << "############################\n"
             << "Options: \n\n"
             << "1) Play\n"
             << "2) Select Difficulty\n"
             << "3) Display Score\n"
             << "4) Exit to main menu\n";

        inputInteger(choice, 3);

        if (choice == 4){goto Menu;}

        else if(choice == 3)
        {
            cout << "\nYour current score is "
                 << playerScore
                 << "!\n";
            goto Game;
        }

        else if(choice == 2)
        {
        cout << "\nPlease select a difficulty.\n"
             << "1) Easy\n"
             << "2) Medium\n"
             << "3) Hard\n";
        inputInteger(difficulty, 3);
        cout << "\nChanges made. Returning to trivia menu.\n";
        goto Game;
        }

        else if(choice == 1)
        {
            cout << "\nPlease select a category.\n"
                 << "1) Science\n"
                 << "2) History\n"
                 << "3) Literature\n";

            inputInteger(choice, 3);

            if(choice == 1){scienceTrivia(playerScore); goto Game;}
            else
                cout << "\nNot available at this time.\n";
                goto Game;
        }

    Exit:
    cout << "\n\nThanks for playing\n";
    cin.get();
    return 0;
}

void inputInteger(int & test, int selections)
{
    //Tests user input a valid integer for menu choices

    Redo:
    cin  >> test;
    while(!cin)
    {
    cout << "\nPlease input an integer\n";
    cin.clear();
    cin.ignore();
    cin >> test;
    }

    if(test < 0 || test > selections)
    {cout << "\nPlease Input an integer between 1 and 3\n"; goto Redo;}
}

void inputUsername(string & test, vector<string> test2)
{
    //Tests for valid string input and for compares it to other user names
    //so that all user names are unique

    Redo:
    cout << "\nPlease input your desired User Name\n";
    cin  >> test;
    while(!cin)
    {
    cout << "\nPlease input a User Name\n";
    cin.clear();
    cin.ignore();
    cin >> test;
    }
    for(vector<string>::iterator it = test2.begin() ; it != test2.end(); ++it)
    {if(*it == test){cout << "\nUser Name already taken.\n"; goto Redo;}}

    cout << "\nUser Name accepted.\n";
}

void inputPassword(string & test)
{
    //Tests for valid password input. Must include an uppercase letter,
    //a lowercase, a number and a special character

    int i;
    bool upper   = false,
         lower   = false,
         special = false,
         number  = false;

    Redo:
    cout << "\n\nPlease input your desired Password\n";
    cin  >> test;
    while(!cin)
    {
    cout << "\nPlease input a Password\n";
    cin.clear();
    cin.ignore();
    cin >> test;
    }

    if(test.length() < 8)
    {cout << "\nPassword must be at least 8 Characters/n"; goto Redo;}

    i = 0;
    while(i < test.length())
    {
        if     (test[i] >= 0x41 && test[i] <= 0x5A) {upper   = true;}
        else if(test[i] >= 0x61 && test[i] <= 0x7A) {lower   = true;}
        else if(test[i] >= 0x30 && test[i] <= 0x39) {number  = true;}
        else if(test[i] >= 0x21 && test[i] <= 0x2E) {special = true;}
        i++;
    }

    if(!upper)  {cout << "\nPassword must include an uppercase letter.\n";}
    if(!lower)  {cout << "\nPassword must include a lowercase letter.\n";}
    if(!number) {cout << "\nPassword must include a number.\n";}
    if(!special){cout << "\nPassword must include a special character.\n";}

    if(upper && lower && number && special) {cout << "\nPassword accepted.\n";}
    else
    {upper = false; lower = false; number = false; special = false; goto Redo;}

}

void searchUserName(vector<string> test2, int & userNumber, bool& exitStatus)
{
    //Searches for user name in database. Gives information on if the user wants
    //to continue or go back to main menu. If user is found we store the number
    //for that user in userNumber to make password finding easier.

    int i = 0, exitChoice = 0;
    string test;

    userNumber = 0;

    Redo:
    cout << "\nPlease input your User Name\n";
    cin  >> test;
    while(!cin)
    {
    cout << "\nPlease input your User Name\n";
    cin.clear();
    cin.ignore();
    cin >> test;
    }

    for(vector<string>::iterator it = test2.begin() ; it != test2.end(); ++it)
    {
        if(*it == test){goto Success;}
        else{i++; userNumber++;}
    }
    cout << "\nUser Name not found.\n";
    Redo2:
    cout << "\nWould you like to search again?\n"
         << "1) Yes\n2) No\n";
    cin  >> exitChoice;
    while(!cin)
    {
    cout << "\nPlease input an Integer\n";
    cin.clear();
    cin.ignore();
    cin >> exitChoice;
    }

    if(exitChoice != 1 && exitChoice != 2)
    {
        cout << "\nNumber is not one of the choices. Please try again.\n";
        goto Redo2;
    }

    else if(exitChoice == 1){goto Redo;}
    else{goto Exit;}

    Success:
    exitStatus = false;
    cout << "\nUser Name found.\n";
    return;

    Exit:
    exitStatus = true;
    cout << "\nExiting.\n";
    return;
}

void testPassword(string userTemp, vector<string> test, int userNumber, bool& exit)
{
    //Tests for correct password. If incorrect 3 times returns to main menu.

    int tries = 3;
    string comparedString;
    vector<string>::iterator it = test.begin();

    Redo:
    cout << "\nPlease input your Password for '" << userTemp << "'.\n";
    cin  >> comparedString;
    while(!cin)
    {
    cout << "\nPlease input your Password for '" << userTemp << "'.\n";
    cin.clear();
    cin.ignore();
    cin >> comparedString;
    }

    if(*(it + userNumber) == comparedString){cout << "4";goto Success;}
    else
        {
            tries--;
            if(tries == 0){goto Failure;}
            cout << "\nPassword incorrect. " << tries << " attempt(s) left.\n";
            goto Redo;
        }

    Success:
        cout << "\nPassword verified.\n";
        exit = false;
        return;

    Failure:
        cout << "\nPassword not verified.\n";
        exit = true;
        return;

}

void scienceTrivia(int& playerScore)
{
    int tempAnswer, question = 0;
    int answers[] = {1,1};
    string Questions[][5] =
    {
        {"Bronze is an alloy consisting primarily of what two elements?\n",
        "1) Copper & Tin\n", "2) Silver & Helium\n", "3) Gold & Carbon\n",
        "1) Copper & Oxygen\n"},
        {"What is the farthest human-made object from planet Earth?\n",
        "1) Voyager 1\n", "2) Voyager 2\n", "3) Challenger 1\n", "4) Spudnik\n"}
     };

    cout << "\n\nWelcome to Science Trivia. Are you ready to begin?\n"
         << "1) Yes\n"
         << "2) No\n";
    inputInteger(tempAnswer, 2);

    if(tempAnswer == 2){goto ExitFunction;}
    cout << "\nExcellent! Let us begin:\n";

    while(question < 2)
    {
        cout << "\nQuestion:\n"
             << Questions[question][0]
             << Questions[question][1]
             << Questions[question][2]
             << Questions[question][3]
             << Questions[question][4];

        inputInteger(tempAnswer, 4);

        if(tempAnswer == answers[question])
        {cout << "\nCorrect Answer!\n"; playerScore++;}

        else
        {cout << "\nWrong answer.\n";}

        question++;

    cout << "\nWould you like to continue?\n"
         << "1) Yes\n"
         << "2) No\n";
    inputInteger(tempAnswer, 2);
    if(tempAnswer == 2){goto ExitFunction;}

    }

    cout << "\nNo more questions available. Returning to trivia menu.\n";
    return;


    ExitFunction:
    cout << "\nReturning to trivia menu.\n";
    return;
}
