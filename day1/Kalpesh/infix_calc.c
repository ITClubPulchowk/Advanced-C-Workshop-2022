#include <stdio.h>
#include <stdlib.h>

typedef struct Node{
    union{
        float data;
        int operator;
    };
    int NodeType;
    struct Node * left, *right;
}Node;

typedef struct Tree{
    Node * root;
    int height;
}Tree;

int isOperator(char c){
    return((c == '+' || c=='-'|| c=='*'||c=='/'));
}

int isNumber(char c){
    return((c >= '0')&&(c<='9'));
}

int getIntFromBuffer(char * buffer, int *index){
    while (!isNumber(buffer[*index]) && buffer[*index]==' '){
        (*index)++;
    }
    int a = 0;
    while (isNumber(buffer[*index])){
        a = a*10 + (buffer[*index]-'0');
        (*index)++;
    }
    return(a);
}

// lmao 
int precedence(char operator){
    if (operator == '+')
        return(0);
    if (operator == '-')
        return(1);
    if (operator == '*')
        return(2);
    if (operator == '/')
        return(3);
    return(-1);
}



void TreeDump(Node * start){
    if (start){
        if (start->NodeType == 1)
            printf("%c\t", start->operator);
        else {
            printf("%f\t", start->data);
        }
        TreeDump(start->left);
        TreeDump(start->right);
    }
}

Node * searchForParent(Node *root, Node *child){
    if (root){
        if(root->left && root->left == child){
            return(root);
        }
        else if (root->right && root->right == child){
            return(root);
        }
        else{
            Node * a = searchForParent(root->left, child);
            if(a){
                return(a);
            }
            a = searchForParent(root->right, child);
            if(a){
                return(a);
            }
        }
    }
    return(NULL);
} 

void deleteNode(Node * node){
    free(node);
}

void deleteTree(Node * root){
    if (root->left){
        deleteTree(root->left);
    }
    if (root->right){
        deleteTree(root->right);
    }
    deleteNode(root);
}


void evaluate(Node * root){
    if (root && root->NodeType == 1){
        if (root->left->NodeType == 1){
            evaluate(root->left);
        }
        if (root->right->NodeType == 1){
            evaluate(root->right);
        }
        if (root->left->NodeType == 0 && root->right->NodeType == 0){
            switch (root->operator){
            case '+':
                root->data = root->left->data + root->right->data; 
                break;
            case '-':
                root->data = root->left->data - root->right->data; 
                break;
            case '*':
                root->data = root->left->data * root->right->data; 
                break;
            case '/':
                root->data = root->left->data / root->right->data; 
                break;
            default:
                break;
            }
            root->NodeType = 0;
        }
        
    }
}




void main(){
    Tree t;
    t.root = NULL;
    t.height = 0;
    // no input file :stab:
    char * buffer = "1-6*9+5/9-1+2\n12/2+6*4-2";
    int index = 0;
    Node * prev = NULL;
    Node * a = NULL;
    FILE * f = NULL;
    fopen_s(&f, "infix_output.txt", "w");
    while (buffer[index] != 0){
        if (!t.root){
            a = (Node *)malloc(sizeof(Node));
            a->data = getIntFromBuffer(buffer, &index);
            a->left = NULL;
            a->right = NULL;
            a->NodeType = 0;
            prev = NULL;
        }

        Node * current = NULL;
    
        // read operator
        current = (Node *)malloc(sizeof(Node));
        current->operator = buffer[index];
        current->NodeType = 1;
        current->left = NULL;
        index++;

        // read second operand
        Node * b = (Node *)malloc(sizeof(Node));
        b->data = getIntFromBuffer(buffer, &index);
        b->left = NULL;
        b->right = NULL;
        b->NodeType = 0;
        current->right = b;

        // first tree edge
        if (!t.root){
            current->left = a;
            t.root = current;
        }
        // check precedence with previous operators
        if (prev){
            // if current has more precedence, add as a child to prev
            if (precedence(current->operator) > precedence(prev->operator)){
                current->left = prev->right;
                prev->right = current;
            }
            else{
                Node *parent = prev;
                // if current has less precedence, add as parent to parent
                while (parent && (precedence(current->operator) <= precedence(parent->operator))){       
                    if (current->left)
                        parent->right = current->left;
                    current->left = parent;
                    if (t.root == parent){
                        t.root = current;
                        break;
                    }
                    else{
                        Node * gp = searchForParent(t.root, parent);
                        // if grandparent exists, replace pointer to parent with current
                        if (gp){
                            if(gp->left == parent){
                                gp->left = current;
                            }
                            else{
                                gp->right = current;
                            }
                        }
                        parent = gp;
                    }
                }
            }
        }
        prev = current;
        while (!isOperator(buffer[index])){
            if (buffer[index] == '\n' || buffer[index] == 0){
                if (t.root){
                    evaluate(t.root);
                    printf("%f\n", t.root->data);
                    fprintf(f,"%f\n", t.root->data);
                    deleteTree(t.root);
                    t.root = NULL;
                }
                if (buffer[index])
                    index++;
                break;
            }
            index++;
        }
    }

    fclose(f);
    // free(buffer);
}
