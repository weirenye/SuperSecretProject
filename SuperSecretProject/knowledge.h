#ifndef KNOWLEDGE_H
#define KNOWLEDGE_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "chat1002.h"

#define WHAT_TEXT "what"
#define WHERE_TEXT "where"
#define WHO_TEXT "who"
#define IS_TEXT "is"
#define ARE_TEXT "are"

#define RESET_TEXT "reset"
#define LOAD_TEXT "load"
#define SAVE_TEXT "save"
#define EXIT_TEXT "exit"

enum intent {
	WHERE,
	WHAT,
	WHO,
	INTENT_BASIC,

	RESET,
	LOAD,
	SAVE,
	EXIT,

	INTENT_COUNT,
	INTENT_UNKNOWN
};

typedef enum intent tIntent;

static char* intent_string(const tIntent intent) {
	switch (intent) {
	case WHERE:
		return WHERE_TEXT;
	case WHAT:
		return WHAT_TEXT;
	case WHO:
		return WHO_TEXT;
	case RESET:
		return RESET_TEXT;
	case LOAD:
		return LOAD_TEXT;
	case SAVE:
		return SAVE_TEXT;
	case EXIT:
		return EXIT_TEXT;
	default:
		return NULL;
	}
}

static tIntent intent_code(const char* string, const size_t string_len) {
	tIntent i;
	for (i = 0; i < INTENT_COUNT; i++) {
		char* compare_string = intent_string(i);
		if (compare_string == NULL) {
			continue;
		}
		if (compare_token(string, compare_string) == 0) {
			return i;
		}
	}
	return INTENT_UNKNOWN;
}

struct knowledge_item {
	enum intent intent;
	char* key;
	char* value;
	struct knowledge_item* next;
};

typedef struct knowledge_item tKnowledgeItem;

static char* database_string_create(const char* source, const size_t max_source) {
	size_t source_len = strnlen(source, max_source);
	char* string = (char*)malloc(source_len + 1);
	if (string == NULL) {
		return NULL;
	}
	strncpy(string, source, source_len);
	string[source_len] = '\0';
	return string;
}

static void database_string_destroy(char* string) {
	free(string);
}

static tKnowledgeItem* database_create() {
	tKnowledgeItem* head = (tKnowledgeItem*)malloc(sizeof(tKnowledgeItem));
	if (head == NULL) { // This line is not necessary but it makes it clear it can return NULL
		return NULL;
	}
	return head;
}

static void database_insert(tKnowledgeItem* new, tKnowledgeItem* prev) {
	new->next = prev->next;
	prev->next = new;
}

static void database_destroy(tKnowledgeItem* head) {
	tKnowledgeItem* temp = head;
	while (head != NULL) {
		temp = head;
		head = head->next;
		database_string_destroy(temp->key);
		database_string_destroy(temp->value);
		free(temp);
	}
}

static int database_add(tKnowledgeItem* head, const tIntent intent, const char* key, const size_t max_key, const char* value, const size_t max_value) {
	tKnowledgeItem* new = database_create();
	if (new == NULL) {
		return KB_NOMEM;
	}
	new->intent = intent;
	new->key = database_string_create(key, max_key);
	new->value = database_string_create(value, max_value);
	if (new->value == NULL || new->value == NULL) {
		return KB_NOMEM;
	}
	database_insert(new, head);
	return 0;
}

/*
 * USAGE:
 *
 * knowledge_item_t *next = database_head_from_database_create;
 * database_foreach(next) {
 *   printf("%i %10s: %10s\n", next->intent, next->key, next->value);
 * }
 */
#define database_foreach(cur) while ((cur = cur->next) != NULL)

#endif
