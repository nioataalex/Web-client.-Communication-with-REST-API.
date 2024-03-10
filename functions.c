#include "helpers.h"
#include "requests.h"
#include "functions.h"
#include "parson.h"

// I will explain in README what every function is doing

void register_request(int sockfd, char* response, char* message,
                    char* username, char* password, char** data) {

    data[0] = calloc(BUFLEN, sizeof(char));
    data[1] = calloc(BUFLEN, sizeof(char));

    printf("username=");
    scanf("%s", username);
    sprintf(data[0], "\"username\":\"%s\"", username);

    printf("password=");
    scanf("%s", password);
    sprintf(data[1], "\"password\":\"%s\"", password);

    message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/register", "application/json", data, 2, NULL, 0, NULL);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, CREATED) != NULL) { 
        printf("User %s succesfully registered! \n", username);
    } else {
        printf("User already exists!\n");
    }

    for (int i = 0; i < 2; i++){
        free(data[i]);
    }

}

void login_request(int sockfd, char* response, char* message, char* username,
                        char* password, char** data, char** cookies) {

    data[0] = calloc(BUFLEN, sizeof(char));
    data[1] = calloc(BUFLEN, sizeof(char));

    printf("username=");
    scanf("%s", username);
    sprintf(data[0], "\"username\":\"%s\"", username);

    printf("password=");
    scanf("%s", password);
    sprintf(data[1], "\"password\":\"%s\"", password);
    message = compute_post_request("34.254.242.81", "/api/v1/tema/auth/login", "application/json", data, 2, NULL, 0, NULL);
            
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, BADREQUEST) != NULL) {
        printf("User has wrong credidentials!\n");
    } else if (strstr(response, OK) != NULL) {
        printf("User %s succesfully logged!\n", username);

        char* parse = strtok(response, "\n");
        while (parse) {   
            if (strstr(parse, "Cookie") != NULL) {
                char* cookie = strtok(parse, " ");

                while (cookie != NULL) {
                    if (cookie[0] == 'c') {
                        printf("Cookie: %s\n", cookie);
                        strcpy(cookies[0], cookie);
                        break;
                    } else {
                        cookie = strtok (NULL, " ;");
                        continue;
                    }
                }
            }
            parse = strtok(NULL,"\n");
        }
    }

    for (int i = 0; i < 2; i++){
        free(data[i]);
    }
            
}

void get_book_request(int sockfd, char* response, char* message, char* token, char** cookies) {

    char *get_id = calloc(BUFLEN, sizeof(char));
    printf("id=");
    scanf("%s", get_id);

    char *url = calloc(BUFLEN, sizeof(char));
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, get_id);

    message = compute_get_request("34.241.4.235", url, token, cookies, 1);
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, FORBIDDEN) != NULL || token == NULL) {
        printf("Log in first\n");
    } else if (strstr(response, NOTFOUND) != NULL) {
        printf("No book with this id!\n");
    } else  if(strstr(response, OK) != NULL) {
        printf("The book with %s was succesfully received!\n", get_id);

        char* parse;
        parse = strstr(response, "{");

        JSON_Value *value = json_parse_string(parse);
        JSON_Object *object = json_value_get_object(value);

        printf("{\n");
        printf("\tid: %s,\n", get_id);
        printf("\ttitle: %s,\n",json_object_get_string(object, "title"));
        printf("\tauthor: %s,\n",json_object_get_string(object, "author"));
        printf("\tpublisher: %s,\n",json_object_get_string(object, "publisher"));
        printf("\tgenre: %s,\n",json_object_get_string(object, "genre"));
        printf("\tpage_count: %d\n",(int)json_object_get_number(object, "page_count"));
        printf("}\n");
    }

    free(url);
}

void get_books_request(int sockfd, char* response, char* message,char* token, char** cookies) {
    message = compute_get_request("34.254.242.81", "/api/v1/tema/library/books", token, cookies, 1);
            
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, FORBIDDEN) != NULL || token == NULL) {
        printf("Log in first!!\n");
    } else if(strstr(response, OK) != NULL) {
        printf("Books: \n");

        char* parse;
        parse = strtok(response, "\n");

        printf("[\n");
        while (parse != NULL) {
            if (parse[0] == '[')  {   
                JSON_Value *value = json_parse_string(parse);
                JSON_Array *books = json_value_get_array(value);

                int i = 0;

                while(i < json_array_get_count(books)) {

                    JSON_Object *object = json_array_get_object(books, i);
                    printf("\t{\n");
                    printf("\t\tid: %d,\n", (int)json_object_get_number(object, "id"));
                    printf("\t\ttitle: %s\n",json_object_get_string(object, "title"));
                    printf("\t},\n");
                    i++;
                }
            }

            parse = strtok(NULL, "\n");
        }

        printf("]");
        free(parse);
    }
}

void enter_library_request(int sockfd, char* response, char* message, char* token,
                            char** cookies, bool* in_library) {
    message = compute_get_request("34.254.242.81", "/api/v1/tema/library/access", NULL, cookies, 1);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, UNAUTHORIZED) != NULL) {
        printf("To enter the library, you must loggin first!\n");
    } else if (strstr(response, OK) != NULL) {
        printf("You entered the library! \n");

        char* parse = strtok(response, "\n");
        while (parse) {
            if (parse[0] == '{') {
                JSON_Value *value = json_parse_string(parse);
                JSON_Object *object = json_value_get_object(value);

                const char* tok =  json_object_get_string(object, "token");
    
                printf("Token: %s\n", tok);
                strcpy(token, tok) ;
                        
            }
            parse = strtok(NULL,"\n");
        }

        *in_library = !(*in_library); 
    }
}

void add_book_request(int sockfd, char* response, char* message, char* title, char* author,
                    char* genre, char* publisher, char* page_count, char** data,
                    char** cookies, char* token) {
    data = realloc(data, 5 * sizeof(char *));
    data[0] = calloc(BUFLEN, sizeof(char)); 
    data[1] = calloc(BUFLEN, sizeof(char)); 
    data[2] = calloc(BUFLEN, sizeof(char)); 
    data[3] = calloc(BUFLEN, sizeof(char)); 
    data[4] = calloc(BUFLEN, sizeof(char)); 

    printf("title=");
    scanf("%s", title);
    sprintf(data[0], "\"title\":\"%s\"", title);

    printf("author=");
    scanf("%s", author);
    sprintf(data[1], "\"author\":\"%s\"", author);

    printf("genre=");
    scanf("%s", genre);
    sprintf(data[2], "\"genre\":\"%s\"", genre);

    printf("publisher=");
    scanf("%s", publisher);
    sprintf(data[3], "\"publisher\":\"%s\"", publisher);

    printf("page_count=");
    scanf("%s", page_count);
    if (atoi(page_count) != 0) {
        sprintf(data[4], "\"page_count\":\"%s\"", page_count);
    } else {
        printf("Oops, invalid number of pages\n");
        return;
    }

    message = compute_post_request("34.254.242.81", "/api/v1/tema/library/books", "application/json", data, 5, NULL, 0, token);

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, FORBIDDEN) != NULL || token == NULL) {
        printf("Log in first!!\n");
    } else if (strstr(response, OK) != NULL) {
        printf("Book was succesfully added in the library!\n");
    }
}

void delete_book_request(int sockfd, char* response, char* message, 
                        char* token, char** cookies) {
    char *delete_id = calloc(BUFLEN, sizeof(char));
    printf("id=");
    scanf("%s", delete_id);

    char *url = calloc(BUFLEN, sizeof(char));
    strcpy(url, "/api/v1/tema/library/books/");
    strcat(url, delete_id);

    message = compute_delete_request("34.254.242.81", url, token, cookies);
            
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, FORBIDDEN) != NULL || token == NULL) {
        printf("Log in first!!\n");
    } else if (strstr(response, NOTFOUND) != NULL) {
        printf("No book with this id!\n");
    } else if (strstr(response, OK) != NULL) {
        printf("The book  with id %s was deleted!\n", delete_id);
    }

    free(url);
}

void logout_request(int sockfd, char* response, char* message, char** cookies,
                    char *token, bool* in_library) {
    message = compute_get_request("34.254.242.81", "/api/v1/tema/auth/logout", NULL, cookies, 1);
            
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);

    if (strstr(response, BADREQUEST) != NULL) {
        printf("Log in first!!\n");
    } else if (strstr(response, OK) != NULL) {
        printf("User logged out succesfully!\n");
        in_library = false;
    }
}
