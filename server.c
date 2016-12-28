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
    char username[] = "Test_u", password[] = "Test_p";
    char username_request[] = "Enter username!", password_request[] = "Enter password to connect!";
    char wrong_u[] = "Wrong username!", wrong_p[] = "Wrong password!";

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
    puts("Connection accepted");

    ////////////////////

    if (connect(sock_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
    {
        perror("connect failed. Error");
        return 1;
    }
    puts("Connected\n");



     ///////////////////////////////////////////

     if( send(client_sock , username_request , strlen(username_request), 0) < 0)
     {
         puts("Send failed");
         return 1;
     }
     puts("Ide");

    while( (read_size = recv(client_sock, u_buff , DEFAULT_BUFLEN , 0)) > 0 )
    {
        for(i = 0; i < read_size; i++)
        {
          if(u_buff[i] != username[i])
          {
            flag_u = 1;
            break;
          }
        }
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
          for(i = 0; i < read_size; i++)
          {
            if(p_buff[i] != password[i])
            {
              flag_p = 1;
              break;
            }
          }
      }

      if(flag_p == 1)
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


    /*//Receive a message from client
    while( (read_size = recv(client_sock , client_message , DEFAULT_BUFLEN , 0)) > 0 )
    {
        printf("Bytes received: %d\n", read_size);
    }

    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }*/

    return 0;
}
