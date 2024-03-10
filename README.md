Copyright Nioata Alexandra 322CA 2023

HOMEWORK 4 - PCOM
Web client. Communication with REST API.

The purpose is to understand the HTTP protocol, which is used in everyday life, most often in web pages.
The program implements the client that accepts commands from the keyboard and sends them to the server that simulates an online library.

How my implementation works:

Before each read from stdin, I opened a new connection, then checked the incoming command, closed the connection and reopened
it for the next command. If the command is exit, I close the connection with the server and exit the loop, because it means
that the program is finished.

Commands received from the keyboard:

1. REGISTER
    - I used the register_request function
    - I read the username and password from the keyboard and put them in a buffer
    - send the post type request to the server
    - I received an answer, checked the code, and displayed a suggestive message
    depending on it (if everything goes well, it means that the user has successfully registered,
    otherwise the user is already in the database)
    - I save cookies

2. LOGIN
    - I used login_request function
    - I read the username and password from the keyboard and put them in a buffer
    - send the post type request to the server
    -  I received an answer, checked the code, and displayed a suggestive message depending on it
    (if everything goes well, it means that the user has successfully logged in,
    otherwise the user has wrong credentials)
    -  if the user has logged in, I will display cookies

3. ENTER_LIBRARY
    - I used enter_library_request function
    - send get type request to server, which will contain the cookie received at login, and the token, if we have one
    - received an answer from the server, checked the code and displayed a suggestive message depending on it
    (if everything goes well, it means that the user has successfully entered the  library,
    otherwise the user must log in first)
    - if the user has entered, I will display the token
    

4. GET_BOOKS
    - I used get_books_request function
    - send get type request to server, which will contain the cookie received at login, and the token, if we have one
    - received an answer from the server, checked the code and displayed a suggestive message depending on it
    (if everything goes well, it means that I will display all books from the library (id + title),
    otherwise the user must loggin first)
    - I extract the books from json using the functions from parson.c

5. GET_BOOK
    - I used get_book_request function
    - I read from stdin the id of the book that I need to display
    - send get type request to server, which will contain the cookie received at login, and the token, if we have one
    - received an answer from the server, checked the code and displayed a suggestive message depending on it
    (if everything goes well, it means that I will display the book from the library (title + author + publisher + genre + page_count),
    otherwise the user must loggin first or the id is not correct)
    - I extract the book from json using the functions from parson.c

6. ADD_BOOK
    - I used add_book_request function
    - I read from stdin the the title, author, genre, publisher, page count and put them in a buffer
    - if the page count is not a number, then I can't add the book
    - send post type request to server
    - received an answer from the server, checked the code and displayed a suggestive message depending on it
    (if everything goes well, it means that the book was added in the library, otherwise the user is not logged in)
  
7. DELETE_BOOK
    - I used delete_book_request function
    - I read from stdin the id of the book that I need to delete
    - I created the new url by concatenating the normal route with the book id
    - send delete type request to server 
    - received an answer from the server, checked the code and displayed a suggestive message depending on it
    (if everything goes well, it means that the book is deleted, otherwise the id is not correct or the user is not logged in)
  
8. LOGOUT
    - I used logout_request function
    - send get type request to server
    - received an answer from the server, checked de code and displayed a suggestive message depending on it
    (if everything goes well, it means that the user has successfully logged out, otherwise the user must loggin first)
    - the token will receive a null value so that it can be changed when a new user connects

9.  EXIT
    - closing the connection with the server
    - the end of the program

Disclaimer:
    - very fun homework to do. Congrats!!
    - I understand better HTTP concepts
    - The homework was made in C language
    - the starting point was the skel from HTTP laboratory
    - I used the Makefile from the laboratory, the files buffer.c, buffer.h, helpers.c, helpers.h
    - also, in the requests.c, I made some changes, for example I added a new function for
    delete request and I changed the parameters for post request by adding char* query_params
    (used for the token)
    - I used parson.h, parson.c files from the link given in pdf
    - In order for my homework to look better, I put all the functions I used for the commands in the functions.c file,
    and in the client.c I left only the initializations, dynamic memory allocations, and the logic of the program,
    in which the respective functions are called
    - for testing, I used an account with this credentials: username=nini, password=nini
    - also, I had a variable called in_library that i wanted to use to see if the user entered the library or not, but
    in the meantime I realized I just needed to check the token
    
Resources:
    - https://github.com/kgabis/parson
    - https://gitlab.cs.pub.ro/pcom/pcom-laboratoare-public/-/tree/master/lab9
    - https://pcom.pages.upb.ro/labs/lab9/lecture.html
    - https://www.youtube.com/watch?v=XcOtEapD3qU&ab_channel=%C5%9Ecoalainformal%C4%83deIT
