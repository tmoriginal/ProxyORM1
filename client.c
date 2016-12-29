/*
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2016/2017
    Semestar:       Zimski (V)

    Ime fajla:      client.c
    Opis:           TCP/IP klijent

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>      //printf
#include<stdlib.h>
#include<string.h>     //strlen
#include<sys/socket.h> //socket
#include<arpa/inet.h>  //inet_addr
#include <fcntl.h>     //for open
#include <unistd.h>    //for close

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015
#define login_length 20000

int main(int argc , char *argv[])
{
    int sock,read_size;
    struct sockaddr_in server;
    char username_request[login_length], password_request[login_length];
    char username[login_length], password[login_length];
    char status_buff[login_length];

    //Create socket
    sock = socket(AF_INET , SOCK_STREAM , 0);
    if (sock == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(DEFAULT_PORT);

    //Connect to remote server
    if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }

    puts("Connected\n");
    fflush(stdout);

    //Receive username request!
    while( (read_size = recv(sock , username_request , DEFAULT_BUFLEN , 0)) > 0 )
    {
        puts(username_request);
        fflush(stdout);
        break;
    }

    if(read_size == -1)
    {
      perror("recv failed");
      return 0;
    }

    //Enter username
    gets(username);
    fflush(stdin);

    //Send username
    if( send(sock , username , strlen(username), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    //Receive password request!
    while( (read_size = recv(sock , password_request , DEFAULT_BUFLEN , 0)) > 0 )
    {
        puts(password_request);
        fflush(stdout);
        break;
    }

    if(read_size == -1)
    {
      perror("recv failed");
    }

    //Enter password

    gets(password);
    fflush(stdin);

    //Send password
    if( send(sock , password , strlen(password), 0) < 0)
    {
        puts("Send failed");
        return 1;
    }

    //Login successfully?
    while( (read_size = recv(sock , status_buff , DEFAULT_BUFLEN , 0)) > 0 )
    {
        puts(status_buff);
        fflush(stdout);
        break;
    }

    if(read_size == -1)
    {
      perror("recv failed");
    }

    close(sock);

    return 0;
}
