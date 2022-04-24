#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {

    TreeMap *tree = (TreeMap*)malloc(sizeof(TreeMap));
    tree->root = NULL;
    tree->current = NULL;
    tree->lower_than = lower_than;
    return tree;

}


void insertTreeMap(TreeMap * tree, void* key, void * value) 
{
    if (tree == NULL) return;

    TreeNode * aux = tree->root;
    TreeNode * parent = NULL;

    while ( aux != NULL )
    {
        parent = aux;
        
        if (tree->lower_than(key, aux->key))
        {
            aux = aux->left;
        }
        else if(tree->lower_than(aux->key,key))
        {
            aux = aux->right;
        }
        else return;
    }

    TreeMap* new = createTreeNode(key, value)
    
    if(new->parent == NULL)
    {
        tree->root = new;
    
    }else if(tree->lower_than(key, new->parent->pair->key))
    {
        new->parent->left= new;
    }
    else
    {
        new->parent->right = new;
    }
    tree->current = new;

}

TreeNode * minimum(TreeNode * x){
    TreeNode *aux = x;
    while (aux != NULL)
    {
      if (aux->left == NULL)
        {
          return aux;
        }
      aux = aux->left;
    }

    return NULL;
}


void removeNode(TreeMap * tree, TreeNode* node) {

}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode * aux = tree->root;
    while (aux != NULL)
    {
      if (tree->lower_than(key, aux->pair->key) == 1)
      {
         aux = aux->left ;
      }
      else if(tree->lower_than(aux->pair->key , key) == 1)
      {
        aux = aux->right ;
      }
      else
      {
        tree->current = aux;
        return aux->pair;
      }
    }
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {


    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root;
    return (minimum(aux)->pair);
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->current;
    
    if (aux->right != NULL)
    {
        aux = aux->right;
        tree->current = aux;
        return minimum(aux)->pair;
    }

    else if (aux->right == NULL)
    {
        while (aux->parent != NULL)
        {

            if (tree->lower_than(tree->current->pair->key, aux->parent->pair->key) == 1)
            {
                //aux = aux->parent;
                tree->current = aux->parent;
                return aux->parent->pair;
            }
             
            else if ((tree->lower_than(aux->parent->pair->key,tree->current->pair->key) == 1))
            {
                aux = aux->parent;
            }
            else if ((is_equal(tree,aux->pair->key, aux->parent->pair->key ) == 1))
            {
                aux= aux->parent;
            }
            
        }
        
    }
    return NULL;
}
