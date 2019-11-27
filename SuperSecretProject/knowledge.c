/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the chatbot's knowledge base.
 *
 * knowledge_get() retrieves the response to a question.
 * knowledge_put() inserts a new response to a question.
 * knowledge_read() reads the knowledge base from a file.
 * knowledge_reset() erases all of the knowledge.
 * kowledge_write() saves the knowledge base in a file.
 *
 * You may add helper functions as necessary.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "chat1002.h"

/*
 * Get the response to a question.
 *
 * Input:
 *   intent   - the question word
 *   entity   - the entity
 *   response - a buffer to receive the response
 *   n        - the maximum number of characters to write to the response buffer
 *
 * Returns:
 *   KB_OK, if a response was found for the intent and entity (the response is copied to the response buffer)
 *   KB_NOTFOUND, if no response could be found
 *   KB_INVALID, if 'intent' is not a recognised question word
 */

void wordsplit(char* sen, char arr[][MAX_ENTITY], int* len) {
	int j = 0;
	int ctr = 0;
	for (int i = 0; i <= (strlen(sen)); i++)
	{
		if (sen[i] == ' ' || sen[i] == '\0')
		{
			arr[ctr][j] = '\0';
			ctr++;  //for next word
			j = 0;    //for next word, init index to 0
			(*len)++;
		}
		else
		{
			arr[ctr][j] = sen[i];
			j++;
		}
	}
}

int knowledge_get(const char *intent, const char *entity, char *response, int n, Know *know) {
	
	Know* now = know;
	/* ==================================================================================================================================== */
/* ----------------------------------------------------- If intent=="WHO" ----------------------------------------------------- */
	if (compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0 || compare_token(intent, "how") == 0 || compare_token(intent, "why") == 0 || compare_token(intent, "when") == 0) {
		char questionArray[MAX_ENTITY][MAX_ENTITY];
		int questionLen = 0;
		wordsplit(entity, &questionArray, &questionLen);
		int iHighestMatch = 0;
		char currResponse[MAX_RESPONSE];
		if (now != NULL) {                                    
			while (now) {                                                    
				if (compare_token(now->intent, intent) == 0) {         
					int iCurrMatch = 0;
					int iHighestpossible = 0;
					for (int i = 0; i < questionLen; i++)
					{
						char nodeEntityArray[MAX_ENTITY][MAX_ENTITY];
						int nodeEntityLen = 0;
						wordsplit(now->entity, &nodeEntityArray, &nodeEntityLen);
						iHighestpossible = nodeEntityLen * questionLen;
						for (int x = 0; x < nodeEntityLen; x++)
						{
							if (compare_token(questionArray[i], nodeEntityArray[x]) == 0)
								iCurrMatch++;
						}
					}
				
					if (iCurrMatch > iHighestMatch)
					{
						iHighestMatch = iCurrMatch;
						strncpy(response, now->value, n);        /* Copy it to the response buffer */
					}
					if (iCurrMatch >= iHighestpossible) //if complete match
					{
						strncpy(response, now->value, n);
						return KB_FOUND;
					}
				}
				now = now->next;                      /* If program did not enter the if statement (no entity match), point the iterator to the next node */
			}
			if (iHighestMatch != 0) //if partial match
			{
				return KB_FOUND;
			}

			/* If code reaches this point, it means that after iterating through all nodes in Linked-list, no match was found */
			/* Hence, return a KB_NOTFOUND code (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
		else {                                                       /* Else if linked-list does not exist, return KB_NOTFOUND (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
	}
	/* -------------------------------- Else if not recognised intent, return KB_INVALID (no such intent) -------------------------------- */
	else {
		return KB_INVALID;
	}
	/* ==================================================================================================================================== */
	
}


/*
 * Insert a new response to a question. If a response already exists for the
 * given intent and entity, it will be overwritten. Otherwise, it will be added
 * to the knowledge base.
 *
 * Input:
 *   intent    - the question word
 *   entity    - the entity
 *   response  - the response for this question and entity
 *
 * Returns:
 *   KB_FOUND, if successful
 *   KB_NOMEM, if there was a memory allocation failure
 *   KB_INVALID, if the intent is not a valid question word
 */
int knowledge_put(const char* intent, const char* entity, char* response, int n, Know *know)
{
	/* ===================================================================================================================================== */
	char temp_BUFFER[MAX_RESPONSE];                                  /* Create a temp buffer for response (needed to call knowledge_get()) */

	Know* storeknow = know;

	int get_code = knowledge_get(intent, entity, temp_BUFFER, MAX_RESPONSE, storeknow);   /* Call knowledge_get() to check if the Intent and Entity pair already exists */
	memset(temp_BUFFER, '\0', MAX_RESPONSE);                           /* Clear the temp_response buffer */

	/* Return KB_INVALID if intent is invalid */
	if (abs(get_code) == 2) {
		return KB_INVALID;
	}

	/* Else, either overwrite or insert the node depending on the return code, and the Intent */
	else {
		/* -------------------------------------------------- Intent == 'WHO' ----------------------------------------------------------------- */
		if (compare_token(intent, "what") == 0 || compare_token(intent, "where") == 0 || compare_token(intent, "who") == 0 || compare_token(intent, "how") == 0 || compare_token(intent, "why") == 0 || compare_token(intent, "when") == 0) {                    /* If intent is 'WHO' */
			//  If Intent and Entity pair already exists, iterate the 'WHO' linked-list & overwrite current response data
			if (abs(get_code) == 0) {                                    /* Point the whoIterator to head of 'WHO' linked-list */
				do {                                                          /* While no match is found, Make the iterator point to the next node (if last node, it will point to NULL) */
					if (compare_token(storeknow->entity, entity) == 0) {              /* If a node with the same Entity exists */
						strncpy(storeknow->value, response, MAX_RESPONSE);        /* Overwrite the existing response */
						return KB_FOUND;                                        /* Return code 'KB_OK', which is of value 0 */
					}

					storeknow = storeknow->next;
				} while (storeknow != NULL);
			}

			//  If no existing record of Intent and Entity exists, create & insert the node to the head of 'WHO' linked-list */
			else if (abs(get_code) == 1) {
				if (storeknow == NULL) {                                             /* If not enough memory (memory failure) */
					return KB_NOMEM;                                         /* Return code 'KB_NOMEM' which is of value -4 */
				}
				while (storeknow->next != NULL) //skip to the last node
					storeknow = storeknow->next;
				Know* sknow = (Know*)malloc(sizeof(Know));
				strcpy(sknow->intent, intent);
				strcpy(sknow->entity, entity);
				strcpy(sknow->value, response);
				sknow->next = NULL;
				storeknow->next = sknow; //set new node to the next of the last node
                
				return KB_FOUND;                                                /* Return code 'KB_OK' which is of value 0 */
			}
		}
	}
}


/*
 * Read a knowledge base from a file.
 *
 * Input:
 *   f - the file
 *
 * Returns: the number of entity/response pairs successful read from the file
 */
int knowledge_read(FILE* f, Know* know) {
	int iCount = 0;
	while (!feof(f))
	{
		char tLine[152]; //line to read each line of data

		char iIntent[MAX_INTENT];
		char iEntity[MAX_ENTITY];
		char iValue[MAX_RESPONSE];
		iIntent[0] = '\0';
		iEntity[0] = '\0';
		iValue[0] = '\0'; //turn them into null terminated empty strings

		fscanf(f, "%151[^\n]\n", &tLine);

		int iType = 1; //1 = intent, 2 = entity, 3=value
		int iSkip = 0;
		for (int i = 0; i < strlen(tLine); i++)
		{
			iSkip = 0; //reset skip for every iteration
			char iC = tLine[i];
			if ((iC == ',') || (iC == '='))
			{
				iType++;
				iSkip++;
			}

			if (iSkip == 0) //if skip is not incremented
			{
				char tC[2];
				tC[0] = iC;
				tC[1] = '\0'; //turn char into a null terminated string so strcat can work

				if (iType == 1)
					strcat(iIntent, tC);
				else if (iType == 2)
					strcat(iEntity, tC);
				else
					strcat(iValue, tC);
			}
		}
		Know* tKnow = know;
		while (tKnow->next != NULL) //skip to the last node
			tKnow = tKnow->next;
		Know* tIKnow = (Know*)malloc(sizeof(Know));
		strcpy(tIKnow->intent, iIntent);
		strcpy(tIKnow->entity, iEntity);
		strcpy(tIKnow->value, iValue);
		tIKnow->next = NULL;
		tKnow->next = tIKnow; //set new node to the next of the last node
		iCount++;
	}
	return iCount;
}


/*
 * Reset the knowledge base, removing all know entitities from all intents.
 */
void knowledge_reset() {
	//mabel

	Know* temp = headp;
	Know* nextitem;

	if (headp != NULL) {

		nextitem = temp->next;
		temp->next = NULL;
		temp = nextitem;
		while (temp) {
			Know* temp1 = temp;
			temp = temp->next;
			free(temp1);
		}
		headp = NULL;
	}

}


/*
 * Write the knowledge base to a file.
 *
 * Input:
 *   f - the file
 */
int knowledge_write(FILE* f, Know* know) {
	//the argument passed will be the header and it will be null except for its next
	Know* tknow = know;
	while (tknow->next != NULL)
	{
		tknow = tknow->next;
		fprintf(f, "%s,%s=%s\n", tknow->intent, tknow->entity, tknow->value);
		//format of each line will be <INTENT>,<ENTITY>=<VALUE>
	}
	return 1;
}