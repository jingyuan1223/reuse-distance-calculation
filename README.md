# Reuse Distance Calculation

## Definition: Reuse Distance

Reuse distance is the number of distinct memory references that have happened between two memory references to the same location. In short, it tells you when was the last time you used this address.

## Implementation Detail

The basic solution is based on the stack and the time complexity of this stack-based algorithm will be O(N).

My solution is the tree based method. The basic data structure of this solution is the Dynamically Balanced Binary Search Tree. The most common Dynamically Balanced Binary Search Tree is the AVL tree. But in my solution, I used a similar tree called Treap, which is a randomized binary search tree and it also can dynamically balance itself. And the biggest advantage of this data structure is that this Treap tree is much easier to implement and it is widely used to calculate the Rank of each node in the Tree in O(logN) time.

In my C code solution, the tree Node structure of the Treap is as following.

```c
struct Node
{
	Node* ch[2]; // ch[0] is leftchid node and ch[1] is the rightchild one
	// This r variable represens the priority of this node. The Treap tree is also like 	// a heap and this r is the priority of this node in this heap. r variable is used to 	  // decide zig or zag operation. And this value is randomly given so that this BST can 	// be balanced.  
	int r;
    
    // This v variable is the order of the memory address in the trace.txt file. This v 	// variable is placed in the Treap based on the BST rules. And I also change the 		// compare function so that the newly added node with the biggest v will be placed in 	  // the left most place of the tree. And the Rank of each node is also based on this 	  // value v
	int v;
    
    // This s variable is the augmented data, which represents the number of all the 		// nodes under current node(including current node itself) 
	int s; 
    
	Node(int v) :v(v) { ch[0] = ch[1] = null; r = rand(); s = 1; }
	bool operator<(const Node& rhs)const {
		return r < rhs.r;
	}
	int cmp(int x)const {
		if (x == v)return -1;
		return x > v ? 0 : 1;
	}
    // This is function to maintain the augmented data s.
	void maintain() {
		s = ch[0]->s + ch[1]->s + 1;
	}
};
```

And the 4 main functions of this Treap tree used to calculate the reuse distance (Rank of the node in the tree) is "rotate", "insert", "remove", and "Rank".

```c
// This is the function to perform zig or zag operation based on the value d
void rotate(Node*& o, int d)
{
	Node* k = o->ch[d ^ 1]; o->ch[d ^ 1] = k->ch[d];
	k->ch[d] = o; o->maintain(); k->maintain();
	o = k;
}
// This is the function to insert new node into the Treap
void insert(Node*& o, int x)
{
	if (o == null) o = new Node(x);
	else {
		int d = o->cmp(x);
		insert(o->ch[d], x);
		if (o->ch[d] > o)rotate(o, d ^ 1);
	}
	o->maintain();
}
// This is the function to remove one node from the Treap
void remove(Node*& o, int x) {
	int d = o->cmp(x);
	if (d == -1) {
		Node* u = o;
		if (o->ch[0] != null && o->ch[1] != null) {
			int d2 = (o->ch[0] > o->ch[1] ? 1 : 0);
			rotate(o, d2); remove(o->ch[d2], x);
		}
		else {
			if (o->ch[0] == null) o = o->ch[1];
			else o = o->ch[0];
			delete u;
		}
	}
	else
		remove(o->ch[d], x);
	if (o != null)o->maintain();
}
// This is the functin to calculate the Rank of node in the Treap
int Rank(Node* o, int x, int sum)
{
	int d = o->cmp(x);
	if (d == -1) return o->ch[0]->s + 1 + sum;
	else return Rank(o->ch[d], x, sum + d * (o->ch[d ^ 1]->s + 1));
}
```

When reading the "memory address" from the "trace.txt" file, if this memory address is a new one, then my program just insert this address into the Treap. 

If this memory address is already in the Treap, then my program first call the Rank() function to calculate the reuse distance and then remove this node from the Treap. And finally give this memory address a new "v" value, which is the biggest in the Treap and insert this node back to the Treap.

## Complexity Analysis

The time complexity of the "insert", "remove", and "Rank" operations are all O(logN)

The time complexity of the "rotate" operation is O(1)

The space complexity of this whole Treap tree is O(N)

## Algorithm Optimization

After running this program and getting the whole reuse distance counts, I find that the reuse distance 1 has 181842424 counts, which means that the same memory address continuously appears about 181842424 times and this occupies a quarter of the "trace.txt" file content.

If this situation occurs, I won't try to calculate the rank and remove the node and insert it again. I just simply add the reuse distance 1 count by 1. And this optimization will help me reduce a quarter of the time.

## Remark

In my program, I see the reuse distance of two adjacent same memory address as 1, which is just a little different from the lecture we learned but I think it is not a big deal.