#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 4
#define NxN (N * N)
#define TRUE 1
#define FALSE 0

struct node {
    int tiles[N][N];
    int f, g, h;
    short zero_row, zero_column;
    struct node *next;
    struct node *parent;
};

int goal_rows[NxN];
int goal_columns[NxN];
struct node *start, *goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];

void print_a_node(struct node *pnode) {
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%2d ", pnode->tiles[i][j]);
        printf("\n");
    }
    printf("\n");
}
struct node *initialize(int argc, char **argv){
	int i,j,k,index, tile;
	struct node *pnode;

	pnode=(struct node *) malloc(sizeof(struct node));
	index = 1;
	for (j=0;j<N;j++)
		for (k=0;k<N;k++) {
			tile=atoi(argv[index++]);
			pnode->tiles[j][k]=tile;
			if(tile==0) {
				pnode->zero_row=j;
				pnode->zero_column=k;
			}
		}
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	pnode->parent=NULL;
	start=pnode;
	printf("initial state\n");
	print_a_node(start);

	pnode=(struct node *) malloc(sizeof(struct node));
	goal_rows[0]=3;
	goal_columns[0]=3;

	for(index=1; index<NxN; index++){
		j=(index-1)/N;
		k=(index-1)%N;
		goal_rows[index]=j;
		goal_columns[index]=k;
		pnode->tiles[j][k]=index;
	}
	pnode->tiles[N-1][N-1]=0;
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	goal=pnode; 
	printf("goal state\n");
	print_a_node(goal);

	return start;
}

void merge_to_open() {
    //put succ_nodes into open based on open priority
    for(int i = 0; i < N; i++){
        if(succ_nodes[i] == NULL){
            continue;
        }
        //Create a new node to insert, as succ_nodes will soon be cleared for next iteration
        struct node *toInsert = (struct node *) malloc(sizeof(struct node));
        memcpy(toInsert->tiles, succ_nodes[i]->tiles, NxN*sizeof(int));
        toInsert->f = succ_nodes[i]->f;
        toInsert->g = succ_nodes[i]->g;
        toInsert->h = succ_nodes[i]->h;
        toInsert->zero_row = succ_nodes[i]->zero_row;
        toInsert->zero_column = succ_nodes[i]->zero_column;
        toInsert->parent = succ_nodes[i]->parent;

        if (open == NULL)
        {
            open = toInsert;
            continue;
        }

        struct node *temp = open;

        int hasInserted = FALSE;

        while(temp != NULL && temp->next != NULL){
            if(toInsert->f < temp->next->f){
                toInsert->next = temp->next;
                temp->next = toInsert;
                hasInserted = TRUE;
                break;
            }
        temp = temp->next;
        }

        //temp should be either the last node, or the node where toInsert should be inserted
        if(hasInserted == FALSE){
            temp->next = toInsert;
        }

    }

}

/*swap two tiles in a node*/
void swap(int row1,int column1,int row2,int column2, struct node * pnode){
    //swap WITHOUT A TEMP VARIABLE YEAH IM THAT COOL
    pnode->tiles[row1][column1] += pnode->tiles[row2][column2];
    pnode->tiles[row2][column2] = pnode->tiles[row1][column1] - pnode->tiles[row2][column2];
    pnode->tiles[row1][column1] -= pnode->tiles[row2][column2];
}

//used for h2
int manhattanDist(int entry, int row, int col){
    //ignore 0
    if(entry == 0){
        return 0;
    }
    //find entry in goal
    for(int i = 0; i < NxN; i++){
        for(int j = 0; j < NxN; j++){
            if(goal->tiles[i][j] == entry){
                //Goal destination is i, j
                return abs(row - i) + abs(col - j);
            }
        }
    }
}

//used to decide between h1 and h2
int max(int a, int b){
    if(a > b){
        return a;
    }else{
        return b;
    }
}

/*update the f,g,h function values for a node */
void update_fgh(struct node *pnode){
    //g is amount of steps from top
    if(pnode->parent != NULL){
        pnode->g = pnode->parent->g + 1;
    }else{
        pnode->g = 1;
    }

    //h is the max of h1 or h2
    int h1 = 0, h2 = 0;
    int i, j;
    int correct = 0;
    //h1 is number of misplaced tiles, h2 is how far the tile is from desired location
    for(i = 0; i < NxN; i++){
        for(j = 0; i < NxN; i++){
            correct++;
            if(pnode->tiles[j][i] != correct ){
                h1++;
            }
            h2 += manhattanDist(pnode->tiles[j][i], j, i);
        }
    }
    pnode->h = max(h1, h2);

    //f is g + h
    pnode->f = pnode->g + pnode->h;
}



/* 0 goes down by a row */
void move_down(struct node * pnode){
    if(pnode->zero_row+1 < N){
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row+1, pnode->zero_column, pnode);
        pnode->zero_row++;
    }else{
        pnode = NULL;
    }
}

/* 0 goes right by a column */
void move_right(struct node * pnode){
    if(pnode->zero_column+1 < N){
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column+1, pnode);
        pnode->zero_column++;
    }else{
        pnode = NULL;
    }
}

/* 0 goes up by a row */
void move_up(struct node * pnode){
    if(pnode->zero_row-1 > -1){
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row-1, pnode->zero_column, pnode);
        pnode->zero_row--;
    }else{
        pnode = NULL;
    }

}

/* 0 goes left by a column */
void move_left(struct node * pnode){
    if(pnode->zero_column-1 > -1) {
        swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column - 1, pnode);
        pnode->zero_column--;
    }else{
        pnode = NULL;
    }
}

/* expand a node, get its children nodes, and organize the children nodes using
 * array succ_nodes.
 */
void expand(struct node *selected) {
    for(int i = 0; i < N; i++){
        succ_nodes[i] = (struct node *) malloc(sizeof(struct node));
        memcpy(succ_nodes[i]->tiles, selected->tiles, NxN*sizeof(int));
        succ_nodes[i]->zero_row = selected->zero_row;
        succ_nodes[i]->zero_column = selected->zero_column;
        succ_nodes[i]->parent = selected;

    }

    move_down(succ_nodes[0]);
    move_right(succ_nodes[1]);
    move_up(succ_nodes[2]);
    move_left(succ_nodes[3]);

    for(int i = 0; i < N; i++){
        update_fgh(succ_nodes[i]);
    }
}

int nodes_same(struct node *a,struct node *b) {
	int flg=FALSE;
	if (memcmp(a->tiles, b->tiles, sizeof(int)*NxN) == 0)
		flg=TRUE;
	return flg;
}

void filter(int i, struct node *pnode_list){
    if(pnode_list == NULL || succ_nodes[i] == NULL){
        return;
    }
    struct node *temp = pnode_list;
    while(temp != NULL){
        if(nodes_same(succ_nodes[i], temp)){
            succ_nodes[i] = NULL;
            return;
        }
        temp = temp->next;
    }
}
int main(int argc, char **argv) {
    int iter, cnt;
    struct node *copen, *cp, *solution_path;
    int ret, i, pathlen = 0, index[N - 1];

    solution_path = NULL;
    start = initialize(argc, argv);
    open = start;

    iter = 0;
    while (open != NULL) {
        copen = open;
        open = open->next;
        if (nodes_same(copen, goal)) {
            do {
                copen->next = solution_path;
                solution_path = copen;
                copen = copen->parent;
                pathlen++;
            } while (copen != NULL);
            printf("Path (length=%d):\n", pathlen);
            for (; solution_path != NULL;) { // print nodes on the list
                print_a_node(solution_path);
                solution_path = solution_path->next;
            }
            break;
        }
        expand(copen);
        for (i = 0; i < 4; i++) {
            filter(i, open);
            filter(i, closed);
        }
        merge_to_open();
        copen->next = closed;
        closed = copen;
        iter++;
        if (iter % 1000 == 0)
            printf("iter %d\n\n", iter);
        if (iter >= 60000) {
            printf("no solution\n");
            break;
        }
    }

    return 0;
}