/* a possible realization of tree structure in C */
/* tree_7.c */
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tree_7.h"

/* static local data structure */
typedef struct pair {
    Node *parent;
    Node *child;
} Pair;

/* static local helper functions */
static Node *MakeNode(const Item *pi);
static bool ToLeft(const Item *i1, const Item *i2);
static bool ToRight(const Item *i1, const Item *i2);
static void AddNode(Node *new_node, Node *root);
static void InOrder(const Node *root, void (*pfun)(Item *item));
static SeekItem(const Item *pi, const Tree *ptree);
static void DeleteNode(Node **ptr);
static void DeleteAllNodes(Node *ptr);

void InitializeTree(Tree *ptree)
{
    ptree->root = NULL;
    ptree->size = 0;
}

bool TreeIsEmpty(const Tree *ptree)
{
    if (ptree->root == NULL)
        return true;
    else
        return false;
}

bool TreeIsFull(const Tree *ptree)
{
    if (ptree->size >= MAXITEM)
        return true;
    else
        return false;
}

int TreeItemCount(const Tree *ptree)
{
    return ptree->size;
}

bool AddItem(const Item *pi, Tree *ptree)
{
    Node *new_node;
    if (TreeIsFull(ptree))
    {
        fprintf(stderr, "Tree is full.\n");
        return false;
    }
    if (SeekItem(pi, ptree).child != NULL)
    {
        fprintf(stderr, "Attempt to add duplicate item\n");
        return false;
    }
    new_node = MakeNode(pi);
    if (new_node == NULL)
    {
        fprintf(stderr, "Failed to create new node\n");
        return false;
    }
    ptree->size++;
    if (ptree->root == NULL)
        ptree->root = new_node;
    else
        AddNode(new_node, ptree->root);
    return true;
}

bool InTree(const Item *pi, const Tree *ptree)
{
    return (SeekItem(pi, ptree).child == NULL)? false: true;
}

bool DeleteItem(const Item *pi, Tree *ptree)
{
    Pair look;
    look = SeekItem(pi, ptree);
    if (look.child == NULL)
        return false;
        
    if (look.parent == NULL)        // item is in root node
        DeleteNode(ptree->root);
    else if (look.parent->left == look.child)
        DeleteNode(&look.parent->left);
    else
        DeleteNode(&look.parent->right);
    ptree->size--;
    
    return true;
}

void Traverse(const Tree *ptree, void (*pfun)(Item item))
{
    if (ptree != NULL)
        InOrder(ptree->root, pfun);
}

void DeleteAll(Tree *ptree)
{
    if (ptree != NULL)
        DeleteAllNodes(ptree->root);
    ptree->root = NULL;
    ptree->size = 0;
}

* local functions (static functions) */
static void InOrder(const Node *root, void (*pfun)(Item item))
{
    if (root != NULL)
    {
        InOrder(root->left, pfun);
        (*pfun)(root->item);
        InOrder(root->right, pfun);
    }
}

static void DeleteAllNodes(Node *root)
{
    Node *pright;
    if (root != NULL)
    {
        pright = root->right;
        DeleteAllNodes(root->left);                 // inorder traversal
        free(root);                                 // free memory of the nodes
        DeleteAllNodes(pright);
    }
}

static void AddNode(Node *new_node, Node *root)     // add a new node into a tree using Node pointers
{
    if (ToLeft(&new_node->item, &root->item))
    {
        if (root->left == NULL)                     // if the left sub tree is empty then add new_node here
            root->left = new_node;
        else
            AddNode(new_node, root->left);          // recurse
    }
    else if (ToRight(&new_node->item, &root->item))
    {
        if (root->right == NULL)                    // if the left sub tree is empty then add new_node here
            root->right = new_node;
        else
            AddNode(new_node, root->left);          // recurse
    }
    else                                            // goes to this brunch means have duplicate nodes which
    {                                               // should have been avoided in function AddItem()
        fprintf(stderr, "Location error in AddNode()\n");
        exit(1);
    }
}

static bool ToLeft(const Item *i1, const Item *i2)  // this function and ToRight() should be modified according to certain problems
{
    int comp1;
    if ((comp1 = strcmp(i1->petname, i2->petname)) < 0)
        return true;
    else if (comp1 == 0 && strcmp(i1->petkind, i2->petkind) < 0)
        return true;
    else
        return false;   
}

static bool ToRight(const Item *i1, const Item *i2)
{
    int comp2;
    if ((comp2 = strcmp(i1->petname, i2->petname)) > 0)
        return true;
    else if (comp2 == 0 && strcmp(i1->petkind, i2->petkind) > 0)
        return true;
    else
        return false;
}

static Node *MakeNode(const Item *pi)
{
    Node *new_node;
    new_node = (Node *)malloc(sizeof(Node));
    if (new_node != NULL)                           // memory allocation success
    {
        new_node->item = *pi;
        new_node->left = NULL;
        new_node->right = NULL;
    }
    return new_node;
}

static Pair SeekItem(const Item *pi, const Tree *ptree)
{
    Pair look;
    look.parent = NULL;
    look.child = ptree->root;                             // initialize look.chile to tree root node
    if (look.child == NULL)                         // if the tree is an empty tree( root is NULL)?
        return look;                                // return NULL pair if so
    while (look.child != NULL)
    {
        if (ToLeft(pi, &(look.child->item)))        // should go to left sub tree to search item?
        {
            look.parent = look.child;
            look.child = look.child->left;
        }
        else if (ToRight(pi, &(look.child->item)))  // should go to right sub tree to search item?
        {
            look.parent = look.child;
            look.child = look.child->right;
        }
        else                                        // matched
            break;                                  // look.child at this loop is the item's matching node
    }
    return look;
}

static void DeleteNode(Node **ptr)                  
{                                                   
    Node *temp;
    puts((*ptr)->item.petname);
    if ((*ptr)->left == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->right;
        free(temp);
    }
    else if ((*ptr)->right == NULL)
    {
        temp = *ptr;
        *ptr = (*ptr)->left;
        free(temp);
    }
    else
    {
        /* find right sub tree's attaching node */
        for (temp = (*ptr)->left; temp->right != NULL; temp = temp->right)
            continue;
        temp->right = (*ptr)->right;
        temp = (*ptr);
        *ptr = (*ptr)->left;
        free(temp);
    }
}









