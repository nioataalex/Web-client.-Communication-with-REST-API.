#ifndef _FUNCTIONS_
#define _FUNCTIONS

#include <stdio.h>
#include <stdlib.h>     
#include <unistd.h>     
#include <string.h>    
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include <ctype.h>
#include <stdbool.h>

#define PORT 8080
#define OK "HTTP/1.1 200"
#define FORBIDDEN "HTTP/1.1 403"
#define NOTFOUND "HTTP/1.1 404"
#define CREATED "HTTP/1.1 201"
#define  BADREQUEST "HTTP/1.1 400"
#define UNAUTHORIZED "HTTP/1.1 401"

// function for register action
void register_request(int sockfd, char* response, char* message,
                        char* username, char* password, char** data);

// function for login action
void login_request(int sockfd, char* response, char* message,
                        char* username, char* password, char** data, char** cookies);

// function for get_book action
void get_book_request(int sockfd, char* response, char* message, char* token, char** cookies);

// function for get_books action
void get_books_request(int sockfd, char* response, char* message,char* token, char** cookies);

// function for enter_library action
void enter_library_request(int sockfd, char* response, char* message,char* token, char** cookies, bool* in_library);

// function for add_book action
void add_book_request(int sockfd, char* response, char* message, char* title, char* author,
                    char* genre, char* publisher, char* page_count, char** data, char** cookies, char* token);

// function for delete_book action
void delete_book_request(int sockfd, char* response, char* message,char* token, char** cookies);

// function for logout action
void logout_request(int sockfd, char* response, char* message, 
                        char** cookies, char *token, bool* in_library);
#endif