//Program by Zander3312. Allows users to have their own custom titles in UNICLR. Check readme for usage instructions.

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main() {
    ifstream saveFile("SYS-DATA", ios::binary); //Initialize input file stream reading from UNICLR save as a binary file

    if (!saveFile) //Checks if given input file exists
    {
        cerr << "UNICLR save data not found. Make sure it's in the same directory and named 'SYS-DATA'.\nPress enter to continue...";
        cin.get();
        return 0;
    }

    saveFile.seekg(0, ios::end); //Get the length of the save file, should match all other save files
    int saveLength = saveFile.tellg(); //428194
    if (saveLength != 428194) {
        cerr << "Save file is invalid. Make sure it's a UNICLR save file called 'SYS-DATA'.\nPress enter to continue...";
        cin.get();
        return 0;
    }

    cout << "Save data found." << endl;
    saveFile.seekg(282597);
    char endChar;
    endChar = saveFile.get();
    int length = 0;
    for (int i = 0; i <= 63; i++) {
        saveFile.seekg(282598 + i);
        if (saveFile.get() == endChar) {
            break;
        }
        length++;
    }
    saveFile.seekg(282598);
    char* buffer = new char[length];

    saveFile.read(buffer, length);

    cout << "Current title: ";
    cout.write(buffer, length);

    delete[] buffer;

    string newTitle = "";
    string newTitle1 = ""; //Three titles, one for each segment of the online title
    string newTitle2 = "";
    string newTitle3 = "";
    cout << "\nEnter a new title: ";
    getline(cin, newTitle);
    length = newTitle.length();
    if (length > 63) {
        cerr << "Error: Maximum title length is 63 characters. Please rerun the program and try again.\nPress enter to continue...";
        cin.get();
        return 0;
    }
    if (length > 42) { //Set new title strings for online title
        newTitle1 = newTitle.substr(0, 21);
        newTitle2 = newTitle.substr(21, 21);
        newTitle3 = newTitle.substr(42, newTitle.length() - 42);
    }
    else if (length > 21) {
        newTitle1 = newTitle.substr(0, 21);
        newTitle2 = newTitle.substr(21, newTitle.length() - 21);
    }
    else {
        newTitle1 = newTitle.substr(0, newTitle.length());
    }

    ofstream output;
    output.open("SYS-DATAoutput", ios::binary);

    if (output.is_open()) {
        saveFile.seekg(0); //Go back to beginning of save file
        output << saveFile.rdbuf(); //Copy save file to new output file
        saveFile.close(); //Close save file

        output.seekp(282598); //Go to offline title location in output
        output.write(newTitle.c_str(), sizeof(char) * newTitle.size()); //Write new title in offline location
        output.put(endChar); //Add ending character to tell UNICLR not to read any further characters

        output.seekp(8763); //Go to online title location 1 in output
        output.write(newTitle1.c_str(), sizeof(char) * newTitle1.size()); //Write new title in online location
        output.put(endChar); //Add ending character to tell UNICLR not to read any further characters

        output.seekp(8796); //Go to online title location 2 in output
        output.write(newTitle2.c_str(), sizeof(char) * newTitle2.size()); //Write new title in online location
        output.put(endChar); //Add ending character to tell UNICLR not to read any further characters

        output.seekp(8829); //Go to online title location 3 in output
        output.write(newTitle3.c_str(), sizeof(char) * newTitle3.size()); //Write new title in online location
        output.put(endChar); //Add ending character to tell UNICLR not to read any further characters
        output.close();

        cout << "Title changed. Press enter to close program.";
        cin.get();
        return 0;
    }
    else {
        cerr << "Error: Could not create output file." << endl;
    }

}
