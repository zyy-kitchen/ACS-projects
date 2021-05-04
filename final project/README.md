# ACS-Final Project
## In-memory key-value store
The team will implement an in-memory key-value store using hash table indexing data structure. Hash table is one of the most widely studied data structure and has been utilized for industry application for many year. The implementation will support operations like GET, PUT and DELETE and support concurrency control. Moreover, re-hashing functionality will been
also implemented to balance hash table size.
## supported operations:
  GET, PUT, SET, DELETE, Container Status  
  Use mutex for concurrency control 
## Compile && Run the program
gcc -Wall -pthread main.c operation.c
./a.out
## Detailed analysis
  See the simplified memcached pdf file for details.
