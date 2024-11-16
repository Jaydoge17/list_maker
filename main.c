#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <dirent.h>

void hub();
void newFileCreate();
void readFromList();
void readOrDeleteFromList(int);
void displayFileContents(char[]);
void deleteFileContents(char[]);

struct list
{
    char name[32];
    int price;
    char location[64];
};


int main()
{
    hub();
}

void deleteFileContents(char file[])
{
    char buffer[255];
    snprintf(buffer, 255, "lists\\%s.csv",file);
    int result = remove(buffer);
    if(result != 0)
    {
        printf("Failed to remove file");
    }
}

void displayFileContents(char file[32])
{
    char name[32], location[64];
    int price;

    char buffer[256];
    snprintf(buffer, sizeof(buffer), "lists\\%s.csv",file);
    FILE *textRead = fopen(buffer, "r");
    if(textRead == NULL)
    {
        printf("Failed to read file in function displayFileContents");
    }
    fgets(buffer, sizeof(buffer), textRead);
    int result = sscanf(buffer,"%31[^,],%d,%63[^,   ]", name, &price, location); //checks that all data is parsed

    memset(buffer, 0, sizeof(buffer));
    fclose(textRead);

    if(result == 3)
    {
        printf("\nThis item is called \"%s\" and it costs £%d, you can buy it for that price at %s",name, price, location);
    }
    else
    {
        exit(0);
        printf("Failed to parse all data");
    }

}

void hub()
{
    char waste[1];
    char user_input;

    printf("\nWhat would you like to do\n(0) Exit\n(1) Create list\n(2) Open exisitng list\n(3) Delete an existing file\n: ");
    scanf("%d", &user_input);
    fgets(waste, 1, stdin);

    switch(user_input)
    {
        case 0:
            exit(0);

        case 1:
            printf("You have selected to create a new list\n");
            newFileCreate();
            break;
        case 2:
            printf("You have selected to read from an existing list\n");
            readOrDeleteFromList(0);
            break;
        case 3:
            printf("You have selected to delete an existing file");
            readOrDeleteFromList(1);
            break;
        default:
            printf("\nFailed to collect input\n");
            exit(0);
    }
}

void newFileCreate()
{
    char buffer[255];
    int price;
    char name[32];
    char location[64];

    printf("What is the name of this item\n: ");
    scanf("%s", &name);

    printf("What is the price of this item\n: ");
    scanf("%d", &price);

    printf("Where would you get this item\n: ");
    scanf("%s", &location);

    
    snprintf(buffer, sizeof(buffer), "lists\\%s.csv", name);

        for(int i = 0; i < sizeof(buffer)/sizeof(buffer[0]); i++)
        {
            buffer[i] = tolower(buffer[i]);
        }

    FILE *item = fopen(buffer, "w");
    if(item == NULL)
    {
        printf("\nfailed to open %s, closing program\n", name);
        exit(0);
    }
    fprintf(item,"%s,%d,%s,", name, price, location);
    fclose(item);

    memset(buffer, 0, sizeof(buffer));
    //struct list item = {name, price, location};

    printf("You have successfully created a new list\n");
    hub();
}

void readOrDeleteFromList(int readOrDelete)
{
    char line[256];
    int i = 1;
    char fileReadFrom[32];
    
    DIR *dir;
    struct dirent *entry;
    int file = 0;

    const char *folder_path = "./lists"; // path to folder

    //open directory
    dir = opendir(folder_path);
    if(dir == NULL)
    {
        printf("\nFailed to open directory lists\n");
        exit(0);
    }

    entry = readdir(dir);
    printf("Please enter the name of the file you wish to access\n");
    while(  (entry = readdir(dir))  )
    {
        file ++;
        if(file == 1)
        {
            continue;
        }
        file -= 1; // changes file value for display
        printf("File %3d: %s\n",
                file,
                entry->d_name);
        file += 1; //changed to pass the check
        }
    scanf("%s", &fileReadFrom);
    for(int index = 0; index < sizeof(fileReadFrom)/sizeof(fileReadFrom[0]); index++)
    {
        fileReadFrom[index] = tolower(fileReadFrom[index]);
    }
    //printf("%s", fileReadFrom); for deving and debugging

    if(readOrDelete == 0)
    {
        displayFileContents(fileReadFrom);
    }
    else if(readOrDelete == 1)
    {
        deleteFileContents(fileReadFrom);
    }
    closedir(dir);
    hub();
}