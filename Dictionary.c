#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define LENGTH 100
#define API_LENGTH 200
#define DEFINITION_LENGTH 1000

FILE *fp_dictionary;
int file_size;
char *definition;

char *get_definition(char*);

void main() 
{
    char word[LENGTH];
    char dictionary_api[API_LENGTH], final_api[API_LENGTH], audio_api[API_LENGTH];
    printf("Enter a word: ");
    scanf("%s", word);
    
    char *api_1 = "curl -s \"https://api.dictionaryapi.dev/api/v2/entries/en/";
    char *api_2 = "\"";
    char *audio_api_1 = "curl -s \"https://api.dictionaryapi.dev/media/pronunciations/en/";
    char *audio_api_2 = "-us.mp3\"";
    
    sprintf(dictionary_api, "%s%s%s%s", api_1, word, api_2, ">dictionary.txt");
    system(dictionary_api);

    fp_dictionary = fopen("dictionary.txt", "r");
    fseek(fp_dictionary, 0, SEEK_END);
    file_size = ftell(fp_dictionary);
    fseek(fp_dictionary, 0, SEEK_SET);

    char *dictionary_api_response = malloc(DEFINITION_LENGTH);
    definition = malloc(DEFINITION_LENGTH);
    
    fgets(dictionary_api_response, DEFINITION_LENGTH, fp_dictionary);

    definition = get_definition(dictionary_api_response);
    if (!strcmp(definition, "Not found"))
    {
    	printf("%s not found in dictionary.", word);
    }
    else
    {

	    printf("Definition: %s\n", definition);

	    sprintf(audio_api, "%s%s%s%s", "espeak \"", word,"\"");
         // audio_api_1, word, audio_api_2, ">word_audio.mp3");
        system(audio_api);
	    // system("vlc word_audio.mp3");

       
    }

    fclose(fp_dictionary);
    
 }

char *get_definition(char dictionary_api_response[]) 
{
	strcpy(definition, "Not found");
    char *token = strtok(dictionary_api_response, ":");
    while (token != NULL) 
    {
        if (strcmp(token, "[{\"definition\"") == 0) 
        {
            token = strtok(NULL, "\";.");
            strcpy(definition, token);
            break;
        } 
        else 
        {
            token = strtok(NULL, ":");
        }
    }
    return definition;
}

