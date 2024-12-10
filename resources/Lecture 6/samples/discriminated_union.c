#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <stdlib.h>

typedef union Values {
    int val;
    char id;
} Values;

enum Type {
    TYPE_EOF,
    TYPE_ADD, 
    TYPE_SUBTRACT, 
    TYPE_MULTIPLY,
    TYPE_DIVIDE, 

    TYPE_INTEGER, 
};


typedef struct Token {
    enum Type dtype;
    Values value;
} Token;

typedef struct Tokenizer {
    char *text;
    Token token;
}Tokenizer; 

typedef struct ExpressionTree {
    enum Type dtype;
    Values value; 
    struct ExpressionTree *left;
    struct ExpressionTree *right;
} ExpressionTree;

int priority(enum Type type) {
    switch (type){
        case TYPE_ADD:
            return 0;
            break;
        case TYPE_SUBTRACT:
            return 0;
            break;
        case TYPE_MULTIPLY:
            return 1;
            break;
        case TYPE_DIVIDE:
            return 1;
            break;
        default:
            return 4;
    }
}

ExpressionTree* insert_expr(ExpressionTree *tree, Token token) {
    if (token.dtype == TYPE_INTEGER) {
        if(!tree) {
            tree = (ExpressionTree *)malloc(sizeof(ExpressionTree));
            tree->left = (ExpressionTree *)malloc(sizeof(ExpressionTree));
            tree->left->dtype = token.dtype;
            tree->left->value.val = token.value.val;

            return tree;
        }else if (tree && !tree->right && tree->left) {
            tree->right = (ExpressionTree *)malloc(sizeof(ExpressionTree));
            tree->right->dtype = token.dtype;
            tree->right->value.val = token.value.val;
            
            return tree;
        } else {
            tree->right = insert_expr(tree->right, token);
            return tree;
        }
    }


    if (token.dtype != TYPE_INTEGER) {
        if (tree->left && !tree->right){
            tree->dtype = token.dtype;
            tree->value.id = token.value.id;
            return tree;
        }
    
        if (priority(tree->dtype) >= priority(token.dtype)) {
            ExpressionTree *new_tree = (ExpressionTree *)malloc(sizeof(ExpressionTree));
            new_tree->dtype = token.dtype;
            new_tree->value.id = token.value.id;
            new_tree->left = tree;

            return new_tree;
        } else {
            if (priority(token.dtype) < priority(tree->dtype)) {
                tree->left = tree;
                tree->right = NULL;
            } else {
                tree->right = insert_expr(tree->right, token);
            }

            return tree;
        }
    }

    return NULL;
}


ExpressionTree* insert_subtree(ExpressionTree *tree, ExpressionTree *sub_tree) {
    if (!tree) {
        tree = sub_tree;
    } else if(!tree->left) {
        tree->left = sub_tree;
    } 
    else if(tree->left) {
        tree->right = sub_tree;
    } else if(tree->left && tree->right) {
        tree->right = insert_subtree(tree->right, sub_tree);
    }

    return tree;
}

typedef int bool; 

bool is_whitespace(char c) {
    return (c == ' ') || (c == '\t') || (c == '\n') || (c == '\r');
}

int is_num(char c) {
	return (c >= '0' && c <= '9');
}

Token tokenize(Tokenizer *t) {
    Token token; 
    if (is_whitespace(t->text[0])) {
        t->text++;
    }

    switch (t->text[0]) {
        case '\0':
            token.dtype = TYPE_EOF;
            token.value.id = t->text[0];
            t->text++;
            break;

        case '\n':
            token.dtype = TYPE_EOF;
            token.value.id = t->text[0];
            t->text++;
            break;
        
        case '+':
            token.dtype = TYPE_ADD;
            token.value.id = t->text[0];
            t->text++;
            break;

        case '-':
            token.dtype = TYPE_SUBTRACT;
            token.value.id = t->text[0];
            t->text++;
            break;
        
        case '*':
            token.dtype = TYPE_MULTIPLY;
            token.value.id = t->text[0];
            t->text++;
            break;
        
        case '/':
            token.dtype = TYPE_DIVIDE;
            token.value.id = t->text[0];
            t->text++;
            break;
        
        default: 
            if (is_num(t->text[0])) {
                token.dtype = TYPE_INTEGER;
                const char *start = t->text;
                while(is_num(t->text[0]) || t->text[0] == '.'){
                    t->text++;
                }
                token.value.val = strtoull(start, (char **)&t->text, 10);
            }
    }
    t->token = token;
    return token;
}

bool accept_token(Tokenizer *t, enum Type kind){
    if (t->token.dtype == kind) {
        tokenize(t);
        return 1;
    }
    return 0;
}


bool require_token(Tokenizer *t, enum Type kind){
    bool result = t->token.dtype == kind;
    tokenize(t);
    return result;
}


bool parse_expressions(Tokenizer *t, ExpressionTree **tree) {
    Token token;
    bool is_remaining_expression = t->token.dtype != TYPE_EOF;
    if (!is_remaining_expression) {
        return 1;
    }
    token = t->token;
    if (require_token(t, TYPE_INTEGER)) {
        *tree = insert_expr(*tree, token);

        is_remaining_expression = t->token.dtype != TYPE_EOF;
        token = t->token;
        if (is_remaining_expression) {
            if (accept_token(t, TYPE_ADD) || accept_token(t, TYPE_SUBTRACT) || accept_token(t, TYPE_MULTIPLY) || accept_token(t, TYPE_DIVIDE)) {
                *tree = insert_expr(*tree, token);
            }

            return parse_expressions(t, tree);
        }
    }
}


void print_inorder(ExpressionTree *node) {
    if (node == NULL)
        return;
 
    /* first recur on left child */
    print_inorder(node->left);
 
    if (node->dtype == TYPE_INTEGER) {
        printf("%d\n", node->value.val);
    } else {
        printf("%c\n", node->value.id);
    }
 
    /* now recur on right child */
    print_inorder(node->right);
}

int get_value(ExpressionTree *tree, int value) {
    if (!tree) {
        return value;
    } else if (tree->right && tree->left) {
        switch(tree->value.id) {
            case '+':
                return get_value(tree->right, 0) + get_value(tree->left, 0);
                break;

            case '-':
                return get_value(tree->left, 0) - get_value(tree->right, 0);
                break;

            case '*':
                return get_value(tree->right, 0) * get_value(tree->left, 0);
                break;

            case '/':
                return get_value(tree->left, 0) / get_value(tree->right, 0);
                break;
        }
    } else if (!tree->right && !tree->left) {
        return tree->value.val;
    }
}

int main() {
    const char *str = "9 / 3 * 4";
    bool parsing = 1;
    Tokenizer t;
    ExpressionTree *tree = NULL;
    t.text = str;
    while (parsing) {
        tokenize(&t);
        switch (t.token.dtype) {
            case TYPE_EOF:
                parsing = 0;
                break;
            default:
                parse_expressions(&t, &tree);
                break;
        }
    }

    printf("%d\n", get_value(tree, 0));
}