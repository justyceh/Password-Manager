#include "database.h"
using namespace std;

int main(){
    Database h;
    bool myBool = h.intialize();
    if(myBool == false){
        cout << "Account created, Login please!" << endl;
        return 0;
    }
    h.openMainMenu(myBool);
    
    return 0;
}