#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */


static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

char* readJSONFILE(){
	FILE *f=NULL;
	f=fopen("data3.json","r");

	char oneline[255];
	char* JSON_STRING=(char*)malloc(50);

	int count=0;

	while(1)
	{
			fgets(oneline,50,f);
			if(feof(f)) break;
			count+=strlen(oneline);
			JSON_STRING=realloc(JSON_STRING,count+1);
			strcat(JSON_STRING,oneline);
	}
	fclose(f);

	return JSON_STRING;
}


void jsonNameList(char *jsonstr, jsmntok_t *t, int tokcount,int *nameTokIndex){
	int i;
	int count=0;
	for(i=0; i<tokcount; i++){
		if(t[i].parent==0){ printf("%d %.*s\n",i,t[i].end-t[i].start,
				jsonstr + t[i].start); // size 가 0보다 높은게 key 값임, i값으로 해야 key 값이 나옴
				nameTokIndex[count++]=i;
			}
	}

	//for(i=0; i<count; i++) printf("%d\n",nameTokIndex[i]);

}


/*
void printNameList(char *jsonstr, jsmntok_t *t, int* nameTokIndex){
	int i;
	//printf("%zu\n",sizeof(nameTokIndex)/sizeof(nameTokIndex[0]));
	for(i=0; i<10; i++)
		printf("%.*s\n", t[nameTokIndex[i]].end-t[nameTokIndex[i]].start,jsonstr + t[nameTokIndex[i]].start);
}
*/



void selectNameList(char *jsonstr, jsmntok_t *t, int* nameTokIndex){
	while(1){
		int i;
		printf("Select Name's no (exit:0) >>");
		scanf("%d",&i);
		if(i==0) exit(1);
		printf("%.*s\n", t[nameTokIndex[i-1]].end-t[nameTokIndex[i-1]].start,jsonstr + t[nameTokIndex[i-1]].start);
		//printf("%.*s\n", t[nameTokIndex[i-1]+1].end-t[nameTokIndex[i-1]+1].start,jsonstr + t[nameTokIndex[i-1]+1].start);
	}
}



int main() {
	//int* nameTokIndex=(int*)malloc(sizeof(int)*100);
	int nameTokIndex[100];
	int i;
	char* JSON_STRING=readJSONFILE();

	jsmntok_t t[128]; // We expect no more than 128 tokens, 이게 토큰이고 여기다가 parse해서 채움

	/* Creates a new parser based over a given  buffer with an array of tokens available. */
	jsmn_parser p;
	jsmn_init(&p);

	int r;
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0])); //number of token 받음

/*
	printf("Token's cout : %d\n",r);
	for (i = 0; i < r; i++){
		printf("%.*s %d: start:%d, end:%d, size:%d, type:%d parent:%d\n",t[i].end-t[i].start,
				JSON_STRING + t[i].start,i,t[i].start,t[i].end,t[i].size,t[i].type,t[i].parent);
	}
*/

	jsonNameList(JSON_STRING,t,r,nameTokIndex);
	//printNameList(JSON_STRING,t,nameTokIndex);
	selectNameList(JSON_STRING,t,nameTokIndex);
	return EXIT_SUCCESS;
}
