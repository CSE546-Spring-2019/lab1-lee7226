#include <stdio.h>
#include <string.h>

// set the buffer size to be 20 since we assume the search string will be 20 bytes or less
int BUFFER_SIZE = 20;

char buffer[20];
long  filePtr = 0;
long  fileSize = 0;
long  numMatch = 0;

// open the input file to be read
FILE* readFile(char* fileName) {
    FILE* file = fopen(fileName, "rb");
    return file;
}

// write the final output to both the terminal and the specified output file
int writeFile(char* fileName, long fileSize, long numMatches) {
    FILE* file = fopen(fileName, "wb");
    // arbitrary sizes for the output strings, if the size of the input file and/or number of matches takes more than 100 bytes worth of string to print then you've got bigger problems
    char sizeStr[100];
    char numMatchStr[100];
    sprintf(sizeStr, "Size of file is %lu\n", fileSize);
    sprintf(numMatchStr, "Number of matches = %lu\n", numMatches);

    // print to terminal
    printf("%s", sizeStr);
    printf("%s", numMatchStr);
    // print to file, giving error message if necessary
    int sizeWrite = fwrite(sizeStr, strlen(sizeStr), 1, file);
    int numMatchWrite = fwrite(numMatchStr, strlen(numMatchStr), 1, file);
    if (sizeWrite < 1 || numMatchWrite < 1) {
        printf("Error: Unable to write to output file\n");
        return -1;
    }
    return 0;
}

// increments the running count of matches that have been found so far
long incCount(char* input, char* match) {
    // match the first |match| characters of input with match, if same then increment match count
    if (strncmp(match, input, strlen(match)) == 0) {
        numMatch++;
    }
    return numMatch;
}

// returns the size of the input file
long getFileSize(FILE* file) {
    fseek(file, 0, SEEK_END);
    long bytes = ftell(file);
    rewind(file);
    return bytes;
}

// reads the first BUFFER_SIZE (20) characters into the buffer
int readIntoBuffer(FILE* file) {
    fread(buffer, BUFFER_SIZE, 1, file);
    filePtr = 0;
    return 0;
}

// shift the buffer along the file stream, shifting down by one character at a time
int shiftBuffer(FILE* file) {
    filePtr++;
    fseek(file, filePtr, 0);
    fread(buffer, BUFFER_SIZE, 1, file);

    return 0;
}

int main(int argc, char **argv) {
    // number of args error message
    if (argc != 4) {
	printf("Error: Must use exactly 3 arguments\n");
	return -1;
    }
    // read input file
    FILE* file = readFile(argv[1]);
    // error if file cannot be read or doesn't exist
    if (!file) {
        printf("Error: Unable to read from input file\n");
        return -1;   
    }
    // read the start of file into the buffer
    readIntoBuffer(file);
    // get the file size
    long fileSize = getFileSize(file);
    // shift along the file, counting the number of times we get a match
    long i;
    for (i = 0; i < fileSize; i++) {
        incCount(buffer, argv[2]);
        shiftBuffer(file);
    }
    // write the output
    writeFile(argv[3], fileSize, numMatch);
    return 0;
}
