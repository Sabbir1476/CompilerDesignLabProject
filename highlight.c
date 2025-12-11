#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define RESET   "\033[0m"
#define BLUE    "\033[34m"   // identifiers
#define RED     "\033[31m"   // keywords
#define GREEN   "\033[32m"   // numbers
#define YELLOW  "\033[33m"   // strings
#define GREY    "\033[90m"   // comments
#define MAGENTA "\033[35m"   // operators

// Keywords list
char *keywords[] = {
    "int","float","char","double","long","short","void","if","else",
    "while","for","return","break","continue","switch","case","default",
    "struct","typedef","include","define","const","unsigned","signed","sizeof"
};

int keywordCount = sizeof(keywords)/sizeof(keywords[0]);

// Token counters
int count_keyword = 0, count_identifier = 0, count_number = 0;
int count_string = 0, count_single_comment = 0, count_multi_comment = 0;
int count_operator = 0;

// Check if a word is keyword
int isKeyword(char *word) {
    for(int i=0;i<keywordCount;i++)
        if(strcmp(word,keywords[i])==0) return 1;
    return 0;
}

// Check if a word is a number
int isNumber(char *word) {
    int dotCount = 0;
    for(int i=0; word[i]; i++){
        if(word[i]=='.') dotCount++;
        else if(!isdigit(word[i])) return 0;
    }
    return 1;
}

int main() {
    FILE *in = fopen("input.c","r");
    FILE *html = fopen("output.html","w");

    if(!in){
        printf("Error: Cannot open input.c\n");
        return 1;
    }

    // HTML header
    fprintf(html,"<html><body style='font-family: monospace; white-space: pre;'>\n");

    char line[1000];
    int lineNumber = 1;

    printf("\n====== Highlighted Code ======\n\n");

    while(fgets(line,sizeof(line),in)){
        printf(YELLOW "%3d | " RESET,lineNumber);
        fprintf(html,"%3d | ",lineNumber);

        int i=0;
        while(line[i]!='\0'){
            // Single-line comment
            if(line[i]=='/' && line[i+1]=='/'){
                printf(GREY "//");
                fprintf(html,"<span style='color:gray'>//");
                i+=2;
                count_single_comment++;
                while(line[i]!='\0' && line[i]!='\n'){
                    printf("%c",line[i]);
                    fprintf(html,"%c",line[i]);
                    i++;
                }
                printf(RESET);
                fprintf(html,"</span>");
            }
            // Multi-line comment
            else if(line[i]=='/' && line[i+1]=='*'){
                printf(GREY "/*");
                fprintf(html,"<span style='color:gray'>/*");
                i+=2;
                count_multi_comment++;
                while(line[i]!='\0'){
                    if(line[i]=='*' && line[i+1]=='/'){
                        printf("*/");
                        fprintf(html,"*/</span>");
                        i+=2;
                        break;
                    }
                    printf("%c",line[i]);
                    fprintf(html,"%c",line[i]);
                    i++;
                }
            }
            // String literal
            else if(line[i]=='"'){
                printf(YELLOW "\"");
                fprintf(html,"<span style='color:orange'>\"");
                i++;
                count_string++;
                while(line[i]!='\0' && line[i]!='"'){
                    printf("%c",line[i]);
                    fprintf(html,"%c",line[i]);
                    i++;
                }
                if(line[i]=='"'){
                    printf("\"" RESET);
                    fprintf(html,"\"</span>");
                    i++;
                }
            }
            // Identifier or keyword
            else if(isalpha(line[i]) || line[i]=='_'){
                char word[100];
                int idx=0;
                while(isalnum(line[i]) || line[i]=='_'){
                    word[idx++]=line[i];
                    i++;
                }
                word[idx]='\0';
                if(isKeyword(word)){
                    printf(RED "%s" RESET,word);
                    fprintf(html,"<span style='color:red;font-weight:bold;'>%s</span>",word);
                    count_keyword++;
                } else {
                    printf(BLUE "%s" RESET,word);
                    fprintf(html,"<span style='color:blue;'>%s</span>",word);
                    count_identifier++;
                }
            }
            // Number
            else if(isdigit(line[i])){
                char num[50];
                int idx=0;
                while(isdigit(line[i]) || line[i]=='.'){
                    num[idx++]=line[i];
                    i++;
                }
                num[idx]='\0';
                printf(GREEN "%s" RESET,num);
                fprintf(html,"<span style='color:green;'>%s</span>",num);
                count_number++;
            }
            // Operators / symbols
            else{
                if(!isspace(line[i])) count_operator++;
                printf("%c",line[i]);
                fprintf(html,"%c",line[i]);
                i++;
            }
        }
        lineNumber++;
    }

    // Close HTML
    fprintf(html,"</body></html>");
    fclose(html);
    fclose(in);

    // Summary
    printf("\n\n====== Token Summary ======\n");
    printf("Keywords: %d\n",count_keyword);
    printf("Identifiers: %d\n",count_identifier);
    printf("Numbers: %d\n",count_number);
    printf("Strings: %d\n",count_string);
    printf("Single-line Comments: %d\n",count_single_comment);
    printf("Multi-line Comments: %d\n",count_multi_comment);
    printf("Operators/Symbols: %d\n",count_operator);
    printf("\nHTML file generated: output.html\n");

    return 0;
}
