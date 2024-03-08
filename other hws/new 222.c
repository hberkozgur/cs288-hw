struct node * MoveNode2End(struct node * list, struct node * node_to_move){

node curr = list;

while(curr->next!=NULL && curr->next != node_to_move){

curr=curr->next;

}

if(curr->next==NULL){

printerr("No node found in the list");

}

curr->next = curr->next->next; //change the node the points to node_to_move to the next one after that

while(curr->next!=NULL){

curr=curr->next;

}

curr->next = node_to_move;

node_to_move -> next = NULL;

return list;

}