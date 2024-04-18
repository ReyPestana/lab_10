#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26
#define MAX_WORDS 256

// Trie node structure
struct TrieNode
{
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Function to initialize a Trie node
struct TrieNode *createNode()
{
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node)
    {
        node->isEndOfWord = 0;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Function to insert a word into the Trie
void insert(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            current->children[index] = createNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    current->count++;
}

// Function to compute the number of occurrences of a word in the Trie
int numberOfOccurrences(struct TrieNode *root, char *word)
{
    struct TrieNode *current = root;
    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (!current->children[index])
        {
            return 0;
        }
        current = current->children[index];
    }
    return (current && current->isEndOfWord) ? current->count : 0;
}

// Function to deallocate the memory allocated for the Trie
void deallocateTrie(struct TrieNode *root)
{
    if (root)
    {
        for (int i = 0; i < ALPHABET_SIZE; i++)
        {
            if (root->children[i])
            {
                deallocateTrie(root->children[i]);
            }
        }
        free(root);
    }
}

// Function to read words from a file into an array
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Error opening file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    int numWords = 0;
    char buffer[256];

    while (fgets(buffer, sizeof(buffer), file) != NULL && numWords < MAX_WORDS)
    {
        buffer[strcspn(buffer, "\n")] = 0;
        pInWords[numWords] = strdup(buffer);
        numWords++;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];
    int numWords = readDictionary("dictionary.txt", inWords);

    struct TrieNode *root = createNode();
    for (int i = 0; i < numWords; i++)
    {
        insert(root, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);

    return 0;
}
