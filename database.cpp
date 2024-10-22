#include "database.h"
void Database::loadPasswords(){
    ifstream read(passHashFile, ios::binary);
    if(read.is_open()){
        unsigned char iv[16];
        read.read(reinterpret_cast<char*>(iv), sizeof(iv));
        
        vector<unsigned char> cipherText((istreambuf_iterator<char>(read)), istreambuf_iterator<char>());
        read.close();

        vector<unsigned char> key = deriveKey(masterPassword, salt);

        string plaintext = decryptAES(cipherText, key.data(), iv);

    istringstream stream(plaintext);
    string appName;
    string pass;
    while(getline(stream, appName, ',')){
    if(appName.empty()){
        continue;
    }
    getline(stream, pass);
    Passwords[appName] = pass;
    }
    }
}
void Database::savePasswords(){
    ofstream file(passHashFile);
    string plaintext;
    for(const auto& pair : Passwords){
        plaintext += pair.first + "," + pair.second + '\n';
    }
    unsigned char iv[16];
    RAND_bytes(iv, sizeof(iv));

    vector<unsigned char> key = deriveKey(masterPassword, salt);

    vector<unsigned char> cipherText = encryptAES(plaintext, key.data(), iv);
    file.write(reinterpret_cast<const char*>(iv), sizeof(iv));
    file.write(reinterpret_cast<const char*>(cipherText.data()), cipherText.size());
    file.close();
}
void Database::deletePassword(){
    string appToDelete;
    cout << "===All Stored Passwords===" << endl;
    int i = 1;
    for(const auto& pair : Passwords){
                cout << i << ". " << pair.first << endl;
                ++i;
            }
    cout << "Enter app name to delete password" << endl << "Your Choice: ";
    cin >> appToDelete;
    for(int i = 0; i < appToDelete.length(); i++){
        appToDelete[i] = tolower(appToDelete[i]);
    }
    

    if(Passwords.find(appToDelete) != Passwords.end()){
        Passwords.erase(appToDelete);
        cout << appToDelete << "'s password successfully deleted!" << endl;    
    }
    else{
        cout << "App name not found, Try again!" << endl;
        }
         
}
void Database::viewPassword(){
            string appNameToView;
            cout << "===All Stored Passwords===" << endl;
            int i = 1;
            for(const auto& pair : Passwords){
                cout << i << ". " << pair.first << endl;
                i++;
            }
            cout << endl << "What app's password would you like to view? (Enter it's name)" << endl << "Your Choice: ";
            cin >> appNameToView;
            for(int i = 0; i < appNameToView.length(); i++){
                appNameToView[i] = tolower(appNameToView[i]);
            }
            if(Passwords.find(appNameToView) != Passwords.end()){

            cout << appNameToView << "'s password is: " << Passwords[appNameToView] << endl;
            }
            else{
                cout << "App name not found, Try again!" << endl;
            }
}
void Database::addPassword(){
            string nameOfApp;
            string password;
            cout << "Enter name of the App: ";
            cin >> nameOfApp;
            for(int i = 0; i < nameOfApp.length(); i++){
                nameOfApp[i] = tolower(nameOfApp[i]);
            }
            cout << "Enter the Password: ";
            cin >> password;
            Passwords[nameOfApp] = password;
            cout << endl << "Successfully created Password" << endl;
}
void Database::promptMenu(){
        cout << "\n" << "Main Menu" << "\n";
        cout << "Please Select a Option" << endl;
        cout << "1. Add a Password" << endl << "2. View a Password" << endl << "3. Delete a Password" << endl << "4. Exit" << endl << endl;
        cout << "Your Choice: ";
}
void Database::openMainMenu(bool a){
    int userChoice = 0;
    if(a == false){
        cout << "Could Not Open Menu";
    }
    else{
        while(userChoice != 4){
        promptMenu();
        cin >> userChoice;
        if(userChoice != 1 && userChoice != 2 && userChoice != 3 && userChoice != 4){
            cout << "Invalid Option" << endl;
           break;
        }
        switch(userChoice){
            case 1:
            {
            addPassword();
            savePasswords();
            }
            break;
            case 2:
            {
            viewPassword();
            }
            break;
            case 3:
            {
            deletePassword();
            savePasswords();
            }
            break;
            case 4:
            {
                cout << "Goodbye!" << endl;
            }
            break;
        }
        }
    }
}
bool Database::hashExist(){
    ifstream file(hashFile);
    return file.good();
}
void Database::saveHash(string& newPass){
    ofstream file(hashFile);
    file << newPass;
    file.close();
}
void Database::setMasterPassword(){
    int validInput = 0;
    string word;
    string word2;
    while(validInput == 0){
        cout << "Enter your Master Password (Choose Carefully as once you enter it, you wont be able to see it again)" << endl;
        cin >> word;
        cout << "Please enter again: " << endl;
        cin >> word2;
        if(word == word2){
            cout << "Master Password succesfully created!" << endl;
            
           word = hashPassword(word);
           masterPassword = word;
        //    masterPassword = word;
            saveHash(word);
            validInput = 1;
        }
        else{
            cout << "Invalid Password, Try again.." << endl;
            validInput = 0;
        }
    };
}
bool Database::verifyMasterPassword(){
    string enteredPassword;
    string storedPassword = loadHash();
    do{
    cout << "Enter your Master Password: " << endl;
    cin >> enteredPassword;
    enteredPassword = hashPassword(enteredPassword);
    if(enteredPassword != storedPassword){cout << "Incorrect Password" << endl << "Try again.." << endl;}
    }while(enteredPassword != storedPassword);

    return enteredPassword == storedPassword;
}
bool Database::intialize(){
    cout << "===Justyce's Password Manager===" << endl;
    if(!hashExist()){
        setMasterPassword();
        return 0;
    }
    else{
    if(verifyMasterPassword()){
        cout << "Correct Password!" << endl << "Logging in..." << endl;
        loadPasswords();
        return true;
    }
    else{
        return false;
    }
    }
    return true;
}
vector<unsigned char> Database::deriveKey(const string& masterPassword, const vector<unsigned char>& salt) {
    const int keyLength = 32; 
    vector<unsigned char> key(keyLength);
    int result = PKCS5_PBKDF2_HMAC(masterPassword.c_str(), masterPassword.length(), salt.data(), salt.size(), 10000, EVP_sha256(), keyLength, key.data());
    
    if (result != 1) {
        throw runtime_error("Key derivation failed");
    }
    
    return key;
}
vector<unsigned char> Database::encryptAES(const string& plaintext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw runtime_error("Failed to create cipher context");
    }

    // Initialize encryption
    if (EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Failed to initialize encryption");
    }

    vector<unsigned char> ciphertext(plaintext.size() + EVP_MAX_BLOCK_LENGTH); // Buffer for ciphertext
    int len;
    if (EVP_EncryptUpdate(ctx, ciphertext.data(), &len, reinterpret_cast<const unsigned char*>(plaintext.c_str()), plaintext.length()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Failed to encrypt");
    }
    int ciphertext_len = len;

    // Finalize encryption
    if (EVP_EncryptFinal_ex(ctx, ciphertext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        throw runtime_error("Failed to finalize encryption");
    }
    ciphertext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    ciphertext.resize(ciphertext_len); // Resize to actual length
    return ciphertext;
}
vector<unsigned char> Database::generateSalt(size_t length){
    vector<unsigned char> salt(length);
    if(RAND_bytes(salt.data(), length) != 1){
        throw runtime_error("Failed to generate salt");
    } 
    return salt;
}
string Database::decryptAES(const vector<unsigned char>& ciphertext, const unsigned char* key, const unsigned char* iv) {
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) {
        throw runtime_error("Failed to create cipher context");
    }

    // Initialize decryption
    if (EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        ERR_print_errors_fp(stderr); // Print OpenSSL errors
        throw runtime_error("Failed to initialize decryption");
    }

    vector<unsigned char> plaintext(ciphertext.size() + EVP_MAX_BLOCK_LENGTH); // Buffer for plaintext
    int len;

    if (EVP_DecryptUpdate(ctx, plaintext.data(), &len, ciphertext.data(), ciphertext.size()) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        ERR_print_errors_fp(stderr); // Print OpenSSL errors
        throw runtime_error("Failed to decrypt");
    }

    int plaintext_len = len;

    // Finalize decryption
    if (EVP_DecryptFinal_ex(ctx, plaintext.data() + len, &len) != 1) {
        EVP_CIPHER_CTX_free(ctx);
        ERR_print_errors_fp(stderr); // Print OpenSSL errors
        throw runtime_error("Failed to finalize decryption");
    }
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
    plaintext.resize(plaintext_len); // Resize to actual length
    return string(reinterpret_cast<char*>(plaintext.data()), plaintext_len); // Return plaintext as string
}
string Database::hashPassword(const string& password){
    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash;

    EVP_MD_CTX* context = EVP_MD_CTX_new();

    EVP_DigestInit_ex(context, EVP_sha256(), NULL);

    EVP_DigestUpdate(context, password.c_str(), password.size());

    EVP_DigestFinal_ex(context, hash, &lengthOfHash);

    EVP_MD_CTX_free(context);

    string hexHash;
    for(unsigned int i = 0; i < lengthOfHash; ++i){
        hexHash += "0123456789abcdef"[hash[i] >> 4];
        hexHash += "0123456789abcdef"[hash[i] & 0xF];
    }
    return hexHash;
}
string Database::loadHash(){
    string hashCode;
    ifstream read(hashFile);
    if(read.is_open()){
        getline(read, hashCode);
        read.close();
    }
    return hashCode;
}
