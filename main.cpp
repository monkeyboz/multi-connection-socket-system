#include <iostream>
#include "headers/portcreator.h"
#include "headers/portconnect.h"

using namespace std;

int main(int a,char** arg){
	int port;
	cout << "Create a server with what port (port number): ";
	cin >> port;
	cout << "Server being created";
	PortCreator* m = new PortCreator(port,0);

	PortConnect* v = new PortConnect(port,(char*)"localhost");
	v->connect_port();
}
