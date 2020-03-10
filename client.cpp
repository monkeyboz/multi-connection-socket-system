#include <iostream>
#include <string.h>
#include "headers/portconnect.h"

using namespace std;

int main(int a,char** arg){
	string host;
	string port;
	cin.ignore();
	while(host != "exit"){
		cout << "Insert the host address: ";
		getline(cin,host,'\n');
		if(host == "exit") break;
		cout << "Insert the port: ";
		getline(cin,port,'\n');
		PortConnect* m = new PortConnect(stoi(port),(char*)host.c_str());
	}
	cout << "Closing client" << endl;
}
