#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../jsmn.h"

/*
 * A small example of jsmn parsing when JSON structure is known and number of
 * tokens is predictable.
 */

static const char *JSON_STRING =
	"{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\n  "
	"\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";
	//1번째 토큰에는 전체가 담기고
	//2번째 토큰에는 user가 담김
	//"시작해서 "가져감
	//3번째 토큰에서는 johndoe가들어감
	//4번째 Admin
	//5번쨰 false
	//6번째 uid
	//7번째 1000
	//8번째 groups
	//9번째 users
	//10번째 wheels
	//11번째 audio
	//12번째 video

static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
	if (tok->type == JSMN_STRING && (int) strlen(s) == tok->end - tok->start &&
			strncmp(json + tok->start, s, tok->end - tok->start) == 0) {
		return 0;
	}
	return -1;
}

void readJSONFILE(){
	FILE *f=NULL;
	f=fopen("data.json","r");

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
	printf("%s",JSON_STRING);
}


int main() {
	int i;
	readJSONFILE();
	jsmntok_t t[128]; // We expect no more than 128 tokens, 이게 토큰이고 여기다가 parse해서 채움

	/* Creates a new parser based over a given  buffer with an array of tokens available. */
	jsmn_parser p;
	jsmn_init(&p);

	int r;
	r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t)/sizeof(t[0])); //number of token 받음

	if (r < 0) // 이 말은 count가 return이 안 되고 jsmnerr중에서 하나가 반환됬다는 소리
	{
		printf("Failed to parse JSON: %d\n", r);
		return 1;
	}

	/* Assume the top-level element is an object */
	if (r < 1 || t[0].type != JSMN_OBJECT) {
		printf("Object expected\n");
		return 1;
	}

	/* Loop over all keys of the root object */
	for (i = 1; i < r; i++) {
		if (jsoneq(JSON_STRING, &t[i], "user") == 0) {
			/* We may use strndup() to fetch string value */
			printf("- User: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "admin") == 0) {
			/* We may additionally check if the value is either "true" or "false" */
			printf("- Admin: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "uid") == 0) {
			/* We may want to do strtol() here to get numeric value */
			printf("- UID: %.*s\n", t[i+1].end-t[i+1].start,
					JSON_STRING + t[i+1].start);
			i++;
		} else if (jsoneq(JSON_STRING, &t[i], "groups") == 0) {
			int j;
			printf("- Groups:\n");
			if (t[i+1].type != JSMN_ARRAY) {
				continue; /* We expect groups to be an array of strings */
			}
			for (j = 0; j < t[i+1].size; j++) {
				jsmntok_t *g = &t[i+j+2];
				printf("  * %.*s\n", g->end - g->start, JSON_STRING + g->start);
			}
			i += t[i+1].size + 1;
		} else {
			printf("Unexpected key: %.*s\n", t[i].end-t[i].start,
					JSON_STRING + t[i].start);
		}
	}
	return EXIT_SUCCESS;
}
