#include "ProductBST.h"


#define BASE 10

/***
 * printing the error with its initial, aka "Error: "
 * @param type pointer to C-string of the specific type of the error.
 */
void print_error(char* type);

/***
 * recursive function to traverse the BST till the correct branch to
 * be inserted. if product already exist prints an error massage.
 * @param root pointer to the root of the BST
 * @param name pointer to the name of the product
 * @param q quantity of the product in stock
 * @return the root after insertion
 */
Node* recursive_add(Node *root, char *name,int q);

/***
 * parsing the crud line written in the file.
 * if line continue after quantity prints an error and return NULL
 * @param root pointer to pointer to the root of the BST
 * @param line a C string of all line, without its next line mark.
 * @return Node of the root of the BST after insertion,
 * NULL in invalid case as specified
 */
Node* handle_line_and_add(Node **root, char *line);

/***
 * traverse the BST within its left sides, to find the minimum value.
 * @param root pointer to the root of the BST
 * @return the left leaf, aka the minimum node in the tree.
 */
Node* min_value_node(Node *root);

void print_error(char* type){
    fprintf(stderr,"%s%s",ERROR,type);
}

Node* init_node(char* name, int qu){
    Node * node = (Node*) malloc(sizeof (Node));
    if(!node){
        print_error(ALLOCATION_FAILED);
        return NULL;
    }
//    Product * product = (Product*) malloc(sizeof (Product));
//    if (!product){
//        print_error(ALLOCATION_FAILED);
//        free(node);
//        return NULL;
//    }
    node->product.name = (char *) malloc(strlen(name)+1);
    if(!node->product.name){
        free(node);
        print_error(ALLOCATION_FAILED);
        return NULL;
    }
//    node->product = product;
    strcpy(node->product.name,name);
    node->product.quantity = qu;
    node->right_child = NULL;
    node->left_child = NULL;
    return node;
}


Node* recursive_add(Node *root, char * name,int quantity){
    if (root == NULL){
        root = init_node(name,quantity);
        return root;
    }
    if (strcmp(name,root->product.name) > 0){
        root->right_child =  recursive_add(root->right_child,name,quantity);
    }
    else if (strcmp(name,root->product.name) < 0){
        root->left_child =  recursive_add(root->left_child,name,quantity);
    }else{
        print_error(PRODUCT_EXISTS);
    }
    return root;
}

Node *add_product (Node *root, char *name, int quantity){
    if (quantity <= 0){
        print_error(INVALID_QUANTITY);
        return NULL;
    }
    if (root == NULL){
        root = init_node(name,quantity);
        return root;
    }
    root = recursive_add(root, name,quantity);
    return root;
}

Node *build_bst (const char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, ERROR);
        fprintf(stderr, FILE_OPEN_FAILED);
        return NULL;
    }
    Node * root = NULL;
    char buff[MAX_LINE_LENGTH];
    char* new_line;
    while (fgets(buff, MAX_LINE_LENGTH, fp)){
        new_line = strtok(buff,"'\n'");
        if (!handle_line_and_add(&root, new_line)){
            return NULL;
        }
    }
    return root;
}

Node* handle_line_and_add(Node **root, char *line){
    char * name = NULL;
    char * quantity_str = NULL;
    int quantity;
    name = strtok(line,":");
    quantity_str = strtok(NULL,":");
    char *  ptr;
    quantity = strtol(quantity_str,&ptr,BASE);
    if(strcmp(ptr,"")!=0){
        print_error(INVALID_LINE);
        return NULL;
    }
    *root =  add_product(*root, name, quantity);
    return *root;
}

Product *search_product (Node *root, char *name){
    if (root == NULL){
        return NULL;
    }
    if (strcmp(name,root->product.name) == 0){
        return &root->product;
    }
    if (strcmp(name,root->product.name) > 0 && root->right_child){
        return search_product(root->right_child, name);
    }
    if (strcmp(name, root->product.name) < 0 && root->left_child){
        return search_product(root->left_child, name);
    }
    print_error(PRODUCT_NOT_FOUND);
    return NULL;
}

Node* update_quantity (Node *root, char *name, int amount_to_update){
    if(!root){
        print_error(INVALID_POINTER);
        return NULL;
    }
    Product *product = search_product(root, name);
    if (!product){
        return root;
    }
    int updated_quantity = product->quantity + amount_to_update;
    if (updated_quantity < 0){
        print_error(INVALID_QUANTITY);
        return root;
    }
    if (updated_quantity == 0){
        return delete_product(root, product->name);
    }
    product->quantity = updated_quantity;
    return root;
}

void delete_tree (Node *root){
    if (!root) {
        return;
    }
    delete_tree(root->left_child);
    delete_tree(root->right_child);
    free(root->product.name);
    root->product.name =NULL;
//    free(root->product);
//    root->product = NULL;
    free(root);
    root = NULL;
}

Node *delete_product (Node *root, char *name){
    if (root == NULL){
        return root;
    }
    if (strcmp(name, root->product.name) > 0){
        root->right_child =
                delete_product(root->right_child, name);
    }
    else if (strcmp(name, root->product.name) < 0){
        root->left_child =
                delete_product(root->left_child, name);
    }
    else{
        if (!root->left_child){
            Node *temp = root->right_child;
            free(root->product.name);
            root->product.name = NULL;
//            free(root->product);
//            root->product = NULL;
            free(root);
            root = NULL;
            return temp;
        }
        else if (!root->right_child){
            Node *temp = root->left_child;
            free(root->product.name);
            root->product.name = NULL;
//            free(root->product);
//            root->product = NULL;
            free(root);
            root = NULL;
            return temp;
        }
        Node *temp = min_value_node(root->right_child);
        strcpy(root->product.name,temp->product.name);
        root->product.quantity = temp->product.quantity;
        root->right_child = delete_product(
                root->right_child, temp->product.name);
    }
    return root;
}

Node* min_value_node(Node *root){
    Node *cur = root;
    while (cur && cur->left_child){
        cur = cur->left_child;
    }
    return cur;
}