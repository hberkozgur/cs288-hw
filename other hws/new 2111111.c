struct node *remove_repeated_integer(struct node *list){

node curr = list;


node fr;


int num[1000];

if(curr->next == NULL){


printerr("Empty List")}


else{


num[0]=curr->data;


int yes=0;

int nums=1;

 

 

while(curr->next != NULL && curr->next->next != NULL){


for(int i = 0; i<nums; i++){

if(curr->next->data == num[i]){

yes=1;}


}


if(yes){


curr = curr->next

}


else{

nums++;

num[nums]=curr->next->data;

fr = curr;


curr->next = curr->next->next;

curr=curr->next;


free(fr);

}


yes=0;

}


return list;


}