#include "gt_lib.h"
#include <stdio.h>
#include <string.h>

#define ADJ 33
#define TRIE_CHILD 95
#define CONFIG_BUFFER_SIZE 2000000

char buffer[CONFIG_BUFFER_SIZE];

typedef struct TrieNode {
	int valLen;
	void *value;
	struct TrieNode *child[TRIE_CHILD];
}TrieNode;

TrieNode *root;

// ASC = [33,127]
// [KEY (ascii)] [...] [=] [VAL (visible)] [\n\r+]
void AddKeyword(char *keyst,char *keyed,char *val,int vallen) {
	TrieNode *cur = root;
	while (1) {
		if (cur->child[*keyst - ADJ] == NULL) {
			cur->child[*keyst - ADJ] = calloc(1,sizeof(TrieNode));
		}

		cur = cur->child[*keyst - ADJ];
		if (keyst == keyed) {
			if (cur->value != NULL) gt_free(cur->value);
			
			cur->value = gt_malloc(sizeof(char) * vallen);
			gt_memcpy(cur->value, val, vallen);

			cur->valLen = vallen;
			
			break;
		}
		keyst++;
	}
}

int GtConfigParser_Open(gt_utf8 *path) {
	int len, eqpos, sharppos;
	int l1,r1,l2,r2;
	int failflag;
	FILE *f;
	if ((f = fopen((char*)path, "rt")) == NULL) {
		//CANNOT OPEN FILE
		return -1;
	}

	root = gt_calloc(1,sizeof(TrieNode));
	while (!feof(f)) {
		fgets(buffer, CONFIG_BUFFER_SIZE, f);
		len = strlen(buffer);
		eqpos = sharppos = -1;
		failflag = 0;
	
		//Get equal & sharp position
		int i;
		for (i=0;i<len;i++) {
			if (buffer[i]=='=') {
				if (eqpos == -1)
					eqpos = i;
				else
					eqpos = -2;
			}
			else if (buffer[i]=='#') {
				if (sharppos == -1)
					sharppos = i;
				break;
			}
		}

		if (sharppos != -1)
			len = sharppos;

		if (failflag)continue;
		if (eqpos < 0)continue;

		//ARGUMENT
		l1=0; r1=eqpos-1;
		while (l1 <= r1) {
			//invisible or not ascii
			if (buffer[l1] < 33 || buffer[l1] < 0)
				l1++;
			else if (buffer[r1] < 33 || buffer[r1] < 0)
				r1--;
			else
				break;
		}
		//CHECK FOR VALID CHARACTERS
		for (i = l1; i <= r1; i++)
			if (buffer[i] < 33 || buffer[i] < 0) {
				failflag=1;
				break;
			}
		if (failflag)continue;

		l2=eqpos+1; r2=len-1;
		while (l2 <= r2) {
			if (buffer[l2] < 33 && buffer[l2] >= 0) {
				l2++;
			}
			else if (buffer[r2] < 33 && buffer[r2] >= 0) {
				r2--;
			}
			else {
				break;
			}
		}

		if (l1 > r1 || l2 > r2) continue;

/*		printf("find key = ");
		for (int i=l1;i<=r1;i++)
			putchar(buffer[i]);

		printf("; and value = ");
		for (int i=l2;i<=r2;i++)
			putchar(buffer[i]);
		printf("; valLength = %d\n",r2-l2+1);
		puts(""); */
		
		AddKeyword(&buffer[l1],&buffer[r1],&buffer[l2],r2-l2+1);
	}
	
	return GT_OK;
}

int AskKeyword(char *key,char *val,int valueSize) {
	TrieNode *cur = root;
	while (*key) {
		if (cur==NULL) {
			break;
		}
		cur = cur->child[*key - ADJ];
		key++;
	}
	
	//NULL
	if (cur==NULL || cur->value==NULL) {
		*val = 0;
		return GT_OK;
	}

	//size 
	if (valueSize < cur->valLen+1) {
		return -1;
	}
	
	gt_memcpy(val, cur->value, cur->valLen);
	val[cur->valLen] = 0;
	
	return GT_OK;
}

int GtConfigParser_Get(gt_utf8 *key, gt_utf8 *value, int valueSize) {
	unsigned char *tmp = key;
	while (*tmp) {
		//INVALID CHAR
		if (*tmp < 33 || *tmp >= 128) {
			return -1;
		}
		tmp++;
	}

	//SIZE
	if (AskKeyword((char*)key,(char*)value,valueSize) != GT_OK) {
		return -1;
	}

	return GT_OK;
}

void delete_node(TrieNode *cur) {
	if (cur==NULL) return;

	int i;
	for (i = 0; i < TRIE_CHILD; i++) {
		delete_node(cur->child[i]);
	}

	if (cur->value != NULL) {
		gt_free(cur->value);
	}

	gt_free(cur);
}

int GtConfigParser_Close() {
	delete_node(root);

	return GT_OK;
}
