# A simple console-type TCP for connecting clients and server via a Linux socket

This app allows to establish connection between clients and a server, and send and receive message between clients and a server.
Messages sent by a client via the console will be displayed on the server console.
Then, the server processes such a message, and saves such a formetted message in a log file, and echoes the original message beck to the client.

The server-side console accepts one parameter: the port number.
The client-side console accepts 3 parameters as a single string: the Client's name, the port number, the connection period in seconds

The example of usage is shown in the below video
