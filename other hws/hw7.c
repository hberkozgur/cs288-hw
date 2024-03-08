#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define N 4 
#define NxN (N * N)
#define TRUE 1 
#define FALSE 0
struct state {
    int tiles[N][N];
    int f, g, h;
    short zero_row, zero_column;   
    struct state *next;
    struct state *parent;         
};
int goal_rows[NxN];
int goal_columns[NxN];
struct state *start,*goal;
struct state *fringe = NULL, *closed = NULL;
struct state *succ_states[4];
void print_a_state(struct state *pstate) {
    int i,j;
    for (i=0;i<N;i++) {
        for (j=0;j<N;j++) 
            printf("%2d ", pstate->tiles[i][j]);
        printf("\n");
    }
    printf("\n");
}
struct state *initialize(char **argv){
    int i,j,k,index, tile;
    struct state *pstate;
    pstate=(struct state *) malloc(sizeof(struct state));
    index = 1;
    for (j=0;j<N;j++)
        for (k=0;k<N;k++) {
            tile=atoi(argv[index++]);
            pstate->tiles[j][k]=tile;
            if(tile==0) {
                pstate->zero_row=j;
                pstate->zero_column=k;
            }
        }
    pstate->f=0;
    pstate->g=0;
    pstate->h=0;
    pstate->next=NULL;
    pstate->parent=NULL;
    start=pstate;
    printf("initial state\n");
    print_a_state(start);
    pstate=(struct state *) malloc(sizeof(struct state));
    goal_rows[0]=3;
    goal_columns[0]=3;
    for(index=1; index<pow(N,2); index++){
        j=(index-1)/N;
        k=(index-1)%N;
        goal_rows[index]=j;
        goal_columns[index]=k;
        pstate->tiles[j][k]=index;
    }
    pstate->tiles[N-1][N-1]=0;      
    pstate->f=0;
    pstate->g=0;
    pstate->h=0;
    pstate->next=NULL;
    goal=pstate; 
    printf("goal state\n");
    print_a_state(goal);
    return start;
}

void merge_to_fringe() { 
    int num = 0;
  while (num < 4) {
    struct state * index = fringe, * swappable = fringe, * mainStruct;
    if (succ_states[num] != NULL) {
      mainStruct = ((struct state * ) calloc(1, sizeof(struct state)));
      for (int x = 0; x < N; x++)
        for (int z = 0; z < N; z++) mainStruct -> tiles[x][z] = succ_states[num] -> tiles[x][z];
      mainStruct -> zero_row = succ_states[num] -> zero_row;
      mainStruct -> zero_column = succ_states[num] -> zero_column;
      mainStruct -> parent = succ_states[num] -> parent;
      mainStruct -> f = succ_states[num] -> f;
      mainStruct -> g = succ_states[num] -> g;
      mainStruct -> h = succ_states[num] -> h;
      if (index == NULL) fringe = mainStruct; //if smallest
      else if (mainStruct -> f < index -> f || mainStruct -> f == index -> f) {
        if (mainStruct -> h == index -> h || mainStruct -> h < index -> h) {
          mainStruct -> next = index;
          fringe = mainStruct;
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
void swap(int row1, int column1, int row2, int column2, struct state *pstate) {  
    int x = pstate -> tiles[row1][column1];
    pstate -> tiles[row1][column1] = pstate -> tiles[row2][column2];
    pstate -> tiles[row2][column2] = x;
}

void update_fgh(struct state *pstate) {  
    int cost1 = 0, cost2 = 0, nl = 0, x = 0, l = 0;
    
    if (pstate->parent == NULL) {
        pstate->g = 1;
    } else {
        int temp = pstate->parent->g + 1;
        pstate->g = temp;
    }
    
    while (nl < pow(N, 2)) {
        while (nl < pow(N, 2)) {
            l++;
            if (pstate->tiles[x][nl] != l) {
                cost1++;
            }
            int entry = pstate->tiles[x][nl];
            if (entry != 0) {
                int goal_row = goal_rows[entry];
                int goal_col = goal_columns[entry];
                cost2 += abs(goal_row - x) + abs(goal_col - nl);
            }
            nl++;
        }
        nl++;
    }
    
    if (cost1 > cost2) {
        pstate->h = cost1;
    } else {
        pstate->h = cost2;
    }
    
    pstate->f = pstate->g + pstate->h;
}

void move_down(struct state *pstate){
    int row1= pstate -> zero_row;
    int column1= pstate -> zero_column;
    int row2= pstate -> zero_row + 1;
    int column2= pstate -> zero_column;
        if (N > (pstate -> zero_row + 1)) {
            swap(row1, column1, row2, column2, pstate);
            pstate->zero_row+=1;
        }
        else pstate = NULL;
}
void move_right(struct state *pstate) { 
    int row1= pstate -> zero_row;
    int column1= pstate-> zero_column;
    int row2= pstate -> zero_row;
    int column2= pstate -> zero_column + 1;
        if (N > (pstate -> zero_column + 1)) {
            swap(row1, column1, row2, column2, pstate);
            pstate -> zero_column+=1;
        }
        else pstate = NULL;
}
void move_up(struct state *pstate) { 
    int row1= pstate -> zero_row;
    int column1= pstate -> zero_column;
    int row2= pstate -> zero_row - 1;
    int column2= pstate->zero_column;
        if (-1 < (pstate -> zero_row - 1)) {
            swap(row1, column1, row2, column2, pstate);
            pstate -> zero_row-=1;
        }
        else pstate = NULL;
}
void move_left(struct state *pstate) { 
    int row1= pstate -> zero_row;
    int column1= pstate -> zero_column;
    int row2= pstate -> zero_row;
    int column2= pstate -> zero_column - 1; 
        if (-1 < (pstate -> zero_column - 1)) {
            swap(row1, column1, row2, column2, pstate);
            pstate -> zero_column-=1;
        }
        else pstate = NULL;
}
void expand(struct state *selected) { 
    int j = 0;
    while (j < N) {
        succ_states[j] = ( struct state * )calloc( 1, sizeof(struct state) );
        memcpy(succ_states[j]->tiles, selected->tiles, pow(N,2) * sizeof(int));
        succ_states[j] -> zero_column= selected->zero_column;
        succ_states[j] -> zero_row= selected->zero_row;
        succ_states[j] -> parent= selected; 
        j+=1; 
    }
    move_down(succ_states[0]);
    move_up(succ_states[2]);
    move_left(succ_states[3]);
    move_right(succ_states[1]);
    j =0;
    while (j < N) {update_fgh(succ_states[j]); j++; }
}
int states_same(struct state *a,struct state *b) {
    int flg=FALSE;
    if (memcmp(a->tiles, b->tiles, sizeof(int)*pow(N,2)) == 0)
        flg=TRUE;
    return flg;
}
void filter(int i, struct state *pstate_list) { 
  struct state * statel = pstate_list;
  if (succ_states[i] != NULL) {
    for (;statel != NULL; statel= statel ->next) {
      if (states_same(succ_states[i], statel)) {
        if (statel-> f < succ_states[i] ->f || statel-> f == succ_states[i] ->f) {
          succ_states[i] = NULL;
          break;  }
        else continue;   }
      else continue;   }  }  }
int main(int argc, char **argv) {
    int iter, cnt;
    struct state *cfringe, *cp, *solution_path;
    int ret, i, pathlen = 0, index[N - 1];

    solution_path = NULL;
    start = initialize(argv); 
    fringe = start;

    iter = 0;
    while (fringe != NULL) {     
        cfringe = fringe;
        fringe = fringe->next; 
        if (states_same(cfringe, goal)) { 
            do { 
                cfringe->next = solution_path;
                solution_path = cfringe;
                cfringe = cfringe->parent;
                pathlen++;
            } while (cfringe != NULL);
            printf("Path (lengh=%d):\n", pathlen);
            for (;solution_path!=NULL;) { // print states on list
                print_a_state(solution_path);
                solution_path = solution_path->next;
            }
            break;
        }
        expand(cfringe);
        for (i = 0; i < 4; i++){
            filter(i, fringe);
            filter(i, closed);
        }
        merge_to_fringe(); 
        cfringe->next = closed;
        closed = cfringe;
        iter++;
        if (iter % 1000 == 0)printf("iter %d\n\n", iter);
        if (iter >= 60000) { printf("no solution\n");  break; } }
    return 0;
} 
