#include "helpers.h"
#include "requests.h"
#include "parson.h"
#include "functions.h"

int main(int argc, char *argv[])
{
    // dynamic memory allocation
    char *message = calloc(BUFLEN, sizeof(char));
    char *response = calloc(BUFLEN, sizeof(char));
    int sockfd;

    char **cookies = calloc(1, sizeof(char *));
    cookies[0] = calloc(BUFLEN, sizeof(char));

    bool in_library = false;

    char *token = calloc(BUFLEN, sizeof(char));
    char *title = calloc(BUFLEN, sizeof(char));
    char *author = calloc(BUFLEN, sizeof(char));
    char *genre = calloc(BUFLEN, sizeof(char));
    char *publisher = calloc(BUFLEN, sizeof(char));
    char *page_count = calloc(BUFLEN, sizeof(char));
    char *username = calloc(BUFLEN, sizeof(char));
    char *password = calloc(BUFLEN, sizeof(char));


    // handling commands from stdin
    while (true) {
        sockfd = open_connection("34.254.242.81", PORT, AF_INET, SOCK_STREAM, 0);
        char *action = calloc(BUFLEN, sizeof(char));;
        char **data = malloc(2 * sizeof(char *));
        
        scanf("%s", action);

        if (strcmp(action, "register") == 0) {
            register_request(sockfd, response, message, username, password,  data);
        } else if (strcmp(action, "login") == 0) {
            login_request(sockfd,response, message, username, password, data, cookies);
        } else if (strcmp(action, "enter_library") == 0) {
            enter_library_request(sockfd, response, message, token, cookies, &in_library);
        } else if (strcmp(action, "get_books") == 0) {
            get_books_request(sockfd, response, message, token, cookies);
        } else if (strcmp(action, "get_book") == 0) {
            get_book_request(sockfd, response, message, token, cookies);
        } else if (strcmp(action, "add_book") == 0) {
            add_book_request(sockfd, response, message, title, author,
                    genre, publisher, page_count,data,  cookies,  token);
        } else if (strcmp(action, "delete_book") == 0) {
            delete_book_request(sockfd,response, message, token,cookies);
        } else if (strcmp(action, "logout") == 0) {
            logout_request(sockfd, response, message, cookies, token, &in_library);
            token = NULL;
        } else if (strcmp(action, "exit") == 0)  {
            close_connection(sockfd);
            break;
        } else {
            printf("WRONG action!");
        }

        close_connection(sockfd);
        free(action);
    }

    // free everything
    free(title); 
    free(author);
    free(genre);
    free(publisher); 
    free(page_count); 
    free(username);
    free(password);
    free(message);
    free(response);
    free(token);
    free(cookies);
    return 0;
}
