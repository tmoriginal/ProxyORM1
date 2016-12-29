/*
    ********************************************************************
    Odsek:          Elektrotehnika i racunarstvo
    Departman:      Racunarstvo i automatika
    Katedra:        Racunarska tehnika i racunarske komunikacije (RT-RK)
    Predmet:        Osnovi Racunarskih Mreza 1
    Godina studija: Treca (III)
    Skolska godina: 2016/2017
    Semestar:       Zimski (V)

    Ime fajla:      server.c
    Opis:           TCP/IP server

    Platforma:      Raspberry Pi 2 - Model B
    OS:             Raspbian
    ********************************************************************
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT   27015

int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    int i, flag_u = 0 , flag_p = 0;
    struct sockaddr_in server , client;
    char u_buff[DEFAULT_BUFLEN], p_buff[DEFAULT_BUFLEN];
    char username[] = "Test_u\0", password[] = "Test_p\0";
    char *username_request = "Enter username!\0";
    char *password_request = "Enter password to connect!\0";
    char wrong_u[] = "Wrong username!\0", wrong_p[] = "Wrong password, you're not connected!\0";
    char connected[] = "Connected to proxy\0";

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");

    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(DEFAULT_PORT);

    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");

    //Listen
    listen(socket_desc , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);

    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }

    //send username_request
     if( send(client_sock , username_request , strlen(username_request), 0) < 0)
     {
         puts("Send failed");
         return 1;
     }

     //recieve username
    while( (read_size = recv(client_sock, u_buff , DEFAULT_BUFLEN , 0)) > 0 )
    {
        for(i = 0; i < strlen(username); i++)
        {
          if(username[i] != u_buff[i])
          {
            flag_u = 1;
            break;
          }
        }
        break;
    }

    if(flag_u == 0)
    {
      if( send(client_sock , password_request , strlen(password_request), 0) < 0)
      {
          puts("Send failed");
          return 1;
      }

      while( (read_size = recv(client_sock , p_buff , DEFAULT_BUFLEN , 0)) > 0 )
      {
          for(i = 0; i < strlen(p_buff); i++)
          {
            if(p_buff[i] != password[i])
            {
              flag_p = 1;
              break;
            }
          }
          break;
      }

      if(flag_p == 0)
      {
        if( send(client_sock , connected , strlen(connected), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
      }
      else if(flag_p == 1)
      {
        if( send(client_sock , wrong_p , strlen(wrong_p), 0) < 0)
        {
            puts("Send failed");
            return 1;
        }
      }

    }
    else if(flag_u == 1)
    {
      if( send(client_sock , wrong_u , strlen(wrong_u), 0) < 0)
      {
          puts("Send failed");
          return 1;
      }
    }


    return 0;
}
