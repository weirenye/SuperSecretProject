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
#include "knowledge.h"

tKnowledgeItem* database = NULL;

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



int knowledge_get(const char *intent, const char *entity, char *response, int n, Know *know) {
	
	/* ==================================================================================================================================== */
/* ----------------------------------------------------- If intent=="WHO" ----------------------------------------------------- */
	if (compare_token(intent, "who") == 0) {
		if (know != NULL) {                                    /* Point the whoIterator to head of 'WHO' linked-list */
			do {                                                     /* While no match is found, Make the iterator point to the next node (if last node, it will point to NULL) */
				if (compare_token(know->entity, entity) == 0) {         /* If a node with the same entity exists */
					strncpy(response, know->value, n);        /* Copy it to the response buffer */
					return KB_FOUND;                                   /* After copying to response buffer, return KB_OK (0) */
				}

				know = know->next;                      /* If program did not enter the if statement (no entity match), point the iterator to the next node */
			} while (know);//

			/* If code reaches this point, it means that after iterating through all nodes in Linked-list, no match was found */
			/* Hence, return a KB_NOTFOUND code (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
		else {                                                       /* Else if linked-list does not exist, return KB_NOTFOUND (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
	}
	/* ----------------------------------------------------- ELSE If intent=="WHAT" -----------------------------------------------------*/
	else if (compare_token(intent, "what") == 0) {
		if (know != NULL) {                                      /* If a linked-list already exists */
			do {                                                     /* While no match is found, Make the iterator point to the next node (if last node, it will point to NULL) */
				if (compare_token(know->entity, entity) == 0) {        /* If a node with the same entity exists */
					strcpy(response, know->value, n);       /* Copy it to the response buffer */   /**************************************************************888888/
					return KB_FOUND;                                   /* After copying to response buffer, return KB_OK (0) */
				}

				know = know->next;                    /* If program did not enter the if statement (entity did not match), point the iterator to the next node */
			} while (know);

			/* If code reaches this point, it means that after iterating through all nodes in Linked-list, no match was found */
			/* Hence, return a KB_NOTFOUND code (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
		else {                                                       /* Else if linked-list does not exist, return KB_NOTFOUND (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
	}
	/* ----------------------------------------------------- ELSE If intent=="WHERE" ----------------------------------------------------- */
	else if (compare_token(intent, "where") == 0) {
		if (know != NULL) {                                     /* If a linked-list already exists */
			do {                                                     /* While no match is found, Make the iterator point to the next node (if last node, it will point to NULL) */
				if (compare_token(know->entity, entity) == 0) {       /* If a node with the same entity exists */
					strncpy(response, know->value, n);      /* Copy it to the response buffer */
					return KB_FOUND;                                   /* After copying to response buffer, return KB_OK (0) */
				}

				know = know->next;                  /* If program did not enter the if statement (entity did not match), point the iterator to the next node */
			} while (know);

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
	/* ==================================================================================================================================== */
	/* ----------------------------------------------------- If intent=="WHO" ----------------------------------------------------- */
	if (compare_token(intent, "who") == 0) {
		if (know != NULL) {                                       /* If a linked-list already exists */
			do {                                                     /* While no match is found, Make the iterator point to the next node (if last node, it will point to NULL) */
				if (compare_token(know->entity, entity) == 0) {         /* If a node with the same entity exists */
					strncpy(response, know->value, n);        /* Copy it to the response buffer */
					return KB_FOUND;                                   /* After copying to response buffer, return KB_OK (0) */
				}

				know = know->next;                      /* If program did not enter the if statement (no entity match), point the iterator to the next node */
			} while (know);

			/* If code reaches this point, it means that after iterating through all nodes in Linked-list, no match was found */
			/* Hence, return a KB_NOTFOUND code (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
		else {                                                       /* Else if linked-list does not exist, return KB_NOTFOUND (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
	}
	/* ----------------------------------------------------- ELSE If intent=="WHAT" -----------------------------------------------------*/
	else if (compare_token(intent, "what") == 0) {
		if (know != NULL) {                                      /* If a linked-list already exists */
			do {                                                     /* While no match is found, Make the iterator point to the next node (if last node, it will point to NULL) */
				if (compare_token(know->entity, entity) == 0) {        /* If a node with the same entity exists */
					strncpy(response, know->value, n);       /* Copy it to the response buffer */
					return KB_FOUND;                                   /* After copying to response buffer, return KB_OK (0) */
				}

				know = know->next;                    /* If program did not enter the if statement (entity did not match), point the iterator to the next node */
			} while (know);

			/* If code reaches this point, it means that after iterating through all nodes in Linked-list, no match was found */
			/* Hence, return a KB_NOTFOUND code (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
		else {                                                       /* Else if linked-list does not exist, return KB_NOTFOUND (-1), this makes chatbot_do_question invoke knowledge_put() */
			return KB_NOTFOUND;
		}
	}
	/* ----------------------------------------------------- ELSE If intent=="WHERE" ----------------------------------------------------- */
	else if (compare_token(intent, "where") == 0) {
		if (know != NULL) {                                     /* If a linked-list already exists */
			do {                                                     /* While no match is found, Make the iterator point to the next node (if last node, it will point to NULL) */
				if (compare_token(know->entity, entity) == 0) {       /* If a node with the same entity exists */
					strncpy(response, know->value, n);      /* Copy it to the response buffer */
					return KB_FOUND;                                   /* After copying to response buffer, return KB_OK (0) */
				}

				know = know->next;                  /* If program did not enter the if statement (entity did not match), point the iterator to the next node */
			} while (know);

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
	/* to be implemented */

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