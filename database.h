#ifndef DATABASE_H
#define DATABASE_H
#include <iostream>
#include <sstream>
#include <fstream>
#include <ctype.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <vector>
#include <unordered_map>
#include <openssl/err.h>
using namespace std;
class Database{
    private:
    string masterPassword;
    vector<unsigned char> salt;
    string hashFile = "masterpasswordhash.txt";
    string passHashFile = "storedpasswordhash.txt";
    unordered_map<string, string> Passwords;
    public:
    void openMainMenu(bool);
    bool intialize();
    void saveHash(string&);
    string loadHash();
    bool verifyMasterPassword();
    void setMasterPassword();
    bool hashExist();
    void promptMenu();
    void addPassword();
    void viewPassword();
    void deletePassword();
    void loadPasswords();
    void savePasswords();
    string hashPassword(const string&);
    vector<unsigned char> generateSalt(size_t length = 16);
    vector<unsigned char> deriveKey(const string& masterPassword, const vector<unsigned char>& salt);
    vector<unsigned char> encryptAES(const string& plaintext, const unsigned char* key, const unsigned char* iv);
    string decryptAES(const vector<unsigned char>& ciphertext, const unsigned char* key, const unsigned char* iv);
};
#endif