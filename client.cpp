#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#define PORT 6921


struct response{
    bool correct;
    char message[256];
    int number;
};




int main(){

    int clientfd, status,valread;
    struct sockaddr_in server_address;
    int numberGuess;
    struct response recievedResponse;

    bool playAgain = true;
    char choice;

    while(playAgain){

        if((clientfd = socket(AF_INET, SOCK_STREAM, 0)) <0){
            printf("Socket creation error \n");
            return -1;
        }

        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(PORT);

        if(inet_pton(AF_INET,"127.0.0.1",&server_address.sin_addr) <=0){
            printf("Invalid address/ Address not supported \n");
            return -1;
        }

        if((status = connect(clientfd,(struct sockaddr*) &server_address,sizeof(server_address))) < 0){
            printf("Connection failed\n");
            return -1;
        }

        printf("Guess a number: ");
        std::cin >> numberGuess;
        send(clientfd,&numberGuess,sizeof(int),0);
        printf("Guess sent to server!\n");

        valread = read(clientfd, &recievedResponse,sizeof(struct response));

        if(recievedResponse.correct)
            std::cout << "Correct Guess you win!!!!!" << std::endl;
        else{
            std::cout << "Wrong guess the correct number was : " << recievedResponse.number << std::endl;
        }

        std::cout << "Would you like to play again [y/n]: ";
        std::cin >> choice;
        if(choice == 'n')
            playAgain = false;
        close(clientfd);
    }
    




    return 0;


}