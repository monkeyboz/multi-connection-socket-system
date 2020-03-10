#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

using namespace std;
class PortConnect{
	private:
		int port;
		char* message;
		char* serverAddress;
		struct sockaddr_in serv_addr;
	public:
		PortConnect(){
		}
		PortConnect(int port,char* serverAddress){
			this->port = port;
			this->serverAddress = serverAddress;
			connect_port();
		}
		bool connect_port(){
			int sock = 0, valread;
			char buffer[1024] = {0};
			if((sock = socket(AF_INET,SOCK_STREAM,0)) < 0){
				printf("\n Socket creation failed. \n");
				return false;
			}
			serv_addr.sin_family = AF_INET;
			serv_addr.sin_port = htons(this->port);

			if(inet_pton(AF_INET,this->serverAddress,&serv_addr.sin_addr) <= 0){
				printf("\n Server address not supported. \n");
				return false;
			}
			if(connect(sock, (struct sockaddr*)&serv_addr,sizeof(serv_addr))){
				printf("\nConnection failed\n");
				return false;
			}
			printf("Connected to Server\n");
			bool test = true;
			string message = "";
			int count = 0;
			cin.ignore();
			while(message != "exit" || message != "close"){
				//cin.ignore();
				getline(cin,message,'\n');
				if(message == "exit") break;
				const char* c = message.c_str();
				cout << message << strlen(c);
				send(sock,c,strlen(c),0);
				printf("Hello messager\n");
				valread = read(sock,buffer,1024);
				std::cout << valread << std::endl;
				cout << buffer << endl;
				++count;
			}
			close(sock);
			return true;
		}
};
