#include<stdio.h>
#include<algorithm>
using namespace std;

struct Node* null;
struct Node
{
	Node* ch[2];
	int r;
	int v;
	int s;
	Node(int v) :v(v) { ch[0] = ch[1] = null; r = rand(); s = 1; }
	bool operator<(const Node& rhs)const {
		return r < rhs.r;
	}
	int cmp(int x)const {
		if (x == v)return -1;
		return x > v ? 0 : 1;
	}
	void maintain() {
		s = ch[0]->s + ch[1]->s + 1;
	}
};
void rotate(Node*& o, int d)
{
	Node* k = o->ch[d ^ 1]; o->ch[d ^ 1] = k->ch[d];
	k->ch[d] = o; o->maintain(); k->maintain();
	o = k;
}
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

int Rank(Node* o, int x, int sum)
{
	int d = o->cmp(x);
	if (d == -1) return o->ch[0]->s + 1 + sum;
	else return Rank(o->ch[d], x, sum + d * (o->ch[d ^ 1]->s + 1));
}

int cnt = 0;
const int maxn = 1e6;
int rcdma[maxn];
int rdcnt[maxn];

int main() {
	null = new Node(0);
	null->s = 0;
	Node* root = null;

	FILE* fpread = fopen("trace.txt", "r");

	int ma;
	int last = -1;
	int cnt1 = 0;
	int maxrd= 0;
	while (fscanf(fpread, "%d\n", &ma) > 0) {
		if (ma == last) {
			cnt1++;
			continue;
		}
		last = ma;
		cnt++;
		if (rcdma[ma]) {
			int tem = Rank(root, rcdma[ma], 0);
			maxrd=max(tem,maxrd);
			rdcnt[tem]++;
			remove(root, rcdma[ma]);
		}
		insert(root, cnt);
		rcdma[ma] = cnt;
	}
	rdcnt[1] += cnt1;
	FILE* fpWrite = fopen("reuse_distance_histo.txt", "w");
	for (int i = 0; i <= maxrd; i++)fprintf(fpWrite, "The Count of Reuse Distance %d is %d \n", i,rdcnt[i]);
}
