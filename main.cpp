#include "ArgumentManager.h"
#include <algorithm>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
using namespace std;

// priority queue implementation ↓↓↓↓↓↓
struct node {
  string task;
  int priority;
  node *next;
};
node *newNode(string task, int prio) {
  node *temp = new node;
  temp->priority = prio;
  temp->next = NULL;
  temp->task = task;
  return temp;
}
string peek(node **head) { return (*head)->task; }
void push(node **head, string task, int prio) {
  node *front = (*head);

  // Create new Node
  node *temp = newNode(task, prio);

  if ((*head)->priority > prio) {
    temp->next = *head;
    *head = temp;
  } else {
    while (front->next != nullptr && front->next->priority <= prio) {
      front = front->next;
    }
    temp->next = front->next;
    front->next = temp;
  }
}
void pop(node **head) {
  node *temp = *head;
  *head = (*head)->next;
  free(temp);
}
int isEmpty(node **head) { return (*head) == NULL; }
// end of priority queue implementation^^^^

// binary search tree implementation
class bst {
    string data;
    bst *left, *right;
public:
// Default constructor.
	bst();
	// Parameterized constructor.
	bst(string);
	// Insert function.
	bst* insert(bst*, string);
	// Inder traversal.
	void inorder(bst*,ofstream&);
	void preorder(bst*,ofstream&);
	void postorder(bst*,ofstream&);
};
 
bst::bst()
    : data("")
    , left(NULL)
    , right(NULL)
{
}

bst::bst(string value)
{
	data = value;
	left = right = NULL;
}
//insert
bst* bst::insert(bst* root, string value)
{
	if (!root) {
		return new bst(value);
	}

	if (value.size() > root->data.size()) {
			root->right = insert(root->right, value);
	}
	else if (value.size() < root->data.size()){
			root->left = insert(root->left, value);
	}
	else if (value.size() == root->data.size()){
			root->data = value;
	}
	

	// Return 'root' node, after insertion.
	return root;
}
//inorder
void bst::inorder(bst* root, ofstream &out)
{
	if (!root) {
			return;
	}
	inorder(root->left,out);
	out << root->data << endl;
	inorder(root->right,out);
}

void bst::preorder(bst* root, ofstream &out)
{
	if (!root) {
			return;
	}
	out << root->data << endl;
	preorder(root->left,out);
	preorder(root->right,out);
}

void bst::postorder(bst* root, ofstream &out)
{
	if (!root) {
			return;
	}
	postorder(root->left,out);
	postorder(root->right,out);
	out << root->data << endl;
}
// end of binary search tree implementation

// command functions
void decode(string s, queue<string> &q) // adds to msgQ
{
  string str = s.substr(s.find('[') + 1);
  str = str.substr(0, str.size() - 1);
  q.push(str);
}

void add(string s, queue<string> &q) 
{
  char a = s[5];                  // first character
	string b = s.substr(s.find(',') + 1, 1); //second character (as a string)
	char c = s[7]; //second character (as a char)
  string temp = q.front(); //front of message queue

	int i = 0;
	do {
			if (temp.at(i) == a) 
			{
				temp.insert(i + 1, b);
			}
			i++;
	} while (i<temp.size());
	//pop from queue and add at end
	q.pop();
	q.push(temp);
}

void replace(string s, queue<string> &q) // takes msgq.front()
{
	char a = s[9]; // first character
	char b = s[11]; // second character
	string temp = q.front();
	for (int i=0;i<temp.size();i++)
		{
			if (temp[i]==a)
			{
				temp[i]=b;
			}
		}
	q.pop();
	q.push(temp);
}

void remove(string s, queue<string> &q) // removes given character
{
  char c = s[8];
  string temp = q.front();
  temp.erase(remove(temp.begin(), temp.end(), c), temp.end());
  q.pop();
  q.push(temp);
}

void swap(string s, queue<string> &q) {
  char a = s[6]; // first character
  char b = s[8]; // second character
  string temp = q.front();
	for (int i=0;i<temp.size();i++)
		{
			if (temp[i]==a)
			{
				temp[i]=b;
			}
			else if (temp[i]==b)
			{
				temp[i]=a;
			}
		}
  q.pop();
  q.push(temp);
}

void BST(bst t, bst *root, queue<string> &q)
{
	if (!q.empty())
	{
		string str = q.front();
		q.pop();
		t.insert(root,str);
	}
	else
		return;
}
// end of command functions

int main(int argc, char *argv[]) {
	
	ArgumentManager am(argc,argv);
  ifstream input(am.get("input"));
  ofstream output(am.get("output"));
	
	/*
  ifstream input("input2.txt");
  ofstream output("output1.txt");
	*/
	bst tree, *root = NULL;
  queue<string> msgQ;
  node *head;
  string firstLn, command, in, bstType;
  getline(input, firstLn);
  firstLn.erase(remove(firstLn.begin(), firstLn.end(), '\n'), firstLn.end());
  firstLn.erase(remove(firstLn.begin(), firstLn.end(), '\t'), firstLn.end());
  firstLn.erase(remove(firstLn.begin(), firstLn.end(), '\r'), firstLn.end());
  int priority;
  // first not alphabetic character
  char test = firstLn.at(firstLn.size() - 2);
  priority = (int)test;
  int pos = firstLn.find(']');
  command = firstLn.substr(0, pos + 1);
  node *pq = newNode(command, priority);

  // reads input into priority queue
  while (getline(input, in)) {
    in.erase(remove(in.begin(), in.end(), '\n'), in.end());
    in.erase(remove(in.begin(), in.end(), '\t'), in.end());
    in.erase(remove(in.begin(), in.end(), '\r'), in.end());
    int priority;
    if (in.find("Inorder") != string::npos) {
      bstType = "in";
      continue;
    }
    if (in.find("Postorder") != string::npos) {
      bstType = "post";
      continue;
    }
    if (in.find("Preorder") != string::npos) {
      bstType = "pre";
      continue;
    }
		if (in.find("BST") != string::npos) {
      char test = in[5];
    priority = (int)test;
    int pos = in.find(')');
    command = in.substr(0, pos + 1);
    push(&pq, command, priority);
    }
    // first not alphabetic character
    char test = in.at(in.size() - 2);
    priority = (int)test;
    int pos = in.find(']');
    command = in.substr(0, pos + 1);
    push(&pq, command, priority);
  }

  // reads through and prints priority queue, loop can and will be used for
  // rest of program

  while (!isEmpty(&pq)) {
    string stri = peek(&pq);
    if (stri.find("DECODE") != string::npos) {
      decode(stri, msgQ);
    } else if (stri.find("ADD") != string::npos) {
      add(stri, msgQ);
    } else if (stri.find("REMOVE") != string::npos) {
      remove(stri,msgQ);
    } else if (stri.find("REPLACE") != string::npos) {
      replace(stri, msgQ);
    } else if (stri.find("SWAP") != string::npos) {
      swap(stri, msgQ);
    } else if (stri.find("BST") != string::npos) {
			root = tree.insert(root, msgQ.front());
			BST(tree,root,msgQ);
    }
    pop(&pq);
  }
	
	if (bstType=="in")
		tree.inorder(root,output);
	else if (bstType=="post")
		tree.postorder(root,output);
	else if (bstType=="pre")
		tree.preorder(root,output);
}