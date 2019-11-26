/*
 * ICT1002 (C Language) Group Project, AY19 Trimester 1.
 *
 * This file implements the behaviour of the chatbot. The main entry point to
 * this module is the chatbot_main() function, which identifies the intent
 * using the chatbot_is_*() functions then invokes the matching chatbot_do_*()
 * function to carry out the intent.
 *
 * chatbot_main() and chatbot_do_*() have the same method signature, which
 * works as described here.
 *
 * Input parameters:
 *   inc      - the number of words in the question
 *   inv      - an array of pointers to each word in the question
 *   response - a buffer to receive the response
 *   n        - the size of the response buffer
 *
 * The first word indicates the intent. If the intent is not recognised, the
 * chatbot should respond with "I do not understand [intent]." or similar, and
 * ignore the rest of the input.
 *
 * If the second word may be a part of speech that makes sense for the intent.
 *    - for WHAT, WHERE and WHO, it may be "is" or "are".
 *    - for SAVE, it may be "as" or "to".
 *    - for LOAD, it may be "from".
 * The word is otherwise ignored and may be omitted.
 *
 * The remainder of the input (including the second word, if it is not one of the
 * above) is the entity.
 *
 * The chatbot's answer should be stored in the output buffer, and be no longer
 * than n characters long (you can use snprintf() to do this). The contents of
 * this buffer will be printed by the main loop.
 *
 * The behaviour of the other functions is described individually in a comment
 * immediately before the function declaration.
 *
 * You can rename the chatbot and the user by changing chatbot_botname() and
 * chatbot_username(), respectively. The main loop will print the strings
 * returned by these functions at the start of each line.
 */
 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "chat1002.h"
 
 
/*
 * Get the name of the chatbot.
 *
 * Returns: the name of the chatbot as a null-terminated string
 */
const char *chatbot_botname() {

	return "Chatbot";
	
}


/*
 * Get the name of the user.
 *
 * Returns: the name of the user as a null-terminated string
 */
const char *chatbot_username() {

	return "User";
	
}


/*
 * Get a response to user input.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop (i.e. it detected the EXIT intent)
 */
int chatbot_main(int inc, char *inv[], char *response, int n, Know* knowlege) {
	
	/* check for empty input */
	if (inc < 1) {
		snprintf(response, n, "");
		return 0;
	}

	/* look for an intent and invoke the corresponding do_* function */
	if (chatbot_is_exit(inv[0]))
		return chatbot_do_exit(inc, inv, response, n);
	else if (chatbot_is_smalltalk(inv[0]))
		return chatbot_do_smalltalk(inc, inv, response, n);
	else if (chatbot_is_load(inv[0]))
		return chatbot_do_load(inc, inv, response, n, knowlege);
	else if (chatbot_is_question(inv[0]))
		return chatbot_do_question(inc, inv, response, n);
	else if (chatbot_is_reset(inv[0]))
		return chatbot_do_reset(inc, inv, response, n);
	else if (chatbot_is_save(inv[0]))
		return chatbot_do_save(inc, inv, response, n, knowlege);
	else {
		snprintf(response, n, "I don't understand \"%s\".", inv[0]);
		return 0;
	}

}


/*
 * Determine whether an intent is EXIT.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "exit" or "quit"
 *  0, otherwise
 */
int chatbot_is_exit(const char *intent) {
	
	return compare_token(intent, "exit") == 0 || compare_token(intent, "quit") == 0 || compare_token(intent, "bye") == 0;
	
}


/*
 * Perform the EXIT intent.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_exit(int inc, char *inv[], char *response, int n) {
	 
	snprintf(response, n, "Goodbye!");
	 
	return 1;
	 
}


/*
 * Determine whether an intent is LOAD.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "load"
 *  0, otherwise
 */
int chatbot_is_load(const char *intent) {
	
	return compare_token(intent, "load") == 0;
	
}


/*
 * Load a chatbot's knowledge base from a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after loading knowledge)
 */
int chatbot_do_load(int inc, char *inv[], char *response, int n, Know* know) {
	//clear existing knowlege
	knowledge_reset();
	//load
	int iSuccess = 1;
	int iLines;
	char* filename = NULL;
	if ((sizeof(inv) / sizeof(inv[0])) < 1)
		iSuccess = 0;

	if (iSuccess == 1)
	{
		filename = inv[1];
		FILE* tFile = fopen(filename, "r"); //create a file object
		iLines = knowledge_read(tFile, know); //pass to function
		fclose(tFile);
	}

	//response
	char* buffer = (char*)malloc(sizeof(char) * n);
	if (iSuccess == 1)
		snprintf(buffer, n, "Read %d responses from %s", iLines, filename);
	else
		strcpy(buffer, "Reading failed or file empty");
	strcpy(response, buffer);
	free(buffer);

	return 0;
}


/*
 * Determine whether an intent is a question.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_question(const char* intent) {

	char* isintent;
	isintent = intent;

	for (int i = 0; i < strlen(intent); i++) {
		isintent[i] = tolower(isintent[i]);
	}


	/* to be implemented */
	if (!compare_token(isintent, "what") || !compare_token(isintent, "where") || !compare_token(isintent, "who")) {
		return 1;
	}
	else {
		return 0;
	}

	return 0;

}


/*
 * Answer a question.
 *
 * inv[0] contains the the question word.
 * inv[1] may contain "is" or "are"; if so, it is skipped.
 * The remainder of the words form the entity.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after a question)
 */
int chatbot_do_question(int inc, char* inv[], char* response, int n) {

	char userintent[MAX_INTENT];				/* Define a char array to store input intent of size MAX_INTENT */
	strncpy(userintent, inv[0], sizeof(userintent) / sizeof(userintent[0]));

	char usernoun[MAX_INPUT];					/* Define a char array to store input noun */
	strcpy(usernoun, "\0");

	char userentity[MAX_ENTITY];				/* Define a char array to store input entity of size MAX_ENTITY */
	snprintf(userentity, n, "");				/* Formats entity portion */

	int get_reply_code;							/* Define an int to store flag of knowlede_get */
	char chatbot_entity[MAX_RESPONSE];			/* Define a char array to store chatbot response from knowledge_get of size MAX_RESPONSE */
	char userresponse_notfound[MAX_RESPONSE];	/* Define a char array to store user input of size MAX_RESPONSE */

	int put_reply_code;							/* Define an int to store flag of knowledge_put */


	/* Validation: Checks if Intent is accompanied by entity */
	if (inc == 1) {
		if (compare_token(userintent, "what") == 0) {
			snprintf(response, n, "Sorry, I did not understand the phrase. Did you mean something like, \"What is ICT?\"");
		}
		else if (compare_token(userintent, "who") == 0) {
			snprintf(response, n, "Sorry, I did not understand the phrase. Did you mean something like, \"Who is the cluster director of ICT?\"");
		}
		else if (compare_token(userintent, "where") == 0) {
			snprintf(response, n, "Sorry, I did not understand the phrase. Did you mean something like, \"Where is SIT?\"");
		}
		return 0;
	}
	else if ((inc == 2 && compare_token(inv[1], "is") == 0) || (inc == 2 && compare_token(inv[1], "are") == 0)) {
		snprintf(response, n, "Sorry, I did not understand the phrase. Please describe your noun.");
		return 0;
	}


	/* Simple Validation */
	for (int i = 1; i < inc; i++) {
		/* Checks for the nouns "is" or "are" */
		if ((i == 1 && compare_token(inv[i], "is") == 0) || (i == 1 && compare_token(inv[i], "are") == 0)) {
			strncpy(usernoun, inv[i], sizeof(inv[i]) / sizeof(inv[i][0]));						/* Store into usernoun */
			continue;
		}
		strcat(strcat(userentity, " "), inv[i]);		/* Store into userentity */
	}
	memmove(userentity, userentity + 1, strlen(userentity));	/* Formats entity portion */


	/* Calls knowledge_get and perform checks for various return values */
	get_reply_code = knowledge_get(userintent, userentity, chatbot_entity, n);	/* Arguments: Intent, Entity, Buffer to store response from knowledge */
	if (get_reply_code == KB_FOUND) {												/* If a response was found for the intent and entity, */
		snprintf(response, n, "%s", chatbot_entity);							/* 	the response is copied to the response buffer. */

	}
	else if (get_reply_code == KB_NOTFOUND) {																			/* If no response could be found, */
		if (compare_token(usernoun, "\0") == 0) {
			prompt_user(userresponse_notfound, MAX_INPUT, "I don't know. %s %s?", userintent, userentity);				/*	asks for user input IF usernoun is not declared, then call knowledge_put. */
		}
		else {
			prompt_user(userresponse_notfound, MAX_INPUT, "I don't know. %s %s %s?", userintent, usernoun, userentity);	/*	asks for user input IF usernoun is declared, then call knowledge_put. */
		}

		if (strcmp(userresponse_notfound, "") == 0) {
			strcpy(response, "-(");
		}
		else {

			/* Calls knowledge_put to insert user response into knowledge base */
			put_reply_code = knowledge_put(userintent, userentity, userresponse_notfound);		/* Arguments: Intent, Entity, Buffer to store user input */

			if (put_reply_code == KB_FOUND) {				/* If knowledge_put is successful */
				snprintf(response, n, "Thank you.");
			}
			else if (put_reply_code == KB_NOMEM) {	/* Else if there is insufficient memory */
				snprintf(response, n, "Memory allocation failure! Failed to create note for:\nIntent '%s'\nEntity '%s'\nResponse '%s'\n", userintent, userentity, userresponse_notfound);
				exit(1);
			}
			else if (put_reply_code == KB_INVALID) {	/* Else if the intent is not valid */
				snprintf(response, n, "Sorry, I didn't get '%s'.", userintent);
			}
		}

	}
	else if (get_reply_code == KB_INVALID) {
		snprintf(response, n, "Sorry, I didn't get '%s'.", userintent);
	}

	return 0;

}


/*
 * Determine whether an intent is RESET.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "reset"
 *  0, otherwise
 */
int chatbot_is_reset(const char *intent) {
	
	/* to be implemented */
	
	return 0;
	
}


/*
 * Reset the chatbot.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after beign reset)
 */
int chatbot_do_reset(int inc, char *inv[], char *response, int n) {
	
	/* to be implemented */
	 
	return 0;
	 
}


/*
 * Determine whether an intent is SAVE.
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is "what", "where", or "who"
 *  0, otherwise
 */
int chatbot_is_save(const char *intent) {
	
	return compare_token(intent, "save") == 0;
}


/*
 * Save the chatbot's knowledge to a file.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0 (the chatbot always continues chatting after saving knowledge)
 */
int chatbot_do_save(int inc, char *inv[], char *response, int n, Know* know) {
	//load
	int iSuccess = 1;
	char* filename = NULL;
	if ((sizeof(inv) / sizeof(inv[0])) < 1)
		iSuccess = 0;

	if (iSuccess == 1)
	{
		filename = inv[1];
		FILE* tFile = fopen(filename, "w"); //create file object
		iSuccess = knowledge_write(tFile, know); //pass to function
		fclose(tFile);
	}

	//response
	char* buffer = (char*)malloc(sizeof(char) * n);
	if (iSuccess == 1)
		snprintf(buffer, n, "My knowlege has been saved to %s", filename);
	else
		strcpy(buffer, "Writing failed");
	strcpy(response, buffer);
	free(buffer);

	return 0;
}
 
 
/*
 * Determine which an intent is smalltalk.
 *
 *
 * Input:
 *  intent - the intent
 *
 * Returns:
 *  1, if the intent is the first word of one of the smalltalk phrases
 *  0, otherwise
 */
int chatbot_is_smalltalk(const char *intent) {
	
	char* smalltalk[] = { "Hello", "Hi", "Hey", "Wassup", "Yo", "Greetings" };
	size_t stLength = sizeof(smalltalk) / sizeof(smalltalk[0]);
	for (int i = 0; i < stLength; i++)
	{
		if (compare_token(intent, smalltalk[i]) == 0)
		{
			return 1;
		}
	}
	
	return 0;
 
}


/*
 * Respond to smalltalk.
 *
 * See the comment at the top of the file for a description of how this
 * function is used.
 *
 * Returns:
 *   0, if the chatbot should continue chatting
 *   1, if the chatbot should stop chatting (e.g. the smalltalk was "goodbye" etc.)
 */
int chatbot_do_smalltalk(int inc, char *inv[], char *response, int n) {
	
	char* stOutput[] = { "Hi, Welcome to ICT1002 Chatbot", "What's up?", "Hello", "Hi", "Hey", "Wassup", "Yo", "Greetings" };
	size_t stLength = sizeof(stOutput) / sizeof(stOutput[0]);
	int randOutput = rand() % stLength;
	snprintf(response, n, stOutput[randOutput]);
	
	return 0;
	
}
  