#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>
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
struct node *start,*goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];
void print_a_node(struct node *pnode) {
    int i,j;
    for (i=0;i<N;i++) {
        for (j=0;j<N;j++) 
            printf("%2d ", pnode->tiles[i][j]);
        printf("\n");
    }
    printf("\n");
}
struct node *initialize(char **argv){
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
    for(index=1; index<pow(N,2); index++){
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
int findAbsolute(int k){
    int z;
    if (k < 0){  z =(-1) * k; return z;  }
    else return k;
}
void merge_to_open() { 
    int num = 0;
  while (num < 4) {
    struct node * index = open, * swappable = open, * mainStruct;
    if (succ_nodes[num] != NULL) {
      mainStruct = ((struct node * ) calloc(1, sizeof(struct node)));
      for (int x = 0; x < N; x++)
        for (int z = 0; z < N; z++) mainStruct -> tiles[x][z] = succ_nodes[num] -> tiles[x][z];
      mainStruct -> zero_row = succ_nodes[num] -> zero_row;
      mainStruct -> zero_column = succ_nodes[num] -> zero_column;
      mainStruct -> parent = succ_nodes[num] -> parent;
      mainStruct -> f = succ_nodes[num] -> f;
      mainStruct -> g = succ_nodes[num] -> g;
      mainStruct -> h = succ_nodes[num] -> h;
      if (index == NULL) open = mainStruct; //if smallest
      else if (mainStruct -> f < index -> f || mainStruct -> f == index -> f) {
        if (mainStruct -> h == index -> h || mainStruct -> h < index -> h) {
          mainStruct -> next = index;
          open = mainStruct;
        } else {
          for (;;) {
            swappable = index;
            index = index -> next;
            if (index == NULL) {
              swappable -> next = mainStruct;
              mainStruct -> next = NULL;
              break;
            }
            if (mainStruct -> h < index -> h || mainStruct -> h == index -> h) {
              mainStruct -> next = index;
              swappable -> next = mainStruct;
              break;  }  }   }
      } else {
        for (;;) { //infinite loop
          swappable = index;
          index = index -> next;
          if (index == NULL) {
            swappable -> next = mainStruct;
            mainStruct -> next = NULL;
            break;
          }
          if (mainStruct -> f < index -> f || mainStruct -> f == index -> f) {
            mainStruct -> next = index;
            swappable -> next = mainStruct;
            break;  }   }   } }
    num++; }   }
void swap(int row1, int column1, int row2, int column2, struct node *pnode) {  
    int x = pnode -> tiles[row1][column1];
    pnode -> tiles[row1][column1] = pnode -> tiles[row2][column2];
    pnode -> tiles[row2][column2] = x;
}
int heurisitcFunction(int entry, int row, int col){ //to find cost2
    int x =0, y =0;
    if (entry == 0) return 0;  //skip 0 case
    while (x < pow(N,2)) {
        while (y < pow(N,2)) {
            if (goal->tiles[x][y] == entry) {
                return findAbsolute(row - x) + findAbsolute(col - y);
                y+=1;  } 
            else y+=1; } 
        x+=1; }  }
void update_fgh(struct node *pnode) {  
    int cost1 = 0, cost2 = 0, nl=0, x=0, l=0;
    if (pnode -> parent == NULL) { //g=steps from ^
        pnode ->g = 1;   }
    else { 
        int temp = pnode->parent->g + 1;
        pnode->g = temp;  }
    while (nl < pow(N,2)) {
        while (nl < pow(N,2)) {
            l++;
            if (pnode -> tiles[x][nl] != l) cost1++;
            int addOn= heurisitcFunction(pnode -> tiles[x][nl], x, nl);
            cost2 += addOn;
            nl++;  }
        nl++;  }
    if (cost1 > cost2) pnode->h = cost1;
    else pnode -> h = cost2;
    pnode -> f = pnode -> g  + pnode -> h;
}
void move_down(struct node *pnode){
    int row1= pnode -> zero_row;
    int column1= pnode -> zero_column;
    int row2= pnode -> zero_row + 1;
    int column2= pnode -> zero_column;
        if (N > (pnode -> zero_row + 1)) {
            swap(row1, column1, row2, column2, pnode);
            pnode->zero_row+=1;
        }
        else pnode = NULL;
}
void move_right(struct node *pnode) { 
    int row1= pnode -> zero_row;
    int column1= pnode-> zero_column;
    int row2= pnode -> zero_row;
    int column2= pnode -> zero_column + 1;
        if (N > (pnode -> zero_column + 1)) {
            swap(row1, column1, row2, column2, pnode);
            pnode -> zero_column+=1;
        }
        else pnode = NULL;
}
void move_up(struct node *pnode) { 
    int row1= pnode -> zero_row;
    int column1= pnode -> zero_column;
    int row2= pnode -> zero_row - 1;
    int column2= pnode->zero_column;
        if (-1 < (pnode -> zero_row - 1)) {
            swap(row1, column1, row2, column2, pnode);
            pnode -> zero_row-=1;
        }
        else pnode = NULL;
}
void move_left(struct node *pnode) { 
    int row1= pnode -> zero_row;
    int column1= pnode -> zero_column;
    int row2= pnode -> zero_row;
    int column2= pnode -> zero_column - 1; 
        if (-1 < (pnode -> zero_column - 1)) {
            swap(row1, column1, row2, column2, pnode);
            pnode -> zero_column-=1;
        }
        else pnode = NULL;
}
void expand(struct node *selected) { 
    int j = 0;
    while (j < N) {
        succ_nodes[j] = ( struct node * )calloc( 1, sizeof(struct node) );
        memcpy(succ_nodes[j]->tiles, selected->tiles, pow(N,2) * sizeof(int));
        succ_nodes[j] -> zero_column= selected->zero_column;
        succ_nodes[j] -> zero_row= selected->zero_row;
        succ_nodes[j] -> parent= selected; 
        j+=1; 
    }
    move_down(succ_nodes[0]);
    move_up(succ_nodes[2]);
    move_left(succ_nodes[3]);
    move_right(succ_nodes[1]);
    j =0;
    while (j < N) {update_fgh(succ_nodes[j]); j++; }
}
int nodes_same(struct node *a,struct node *b) {
    int flg=FALSE;
    if (memcmp(a->tiles, b->tiles, sizeof(int)*pow(N,2)) == 0)
        flg=TRUE;
    return flg;
}
void filter(int i, struct node *pnode_list) { 
  struct node * nodel = pnode_list;
  if (succ_nodes[i] != NULL) {
    for (;nodel != NULL; nodel= nodel ->next) {
      if (nodes_same(succ_nodes[i], nodel)) {
        if (nodel-> f < succ_nodes[i] ->f || nodel-> f == succ_nodes[i] ->f) {
          succ_nodes[i] = NULL;
          break;  }
        else continue;   }
      else continue;   }  }  }
int main(int argc, char **argv) {
    int iter, cnt;
    struct node *copen, *cp, *solution_path;
    int ret, i, pathlen = 0, index[N - 1];

    solution_path = NULL;
    start = initialize(argv); 
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
            printf("Path (lengh=%d):\n", pathlen);
            for (;solution_path!=NULL;) { // print nodes on list
                print_a_node(solution_path);
                solution_path = solution_path->next;
            }
            break;
        }
        expand(copen);
        for (i = 0; i < 4; i++){
            filter(i, open);
            filter(i, closed);
        }
        merge_to_open(); 
        copen->next = closed;
        closed = copen;
        iter++;
        if (iter % 1000 == 0)printf("iter %d\n\n", iter);
        if (iter >= 60000) { printf("no solution\n");  break; } }
    return 0;
} 
