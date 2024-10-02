/*
    Name: ved
    Date: 2022-03-28
    Description: Create a passphrase generator using the Diceware™ word lists.
*/

#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <cstdlib>
#include <ctime>
#include <fstream>

using namespace std;

// global stuff
// varriables list
string diceCode;
string passphrase;
const char specialCharactersArray[] = "0123456789!@#$%^&*";
char numberArray[6] = {'1', '2', '3', '4', '5', '6'}; // array for list of numbers
const char alphanum[] = "0123456789!@#$%^&*abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
string securityKey; // if user decides to user password, security key is a password, if user decides passphrase, security key is passphrase

void throwError()
{
    // Description: Outputs error message to user
    cout << "\nError: Invalid entry: Please try again.\n"
         << endl;
}

void toUpper(char &chr)
{
    if (chr >= 'a' && chr <= 'z')
    {
        // chr -= 32;
        chr = toupper(chr);
    }
}

// validate answers (Y/N)
void checkAnswer(string str, char &chr)
{
    do
    {
        cout << str;  // cout the string
        cin >> chr;   // console input into chr
        toUpper(chr); // automatically uppercase

        // Checks if input is valid
        if (!cin)
        { // !cin means the input failed, meaning a value other than a char was inputted
            cin.clear();
            cin.ignore();
            cout << "Invalid Input\n";
        }
        else if (chr != 'Y' && chr != 'N')
        {
            cout << "Invalid Input\n\n";
        }

    } while (chr != 'Y' && chr != 'N'); // condition to validate
}

// validate integers
void CheckAnswerInt(string str, int &integ)
{
    cout << str;            // outut stirng where function is called
    while (!(cin >> integ)) // while not an integer
    {
        cout << str;           // output string again
        cin.clear();           // clear previous input
        cin.ignore(123, '\n'); // ignore last 123 dig
    }
}

// Enhancement: if user wants password instead of passphrase
void GeneratePassword()
{
    int string_length = sizeof(alphanum) - 1; // size of array
    string password;

    int lengthOfPassword;
    CheckAnswerInt("Enter the length of password: ", lengthOfPassword); // prompt for length

    srand(time(0)); // random call
    cout << "Your password is: ";
    for (int i = 0; i < lengthOfPassword; i++)    // loop length amout of times
        cout << alphanum[rand() % string_length]; // generate the random characetr
}

// function for all prompts
void PassphraseGeneration(map<int, string> &dicewareList)
{
    // varraibles declare
    char capitalizeLetters;
    char appendSpecialCharacters;
    char passwordInstead;
    int wordsWanted;

    int x;

    checkAnswer("Would you like to generate a password instead of a passphrase? ", passwordInstead); // ask password
    if (passwordInstead == 'Y')
    {
        GeneratePassword();
    }
    else
    {

        CheckAnswerInt("\nHow many words would you like? ", wordsWanted);
        checkAnswer("\nWould you like to append special characters to your passphrase (Y/N)? ", appendSpecialCharacters);
        checkAnswer("\nWould you like your passphrase randomly capitalized (Y/N)? ", capitalizeLetters);

        passphrase = "";

        for (int i = 0; i < wordsWanted; i++) // looping to generate random code
        {
            // cout << passphrase;
            diceCode = "";
            for (int i = 0; i < 5; i++)
            {
                diceCode += numberArray[rand() % 6]; // rand function
            }

            passphrase += dicewareList[stoi(diceCode)] + " ";
        }

        if (appendSpecialCharacters == 'Y')
        {
            for (int i = 0; i < passphrase.length(); i++)
            {
                if (passphrase[i] != ' ' && rand() % 3 == 1)
                {
                    passphrase[i] = specialCharactersArray[rand() % 18];
                }
            }
        }

        if (capitalizeLetters == 'Y')
        {
            // loop the length of the passphrase amount of time
            for (int i = 0; i < passphrase.length(); i++)
            {
                if (rand() % 2 == 0)
                {
                    passphrase[i] = toupper(passphrase[i]); // uppercase the password at index i
                }
            }
        }
        cout << passphrase << endl; // output passphrase
    }
}

int main()
{
    srand(time(NULL));
    char languageWanted;
    // string dicewarePath;
    checkAnswer("\nWhat language would you like your passphrse to be? \nY for English\nN for French\n", languageWanted);

    map<int, string> dicewareList; // make map
    // if(languageWanted=='Y'){
    //   dicewarePath="diceware.txt";
    // }
    // else if(languageWanted=='N'){
    //   dicewarePath="frenchdiceware.txt";
    // }
    // ifstream file(dicewarePath);
    ifstream file(languageWanted == 'Y' ? "diceware.txt" : "frenchdiceware.txt");

    if (!file.good()) // run if file not found
    {
        cout << "Error: File not found!"; // give this error incase .txt file isnt found
        return -1;
    }

    while (!file.eof())
    {
        string currentLine;
        getline(file, currentLine);
        string stringNumber;
        string word;

        for (int i = 0; i < currentLine.length(); i++)
        {
            if (isdigit(currentLine[i]) && stringNumber.length() < 5) // loop through each current key index and add into map
            {
                stringNumber += currentLine[i]; // add current line index to string number
            }
            else if (!isspace(currentLine[i]))
            {
                word += currentLine[i]; // adding it to word
            }
        }
        dicewareList[stoi(stringNumber)] = word;
    }
    file.close();
    PassphraseGeneration(dicewareList);
}
