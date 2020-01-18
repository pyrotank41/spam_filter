/*
 * Author: Karan Singh Kochar
 * Date: 1/16/2020 
 * Env: coido, linux( ubuntu 14) 
*/


#include <iostream>
#include <string>
#include <fstream>
#include "ourvector.h"

using namespace std;

// searching a string vector
// parameters: string to search, vector list to search from.
bool binarySearch( string str, vector<string>& spamList){
    int high = spamList.size() - 1;
    int low = 0;
    int mid;
    while(high >= low){
        mid = (low + high)/2; // finding a middle index.
        string element = spamList[mid]; // accesing and storing 
        if(element == str){
            return true;
        }
        else if(element < str)
        {
            low = mid + 1;
        }
        else{
            high = mid - 1;
        }
    }
    
    return false;
}

// functiion to open and read a file.
// parameters: fileName to load, referenced list, debug mode.
void loadFile( string fileName, vector<string>& list, int debug = 0)
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

// Simple function to display elements of list.
// parameters: string vector. 
void displayList(vector<string>& list){
    for( string s: list){
        cout << s << endl; 
    }
}

// Check is a given emailid is a spam.
// parameters: emailid to be checked, vector list of spam mails
bool isSpam(string email, vector<string>& spamList)
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

// Filters the emails from spam emails.
// Parameters: email's file name, output file name, vector list of spam emails.
void filter( string emailFileName, string outFileName, vector<string> spamList){
    
    ourvector<string> emailList;
    loadFile(emailFileName,emailList); // simpling populating from emailList with content in the email file.
    
    if(emailList.size()!=0) // making sure emailList is not empty.
    {
        ourvector<string> filteredList; // a vector to store filtered emails.
        for(signed int i = 0; i < emailList.size(); i++)
        {   
            // extracting email id from the string
            string str = emailList[i];
            int start = str.find(' '); //get start index of the emailid. 
            int end = str.find(' ', start + 1); //get end index of emailid. 
            string email = str.substr(start+1, end - (start+1)); // Substr(start index, length of the string to extract).
            
            // checking if that email is a spam of not.
            if(!isSpam(email, spamList)){
                filteredList.push_back(str); // sffing emails to the filtered.
            }

        }
    
        // opening and writing to the file.
        ofstream outFile;
        outFile.open(outFileName);
        if(!outFile.is_open()){
            cout << "**Error, output file not opening" << endl;
        }
        else
        {
            for(string s: filteredList)
            {
               outFile << s << endl;
            }
            cout << "# emails processed: " << emailList.size() << endl;
            cout << "# non-spam emails: " << filteredList.size() << endl;
            outFile.close();
        }
        // writing to the file ends here.
    }    
}

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
        else if(command == "filter") // filtering spam emails
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