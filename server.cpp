#include <iostream> //Standard io stuff
#include <sys/socket.h> //For sockets
#include <string.h> // For string stuff
#include<stdlib.h> // For exit_failure macro
#include <netinet/in.h> // For sockaddr_in struct
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <cstdlib> //For random numbers 
#define PORT 6921

struct response{
    bool correct;
    char message[256] ;
    int number;
};






int main(){

    int sockfd, newSockfd;
    int opt = 1;
    struct sockaddr_in server_address;
    socklen_t addrlen = sizeof(server_address);
    ssize_t clientRead;
    int clientGuess;
    char * failureMessage = "You guessed the wrong number, the correct number was... ";
    char * correctMessage = "You guessed correctly congradulations!!!";
    response serverResponse;

    srand(time(0));//seeding rand value

    if((sockfd = socket(AF_INET,SOCK_STREAM,0))<0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt))){
        perror("setsockopt reuseaddr");
        exit(EXIT_FAILURE);
    }
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEPORT,&opt,sizeof(opt))){
        perror("setsockopt reuseport");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);

    if(bind(sockfd,(struct sockaddr*) &server_address,sizeof(server_address)) < 0){
        perror("bind failure");
        exit(EXIT_FAILURE);
    }

    std::cout << "[+]Server commencing listening process..." << std::endl;

    if(listen(sockfd,3) < 0){
        perror("listen fail");
        exit(EXIT_FAILURE);
    }
    
    while(1){
        int randomNumber = (rand() % 100);
        std::cout << "The servers selected number is : "<< randomNumber << std::endl;

        if((newSockfd = accept(sockfd, (struct sockaddr*) &server_address,&addrlen)) < 0){
            perror("accepting fail");
            exit(EXIT_FAILURE);
        }

        if(recv(newSockfd,&clientGuess,sizeof(int),0) < 0){
            perror("Recieve failed");
            exit(EXIT_FAILURE);

        }

        std::cout << "Client guessed: " << clientGuess << std::endl;
        if(clientGuess == randomNumber){
            serverResponse.correct = true;
            strcpy(serverResponse.message , correctMessage);
            serverResponse.number = randomNumber;
            
            send(newSockfd,&serverResponse,sizeof(struct response),0);
        }
        else{
            serverResponse.correct = false;
            strcpy(serverResponse.message , failureMessage);
            serverResponse.number = randomNumber;
            send(newSockfd,&serverResponse,sizeof(struct response),0);
        }
        close(newSockfd);


    }
    
    close(sockfd);
    return 0;



}