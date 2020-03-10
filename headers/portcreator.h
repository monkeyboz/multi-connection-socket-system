#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

class PortCreator{
	private:
		struct sockaddr_in address;  //socket address
		int max_clients = 30;
		int client_sockets[30];
		int port; //port number
		int timeout; //timeout length
		bool open; //check if port is open
		fd_set readfds;
		int addrlen;
	public:
		PortCreator(){
			this->client_sockets[this->max_clients] = {0};
			this->port = 0; //set port to 0 until specified
			this->timeout = 0; //set timeout to 0
			this->open = false; //set open to false
			this->addrlen = sizeof(this->address); //set addrlen to address length
		}
		PortCreator(int port, int timeout,int open){
			this->client_sockets[this->max_clients] = {0};
			this->port  = port; //set port to passed variable
			this->timeout = timeout; //set timeout to passed variable
			this->open = open; //set open to passed variable
			this->addrlen = sizeof(this->address); //set addrlen to address length
		}
		PortCreator(int port,int timeout){
			this->client_sockets[this->max_clients] = {0};
			this->port = port; //set port to passed variable
			this->timeout = timeout; //set timeout to passed variable
			this->open = this->testPort(); //set open to return of testPort
		}
		//test port will check to see if port is able to be created and then will listen
		//to the port until a command is given to disconnect the port
		bool testPort(){
			bool open = false; // open set to false
			this->max_clients = 10;
			this->client_sockets[this->max_clients];
			for(int i = 0; i < this->max_clients; ++i){
				this->client_sockets[i] = 0;
			}
			this->addrlen = sizeof(this->address); //addrlen set to address length
			int opt = 1; //opt used for options (default 1)
			int server_fd,new_socket,valread; // create variables for server_fd new_socket and valread
			//if server_fd variable can be instantiated then continue otherwise error
			if((server_fd = (int) socket(AF_INET,SOCK_STREAM,0)) == 0){
				perror("socket failed");
				exit(EXIT_FAILURE);
			}
			cout << server_fd << "created" << endl;
			//if setsockopt returns false then continue otherwise error
			if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR | SO_REUSEPORT, &opt,sizeof(opt))){
				perror("setsockopt");
				exit(EXIT_FAILURE);
			}
			//if both tests pass then set address object variables
			this->address.sin_family = AF_INET;
			this->address.sin_addr.s_addr = INADDR_ANY;
			this->address.sin_port = htons(this->port);
			cout << "port created" << endl;
			//check to make sure that server_fd can be binded to address otherwise error
			if(bind(server_fd, (struct sockaddr *)&this->address,sizeof(this->address)) < 0){
				perror("bind failed");
				exit(EXIT_FAILURE);
			}
			cout << "bind created" << endl;
			//listen to server_fd maximum of 3 pending connections for the master socket otherwise error
			if(listen(server_fd,3) > 0){
				perror("listen");
				exit(EXIT_FAILURE);
			}
			
			cout << "Listening" << endl;
			char buffer[1024] = {0}; // receiving buffer max length 1024
			//if new_socket can be created then continue otherwise error
			
			while(true){
				FD_ZERO(&this->readfds);
				
				FD_SET(server_fd,&this->readfds);
				int max_sd = server_fd;
				int sd;
				for(int i = 0; i < this->max_clients;++i){
					sd = this->client_sockets[i];
					if(sd > 0){
						FD_SET(sd,&this->readfds);
					}
					if(sd > max_sd){
						max_sd = sd;
					}
				}
				int activity = select(max_sd+1,&this->readfds,NULL,NULL,NULL);
				
				if((activity < 0) && (errno != EINTR)){
					printf("Select error");
				}
				
				if(FD_ISSET(server_fd,&this->readfds)){
					if((new_socket = accept(server_fd, (struct sockaddr *)&address,(socklen_t*)&addrlen)) < 0){
						perror("accept");
						exit(EXIT_FAILURE);
					}
					
					cout << "Port created ... now listening ... " << server_fd << endl;
					char* connection = (char*)"Looks like you're connected: ";
					string m = "";
					//loop without issue
					valread = read(new_socket, buffer, 1024);
					cout << (string)buffer << endl;
					if(send(new_socket,connection,strlen(connection),0) != strlen(connection)){
						perror("send");
					}
					printf("Hello message sent\n");
					for(int i = 0;i < this->max_clients; ++i){
						if(this->client_sockets[i] == 0){
							cout << "endl" << new_socket << endl;
							this->client_sockets[i] = new_socket;
							cout << "Added client to socket list" << endl;
							printf("Added client to socket list");
							break;
						}
					}
					int count = 0;
					string compiled = "";
				}else{
					for(int i = 0; i < this->max_clients;++i){
						sd = this->client_sockets[i];
						if(FD_ISSET(sd,&this->readfds)){
							if((valread = read(sd,buffer,1024)) == 0){
								getpeername(sd,(struct sockaddr*)&address,(socklen_t*)&addrlen);
								printf("Disconnected, ip %s, port %d\n",inet_ntoa(address.sin_addr),ntohs(address.sin_port));
								close(sd);
								client_sockets[i] = 0;
							}else{
								buffer[valread] = '\0';
								send(sd,buffer,strlen(buffer),0);
								cout << (string)buffer << endl;
							}
						}
					}
				}
			}
			cout << "exiting" << endl;
			bzero(buffer,1024);
			close(server_fd);
			close(new_socket);

			return true;
		}
};
