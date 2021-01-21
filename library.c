/*
  this progrem is a library program which have a manu that the user can choose
  to search a book by book name
  to search a book by author name
  to loan a book
  and saving data into files*/

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX 100

/*=================================================================================================================================================================================*/

typedef struct{

    int day, month, year;

}date;

typedef struct{
    
    char *name;
    char *author;
    int copies_num;
    int status;
    int publish_year;
    date loan_date;
    long int seriel_number;

}books;

/*=================================================================================================================================================================================*/
void books_loan(books *b, char *a, int size);         //loan function
int line_counter(FILE *fp);                           //counts amount of books
void book_fill(books *b, FILE *fp);                   // fill book parameters
void file_saving (books *b, int size);                //saving to txt file
void printing_book(books *b);                         //printing the book
char next (FILE *fp);                                 //next element for input stream
void book_search(books *b, char *a, int size);        //search for book name
void author_search(books *b, char *a, int size);      //search for author name
void file_exit (books *b, int size, FILE *fp);        //exit function
void my_tolower (char *a);                            //recveies input fron user and lowercase it

/*=================================================================================================================================================================================*/

int main (int argc, char *argv[]){
    
    if (argc < 2){
        printf("ERROR! wrong usge in program! enter more then 1 argument");
        return 0;
    }

    FILE *fin = fopen(argv[1],"r");
    if (fin == 0){
        printf("Could Not Open File");
        return 0;
    }

    int i, option, size;
    char a[MAX];

    size = line_counter(fin);
    books book[size];

    for (i=0; i<size; i++){
        book_fill(&book[i],fin);
    }

    printf("WELCOME TO MY LIBRERY!\n");

    while (1){
        printf("please choose option:\n");
        printf("[1] search for book\n");
        printf("[2] search for author\n");
        printf("[3] books_loan book\n");
        printf("[4] file_saving & exit\n");
        printf("[5] exit\n");
        
        if(scanf("%d", &option)!=1){
            
            printf("wrong option\n");
            file_exit(book,size,fin);
        }

        switch(option){

            default: break;

            case 1:
                printf("enter the name of the book to look for: \n");
                my_tolower(a);
                book_search(book,a,size);
                break;

            case 2:
                printf("enter the name of the author to look for: \n");
                my_tolower(a);
                author_search(book,a,size);
                break;

            case 3:
                printf("enter the book name: \n");
                my_tolower(a);
                books_loan(book,a,size);
                break;

            case 4:
                file_saving(book,size);

            case 5:
                file_exit(book,size,fin);
        }
    }
    return 0;
}

/*=================================================================================================================================================================================*/

void file_exit (books *b, int size, FILE *fp){       //exit function

    int i;

    for (i=0 ; i < size ; i++){
        free(b[i].name);
        free(b[i].author);
    }
    printf("THANK YOU SEE YOU NEXT TIME :)\n");
    fclose(fp);
    exit(1);

}

/*=================================================================================================================================================================================*/

char next (FILE *fp){       //next element for input stream

    char c;

    c = fgetc(fp);
    while(c == ' ')
        c = fgetc(fp);

    return c;
}

/*=================================================================================================================================================================================*/

void book_fill(books *b, FILE *fp){     // fill book parameters

    char a[MAX];
    int i;

    a[0] = fgetc(fp);
    if (a[0] == '\n')
        a[0] = fgetc(fp);

    for (i=1 ; ; i++){
        
        a[i] = fgetc(fp);

        if (a[i] == ',')
            break;
    }
    a[i] = '\0';

    b->name = (char*)malloc((i+1)*sizeof(char));
    if(b->name == NULL){
        printf("ERROR! No Memory Avileable\n");
        exit(1);
    }
    strcpy(b->name , a);
 
    a[0] = next(fp);
    for (i=1 ; ; i++){
        
        a[i] = fgetc(fp);

        if (a[i] == ',')
            break;
    }
    a[i] = '\0';

    b->author = (char*)malloc((i+1)*sizeof(char));
    if(b->author == NULL){
        printf("ERROR! No Memory Avileable\n");
        exit(1);
    }
    strcpy(b->author,a);
    
    fscanf(fp,"%d,%d,%d", &(b->publish_year), &(b->copies_num), &(b->status));
    if (b->status == -1)
        fscanf(fp,",%d/%d/%d,%ld",&(b->loan_date.day), &(b->loan_date.month), &(b->loan_date.year), &(b->seriel_number));
    else
        fscanf(fp,",%ld", &(b->seriel_number));

}

/*=================================================================================================================================================================================*/

void printing_book(books *b){       //printing_book book

    printf("Name: %s   Author: %s   Publish year: %d   \n",b->name, b->author ,b->publish_year);
    if (b->copies_num==0)
        printf("Copies: 1   \n");
    else if (b->copies_num>0)
        printf("Copies: %d   \n", b->copies_num);
    else if (b->copies_num<0)
        printf("Copies: --   \n");
    b->status==1 ? printf("Status: available    ") : printf("Status: loaned    ");
    if (b->status==-1)
        printf("Loan date: %d/%d/%d  ISBN: %ld\n",b->loan_date.day, b->loan_date.month, b->loan_date.year, b->seriel_number);
    else
        printf("ISBN: %ld\n", b->seriel_number);
}

/*=================================================================================================================================================================================*/

int line_counter(FILE *fp){        //counts amount of books

    int size=0;
    char c;

    while (!feof(fp)){
       
        c=fgetc(fp);
        
        if (c=='\n')
            ++size;
    }
    
    rewind(fp);
    return ++size; 
}

/*=================================================================================================================================================================================*/

void my_tolower (char *a){       //receives input fron user and lowercase it

    a[0]=getchar();

    int i;
    for (i=0; i<MAX; i++){
        
        a[i]=tolower(getchar());

        if (a[i]=='\n')
            break;
    }
    a[i]='\0';

}

/*=================================================================================================================================================================================*/

void book_search(books *b, char *a, int size){       //search for book name

    int i, j, flag=0;
    char temp[MAX];
    
    for (i=0; i<size; i++){

        strcpy(temp,b[i].name);

        for (j=0; j<strlen(temp); j++)      //lowercase of book name
            temp[j]=tolower(temp[j]);

        if (strstr(temp,a)){
            printing_book(&b[i]);
            flag=1;
        }
    }

    if (flag==0)
        printf("there is no book by this name\n");
}

/*=================================================================================================================================================================================*/

void author_search(books *b, char *a, int size){     //search for author name

    int i, j, flag=0;
    char temp[MAX];

    for (i=0; i<size; i++){

        strcpy(temp,b[i].author);

        for (j=0; j<strlen(temp); j++)      //lowercase of author name
            temp[j]=tolower(temp[j]);

        if (strstr(temp,a)){
            printing_book(&b[i]);
            flag=1;
        }
    }
    if (flag==0)
        printf("there is no author by this name\n");
}

/*=================================================================================================================================================================================*/

void books_loan(books *b, char *a, int size){     //loan function
    
    int i, j, flag=0;
    char temp[MAX];

    for (i=0; i<size; i++){

        strcpy(temp,b[i].name);

        for (j=0; j<strlen(temp); j++)      //lowercase of book name
            temp[j]=tolower(temp[j]);

        if (strstr(temp,a)){
            flag=1;
            break;
        }
    }
    if (flag==0){
        printf("the book not found\n");
        return;
    }

    if (b[i].status==-1){
        printf("this book was alredy loaned!\n");
        return;
    }
    
    b[i].status=-1;
    b[i].copies_num-=1;

    printf("Loan date: ");
    printf("enter date xx/xx/xx: ");
    scanf("%d/%d/%d", &(b[i].loan_date.day), &(b[i].loan_date.month), &(b[i].loan_date.year));

    
}

/*=================================================================================================================================================================================*/

void file_saving (books *b, int size){     //saving to txt file

    int i;
    char a[MAX];
    printf("enter the name of the file to file_saving into it (end with .txt): \n");
    scanf("%s",a);
    FILE *fout=fopen(a,"a");
    if (fout==NULL){
        printf("Faild to create output file\n");
        return;
    }     
    
    for (i=0; i<size; i++){

        fprintf(fout,"Name: %s   Author: %s   Publish year: %d   ",b[i].name, b[i].author ,b[i].publish_year);
        if (b[i].copies_num==0)
            fprintf(fout,"Copies: 1   ");
        else if (b[i].copies_num>0)
            fprintf(fout,"Copies: %d   ", b[i].copies_num);
        else if (b[i].copies_num<0)
            fprintf(fout,"Copies: --   ");
        b[i].status==1 ? fprintf(fout,"Status: available    ") : fprintf(fout,"Status: loaned    ");
        if (b[i].status==-1)
            fprintf(fout,"Loan date: %d/%d/%d  ISBN: %ld\n",b[i].loan_date.day, b[i].loan_date.month, b[i].loan_date.year, b[i].seriel_number);
        else
            fprintf(fout,"ISBN: %ld\n", b[i].seriel_number);
    }
    fclose(fout);

}

/*=================================================================================================================================================================================*/
