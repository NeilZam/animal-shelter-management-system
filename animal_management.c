#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#define INPUT_FILE "tournament.txt"
#define MAX_LEN 25
#define NUM_TRAITS 5

/*
    COP 3502C PA5
    This program is written by: Neil Zambrano
*/

typedef struct Cat_s {
    char *name;                 // dynamically allocated cat name
    char *breed;                // dynamically allocated cat breed
    int charm;                  // unique charm score
    int traits[NUM_TRAITS];     // values corresponding to TRAIT_NAMES
} Cat;

typedef struct BSTNode_s {
    Cat *cat;
    struct BSTNode_s *left;
    struct BSTNode_s *right;
    int subtree_size;           // size of this subtree, not height
} BSTNode;


// Global Variable
const char TRAITS[NUM_TRAITS][MAX_LEN] = {
    "friendly", "grumpy", "playful", "lazy", "curious"
};


// Function Prototypes
void myMain(FILE *ifile);
char **filterByTrait(BSTNode *root, int traitIndex, int traitValue, int *resultSize);

// You may add more functions if necessary
BSTNode *createNode(Cat *cat);
Cat *createCat(char name[], char breed[], int charm, int traits[]);
char *copyString(char str[]);
void freeCat(Cat *cat);
void freeTree(BSTNode *root);
int getSize(BSTNode *root);
void updateSize(BSTNode *root);
int countTraits(Cat *cat);
BSTNode *insertNode(BSTNode *root, Cat *cat, int *depth, int currentDepth, int *status);
BSTNode *removeNode(BSTNode *root, char *name);
BSTNode *searchNode(BSTNode *root, char *name);
BSTNode *findSuccessor(BSTNode *node);
BSTNode *kthSmallest(BSTNode *root, int k);
void inorderPrint(BSTNode *root);
void collectTraitMatches(BSTNode *root, int traitIndex, int traitValue, char **arr, int *index);

// BEGIN: DO NOT MODIFY THE MAIN FUNCTION
#ifndef MAIN_FUNCTION
int main(void) {
    // Open the input file for reading.
    // This is the only part of the entire code where the file
    // is going to be opened. You should not have any
    // fopen() function call in your functions. Simply
    // pass this existing FILE pointer when necessary.
    FILE *ifile = fopen(INPUT_FILE, "r");

    if( ifile == NULL ) {
        printf("File Does Not Exist!\n");
        return 1;
    }

    // Calls your own main function and passes the file stream
    myMain(ifile);

    // Close the file
    fclose(ifile);

    return 0;
}
#endif
// END: DO NOT MODIFY THE MAIN FUNCTION





// Function Definitions
void myMain(FILE *ifile) {
    // TODO: Complete this function
    // TODO 1 BEGIN
    BSTNode *root = NULL;
    int n, i;

    fscanf(ifile, "%d", &n);

    for (i = 0; i < n; i++) {
        int q;
        fscanf(ifile, "%d", &q);

        if (q == 1) {
            char name[MAX_LEN+1];
            char breed[MAX_LEN+1];
            int charm;
            int traits[NUM_TRAITS];
            int depth = 0;
            int status = 0;
            Cat *cat;

            fscanf(ifile, "%25s %25s %d %d %d %d %d %d",
                   name, breed, &charm,
                   &traits[0], &traits[1], &traits[2], &traits[3], &traits[4]);

            cat = createCat(name, breed, charm, traits);
            root = insertNode(root, cat, &depth, 0, &status);

            if (status == 1) {
                printf("Insert: %d\n", depth);
            }
            else if (status == 2) {
                printf("Replaced\n");
            }
        }
        else if (q == 2) {
            char name[MAX_LEN+1];
            fscanf(ifile, "%25s", name);
            root = removeNode(root, name);
            printf("Deletion Complete\n");
        }
        else if (q == 3) {
            int k;
            BSTNode *ans;
            fscanf(ifile, "%d", &k);

            if (root == NULL || k <= 0 || k > getSize(root)) {
                printf("NO SMALLEST ELEMENT FOUND\n");
            }
            else {
                ans = kthSmallest(root, k);
                printf("%s %s %d\n", ans->cat->name, ans->cat->breed, ans->cat->charm);
            }
        }
        else if (q == 4) {
            int traitIndex, traitValue;
            int resultSize, j;
            char **matches;
            fscanf(ifile, "%d %d", &traitIndex, &traitValue);
            matches = filterByTrait(root, traitIndex, traitValue, &resultSize);

            if (resultSize == 0) {
                printf("NONE FOUND\n");
            }
            else {
                printf("%s:", TRAITS[traitIndex]);
                for (j = 0; j < resultSize; j++) {
                    printf(" %s", matches[j]);
                    free(matches[j]);
                }
                printf("\n");
                free(matches);
            }
        }
        else if (q == 5) {
            int traitIndex, traitValue;
            int resultSize, j;
            char **matches;

            fscanf(ifile, "%d %d", &traitIndex, &traitValue);

            matches = filterByTrait(root, traitIndex, traitValue, &resultSize);

            if (resultSize == 0) {
                printf("NONE REMOVED\n");
            }
            else {
                for (j = 0; j < resultSize; j++) {
                    root = removeNode(root, matches[j]);
                    free(matches[j]);
                }
                free(matches);
                printf("%d\n", resultSize);
            }
        }
        else if (q == 6) {
            if (root == NULL) {
                printf("EMPTY\n");
            }
            else {
                inorderPrint(root);
            }
        }
    }
    freeTree(root);
    // TODO 1 END
}

char **filterByTrait(BSTNode *root, int traitIndex, int traitValue, int *resultSize) {
    // TODO: Complete this function
    // TODO 2 BEGIN
    int total = getSize(root);
    int index = 0;
    char **arr = NULL;

    *resultSize = 0;

    if (total > 0) {
        arr = (char **)malloc(sizeof(char *) * total);
    }
    collectTraitMatches(root, traitIndex, traitValue, arr, &index);

    *resultSize = index;
    if (index == 0) {
        free(arr);
        return NULL;
    }
    arr = (char **)realloc(arr, sizeof(char *) * index);
    return arr;
    // TODO 2 END
}
char *copyString(char str[]) {
    char *copy = (char *)malloc(strlen(str) + 1);
    strcpy(copy, str);
    return copy;
}

Cat *createCat(char name[], char breed[], int charm, int traits[]) {
    int i;
    Cat *cat = (Cat *)malloc(sizeof(Cat));

    cat->name = copyString(name);
    cat->breed = copyString(breed);
    cat->charm = charm;

    for (i = 0; i < NUM_TRAITS; i++) {
        cat->traits[i] = traits[i];
    }

    return cat;
}
BSTNode *createNode(Cat *cat) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    node->cat = cat;
    node->left = NULL;
    node->right = NULL;
    node->subtree_size = 1;
    return node;
}
void freeCat(Cat *cat) {
    if (cat == NULL) {
        return;
    }
    free(cat->name);
    free(cat->breed);
    free(cat);
}
void freeTree(BSTNode *root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    freeCat(root->cat);
    free(root);
}
int getSize(BSTNode *root) {
    if (root == NULL) {
        return 0;
    }
    return root->subtree_size;
}
void updateSize(BSTNode *root) {
    if (root != NULL) {
        root->subtree_size = getSize(root->left) + getSize(root->right) + 1;
    }
}
int countTraits(Cat *cat) {
    int i, count = 0;

    for (i = 0; i < NUM_TRAITS; i++) {
        if (cat->traits[i] == 1) {
            count++;
        }
    }
    return count;
}
BSTNode *searchNode(BSTNode *root, char *name) {
    if (root == NULL) {
        return NULL;
    }
    if (strcmp(name, root->cat->name) == 0) {
        return root;
    }
    if (strcmp(name, root->cat->name) < 0) {
        return searchNode(root->left, name);
    }
    return searchNode(root->right, name);
}

BSTNode *insertNode(BSTNode *root, Cat *cat, int *depth, int currentDepth, int *status) {
    int cmp;

    if (root == NULL) {
        *depth = currentDepth;
        *status = 1;
        return createNode(cat);
    }
    cmp = strcmp(cat->name, root->cat->name);

    if (cmp < 0) {
        root->left = insertNode(root->left, cat, depth, currentDepth + 1, status);
    }
    else if (cmp > 0) {
        root->right = insertNode(root->right, cat, depth, currentDepth + 1, status);
    }
    else {
        if (countTraits(cat) > countTraits(root->cat)) {
            free(root->cat->name);
            free(root->cat->breed);

            root->cat->name = copyString(cat->name);
            root->cat->breed = copyString(cat->breed);
            root->cat->charm = cat->charm;
            memcpy(root->cat->traits, cat->traits, sizeof(int) * NUM_TRAITS);

            freeCat(cat);
            *status = 2;
        }
        else {
            freeCat(cat);
            *status = 0;
        }
        return root;
    }
    updateSize(root);
    return root;
}
BSTNode *findSuccessor(BSTNode *node) {
    BSTNode *ptr = node->right;

    while (ptr->left != NULL) {
        ptr = ptr->left;
    }
    return ptr;
}
BSTNode *removeNode(BSTNode *root, char *name) {
    int cmp;

    if (root == NULL) {
        return NULL;
    }
    cmp = strcmp(name, root->cat->name);
    if (cmp < 0) {
        root->left = removeNode(root->left, name);
    }
    else if (cmp > 0) {
        root->right = removeNode(root->right, name);
    }
    else {
        if (root->left == NULL && root->right == NULL) {
            freeCat(root->cat);
            free(root);
            return NULL;
        }
        else if (root->left == NULL || root->right == NULL) {
            BSTNode *child = root->left ? root->left : root->right;
            freeCat(root->cat);
            free(root);
            return child;
        }
        else {
            BSTNode *s = findSuccessor(root);
            Cat *tempCat = root->cat;
            root->cat = s->cat;
            s->cat = tempCat;
            root->right = removeNode(root->right, tempCat->name);
        }
    }
    updateSize(root);
    return root;
}
BSTNode *kthSmallest(BSTNode *root, int k) {
    int leftSize;

    if (root == NULL) {
        return NULL;
    }
    leftSize = getSize(root->left);

    if (k == leftSize + 1) {
        return root;
    }
    else if (k <= leftSize) {
        return kthSmallest(root->left, k);
    }
    else {
        return kthSmallest(root->right, k - leftSize - 1);
    }
}
void inorderPrint(BSTNode *root) {
    if (root == NULL) {
        return;
    }
    inorderPrint(root->left);
    printf("%s %d %d\n", root->cat->name, root->cat->charm, root->subtree_size);
    inorderPrint(root->right);
}

void collectTraitMatches(BSTNode *root, int traitIndex, int traitValue, char **arr, int *index) {
    if (root == NULL) {
        return;
    }
    collectTraitMatches(root->left, traitIndex, traitValue, arr, index);
    if (root->cat->traits[traitIndex] == traitValue) {
        arr[*index] = copyString(root->cat->name);
        (*index)++;
    }
    collectTraitMatches(root->right, traitIndex, traitValue, arr, index);
}