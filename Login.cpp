#include <stdio.h>
#include <iostream>
#include <string.h>
#include <vector>

using namespace std;
/*
	PASSWORDS:
		Passwords stored in array of encrypted passwords
		Each password is stored in the same index value as the username
			If username is index 0, password is also stored in index 0
		User inputs password, which is encrpyted based on something in system exclusive to each user.
		If encrypted password matches value stored in array index, then it's a match
*/

// Global Variables
string sample = "username@1234567.com";										// Size is 20, @ is at location 8
string database[100] = 
	{"kpatel@1234567.com", "bkandler@1234567.com", "markmcc2950@1234567.com", "jyee@1234567.com", "test@1234567.com"};
bool validity1 = false;
bool validity2 = false;

// Begin code:
//------------------------------------------------------------------------------------------------------------
// Call this with some string to display the error
void error(string err) {
	cout << "ERROR: " << err << endl;
}

// Checks if the actual username itself is stored in the system
void nameCheck(string n) {
	int dbSize = (sizeof(database)) / 32;
	int i = 0;
	int j = 0;
	while (!validity2) {
		// If we reach an empty index of the database that's yet to be assigned a username, exit the program.
		if (database[j].empty()) {
			error("Empty slot. Username Not Found");
			break;
		}
		int size = database[j].size();
		if (n[i] == database[j][i]) {
			i = (i + 1) % size;
			if (i == size - 12) {
				validity2 = true;
			}
		}
		else {
			j++;
			i = 0;
			validity2 = false;
		}
		if (j >= dbSize) { error("Username Not Found."); break; }
	}
}

// Checks if the username is valid first (Includes @1234567.com for now)
void validityCheck(string v) {
	int size = v.size();

	// Check if user name is valid
	if (size > 12) {
		// Check that we have @1234567.com at the end (referenced against our sample)
		for (int i = 0; i < 12; i++) {
			if (v[size - 12 + i] != sample[8 + i]) {
				error("Invalid Username.");
				validity1 = false;
				break;
			}
			else {
				validity1 = true;
			}
		}
	}
	else {
		error("Invalid Username.");
		validity1 = false;
	}
}

int main(void) {	
	string userName;
	
	cout << "Username:\t";
	cin >> userName;
	
	if (userName.size() > 12 && !userName.empty()) {
		validityCheck(userName);
	}
	else {
		error("Too short of username.");
	}
	if (validity1) { nameCheck(userName); }
	

	if (validity1 && validity2) {
		cout << "Success!" << endl;
	}
}
//------------------------------------------------------------------------------------------------------------
// End code