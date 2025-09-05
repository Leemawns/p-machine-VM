Since I haven't touched C in well over a year:
    I will be asking AI questions related to syntax / processes

HomeWork 1:

Question 1: 
    How does the argc and argv work in C when used to read from a file? How to declare a file in C?

    Summary: 
        - argc = number of arguments.
        - argv = array of strings (file name is just one of them).
        - To work with a file: declare FILE *fp, open with fopen(), and close with fclose().

    My understanding:
        - When running argc will look for the gcc + input file from index 0. So if arc > 1, it means file exists
        - if argc > 1, grab the file at argv[1], then do the work to open and read from a file to begin program logic

Question 2: 
    How does the fgets work? Do buffers exist?

    Summary:
        - Takes 3 arguments: a * to char array, max # of chars to read, input stream.
        - Buffer is used to place specific amount of data into buffer to read.

    My understanding:
        - I'll create a max char array buffer to retreive input.txt contents.
        - Later i'll use buffer in a while loop to retrive each (OP, L, M) 1 by 1 knowing its the correct content
