#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

 //example12

 // flag 가 0이면 object하나만 있는 json data, 1이면 배열이 먼저나오고 object가 두 개인 json data

 /*
 1.namelist 뽑기 : key값만 뽑으면됨
 2.objectlist 뽑기 & 검색했을 시 data출력
 */

char* readJSONFILE();
void nameList(char *jsonstr, jsmntok_t *t, int tokcount,int *nameTokIndex);
void objectList(char *jsonstr, jsmntok_t *t, int flag, int tokcount,int*arrForObjectIndex);
void selectObjectNumAndPrint(char *jsonstr, jsmntok_t *t, int* arrForObjectIndex);
void whatIsStart(jsmntok_t *t, int *flag);

int main() {
	int nameTokIndex[100]; // key값 토큰 인덱스 담기 위해
	int arrForObjectIndex[100]; // object 토큰 인덱스 담기위해

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

	int flag;
	whatIsStart(t,&flag);

	nameList(JSON_STRING,t,r,nameTokIndex);
	objectList(JSON_STRING,t,flag,r,arrForObjectIndex);
	selectObjectNumAndPrint(JSON_STRING,t,arrForObjectIndex);
	return EXIT_SUCCESS;
}

char* readJSONFILE(){
	FILE *f=NULL;

	char file_name[30];
	printf("원하는 파일명 입력");
	scanf("%s",file_name);
	strcat(file_name,".json");
	f=fopen(file_name,"r");

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

void nameList(char *jsonstr, jsmntok_t *t, int tokcount,int *nameTokIndex){
	int i;
	int count=0;
	printf("********NAME LIST********\n\n");
	for(i=0; i<tokcount; i++){
		if(t[i].size==1&&t[i].type==JSMN_STRING) //key 값 찾는 조건
		{
			nameTokIndex[count++]=i;
			printf("[Name %d]%.*s\n",count,t[i].end-t[i].start,jsonstr + t[i].start); // size 가 0보다 높은게 key 값임, i값으로 해야 key 값이 나옴
			}
	}
}

void objectList(char *jsonstr, jsmntok_t *t, int flag, int tokcount, int* arrForObjectIndex){
	int count=0; //object의 개수 의미
	printf("\n********Object List********\n\n");

	if(flag==0)//object하나만, 처음에 object로 시작함
	{
		arrForObjectIndex[count++]=0;
		printf("[Name %d] %.*s\n",count,t[2].end-t[2].start,
			 jsonstr + t[2].start);

	}

	int i;

	if(flag==1)//배열로 시작, object 여러개
	{
		for(i=0; i<tokcount; i++){
			if(t[i].parent==0) //object 찾음
			{
				arrForObjectIndex[count++]=i; // object token의 index 를 이 배열에 넣는다.
				printf("[Name %d] %.*s\n",count,t[i+2].end-t[i+2].start,
					 jsonstr + t[i+2].start); // object의 index에서 +2한 값이 name의 value값임

			}
		}
	}
}

void selectObjectNumAndPrint(char *jsonstr, jsmntok_t *t, int* arrForObjectIndex){
	while(1){
		//selectObjectNum

		int ObjectNum;
		printf("Select Name's no (exit:0) >>");
		scanf("%d",&ObjectNum);
		if(ObjectNum==0) exit(1);

		//print
		printf("%.*s\n", t[arrForObjectIndex[ObjectNum-1]].end-t[arrForObjectIndex[ObjectNum-1]].start,jsonstr + t[arrForObjectIndex[ObjectNum-1]].start);
		// ObjectNum-1 하는 이유 : 실제 인덱스는 0부터 시작하고 선택은 1,2,3,4 이런 식으로 하므로
	}
}


void whatIsStart(jsmntok_t *t, int *flag){
	if(t[0].type==2) *flag=1; // type이 2이면 배열 부터 시작, 그 말은 즉슨, 여러 개의 object가 배열안에 들어있다는 말. 그래서 flag를 1ㄹ로 초기화
	if(t[0].type==1) *flag=0; // object부터 시작, 그래서 flag를 0으로 초기화
}
