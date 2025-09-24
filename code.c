#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//#define FILE_NAME "C:\\Users\\RAZWANUL\\Documents\\Coding_World\\c_programing\\PROJECT\\students.txt"
//#define FILE_NAME "C:\\Users\\RAZWANUL\\Desktop\\12305030\\students.txt"
#define FILE_NAME "students.txt"
void trim(char *str) {
    char *start = str;
    while (*start && (*start==' ' || *start=='\t')) start++;
    memmove(str, start, strlen(start)+1);
    char *end = str + strlen(str) -1;
    while(end>=str && (*end==' ' || *end=='\t')) { *end=0; end--; }
}

int split(char *line, char arr[50][50]) {
    int count=0;
    char *token=strtok(line,",");
    while(token) {
        trim(token);
        strcpy(arr[count++],token);
        token=strtok(NULL,",");
    }
    return count;
}

int find_index(char arr[50][50], int n, char *target) {
    for(int i=0;i<n;i++)
        if(strcmp(arr[i],target)==0) return i;
    return -1;
}

void selectAll(char selectFields[50][50], int selectCount, char tableFields[50][50], int totalFields) {
    FILE *fp=fopen(FILE_NAME,"r");
    if(!fp) { printf("No data found.\n"); return; }

    char line[256];
    fgets(line,sizeof(line),fp); // skip header

    // Print header
    for(int i=0;i<selectCount;i++) printf("%s\t", selectFields[i]);
    printf("\n");

    while(fgets(line,sizeof(line),fp)) {
        line[strcspn(line,"\n")]=0;
        char row[50][50];
        int cnt=split(line,row);
        if(cnt!=totalFields) continue;
        for(int i=0;i<selectCount;i++){
            int idx=find_index(tableFields,totalFields,selectFields[i]);
            printf("%s\t", row[idx]);
        }
        printf("\n");
    }
    fclose(fp);
}

void selectWhere(char selectFields[50][50], int selectCount, char tableFields[50][50], int totalFields, char *whereField, char *whereValue){
    FILE *fp=fopen(FILE_NAME,"r");
    if(!fp) { printf("No data found.\n"); return; }

    char line[256];
    fgets(line,sizeof(line),fp); // skip header

    int whereIdx=find_index(tableFields,totalFields,whereField);
    if(whereIdx==-1){ printf("Error: Field '%s' not found.\n",whereField); fclose(fp); return; }

    // Print header
    for(int i=0;i<selectCount;i++) printf("%s\t", selectFields[i]);
    printf("\n");

    while(fgets(line,sizeof(line),fp)) {
        line[strcspn(line,"\n")]=0;
        char row[50][50];
        int cnt=split(line,row);
        if(cnt!=totalFields) continue;
        if(strcmp(row[whereIdx],whereValue)==0){
            for(int i=0;i<selectCount;i++){
                int idx=find_index(tableFields,totalFields,selectFields[i]);
                printf("%s\t", row[idx]);
            }
            printf("\n");
        }
    }

    fclose(fp);
}

void parseQuery(char *query){
    trim(query);
    if(strlen(query)==0) return;

    // Check exit
    if(strcasecmp(query,"EXIT")==0){
        printf("Exiting...\n"); exit(0);
    }

    // Must end with ;
    if(query[strlen(query)-1]!=';'){ printf("Error: Query must end with ';'\n"); return; }
    query[strlen(query)-1]=0;

    char qCopy[256];
    strcpy(qCopy,query);

    char *wherePtr=strstr(qCopy," WHERE ");
    char wherePart[100]="";
    if(wherePtr){ strcpy(wherePart,wherePtr+7); *wherePtr=0; }

    // Check SELECT
    if(strncmp(qCopy,"SELECT ",7)!=0){ printf("Error: Query must start with SELECT\n"); return; }

    char *fromPtr=strstr(qCopy+7," FROM ");
    if(!fromPtr){ printf("Error: Missing FROM clause\n"); return; }

    *fromPtr=0;
    char selectPart[100];
    strcpy(selectPart,qCopy+7);
    trim(selectPart);

    char tableName[50];
    strcpy(tableName,fromPtr+6);
    trim(tableName);

    // Validate table name
    if(strcmp(tableName,"students")!=0){ printf("Error: Table '%s' not found.\n",tableName); return; }

    // Read header
    FILE *fp=fopen(FILE_NAME,"r");
    if(!fp){ printf("Error: File not found\n"); return; }
    char line[256];
    fgets(line,sizeof(line),fp);
    line[strcspn(line,"\n")]=0;
    char tableFields[50][50];
    int totalFields=split(line,tableFields);
    fclose(fp);

    // Parse select fields
    char selectFields[50][50];
    int selectCount=0;
    if(strcmp(selectPart,"*")==0){
        selectCount=totalFields;
        for(int i=0;i<totalFields;i++) strcpy(selectFields[i],tableFields[i]);
    } else {
        selectCount=split(selectPart,selectFields);
        for(int i=0;i<selectCount;i++){
            if(find_index(tableFields,totalFields,selectFields[i])==-1){
                printf("Error: Field '%s' not found.\n",selectFields[i]);
                return;
            }
        }
    }

    // If WHERE exists
    if(strlen(wherePart)>0){
        char whereField[50], whereValue[50];
        char *eq=strchr(wherePart,'=');
        if(!eq){ printf("Error: Invalid WHERE clause\n"); return; }
        *eq=0;
        strcpy(whereField,wherePart);
        strcpy(whereValue,eq+1);
        trim(whereField); trim(whereValue);
        selectWhere(selectFields,selectCount,tableFields,totalFields,whereField,whereValue);
    } else {
        selectAll(selectFields,selectCount,tableFields,totalFields);
    }
}

// int main(){
//     char query[300];
//     printf("=== SQL-Style Student Console ===\n");
//     while(1){
//         printf("\n> ");
//         if(!fgets(query,sizeof(query),stdin)) break;
//         query[strcspn(query,"\n")]=0;
//         parseQuery(query);
//     }
//     return 0;
// }

int main(){
    char query[300];

    // User Manual
    printf("=== SQL-Style Student Console ===\n\n");
    printf("Welcome! You can run SQL-style queries on the 'students' table.\n");
    printf("Examples of commands:\n");
    printf("1. SELECT * FROM students;\n");
    printf("2. SELECT name, age FROM students;\n");
    printf("3. SELECT name, grade FROM students WHERE age=20;\n");
    printf("4. EXIT;  (to quit the program)\n");
    printf("\nRules:\n");
    printf("- All queries must end with a semicolon ';'\n");
    printf("- Field names must match the table header (id, name, age, grade)\n");
    printf("- WHERE clause format: field=value\n\n");

    while(1){
        printf("> ");
        if(!fgets(query,sizeof(query),stdin)) break;
        query[strcspn(query,"\n")]=0;
        parseQuery(query);
    }

    return 0;
}

