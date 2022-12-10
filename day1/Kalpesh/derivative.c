#include <stdio.h>
#include <stdlib.h>

typedef struct Term{
	int coeff;
	int pow;
}Term;

typedef struct Node{
	Term term;
	struct Node *next;
}Node;


typedef struct LinkedList{
	Node * head;
}LinkedList;

void initList(LinkedList *l){
	l->head = NULL;
}

void applyPowerRule(Term *t){
	t->coeff *= t->pow;
	t->pow--;
}

Node * newNode(Term t){
	Node *a = (Node *)malloc(sizeof(Node));
	a->term = t;
	a->next = NULL;
	return(a);
}

void deleteNode(Node *node){
	free(node);
}

Node * getNodeFromList(LinkedList *l, int pos){
	Node * a = l->head;
	for(int i=0; i<pos; i++){
		a = a->next;
	}
	return(a);
}

void insertToList(LinkedList *list, Node* node){
	if (!list->head){
		list->head = node;
	}
	else{
		Node * a = list->head;
		Node * prev = NULL;
		while (a){
			if (node->term.pow > a->term.pow){
				node->next = a;
				if (a == list->head) {
					list->head = node;
				}
				else {
					prev->next = node;
				}
				return;
			}
			else if (node->term.pow == a->term.pow){
				a->term.coeff += node->term.coeff;
				deleteNode(node);
				return;
			}
			else{
				prev = a;
				a = a->next;
			}
		}
		prev->next = node;
	}
}

void emptyList(LinkedList * l){
	Node * a = l->head;
	Node * b = NULL; 
	while (a){
		b = a->next;
		free(a);
		a = b;
	}
	l->head = NULL;
}

int isOperator(char c){
	return((c == '+' || c=='-'|| c=='*'||c=='/'));
}

int isNumber(char c){
	return((c >= '0')&&(c<='9'));
}

int isDelimiter(char c) {
	return((c == 0) || (c == '\n'));
}

int getIntFromBuffer(char * buffer, int *index, char delimit){
	int multiplier = 1;
	int a = 0;
	while (!isNumber(buffer[*index])){
		if (buffer[*index] == '-')
			multiplier = -1;
		if (buffer[*index] == delimit){
			a = 1;
			break;
		}
		(*index)++;
	}
	
	while (isNumber(buffer[*index])){
		a = a*10 + (buffer[*index]-'0');
		(*index)++;
	}
	return(multiplier * a);
}

char * loadFileToBuffer(const char * filePath){
	FILE * f = NULL;
	fopen_s(&f, filePath, "rb");
	if (!f){
		printf("No file");
		return(0);
	}
	fseek(f, 0, SEEK_END);
	int size = ftell(f);
	fseek(f, 0, SEEK_SET);
	char * buffer = (char *)malloc((size + 1) * sizeof(char));

	fread(buffer, 1, size, f);
	buffer[size] = 0;

	fclose(f);
	return(buffer);
}


Term parseTerm(char * buffer, int * index){
	Term t;
		
	t.coeff = getIntFromBuffer(buffer, index, 'x');

	while (buffer[*index] != 'x'){
		(*index)++;
		if (isOperator (buffer[*index]) || isDelimiter(buffer[*index])){
			t.pow = 0;
			return(t);
		}
	}
	while (!isNumber(buffer[*index])){
		(*index)++;
		if (isOperator (buffer[*index]) || isDelimiter(buffer[*index])){
			t.pow = 1;
			return(t);
		}
	}

	t.pow = getIntFromBuffer(buffer, index, ' ');
	if (buffer[*index] == '\r')
		(*index)++;
	return(t);
}


void computeDerivative(LinkedList *l){
	Node *term = l->head;
	while (term){
		applyPowerRule(&term->term);
		term = term->next;
	}
}

void writeToFile(Term term, FILE *f, int sign){
	if (sign)
		fprintf(f, (term.coeff <0)?" ":" + ");
	if (term.coeff == 0){
		fprintf(f, "0");
	}
	else if (term.pow == 0){
		fprintf(f,"%d", term.coeff);
	}
	else if (term.pow == 1){
		fprintf(f,"%dx", term.coeff);
	}
	else{
		fprintf(f, "%dx^%d", term.coeff, term.pow);
	}
}

void main(){
	LinkedList l;
	l.head = NULL;
	char * buffer = loadFileToBuffer("polynomial_derivative.csv");
	int index = 0;
	FILE * f = NULL;
	fopen_s(&f, "polynomial_derivative_out.txt", "w");
	while (buffer[index]){
		Node * term = newNode(parseTerm(buffer, &index));
		insertToList(&l, term);
		if (buffer[index] == '\n' || buffer[index] == 0){
			computeDerivative(&l);
			writeToFile(l.head->term, f, 0);
			Node* a = l.head->next;
			while (a) {
				writeToFile(a->term, f, 1);
				a = a->next;
			}
			fprintf(f, "\n");
			emptyList(&l);
			if (buffer[index] == 0)
				break;
			index++;
		}
	}
	fclose(f);
	if (buffer)
		free(buffer);
}