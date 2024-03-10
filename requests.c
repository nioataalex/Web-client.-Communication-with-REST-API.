#include <stdlib.h>     
#include <stdio.h>
#include <unistd.h>     
#include <string.h>     
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <netdb.h>      
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "buffer.h"
#include "parson.h"

char *compute_get_request(char *host, char *url, char *query_params,
                            char **cookies, int cookies_count)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    
    sprintf(line, "GET %s HTTP/1.1", url);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    if (query_params != NULL && strlen(query_params) != 0) { 
            sprintf(line, "Authorization: Bearer %s", query_params);
            compute_message(message, line);
    }

    memset(line, 0, LINELEN);
    if (cookies != NULL) {
            sprintf(line, "Cookie: ");

            for (int i = 0; i < cookies_count; i++) {
                strcat(line, cookies[i]);
                strcat(line, "; ");
            }

            compute_message(message, line);
    }
    
    compute_message(message, "\n");
    return message;
}

char *compute_post_request(char *host, char *url, char* content_type, char **body_data,
                            int body_data_fields_count, char **cookies,
                            int cookies_count, char* query_params)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));
    char *buffer = calloc(LINELEN, sizeof(char));

    sprintf(line, "POST %s HTTP/1.1", url);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    memset(line, 0, LINELEN);
    if (query_params != NULL && strlen(query_params) != 0) {
            sprintf(line, "Authorization: Bearer %s", query_params);
            compute_message(message, line);
    }

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Type: %s", content_type);
    compute_message(message, line);

    strcat(buffer, "{\n   ");
    for (int i = 0; i < body_data_fields_count - 1; i++) {
        strcat(buffer, body_data[i]);
        strcat(buffer, ",\n   ");
    }
    strcat(buffer, body_data[body_data_fields_count - 1]);
    strcat(buffer, "\n}");

    memset(line, 0, LINELEN);
    sprintf(line, "Content-Length: %ld", strlen(buffer));
    compute_message(message, line);

    memset(line, 0, LINELEN);
    if (cookies != NULL) {
            sprintf(line, "Cookie: ");
            for (int i = 0; i < cookies_count; i++) {
                strcat(line, cookies[i]);
                strcat(line, "; ");
            }
            compute_message(message, line);
    }

    compute_message(message, "");

    memset(line, 0, LINELEN);
    strcat(message, buffer);

    free(line);
    return message;
}

char *compute_delete_request(char *host, char *url, char *query_params, char **cookies)
{
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    sprintf(line, "DELETE %s HTTP/1.1", url);
    compute_message(message, line);

    sprintf(line, "Host: %s", host);
    compute_message(message, line);

    if (query_params != NULL && strlen(query_params) != 0) {
        sprintf(line, "Authorization: Bearer %s", query_params);
        compute_message(message, line);
    }

    if (cookies != NULL) {
        sprintf(line, "Cookie: ");
        strcat(line, cookies[0]);
        compute_message(message, line);
    }
    
    compute_message(message, "");
    return message;
}
