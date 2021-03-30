# ACS-project4
## Overview
In this project, the team write code to create a dictionary encoder that can read data from input file and write the encoded data to the output file with the help of hash function. After testing with different input data size, the team find out the file size will largely influence the encoder performance.

## Dictionary encoder
The Dictonary encoder which also known as a substitution coder, is a class of lossless data compression algorithms whtich operate by searching for matches between the text to be compressed and a set of strings contained in a data structure (The dictionary) maintained by the encoder. When the encoder finds such a match, it substitutes a reference to the string's position in the data structure.

## Coding Structure
The project is written in C language and is comprised of 1 single main.c file.
## Code Correctness
To ensure the correctness of the code, the team add a decoding mode in our design and verify the correctness of our encoding logic by comparing the decoding data from the output file and the input file. The comparision result testify the correctness of our design.

## Compiling & Running
In order to run the program, you can use the following command to compile and run the main.c code.
#### gcc -Wall main.c
#### ./a.out -e(Mode selection) input.txt(The input file name) output.txt(The output file name)
#### eg: ./a.out -e Small-Size-Column.txt output_small.txt
#### Please do not use Large-Size-Column.txt for testing

## User Interface
Once you compile and run the a.out file, the program will first check if you enter correct arguments, if the program detects that **you do not enter appropriate command, it will exit and return a error message**. Once the command line detection is passed, the program will try to open the input file with the given file name. If the program detect **the input file cannot be open, the program will exit and return a error message**. After finish these checks, the program will start running based on you command and **print proper information and the time usage for the process**.

## Code Description
### Struct phrase
The struct is the core struct of the dictionary, each struct store a encoded phrase. Location represent the calculated hashvalue of the phrase; index represents the location of the particular struct in the dictionary; location_rank represent the placement of the struct for same hashvalue phrase when collision happens; string represents the actually value of the phrase and count represents how many times the phrase exists in the input file. 
```
struct phrase { 
	unsigned long long location;
	unsigned long long index;
	int location_rank;
	char string[LONGEST_ENGLISH_WORD];
	int count;
};
```

### Struct dictionary
The struct is for creating a dictonary. array stores all the phrase entrys in a 2-D phrase array structure; array_data record the hashvalue for each row of the array; collision_data represents the collision information in each row(if there is no collision, in each row there is only one phrase) and current_largest record the number of rows of the dictionary.
```
struct dictionary {
	struct phrase ** array;
	unsigned long long  * array_data;
	int * collision_data;	
	unsigned long long current_largest; 

};
```
### hash function
In place differentiate different phase in the dictionary the time use a hash function to get the hashvalue of each entry.
```
unsigned long long hash(char* raw)
{
	unsigned long long hash = 5381;
	int c;

	while((c = tolower(*raw++)))
	{	
		hash = ((hash << 5) + hash) +c;

	}

	return hash;
}
```
## Testing Hardware
\*i7-6700HQ  
\*GTX 980M  
\*16G DDR4 Memory  
\*Ubuntu 20.04

## Testing result:
For encoding mode:  
|             Command 	                      |	Time(s)|
|---------------------------------------------|--------|
|./a.out -e  Small-Size-Column.txt  output_Small.txt |  0.179915  |
|./a.out -e  Medium-Size-Column.txt output_Middle.txt |   12.01362   |  
  

For decoding mode:  
|             Command 	                      |	Time(s)|
|---------------------------------------------|--------|
|./a.out -e output_Small.txt decoder_Small.txt |  0.173924  |
|./a.out -e output_Middle.txt decoder_Middle.txt |   12.00346   |  


For Query mode:  
|             Command 	                      |	Appear Times| Time(s)| 
|---------------------------------------------|--------|--------|
|./a.out -q output_Middle.txt cioyecge  |  146 |0.00028 |
|./a.out -q output_Middle.txt zikzcpq  |  106  |0.00028|
|./a.out -q output_Middle.txt acknn  |  100  |0.00029 |
|./a.out -q output_Middle.txt qythiesqk  | 192  |0.00028|
|./a.out -q output_Middle.txt fasdfadsfas  | 0  |0.00735|  

## Analysis  && Conclusion: 
In conclusion, the team success create a dictionary encoder in C and it support encoding, decoding and Query functionalities. Based on the testing result, the team find out that the building for the dictionary will take up most of the running time that as the input file become larger, the running time for the program will increase accordingly. Moreoever, the team find out the time for encoding and decoding operation for same file size is similar to each other as that for both operation, the program will take time to build the dictionary. For Query operation, the query time for existing strings will be shorter than the time for non-existing strings because for non-existing strings, the program have to go through every ling of the input file. Moreover, for this program, currently it cannot support too large input file for encoding operation(it will take too long to create the dictionary), in the future, the team will try to optimize the data structure to improve the capability of the program.






