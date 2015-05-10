
/*** M A N I P U L A T E   B I N A R Y   T R E E S   R E C U R S I V E L Y ***

FILE NAME      :  bin-trees-A.cc

ACTION   :  The program allows an interactive user to build binary
            trees with integer labels on the nodes, to get information
            about these trees, and to modify or destroy them.

INPUT    :  All input consists of commands entered by an interactive user.

OUTPUT   :  All output is sent to the screen, including the prompts
            and menus provided to the interactive user and the results
            of the commands entered by the user.  The commands entered
            by the user are echoed by the program to the screen.

COMMENTS :  This program is intended to be given to C243 students as
            a programming assignment.  They are to supply missing code.

******************************************************************************/

#include <iostream>
using namespace std;
#include <cstdlib>

#include <vector>
#include <string>
#include <sstream>

#define A(x)   cout << x << endl;       // Debugging macro
#define B(x,y) cout << x << y << endl;  // Debugging macro


/**************************  D A T A   T Y P E S  ****************************/

struct node
{
    int  datum;
    node *left;
    node *right;
};
typedef node * nodePtr;      // Define a new pointer data type.


/*****************  F U N C T I O N   P R O T O T Y P E S  *******************/

void  buildABinaryTree (nodePtr &rootp);
int   countOfZeroDataValues (nodePtr rootp);
void  destroyABinaryTree (nodePtr &rootp);
int   height (nodePtr rootp);
void  incrementEachDatum (nodePtr rootp);
void  prettyPrint (nodePtr rootp, int margin = 0);
void  graphPrint(nodePtr rootp);
int   graphPrintWalk(nodePtr rootp, int &pos, int level, 
		     vector<string>& output);
ostream &operator<<(ostream &out, nodePtr rootp);
void  reverse (nodePtr &rootp);
int   sumOfNegativeData (nodePtr rootp);

void  printMenuAndGetAction (char & response);
void  printExplanatoryMessage();
void  readAndEchoCharResponse (char & response);
void  readAndEchoIntResponse (int & response);
void  testDynamicAllocation (void * pointerValue);



/************************  M A I N   F U N C T I O N  **********************/

int main()
{
    nodePtr  treep  = NULL;     // "treep" stands for "tree pointer";  will be
    char      response;          // used to point to the root of a binary tree
    int       dataValue;
    
    void  printExplanatoryMessage();

    do  // until interactive user signals to quit
    {
        printMenuAndGetAction (response);  // Insists on a valid response
        switch (response)  // Response is necessarily one of the following.
        {
        case 'B' :
        case 'b' : buildABinaryTree (treep);
        break;
        case 'C' :
        case 'c' : cout << "\nThere are " 
                        << countOfZeroDataValues (treep)
                        << " zero data values in the current tree.\n";
        break;
        case 'D' :
        case 'd' : cout << endl;
        destroyABinaryTree (treep);
        cout << "\nThe tree should now be empty.\n";
        break;
        case 'H' :
        case 'h' : cout << "\nThe height of the tree is " << height(treep)
                        << ".\n";
        break;
        case 'I' :
        case 'i' : incrementEachDatum (treep);
        cout << "\nEvery data value should have been "
             << "incremented.\n";
        break;
        case 'P' : 
        case 'p' : if (!treep)
            cout << "\nThe tree is empty." << endl;
        else
            graphPrint (treep);
        break;
        case 'Q' :
        case 'q' : break; // Do nothing.  The program will quit.
        case 'R' :
        case 'r' : cout << endl;
        reverse(treep);
        cout << "\nThe tree should now be the mirror image "
             << "of what it was.\n";
        break;
        case 'S' :
        case 's' : cout << "\nThe sum of the negative data values is "
                      << sumOfNegativeData (treep) << endl;
        break;
        }
    } while (response != 'Q' && response != 'q');
    
    cout << "\n\n";
    return 0; // Indicate normal termination.
} // main()


/*************  P R I N T   E X P L A N A T O R Y   M E S S A G E  ***********

This function prints a message to the interactive user explaining briefly
what the program is going to do.  Documented and coded by W. Knight. */

void  printExplanatoryMessage()
{
    cout << "\n\nThis program allows an interactive user to create, modify,\n";
    cout << "display, and destroy binary trees.  The program is menu driven.\n";
    cout << "Press the Enter key to continue.";
    while (cin.get() != '\n');  // Clear the input stream up through newline.
                                // Note the empty loop body.
} // printExplanatoryMessage()


/************  R E A D   A N D   E C H O   C H A R   R E S P O N S E  *********

This utility function assumes that an interactive user has been prompted
to enter a one-character response, followed by a newline character.
It reads the first non-white-space response character and then clears all 
other characters from the input stream, up through the first newline character.
It also echoes the character back to the user.  Documented and coded by  
W.Knight. */

void readAndEchoCharResponse (char & response)
{
    char junk;  // Holds chars that trail the desired response char.

    cin >> response;
    while (cin.get() != '\n'); // Clear the line.
    cout << response << endl;
}


/************  R E A D   A N D   E C H O   I N T   R E S P O N S E  *********

This utility function assumes that an interactive user has been prompted
to enter an integer value, followed by a newline character.
It reads the first integer on the line and then clears all 
other characters from the input stream, up through the first newline character.
It also echoes the integer back to the user.  Documented and coded by 
W.Knight. */

void readAndEchoIntResponse (int & response)
{
    cin >> response;
    while (cin.get() != '\n');  // Clear the line.
    cout << response << endl;
} // readAndEchoIntResponse()


/**********  P R I N T   M E N U   A N D   G E T   A C T I O N  *************

This function displays a menu of actions and asks an interactive user to 
select one.  It will not return until it has read an acceptable response. 
Documented and coded by W.Knight. */

void printMenuAndGetAction (char & response)
{
    cout << "\n\n";
    cout << "Which of the following actions do you wish to take?\n";
    cout << "   B)uild or modify the current tree interactively.\n";
    cout << "   C)ount and report number of zero data values in "
        "current tree\n";
    cout << "   D)estroy the current tree.\n";
    cout << "   H)eight of the tree -- determine it.\n";
    cout << "   I)ncrement every data value in the current tree by 1.\n";
    cout << "   P)rint the current tree on the screen.\n";
    cout << "   Q)uit.\n";
    cout << "   R)eplace the current tree by its mirror image.\n";
    cout << "   S)um the negative data values in the current tree.\n";
    cout << "\n";
    cout << "Your choice:  ";
    
    readAndEchoCharResponse (response);
    
    while (1) // loop will continue until user gives an appropriate response
    {
        switch (response)
        {
        case 'B': case 'b': case 'C': case 'c': 
        case 'D': case 'd': case 'H': case 'h': 
        case 'I': case 'i': case 'P': case 'p': 
        case 'Q': case 'q': case 'R': case 'r': 
        case 'S': case 's' :
            return;  // EXIT FROM THE FUNCTION WITH PARAMETER HAVING OK VALUE 
        default : cout << "Improper Response  "
                "Please type an appropriate letter.   ";
        }
        readAndEchoCharResponse (response);
    }
} // printMenuAndGetAction()

  
/*******************  T E S T   D Y N A M I C   A L L O C A T I O N  *********

This utility function assumes that the value of the parameter "pointer" 
has been obtained by a call to the "new" function.  If that function
was unable to dynamically allocate the required amount of memory space,
then it returned the value NULL.  This function tests to see whether
the value returned was NULL, and if it was, it terminates the program.
The parameter type is "pointer-to-void" so that any pointer data type
can be passed to this function.   Documented and coded by W. Knight. */

void testDynamicAllocation (void * pointer)
{
    if (!pointer)  // if "pointer" is NULL
    {
        cout << "\nDynamic allocation failure.  Program being terminated."
             << endl;
        exit(1);  // EXIT FROM THE ENTIRE PROGRAM
    }
} // testDynamicAllocation()

/******************  P R I N T   T R E E   V E R T I C A L L Y  *************

The following function prints a binary tree vertically graphically
showing the connections between parent and child nodes The function
and support functions were written by C. George, C243, Fall 2004. */

void graphPrint(nodePtr rootp)
{
    if (rootp == NULL) return;
	
    // create an array to hold the output as is it generated
    vector<string> output;
    int pos = 0;
    pos = graphPrintWalk(rootp, pos, 0, output);

    // print the root node
    cout << string(pos,' ') << rootp << endl;
    
    // print the other levels from top to bottom
    for(int i=0;i<output.size();i++)
    {
        cout << output[i] << endl;
    }
} // graphPrint()

// This function walks through the tree in-order to calculate the x
// position of each node in the tree. It then prints any child nodes
// to the appropriate output string and also prints inner-connecting
// links

int graphPrintWalk(nodePtr rootp, int &pos, int level, 
                   vector<string>& output)
{
    if (rootp == NULL) pos;
    else
    {
        // Expand the size of the output array if this is the first
        // node on a new level
        if(output.size() <= (level*2))
        {
            output.push_back("");
            output.push_back("");
        }
    
        // Calculate the x position of both child nodes and the current node
        int leftPos = graphPrintWalk(rootp->left, pos, level+1, output);
    
        int currPos = pos;
        
        ostringstream currDatum;
        currDatum << rootp;
        pos += currDatum.str().length()+1;
        
        int rightPos = graphPrintWalk(rootp->right, pos, level+1, output);
    
        // initialize the output streams with the current output for the level
        
        ostringstream linkLine, nodeLine;
    
        linkLine << output[level*2];
        nodeLine << output[level*2+1];
        
        // calculate the center of the current node
        int currOffset = currPos + (currDatum.str().length() / 2) - 1;
        
        // add the left node and its link to the current output for the level
        if(rootp->left != NULL)
        {
            // calculate the center of the left child node
            ostringstream leftDatum;
            leftDatum << rootp->left;
            int leftOffset = leftPos + (leftDatum.str().length() / 2) - 1;
            
            nodeLine << string(leftPos - nodeLine.str().length(), ' ') 
                     << rootp->left;
            
			
            // draw a link from this node to the left child node
            linkLine << string((leftOffset+1) - linkLine.str().length(), ' ') 
                     << string((currOffset)-(leftOffset+1), '_') << "/";
			
        }
    
        // add the right node and its link to the current output for the level
        if(rootp->right != NULL)
        {
            // calculate the center of the right child node
            ostringstream rightDatum;
            rightDatum << rootp->right;
            int rightOffset = rightPos + (rightDatum.str().length() / 2) - 1;
      
      
            nodeLine << string(rightPos - nodeLine.str().length(),' ') 
                     << rootp->right;
            
            // draw a link from this node to the right child node
            linkLine << string((currOffset+1) - linkLine.str().length(),' ') 
                     << "\\" <<	string((rightOffset-1) - (currOffset), '_') 
                     << " ";
        }

        // save the results for the current level
        output[level*2]   = linkLine.str();
        output[level*2+1] = nodeLine.str();
    
        return currPos;
    }
} // graphPrintWalk()

// This stream formatter outputs the contents of a node with a leading digit
// for datum values 0-9

ostream &operator<<(ostream &out, nodePtr rootp)
{
    out << ((rootp->datum >= 0 && rootp->datum < 10)?"0":"")
        << rootp->datum;
    return out;
} // operator<<()

/**************************  P R E T T Y   P R I N T  *************************
                                                                           
The following function sends to standard output a list of the integer data
values of the nodes in the binary tree pointed to by "rootp".  The values
are written in such a way as to show the logical structure of the tree.
The parameter "margin" should have a non-negative value, which determines 
the number of blank spaces that will be printed to the left of the root 
of the tree.  If no second argument is used when this function is called,
then the default value 0 will be used (see the function prototype).  
The function is recursive.   Documented and coded by W.Knight. */

void prettyPrint (nodePtr rootp, int margin) // 
{
    const int INDENTATION = 4;
    
    if (!rootp)
        return;
    
    else
    {
        prettyPrint(rootp->right, margin + INDENTATION);

        for (int i = 1; i <= margin; ++i)
            cout << " ";
    
        cout << rootp->datum << endl;
        
        prettyPrint(rootp->left, margin + INDENTATION);
    }
} // prettyPrint()


/*****************  B U I L D   A   B I N A R Y   T R E E  ******************
                                                                           
The following function allows an interactive user to construct a binary   
tree whose nodes hold integer data values.  The function is recursive. 
Documented and coded by W.Knight. 
Added by D. Vrajitoru in 2012: an option to delete the subtree when the 
rootp pointer is not null. */

void buildABinaryTree (nodePtr &rootp) // Parameter passed by REFERENCE!
{
    char action;

    while(1)  // Endless loop; there are two "return" statements in the loop.
              // They are executed whenever the user wants to move up the tree.
    {
        if (rootp == NULL)  // The tree or subtree pointed to by rootp is empty.
        {
            do  // This loop continues until the user responds with 'U' or 'C'.
            {
                cout << "\nYou are currently at an empty tree. If you wish to ";
                cout << "create\na new node at this position, type C and press";
                cout << " Enter.\nOtherwise ";
                cout << "type U to go up to the parent node (if any).  ";
                readAndEchoCharResponse (action);
            } while (action != 'C' && action!= 'c' && action!= 'U' 
                     && action!= 'u');
            if (action == 'U' || action == 'u')
                return; // Return control to the calling function
            else
            {
                rootp = new node;
                testDynamicAllocation (rootp);
                rootp->left  = rootp->right = NULL;
                cout << "Enter an integer for the new node:  ";
                readAndEchoIntResponse (rootp->datum);
            }	
        }
        else  // tree is not NULL
        {
            do  // This loop continues until user gives a suitable response.
            {      
                cout << "\nThe datum of the node at which you have arrived is\n";
                cout << "         ";
                cout << rootp->datum << endl;
                cout << "Enter  U  to go up to the parent node,\n";
                cout << "       L  to go down to the left,\n";
                cout << "       R  to go down to the right,\n";
                cout << "       P  print the current subtree,\n";
                cout << "       M  to modify value of the datum stored here,\n";
                cout << "       D  to delete the whole subtree.\n";
                readAndEchoCharResponse (action);
            }
            while (action != 'U' && action !='u' && action !='L' && action !='l' 
                   && action != 'R' && action !='r' && action !='M' && action !='m'
                   && action != 'P' && action != 'p' && action != 'D' && action != 'd');
      
            switch (action)
            { 
            case 'U': case 'u': return;   // Go back to the calling function.
            case 'L': case 'l': buildABinaryTree (rootp->left);  break;
            case 'R': case 'r': buildABinaryTree (rootp->right); break;
            case 'P': case 'p': graphPrint(rootp); break;
            case 'M': case 'm': cout << "Enter a new datum value to replace"
                    " the current value.  ";
                readAndEchoIntResponse (rootp->datum);
                break;
            case 'D': case 'd': destroyABinaryTree(rootp); // the parameter is 
                // also passed by reference
            } // end of "switch"
        }   // end of "else"     
    }     // end of "while(1)" 
}       // end of function 


/*************  C O U N T   O F   Z E R O   D A T A   V A L U E S  **********

The following function traverses the binary tree pointed to by "rootp"
and counts every node whose datum value is zero.  The function
returns that count as its value.  The function is recursive.  The function
documentation and heading were written by W. Knight.  */

int countOfZeroDataValues (nodePtr rootp)
{
        if (!rootp) // if tree is empty
            return 0;
        if (rootp->datum == 0) // if datum == 0, add one to counter
            return 1 +
            countOfZeroDataValues(rootp->left) +
            countOfZeroDataValues(rootp->right);
        else // if not, add zero
            return 0 +
            countOfZeroDataValues(rootp->left) +
            countOfZeroDataValues(rootp->right);        
   // STUDENTS MUST SUPPLY THE CODE
} // countOfZeroDataValues()


/****************  D E S T R O Y   A   B I N A R Y   T R E E  ***************
                                                                           
The following function deallocates all the nodes in the binary tree 
pointed to by "rootp", and it makes the tree empty (i.e., it changes to 
NULL the value of the pointer passed to it).  The function is recursive.  
The function documentation and heading were written by W. Knight.  */

void destroyABinaryTree (nodePtr &rootp)
{
    if (rootp) // if tree is not empty
    {
        destroyABinaryTree(rootp->left); // call recursively on each side
        destroyABinaryTree(rootp->right);
        delete rootp;
        rootp = NULL; // set to NULL
    }
    // STUDENTS MUST SUPPLY THE CODE
} // destroyABinaryTree()


/***************************  H E I G H T  ********************************

The following function calculates and returns the height of the binary
tree pointed to by "rootp".  The function is recursive.  The function
documentation and heading were written by W. Knight.  */

int height (nodePtr rootp)
{
    if (!rootp) // if tree is empty
        return -1;
    else
    {
        int leftHeight = height(rootp->left); // find height of left
        int rightHeight = height(rootp->right); // find height of right
        if (leftHeight >= rightHeight) // increment both heights recursively
            return 1 + leftHeight;
        else
            return 1 + rightHeight;
    }
    // STUDENTS MUST SUPPLY THE CODE
} // height()


/********************  I N C R E M E N T   E A C H   D A T U M  **************

The following function traverses the binary tree pointed to by "rootp"
and replaces the datum value in each node by the number that's greater
by  1  in value.  The function is recursive.  The function documentation 
and heading were written by W. Knight.  */

void incrementEachDatum (nodePtr rootp)
{
    if (rootp) // if tree not empty
    {
        (rootp->datum)++; // increment datum in each node recursively
        incrementEachDatum(rootp->left);
        incrementEachDatum(rootp->right);
    }
    // STUDENTS MUST SUPPLY THE CODE
} // incrementEachDatum()


/****************************  R E V E R S E  ******************************
                                                                          
This function replaces the binary tree pointed to by "rootp" with its     
mirror image.  The function is recursive.  The function documentation and
heading were written by W. Knight.                              */

void reverse (nodePtr &rootp)
{
    if (rootp) // if tree not empty
    {
        nodePtr temp; // swap the two nodes
        temp = rootp->left;
        rootp->left = rootp->right;
        rootp->right = temp;
        reverse(rootp->left); // call recursively
        reverse(rootp->right);
    }
    // STUDENTS MUST SUPPLY THE CODE
} // reverse()


/*****************  S U M   O F   N E G A T I V E   D A T A ****************

The following function traverses the binary tree pointed to by "rootp",
sums the data values that are negative, and returns that sum.  For example,
if the tree contains nodes with data values  -2 , -6 , and -1 , and no
other nodes contain negative data, then the function will return the 
number  -9 .  If the tree contains no node with negative data value,
then the function returns zero .  The function is recursive.  The function
documentation and heading were written by W. Knight.  */

int sumOfNegativeData (nodePtr rootp)
{
    if (!rootp) // if tree is empty
        return 0;
    if (rootp->datum < 0)
        return rootp->datum + sumOfNegativeData(rootp->left) + sumOfNegativeData(rootp->right);
    else 
        return 0 + sumOfNegativeData(rootp->left) + sumOfNegativeData(rootp->right);
    // STUDENTS MUST SUPPLY THE CODE
} // sumOfNegativeData()

//int countOfZeroDataValues(nodePtr rootp)
//{
//    if (!rootp) // if tree is empty
//        return 0;
//    if (rootp->datum == 0) // if datum == 0, add one to counter
//        return 1 +
//        countOfZeroDataValues(rootp->left) +
//        countOfZeroDataValues(rootp->right);
//    else // if not, add zero
//        return 0 +
//        countOfZeroDataValues(rootp->left) +
//        countOfZeroDataValues(rootp->right);
//    // STUDENTS MUST SUPPLY THE CODE
//} // countOfZeroDataValues()
