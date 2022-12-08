#include "ProductBST.h"

Node* recursive_add(Node *root, Product product);
Node* handle_line_and_add(Node *root, char *line);
Node* min_value_node(Node *root);

Node* recursive_add(Node *root, Product product){
    if (root == NULL){
        root->product = product;
        return root;
    }
    if (strcmp(product.name,root->product.name) > 0){
        return recursive_add(root->right_child,product);
    }
    if (strcmp(product.name,root->product.name) < 0){
        return recursive_add(root->left_child,product);
    }
    return root;
}

Node *add_product (Node *root, char *name, int quantity){
    if (root == NULL){
        fprintf(stderr, ERROR);
        fprintf(stderr, INVALID_POINTER);
        return NULL;
    }
    if (quantity <= 0){
        fprintf(stderr, ERROR);
        fprintf(stderr, INVALID_QUANTITY);
        return NULL;
    }
//    Product *product = (Product*) calloc(1, sizeof(Product));
//    if (!product){
//        fprintf(stderr, ERROR);
//        fprintf(stderr, ALLOCATION_FAILED);
//        return NULL;
//    }
    root->product.name = name;
    root->product.quantity = quantity;
    return recursive_add(root, root->product);
}

Node *build_bst (const char *filename){
    FILE *fp = fopen(filename, "r");
    if (fp == NULL){
        fprintf(stderr, ERROR);
        fprintf(stderr, FILE_OPEN_FAILED);
        return NULL;
    }
    char *buff = NULL;
    buff = fgets(buff, MAX_LINE_LENGTH, fp);
    buff = strtok(buff,"'\n'");
    Node *root = (Node*) calloc(1, sizeof(Node));
    if (!root){
        fprintf(stderr, ERROR);
        fprintf(stderr, ALLOCATION_FAILED);
        return NULL;
    }
    if (!handle_line_and_add(root, buff)){
        return NULL;
    }
    while (fgets(buff, MAX_LINE_LENGTH, fp)){
        buff = strtok(buff,"'\n'");
        if (!handle_line_and_add(root, buff)){
            return NULL;
        }
    }
    return root;
}

Node* handle_line_and_add(Node *root, char *line){
    char * name = NULL;
    char * quantity_str = NULL;
    int quantity;
    name = strtok(line,":");
    quantity_str = strtok(NULL,":");
    char *  ptr;
    quantity = strtol(quantity_str,&ptr,10);
    if(strcmp(ptr,"")!=0){
        fprintf(stderr, ERROR);
        fprintf(stderr, INVALID_LINE);
        return NULL;
    }
    return add_product(root, name, quantity);
}

Product *search_product (Node *root, char *name){
    if (root == NULL){
        fprintf(stderr, ERROR);
        fprintf(stderr, INVALID_POINTER);
        return NULL;
    }
    if (strcmp(name,root->product.name) == 0){
        return &root->product;
    }
    if (strcmp(name,root->product.name) > 0){
        return search_product(root->right_child, name);
    }
    if (strcmp(name, root->product.name) < 0){
        return search_product(root->left_child, name);
    }
    fprintf(stderr, ERROR);
    fprintf(stderr, PRODUCT_NOT_FOUND);
    return NULL;
}

Node* update_quantity (Node *root, char *name, int amount_to_update){
    Product *product = search_product(root, name);
    if (!product){
        return root;
    }
    int updated_quantity = product->quantity + amount_to_update;
    if (updated_quantity < 0){
        fprintf(stderr, ERROR);
        fprintf(stderr, INVALID_QUANTITY);
        return root;
    }
    if (updated_quantity == 0){
        return delete_product(root, product->name);
    }
    product->quantity = updated_quantity;
    return root;
}

void delete_tree (Node *root){
    if (!root) return;
    delete_tree(root->left_child);
    delete_tree(root->right_child);
    free(root->product.name);
    root->product.name = NULL;
    free(root);
    root = NULL;
}

Node *delete_product (Node *root, char *name){
    if (root == NULL){
        fprintf(stderr, ERROR);
        fprintf(stderr, INVALID_POINTER);
        return NULL;
    }
    if (strcmp(name, root->product.name) > 0){
        root->right_child =
                delete_product(root->right_child, name);
    }
    if (strcmp(name, root->product.name) < 0){
        root->right_child =
                delete_product(root->right_child, name);
    }
    else{
        if (!root->left_child){
            Node *temp = root->right_child;
            free(root->product.name);
            root->product.name = NULL;
            free(root);
            root = NULL;
            return temp;
        }
        if (!root->right_child){
            Node *temp = root->left_child;
            free(root->product.name);
            root->product.name = NULL;
            free(root);
            root = NULL;
            return temp;
        }
        Node *temp = min_value_node(root->right_child);
        root->product = temp->product;
        root->right_child = delete_product(root->right_child, temp->product.name);
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