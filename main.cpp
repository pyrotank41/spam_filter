/*
 * Author: Karan Singh Kochar
 * Date: 1/16/2020 
 * Env: coido, linux( ubuntu 14) 
 * purpose: Program for filter spam emails from a list of emails.
 * How to use it: This program takes inputs and displays output for terminal, hence use it from there.
 * commands: 
 *  Load: use this to load a list of spamList. Syntax: load <spam email list file name>
 *  Display: use this to display the list of spam emails loaded in program. Syntax: diaplay.
 *  Check: use this to check if an email is in spamList. syntax: check <emailname>
 *  filter: use this to filter out the emails from the that are spam. Syntax: filter <email list file> <out file name>
 *  # : used to exit program. Syntax: #
 * 
 * last updated: 1/18/2020
*/


#include <iostream>
#include <string>
#include <fstream>
#include "ourvector.h"

using namespace std;

// --------------------------------------------------------------------
// searching a string vector
// parameters: string to search, vector list to search from.
// return: true if string is found and false if string is not found.
bool binarySearch( string str, ourvector<string> &spamList){
    int high = spamList.size() - 1;
    int low = 0;
    int mid;
    while(high >= low){
        mid = low + (high - low)/2 ;// finding a middle index. ( This method prevents buffer overflow )
        string element = spamList[mid]; // accesing and storing to prevent dublicate access.
        if(element == str){ // if 
            return true;
        }
        else if(element < str)
        {
            low = mid + 1;
        }
        else
        {
            high = mid - 1;
        }
    }
    
    return false;
}

// -------------------------------------------------------------------- 
// functiion to open and read a file.
// parameters: fileName to load, referenced list, debug mode.
void loadFile( string fileName, ourvector<string> &list, int debug = 0)
{
    list.clear(); // resetting vector
    ifstream inFile;
    inFile.open(fileName);
    if(!inFile.is_open()) // if file doesnt open. 
    {
        cout << "**Error, unable to open '" << fileName << "'" << endl;
    }
    else
    {   if(debug != 0)
        {
            cout << "Loading '" << fileName << "'" << endl;
        }
        string temp;
        while(!inFile.eof())
        {
           getline( inFile, temp);
           if(!inFile.fail())
           {
                list.push_back(temp);    
           } 
        }
        inFile.close(); 
    }
    
}

// --------------------------------------------------------------------
// Simple function to display elements of list.
// parameters: string vector. 
void displayList(ourvector<string> &list){
    for( string s: list){
        cout << s << endl; 
    }
}

// Check is a given emailid is a spam.
// parameters: emailid to be checked, vector list of spam mails
// return: true if email is spam and false if email is not spam.
bool isSpam(string email, ourvector<string> &spamList)
{   
    int at = email.find('@');
    if(at == -1) //if the input is invalid
    {
        return false; // since its an invalid email id, it isnt a spam email. Just a quick fix to match with autograder. 
    }
    else
    {
        // splitting username and domain name from the email id.
        string usrName = email.substr(0,at);
        string domain = email.substr(at+1,-1);
        string checkdom = domain + ":*";
        string checkEmail = domain + ":" + usrName;

        if(!binarySearch(checkdom, spamList)) //checking for a spamed domain.
        { 
            if(!binarySearch(checkEmail, spamList)) // checking for spamed user.
            {
                return false; // is not spam.
            }
            else
            {
                return true; //is spam
            }
        }
        
        else 
        {
            return true; //is spam.
        }
    } 
    return false;
}

// --------------------------------------------------------------------
// Filters the emails from spam emails.
// Parameters: email's file name, output file name, vector list of spam emails.
void filter( string emailFileName, string outFileName, ourvector<string> &spamList){
    
    ourvector<string> emailList;
    loadFile(emailFileName,emailList); // simpling populating from emailList with content in the email file.
    
    if(emailList.size()!=0) // making sure emailList is not empty.
    {        
        // opening and writing to the file.
        ofstream outFile;
        outFile.open(outFileName);
        if(!outFile.is_open()){
            cout << "**Error, output file not opening" << endl;
        }
        else // if file opens.
        {   
            int nonSpam = 0;
            for(signed int i = 0; i < emailList.size(); i++)
            {   
                // extracting email id from the string
                string str = emailList[i];
                int start = str.find(' '); //get start index of the emailid. 
                while(str[start+1] == ' '){ // making sure to eleminate any space character.
                    start++;
                }
                int end = str.find(' ', start + 1); //get end index of emailid. 
                string email = str.substr(start+1, end - (start+1)); // Substr(start index, length of the string to extract).

                // checking if that email is a spam of not.
                if(!isSpam(email, spamList)){
                    outFile << str << endl; // storing emails to the output file.
                    nonSpam++; // updating the count of non spam emails.
                }

            }
            cout << "# emails processed: " << emailList.size() << endl;
            cout << "# non-spam emails: " << nonSpam << endl;
            outFile.close();
            // writing to the file ends here.
        }
        
        
        
    
    }    
}

// --------------------------------------------------------------------
// This is our main boy!! very important, no joke.
int main(){
    
    ourvector<string> spamList;

    cout << "** Welcome to spam filtering app **" << endl << endl;
    
    while(1)
    {
        string command;
        cout << "Enter command or # to exit> ";
        cin >> command;
        if(command == "#") // exit the program.
        {
            break;
        }
        
        else if(command == "load") // load a spam list.
        {
            string fileName;
            cin >> fileName;
            loadFile(fileName, spamList, 1);
            if(spamList.size() > 0)
            {
               cout << "# of spam entries: " << spamList.size() << endl; 
            }
        }
        else if(command == "display") // diaplay spam list.
        {
            displayList(spamList);
        }
        else if(command == "filter") // filtering spam emails and output filtered emails to a file.
        {
            string emailFileName, outFileName;
            cin >> emailFileName;
            cin >> outFileName;
            filter(emailFileName, outFileName, spamList);
        }
        else if(command == "check") // check if domain is a spam or not.
        {   
            string email;
            cin >> email;
            bool res = isSpam(email, spamList);
            if(res) // if the responce from isSpam is true then output "is spam".
            {
                cout << email << " is spam" << endl; 
            }
            else
            {
                cout << email << " is not spam" << endl; 
            }
        }
        else
        {
            cout << "**invalid command" << endl;
        }
            
        cout << endl; // add a space between output and new input line.
    }
    
    return 0;
} 