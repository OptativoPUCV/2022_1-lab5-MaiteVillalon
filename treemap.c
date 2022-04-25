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
    TreeNode * padre = createTreeNode(key,value);

    while ( aux != NULL )
    {
        padre->parent = aux;
        
        if (tree->lower_than(key, aux->pair->key))
        {
            aux = aux->left;
        }
        else if(tree->lower_than(aux->pair->key,key))
        {
            aux = aux->right;
        }
        else return;
    }

    //TreeMap* new = createTreeNode(key, value);
    
    if(padre->parent == NULL)
    {
        tree->root = padre;
    
    }else if(tree->lower_than(key, padre->parent->pair->key))
    {
        padre->parent->left= padre;
    }
    else
    {
        padre->parent->right = padre;
    }
    tree->current = padre;

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


void removeNode(TreeMap * tree, TreeNode* node) 
{
    TreeNode * aux = NULL;
    if (tree == NULL) return;

    if (node->right == NULL && node->left == NULL)
    {
        if (tree->lower_than(node->pair->key, node->parent->pair->key))
        {
            node->parent->left = NULL;
        }
        else if (tree->lower_than(node->parent->pair->key, node->pair->key))
        {
            node->parent->right = NULL;
        }
    }
    else if ((node->right != NULL && node->left == NULL) || (node->right == NULL && node->left != NULL))
    {
        //if ((node->right != NULL && node->left == NULL) || (node->right == NULL && node->left != NULL))  
        if ( node->parent != NULL )
        {
            if ((node->right != NULL && node->left == NULL))
            {
                if (node == node->parent->left)
                {
                    node->parent->left = node->right;
                    node->right->parent = node->parent;
                }
                else if (node == node->parent->right)
                {
                    node->parent->right = node->right;
                    node->right->parent = node->parent;
                }
                
            }
            else if ((node->right == NULL && node->left != NULL))
            {
                if (node == node->parent->left)
                {
                    node->parent->left = node->left;
                    node->left->parent = node->parent;
                }
                else if (node == node->parent->right)
                {
                    node->parent->right = node->left;
                    node->left->parent = node->parent;
                }   
            }
        }
    }
    else 
    {
        if (node->right != NULL && node->left != NULL)
        {
            aux = minimum(node->right);
            node->pair->key= aux->pair->key;
            node->pair->value= aux->pair->value;
            removeNode(tree,aux);
        }
    }




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


Pair * upperBound(TreeMap * tree, void* key) 
{
    TreeNode * aux = tree->root;
    TreeNode *node = NULL;

    //if (node == NULL) return NULL;

    if (searchTreeMap(tree, key) != NULL )
    {
        return searchTreeMap(tree, key);
    }
    else
    {
        while ( aux != NULL)
        {
            if((node == NULL || (tree->lower_than(aux->pair->key, node->pair->key) == 1)) && (tree->lower_than(key, aux->pair->key) == 1))
            {
                node = aux;
            }
            if (tree->lower_than(key,aux->pair->key) == 1)
            {
                aux = aux->left;
            }
            else
            {
                aux= aux->right;
            }
        }
    }
    if (node == NULL) return NULL;
    return node->pair;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root;
    return (minimum(aux)->pair);
    return NULL;
}

Pair * nextTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->current;

    if (tree == NULL) return NULL;
    
    if (aux->right != NULL)
    {
        aux = minimum(tree->current->right);
        tree->current = aux;
        return aux->pair;
    }

    else 
    {
        while (aux->parent != NULL)
        {

            if (aux == NULL) return NULL;
            else if (tree->lower_than(tree->current->pair->key, aux->parent->pair->key) == 1)
            {
                aux = aux->parent;
                tree->current = aux;
                return aux->pair;
            }
             
            else if ((tree->lower_than(aux->parent->pair->key,tree->current->pair->key) == 1))
            {
                aux = aux->parent;
            }
            else 
            {
                return aux->pair;
            }
            //else if ((is_equal(tree,tree->current->pair->key, tree->root->pair->key ))) return tree->current->pair;
            
            
        }
        //if ((is_equal(tree,tree->current->pair->key, tree->root->pair->key ))) return tree->current->pair;
        
        
    }
    if (is_equal(tree,tree->current->pair->key, tree->root->pair->key )) 
    {
        return tree->current->pair;
    }
    return NULL;
}
