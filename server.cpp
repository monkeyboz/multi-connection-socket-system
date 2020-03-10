#include "headers/portcreator.h"

using namespace std;
int main(int a,char** arg){
	string port;
	while(port != "exit"){
		cout << "Choose which port to use for this server: ";
		getline(cin,port,'\n');
		if(port == "exit") break;
		PortCreator* m = new PortCreator(stoi(port),0);
		cout << "tesing";
	}
	cout << "Closing server creation" << endl;
	return 0;
}
