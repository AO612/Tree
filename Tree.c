// clang -o tree tree.c

// Usage: Enter ./tree list or ./tree tree

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define length 10  // This is the length of the places array.

struct node // This sets up the struct that will act as a node.
{
  char name[10]; // This will contain a user's name
  int places[ length ]; // This will contain an array of integers representing locations
  struct node *next; // This will point to the next node in the list
};

struct binarynode // This sets up the struct that will act as a binarynode.
{
  char name[10]; // This will contain a user's name
  int places[ length ]; // This will contain an array of integers representing locations
  struct binarynode *left;  // This will point to the node on the left
  struct binarynode *right; // This will point to the node on the right
  struct binarynode *prev;  // This will point to the previous node
};

// This sets up global pointers
struct node *head=0;
struct node *temp;
struct node *temp2;
struct node *temp3;
struct node *ptr;
struct binarynode *root=0;
struct binarynode *btemp;
struct binarynode *btemp2;
struct binarynode *btemp3;
struct binarynode *bptr;
FILE *fp; //This is a pointer that can deal with the file. 

// START OF FUNCTIONS FOR LINKED LIST

void insert_user(char *input) //This function inserts a user into the linked list
{
  if(head==NULL) //This is the first node
  {
    head = (struct node*) malloc(sizeof(struct node));  // This allocates space for the new node
    strcpy(head->name,input); //This copies the inputted name to the node
    for ( int j = 0 ; j < length ; j ++ )
    {
      head->places[j] = 0; // This makes every integer in the array equal to 0
    }
    head->next=NULL;
  }

  else //There are already nodes in the list
  {
    temp = (struct node*) malloc(sizeof(struct node)); // This does the same as before but assigns these values to a temporary node for now instead of the head.
    strcpy(temp->name,input);
    // The string compare functions will determine whether the new node is alphabetically before or after the nodes already in the list.
    if(strcmp(temp->name,head->name) < 0)
    {
      temp->next=head; // This inserts from the front if the name of the new node comes first alphabetically.
      head=temp;
    }

    else if(head->next == NULL)
    {
      head->next=temp; // This inserts at the end when there is only one other node infront.
      temp->next=NULL;
    }

    else
    {
      temp2 = head;
      temp3 = head->next;
      while( temp3->next != NULL && strcmp(temp->name,temp3->name) > 0)
      {
        temp2=temp2->next;
        temp3=temp3->next;
      }
      // This iterates along the list until temp2 and temp3 point to the nodes that the new node will go inbetween. The name of the new node will be alphabetically between the names of those two nodes. It might also just iterate along the list until it reaches the end incase the name of the new node is alphabetically after all of the names in the list.

      if(strcmp(temp->name,temp3->name) < 0)
      {
        temp2->next = temp;
        temp->next  = temp3;
      }
      // This adds the new node inbetween two nodes

      else if(temp3->next == NULL)
      {
        temp3->next=temp;
        temp->next=NULL;
      }        
      // In this case the name of the new node is alphabetically after all of the names in the list and so this adds the new node at the end of the list.
    }
  }
}

void insert_visit(char *input, int location) // This function adds a site visit to the linked list
{
  temp=head;
  while( strcmp(temp->name,input)!=0 )
  {
    temp=temp->next;
  }
  //This iterates along the list until it finds the node that shares a name with the inputted name.
  int j=0;
  while(temp->places[j] !=0) //This finds an index in the array that isn't already storing a location number.
  {
    j++;  
  }
  temp->places[j] = location; //This stores the location number in the array.
}

void check_user(int location) //This function checks which users have been to a certain location in the linked list
{
  int check = 0; // This is used for the case where no one has been to a location.
  temp=head;
  printf("\n");
  while(temp != 0 )
  {
    int j = 0;
    while(temp->places[j] != location && j <= length )
    {
      j++;
    }
    //This goes through each array until it finds the location integer or it reaches the end.
    if ( temp->places[j] == location)
    {
      printf("%s has been to %d.\n", temp->name, location);
      check = 1;
    } 
    // If the array contained the location integer it prints the name of user associated with that array.
    temp=temp->next;
  }
  if( check == 0)
  {
    printf("No one has been to %d.\n", location);
    //This only prints if no one has been to the location         
  }
  printf("\n");
}

void remove_user(char *input) //This function removes a user in the linked list
{
  temp = head;
  if(strcmp(temp->name,input) == 0) // This if statement is true when the first node in the list is the node that needs to be removed.
  {
    if(temp->next != NULL) // This is used when there are nodes following the first node and the first node needs to be removed.
    {
      head=temp->next;
      free(temp);
    }
    else // This is used when there is only one node and that node needs to be removed.
    {
      head=NULL;
      free(temp);
    }
  }
  else // If the node that needs to be removed is not first in the list:
  {
    temp2=temp->next;
    if(strcmp(temp2->name,input) == 0 && temp2->next == NULL) //This is used when there are only two nodes in the list and the second node needs to be removed.
    {
      temp->next=NULL;
      free(temp2);
    }
    else // If the linked list has three or more nodes;
    {
      while( strcmp(temp2->name,input)!=0 )
      {
        temp=temp->next;
        temp2=temp2->next;
      }
      //Iterate along the list until the two temp pointers are on either side of the node that needs to be removed. That node is then removed.
      temp->next=temp2->next;
      free(temp2);
    }
  }
}

void printlist() // This function just prints the current state of the linked list including names in order and the integers in the location arrays.
{
  printf("\n");
  temp=head;
  while(temp != NULL )
  {
    printf("%s\n",temp->name);
    // Print the name
    for ( int j = 0 ; j < length ; j ++ )
    {
      printf("%d ",temp->places[j]);
    }
    // Print the location array
    printf("\n");
    temp=temp->next;
    // Move to next node
  }
  printf("\n");
}

void save_to_file() // This function saves the linked list
{
  fp = fopen("list","wb"); // This opens the file 'list'
  temp=head;
  if(fp!=NULL)
  {
    while(temp != 0 )
    {
      fwrite(temp,sizeof(struct node),1,fp); // This saves each node to the file
      temp=temp->next;
    }
    fclose(fp); // This closes the file
  }
}


void read_from_file() // This function retrives the linked list from the file
{
  int result; // This integer checks if there is information to read in the file
  ptr = (struct node*) malloc(sizeof(struct node));
  fp = fopen("list","rb"); // This opens the list
  if(fp!=NULL)
  {
    result=fread(ptr, sizeof(struct node),1,fp);
    while(result!=0)
    {
      insert_user(ptr->name);
      int i=0;
      while(ptr->places[i]!=0)
      {
        insert_visit(ptr->name, ptr->places[i]);
        i++;
      }
      // This inserts each piece of information from the file into a linked list
      result=fread(ptr,sizeof(struct node),1,fp);
      // This reads the next piece of information in the file
    }
  }  
  fclose(fp); // This closes the file
  printlist();
}

// END OF FUNCTIONS FOR LINKED LIST

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// START OF FUNCTIONS FOR BINARY TREE

void binaryinsertuser(struct binarynode *btemp, char *input)
// This function inserts a user into the list
// Input is the name of the user, btemp is used for recursion
{
  if(root==NULL)
  // This creates the root.
  {
    root = (struct binarynode*) malloc(sizeof(struct binarynode));  // This allocates space for the new node
    strcpy(root->name,input); //This copies the inputted name to the node
    for ( int j = 0 ; j < length ; j ++ )
    {
      root->places[j] = 0; // This makes every integer in the array equal to 0
    }
    root->left = NULL;
    root->right = NULL;
    root->prev = root;
  }
  else if ( strcmp( input,btemp->name) <= 0 )
  // There is already a root.
  {
    if ( btemp->left != NULL)
    {
      binaryinsertuser( btemp->left , input);
    }
    // If the name being inserted comes before alphabetically then it goes down the left branch
    else
    {
      btemp2 = (struct binarynode*) malloc(sizeof(struct binarynode));  // This allocates space for the new node
      strcpy(btemp2->name,input); //This copies the inputted name to the node
      for ( int j = 0 ; j < length ; j ++ )
      {
        btemp2->places[j] = 0; // This makes every integer in the array equal to 0
      }
      btemp->left=btemp2;
      btemp2->left = NULL;
      btemp2->right = NULL;
      btemp2->prev = btemp;
    }
    // When it finds reaches the bottom of the tree the new node is inserted
  }
  else 
  {
    if ( btemp->right != NULL)
    {
      binaryinsertuser( btemp->right , input);
    }
    // If the name being inserted comes after alphabetically then it goes down the right branch
    else
    {
      btemp2 = (struct binarynode*) malloc(sizeof(struct binarynode));  // This allocates space for the new node
      strcpy(btemp2->name,input); //This copies the inputted name to the node
      for ( int j = 0 ; j < length ; j ++ )
      {
        btemp2->places[j] = 0; // This makes every integer in the array equal to 0
      }
      btemp->right=btemp2;
      btemp2->left = NULL;
      btemp2->right = NULL;
      btemp2->prev = btemp;
    }
    // When it finds reaches the bottom of the tree the new node is inserted
  }
}

void binaryinsertlocation(char *input,int location, struct binarynode *btemp)
// This functions adds a site visit to a user
// Input is the name of the user, location is the site integer and btemp is used for recursion
{
  if ( strcmp( input,btemp->name) == 0 )
  // This condition is satisfied when the correct node with the right name has been found
  {
    int j=0;
    while(btemp->places[j] !=0) //This finds an index in the array that isn't already storing a location number.
    {
      j++;  
    }
    btemp->places[j] = location; // This adds the location to the array
  }
  if ( strcmp( input,btemp->name) < 0 )
  {
    binaryinsertlocation(input,location,btemp->left);
  }
  if ( strcmp( input,btemp->name) > 0 )
  {
    binaryinsertlocation(input,location,btemp->right);
  }
  // These two if statements iterate down the binary tree until they find the node with the right name
}

int binarychecklocation(int location,struct binarynode *btemp, int binarycheck)
// This function finds all the users who have been to a location
// Location is the site integer and btemp is used for recursion
{
  if( btemp == NULL)
  {
    return binarycheck;
  }
  binarycheck = binarychecklocation(location,btemp->left, binarycheck);
  // This goes down the left branches
  int j = 0;
  while(btemp->places[j] != location && j <= length)
  {
    j++;
  }
  //This goes through the array until it finds the location integer or it reaches the end.
  if ( btemp->places[j] == location)
  {
    printf("%s has been to %d.\n", btemp->name, location);
    // If a user has been to the location their name is printed.
    binarycheck = 1;
    // If check = 1 then atleast one person has been to the location
  }
  binarycheck = binarychecklocation(location,btemp->right, binarycheck);
  // This goes down the right branches
  return binarycheck;
}

struct binarynode * findmin(struct binarynode *btemp) // Returns minimum data in subtree
{
  if ( btemp->left == NULL )
  {
    return btemp;
  }
  else
  {
    return findmin(btemp->left);
  }
}

void binarydelete(char *input,struct binarynode *btemp)
// This function deletes a node from the tree
// Input is the name of the user to be deleted and btemp is used for recursion
{
  if ( strcmp( input,btemp->name) < 0 )
  {
    binarydelete(input,btemp->left);
  }
  // If the input name comes alphabetically before the current name then move down the left branch
  if ( strcmp( input,btemp->name) > 0 )
  {
    binarydelete(input,btemp->right);
  } 
  // If the input name comes alphabetically after the current name then move down the right branch
  if ( strcmp( input,btemp->name) == 0 )
  // The name has been found
  {
    if (btemp->left == NULL && btemp->right == NULL)
    // The node to be deleted has no children
    {
      if ( btemp == root)
      {
        root = NULL;
        free(btemp);
      }
      // If the node was the root delete the root
      else if(strcmp(btemp->name, btemp->prev->name) > 0 )
      {
        btemp->prev->right = NULL;
        free(btemp);
      }
      // If the node's name comes alphabetically after the previous node's name then it is on a right branch. Delete that branch and the node.
      else
      {
        btemp->prev->left = NULL;
        free(btemp);
      }
      // Else the node's name comes alphabetically before the previous node's name. It is on a left branch. Delete that branch and the node.
    }
    else if (btemp->left == NULL || btemp->right == NULL)
    //The node has one child
    {
      if ( btemp == root)
      // The node to be deleted is the root
      {
        if (btemp->right != NULL)
        {
          root = btemp->right;
          free(btemp);
        }
        // If the root has the child node on its right then make that node the new root
        else
        {
          root = btemp->left;
          free(btemp);
        }
        // Else the root has the child node on its left. Make that node the new root
      }
      else // The node to be deleted is not the root
      { 
        if (btemp->left != NULL) // The node to be deleted has a left branch
        {
          if( strcmp(btemp->name, btemp->prev->name) > 0)
          {
            btemp->prev->right=btemp->left;
          }
          else
          {
            btemp->prev->left=btemp->left;
          }       
          btemp->left->prev = btemp->prev;
        }
        else if (btemp->right != NULL) // The node to be deleted has a right branch
        {
          if( strcmp(btemp->name, btemp->prev->name) > 0)
          {
            btemp->prev->right=btemp->right;
          }
          else
          {
            btemp->prev->left=btemp->right;
          }
          btemp->right->prev = btemp->prev;
        }
        free(btemp);
      }
    }
    else
    //The node to be deleted has two children
    {
      btemp2 = findmin(btemp->right);
      int branch = 0;
      int isroot = 0;
      if (btemp2->right != NULL) 
      {
        btemp3 = btemp2->right;
        branch = 1;
      }
      btemp2->left = btemp->left;
      btemp->left->prev = btemp2;
      if (btemp->right != btemp2)
      {
        btemp2->right = btemp->right;
        btemp->right->prev = btemp2;
        btemp2->prev->left = NULL;
      }
      if (btemp == root)
      {
        isroot = 1;
        root = btemp2;
      }
      else
      {
        if( strcmp(btemp->name, btemp->prev->name) > 0)
        {
          btemp->prev->right = btemp2;
        }
        else
        {
          btemp->prev->left = btemp2;
        } // The if / else determines whether the node to be deleted was the right or left child of its previous node.
      }
    
      if (branch == 1 && isroot == 0)
      {
        btemp2->prev->left = btemp3;
        btemp3->prev = btemp2->prev;
      }
      if (isroot == 1)
      {
        btemp2->prev = root;
      }
      else
      {
        btemp2->prev = btemp->prev;
      }
      free(btemp);
      
    }
  } 
}

void binarysave( struct binarynode *btemp )
// This function saves the current state of the tree to a file
// btemp is used for recursion. The file is open and closed in main.
{
  if( btemp == NULL)
  {
    return;
  }
  if(fp!=NULL)
  {
    fwrite(btemp,sizeof(struct binarynode),1,fp); // This writes the node to the file
  }
  binarysave(btemp->left);
  binarysave(btemp->right);
  // These iterate through the tree
}

void binaryprint( struct binarynode *btemp, int level )
// This function prints the current state of the tree
// btemp is used for recursion. Level is used to represent the level of the node.
{
  if ( btemp == NULL )
  {
    printf("\n");
  }
  // If a node points to NULL just print a newline.
  else {
    binaryprint ( btemp->right, level + 1 );
    // Go to the right nodes first to print in reverse alphabetical order
    for ( int i = 0; i < level; i++ )
    {
      printf("\t");
    }
    // Print tabs depending on the level of the node for formatting.
    printf ("%d. %s - ", level, btemp->name );
    // Print level and name of node.
    for ( int j = 0 ; j < length ; j ++ )
    {
      printf("%d ",btemp->places[j]);
    }
    if (btemp->left != NULL)
    {
      printf("left = %s ",btemp->left->name);
    }
    if (btemp->right != NULL)
    {
      printf("right = %s ",btemp->right->name);
    }
    if (btemp->prev != NULL)
    {
      printf("prev = %s ",btemp->prev->name);
    }
    // Print its array
    printf("\n");
    binaryprint ( btemp->left, level + 1 );
    // Go to left nodes
  }
}

void binaryread()
// This function retrieves the tree from the file and rebuilds it.
{
  int result; // This integer checks if there is information to read in the file
  bptr = (struct binarynode*) malloc(sizeof(struct binarynode));
  fp = fopen("tree","rb"); // This opens the file
  if(fp!=NULL)
  {
    result=fread(bptr, sizeof(struct binarynode),1,fp);
    {
      while(result!=0)
      // This adds each item in the list to the tree
      {
        binaryinsertuser(root, bptr->name);
        int i=0;
        while(bptr->places[i]!=0)
        {
          binaryinsertlocation(bptr->name, bptr->places[i],root);
          i++;
        }
        // This copies the integer array from the file to the tree
        result=fread(bptr,sizeof(struct binarynode),1,fp);
        // This retrieves the next item
      }
    }
  } 
  fclose(fp);           // This closes the file.
  binaryprint(root,0);  // This prints the tree
}

int treetolist(struct binarynode *btemp , int nodenumber)
// This function moves the contents of binary tree into a linked list and also returns the number of nodes in the tree / list.
// btemp is used for recursion, nodenumber is used to count the number of nodes.
{
  if( btemp == NULL)
  {
    return nodenumber;
  }
  nodenumber = treetolist( btemp->left , nodenumber);
  // Go down the left branches first to save in alphabetical order.
  insert_user(btemp->name);
  // Copy the name in the binary tree to the linked list
  nodenumber ++;
  // Increase size by one
  int i=0;
  while(btemp->places[i]!=0)
  {
    insert_visit(btemp->name, btemp->places[i]);
    i++;
  }
  // Copy the location array of each node to the linked list.
  nodenumber = treetolist( btemp->right, nodenumber);
  return nodenumber;
}

void listtotree( struct node *ptr , int start , int end)
// This function move the contents of the linked list back into binary tree form in a more balanced format.
// ptr, start and end are used for recursion
{
  if (start > end)
  {
    return;
  }
  int mid = ( start + end ) / 2;
  // Find the middle value of the current set of values
  int j = 0;
  while(j < mid)
  {
    ptr = ptr->next;
    j++;
  }
  // Move down the linked list until the middle node is found
  binaryinsertuser(root, ptr->name);
  // Insert the name of the node with the middle value into the binary tree
  int i=0;
  while(ptr->places[i]!=0)
  {
    binaryinsertlocation(ptr->name, ptr->places[i] ,root);
    i++;
  }
  // Insert the location array of the node with the middle value into the binary tree
  listtotree( head, start , mid - 1 );
  listtotree( head, mid + 1 , end );
  // Split the linked list in half, find the new middle values and repeat the function. 
}

void balance()
// This function is used to convert a binary tree into a linked list and then rebuild the binary tree to balance it.
{
  int size = treetolist( root , 0 ) ;
  // This converts the binary tree to a linked list.
  // Size is used to determine the length of the linked list.
  printlist();
  while (root != NULL)
  {
    binarydelete(root->name, root);
  }
  // This deletes the old binary tree and frees the memory.
  listtotree(head, 0 , size - 1);
  // This rebuilds the binary tree from the linked list.
  binaryprint(root,0);
  while (head != NULL)
  {
    remove_user(head->name);
  }
  // This deletes the old linked list and frees the memory.
  printlist();
  // This shouldn't print anything.
}

// END OF FUNCTIONS FOR BINARY TREE

void print_options()
{
  printf("Type your option:\n\n1: Introduce user.\n2: Introduce visit to venue.\n3: Remove user.\n4: Check which users have been in a particular place.\n5: Save to file.\n6: Retrieve data from file.\n7: Exit.\n8: Balance binary tree.\n\n");
}

// Enter ./main list or ./main tree

int main( int argc, char *argv[])
{
  int mode;
  if ( strcmp(argv[1], "list") == 0 )
  {
    mode = 0;
    printf("\nLinked list mode.\n\n");
  }
  // Mode 0 is for the linked list
  else if ( strcmp(argv[1], "tree") == 0 )
  {
    mode = 1;
    printf("\nBinary tree mode.\n\n");
  }
  // Mode 1 is for the binary tree
  char inputname[20]; // This allows the user to input a name
  int choice = 0;     // This allows the user to choose which option/case to use.
  int j;              // This is used for for loops.
  int inputlocation;  // This allows the user to input an integer.

  binaryinsertuser(root, "50");
  binaryinsertuser(root, "25");
  binaryinsertuser(root, "75");
  binaryinsertuser(root, "12");
  binaryinsertuser(root, "37");
  binaryinsertuser(root, "62");
  binaryinsertuser(root, "88");
  binaryinsertuser(root, "10");
  binaryinsertuser(root, "20");
  binaryinsertuser(root, "30");
  binaryinsertuser(root, "40");
  binaryinsertuser(root, "60");
  binaryinsertuser(root, "70");
  binaryinsertuser(root, "80");
  binaryinsertuser(root, "90");
  binaryinsertuser(root, "81");
  binaryprint(root,0);

  while(choice!=7)
  {
    print_options();  // This prints the options for the user.
    scanf("%d", &choice);
    switch(choice)
    {
      case 1: // Introduce  user
        printf("Enter a name.\n");
        scanf("%s",inputname);
        if ( mode == 0 )
        {
          insert_user(inputname);
          printlist(); // Print updated list
        }
        else if ( mode == 1 )
        {
          binaryinsertuser(root, inputname);
          binaryprint(root,0);
        }
        break;

      case 2: // Introduce visit to venue
        printf("Which user do you have information about?\n");
        scanf("%s" , inputname);
        printf("Where have they been? Enter an integer.\n");
        scanf("%d" , &inputlocation);
        if ( mode == 0 )
        {
          insert_visit(inputname,inputlocation);
          printlist(); // Print updated list
        }
        else if ( mode == 1 )
        {
          printf("\n");
          binaryinsertlocation(inputname,inputlocation,root);
          binaryprint(root,0);
          printf("\n");
        }
        break;

      case 3: // Remove a user
        printf("Which user do you want to remove?\n");
        scanf("%s" , inputname);
        if ( mode == 0 )
        {
          remove_user(inputname);
          printlist(); // Print updated list
        }
        else if ( mode == 1 )
        {
          binarydelete(inputname,root);
          binaryprint(root,0);
          printf("\n");
        }
        break;

      case 4: // Show which users have been to a location
        printf("Which location do you want information about?\nEnter an integer.\n");
        scanf("%d" , &inputlocation);
        if ( mode == 0 )
        {
          check_user(inputlocation);
        }
        else if ( mode == 1 )
        {
          printf("\n");
          int anyone = binarychecklocation(inputlocation,root, 0 );
          // Anyone is used to determine if no one has been to a location in a binary tree.
          if( anyone == 0)
          {
            printf("No one has been to %d.\n", inputlocation);
            // This only prints if no one has been to the location  
          }
          printf("\n");
        }
        break;

      case 5: // This saves to file
        if ( mode == 0 )
        {
          save_to_file();
          printf("The list has been saved.\n\n");
        }
        else if ( mode == 1 )
        {
          fp = fopen("tree","wb");  // This opens the file
          binarysave( root );       // This saves to the file
          fclose(fp);               // This closes the file
          printf("The tree has been saved.\n\n");
        }
        break;

      case 6: // This retrieves the data from the file
        if ( mode == 0 )
        {
          read_from_file();
          printf("The list has been retrieved.\n\n");
        }
        else if ( mode == 1 )
        {
          binaryread();
          printf("The tree has been retrieved.\n\n");
        }
        break;

      case 8: // This balances the binary tree.
        if ( mode == 0 )
        {
          printf("\nThis is only used for binary trees.\n\n");
        }
         else if ( mode == 1 )
        {
          balance();
        }
        break;
    }
  }
  printf("Exited");
  return 0;
}