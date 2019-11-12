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
string sample = "username@12345.com";										// Size is 20, @ is at location 8
string database[100] = 
	{"kpatel@12345.com", "bkandler@12345.com", "markmcc2950@12345.com", "jyee@12345.com", "test@12345.com"};
string passwords[100] =
	{"password", "password1", "password2", "password3"};

// Booleans to check for validity of inputs
bool validity1 = false;
bool validity2 = false;
bool validity3 = false;

// Begin code:
//------------------------------------------------------------------------------------------------------------
// Call this with some string to display the error
void error(string err) {
	cout << "ERROR: " << err << endl;
}

void passCheck(int n) {
	int index = n;
	string password;
	int i = 0;
	cout << "Password:\t";
	cin >> password;
	if (password.size() == passwords[index].size()) {
		while (!validity3) {
			// If no password saved, return error. (Shouldn't be able to get here without a password though)
			if (passwords[index].empty()) {
				error("Empty password slot. Invalid username slot.");
				break;
			}
			int size = passwords[index].size();
			if (password[i] == passwords[index][i]) {
				i = (i + 1);
				if (i == size) {
					validity3 = true;
					break;
				}
			}
			else {
				error("Invalid password.");
				break;
			}
		}
	}
	else {
		error("Invalid password.");
	}
	
}

// Checks if the actual username itself is stored in the system
void nameCheck(string n) {
	int dbSize = (sizeof(database)) / 32;										// Size of the entire array of usernames
	bool check = true;
	int i = 0;
	int j = 0;
	while (!validity2) {
		int size = database[j].size();
		// Check to see if index value matches input in size and characters. Helps reduce runtime
		if (database[j].size() == n.size() && database[j][i] == n[i]) {
			if (n[i] == database[j][i]) {
				i = (i + 1);
				if (i == size - 12) {
					if (!passwords[j].empty()) {
						validity2 = true;
						passCheck(j);
					}
					else {
						error("No matching password in system for username entered.");
						break;
					}
				}
			}
		}
		else {
			j++;
			i = 0;
			validity2 = false;
		}
		// If we reach an empty index of the database that's yet to be assigned a username, exit the program.
		if (database[j].empty()) {
			error("Username Not Found");
			break;
		}		
		if (j >= dbSize) { error("Username Not Found."); break; }
	}
}

// Checks if the username is valid first (Includes @12345.com for now)
void validityCheck(string v) {
	int size = v.size();

	// Check if user name is valid
	if (size > 12) {
		// Check that we have @12345.com at the end (referenced against our sample)
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
	

	if (validity1 && validity2 && validity3) {
		cout << "Success!" << endl;
	}
}
//------------------------------------------------------------------------------------------------------------
// End code