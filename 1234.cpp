#include<iostream>
#include<vector>
#include <algorithm>
using namespace std;
#include<list>
#define CIRCLE 3
#include<string>

#define NUM_DISK 3
#define NUM_RING 3



class Hanoi_node {
	int Fs;
	int Gs;
	int Hs;

public:
	vector<int> vvi_Board[NUM_DISK];
	Hanoi_node* parent_node;
public:
	Hanoi_node() :Fs(0), Gs(0), Hs(0), parent_node(nullptr) {
		for (int i = 0; i < NUM_RING; ++i)
			vvi_Board[0].emplace_back(NUM_RING - i);
		calculate_Hs();
		Fs = Gs + Hs;
	};
	Hanoi_node(vector<int>* Board) :Fs(0), Gs(0), Hs(0), parent_node(nullptr)
	{
		CopyBoard(Board);
	};
	Hanoi_node(int pastPath, vector<int>* Board, Hanoi_node* parent) : parent_node(parent)
	{
		CopyBoard(Board);
		Gs = pastPath;
		calculate_Hs();
		Fs = Gs + Hs;
	};

	Hanoi_node(const Hanoi_node& node) :Fs(node.Fs), Gs(node.Gs), Hs(node.Hs), parent_node(node.parent_node) {
		CopyBoard(node.vvi_Board);
	}
private:
	void CopyBoard(const std::vector<int>* Board)
	{
		for (int i = 0; i < NUM_DISK; ++i) {
			for (int j = 0; j < Board[i].size(); ++j)
				vvi_Board[i].emplace_back(Board[i][j]);
		}
	};
	void calculate_Hs()
	{
		Hs = NUM_RING;
		for (int n = 0; n < NUM_DISK - 1; ++n) {
			if (vvi_Board[n].size() > 1) {  //만약 목표 타워가 아닌곳에 링이 2개 이상 있다면
				for (int i = 0; i < vvi_Board[n].size() - 1; ++i) {
					Hs += vvi_Board[n].size() - i - 1;
				}
			}
		}
		for (int i = 0; i < NUM_RING; ++i) {
			if (vvi_Board[NUM_DISK - 1].size() <= i)break;

			if (vvi_Board[NUM_DISK - 1][i] == NUM_RING - i) Hs--;
			else { Hs++; }
		}
	};

	void Move_Add_NewNode(int j, int i, std::vector<Hanoi_node>& v_board)
	{
		Hanoi_node temp{ *this };
		temp.vvi_Board[j].emplace_back(vvi_Board[i][vvi_Board[i].size() - 1]); //다른 타워에 현재 타워의 맨위에 것을 삽입
		temp.vvi_Board[i].erase(temp.vvi_Board[i].begin() + vvi_Board[i].size() - 1); // 현재 타워에 맨위에 것을 지움
		temp.Update_newNode(this);
		v_board.emplace_back(temp);
	}
	void Update_newNode(Hanoi_node* parent)
	{
		Gs++;
		calculate_Hs();
		Fs = Gs + Hs;

		parent_node = parent;
	}

public:
	vector<Hanoi_node> possible_NextNode() {
		vector<Hanoi_node> v_board;

		// 다음에 가능한 보드의 모양 찾기
		for (int i = 0; i < NUM_DISK; ++i) {
			for (int j = 0; j < NUM_DISK; ++j) {
				if (i == j)continue;   //같은 타워면 지나가기
				if (vvi_Board[i].size() == 0)continue;
				if (vvi_Board[j].size() == 0) {
					Move_Add_NewNode(j, i, v_board);
				}
				else if (vvi_Board[i][vvi_Board[i].size() - 1] < vvi_Board[j][vvi_Board[j].size() - 1]) { //다른 타워의 최상층 값보다 작다면
					Move_Add_NewNode(j, i, v_board);
				}
			}
		}

		return v_board;
	}

	bool check_OverlapBoard(const vector<int>* Board) const {
		for (int i = 0; i < NUM_DISK; ++i) {
			if (vvi_Board[i].size() != Board[i].size()) return false;
			for (int j = 0; j < vvi_Board[i].size(); ++j)
				if (vvi_Board[i][j] != Board[i][j]) return false;
		}
		return true;
	};

	int getFs() const { return Fs; };
	int getGs() const { return Gs; };
	int getHs() const { return Hs; };

	bool operator==(const Hanoi_node& h) const
	{
		return check_OverlapBoard(h.vvi_Board);
	}
};
struct Node {
	Hanoi_node v;
	int size = 0;

	Node* parent = nullptr;
	Node* next;


	void InsertNode(Hanoi_node h_node)
	{
		size++;
		Node* New = new Node;

		New->v = h_node;
		if (next == nullptr) {
			next = New;
			New->next = nullptr;
			return;
		}

		Node* curr = this;
		while (curr != nullptr) {
			if (curr->next == nullptr) {
				New->next = curr->next;
				curr->next = New;
				return;
			}
			if (New->v.getFs() <= curr->next->v.getFs()) {
				New->next = curr->next;
				curr->next = New;
				return;
			}
			curr = curr->next;
		}
	}
	void InsertNode(Node* node)
	{
		size++;
		if (next == nullptr) {
			next = node;
			node->next = nullptr;
			return;
		}
		Node* curr = this;
		while (curr != nullptr) {
			if (curr->next == nullptr) {
				node->next = curr->next;
				curr->next = node;
				return;
			}
			if (node->v.getFs() <= curr->next->v.getFs()) {
				node->next = curr->next;
				curr->next = node;
				return;
			}
			curr = curr->next;
		}
	}
	void InsertFront(Node* node)
	{
		size++;
		if (next == nullptr) {
			next = node;
			node->next = nullptr;
			return;
		}
		node->next = this->next;
		this->next = node;
	}
	void DeleteFrontNode()
	{
		if (next == nullptr) {
			cout << "지울게 없습니다." << endl;
			return;
		}
		size--;
		Node* prev = this;
		Node* curr = next;

		prev->next = curr->next;
		delete curr;
	}

	void DeleteFindNode(Node* key)
	{
		if (next == nullptr) {
			cout << "지울게 없습니다." << endl;
			return;
		}
		Node* prev = this;
		Node* curr = next;

		while (curr != nullptr) {
			if (curr == key) {
				prev->next = curr->next;
				size--;
				delete curr;
				return;
			}
			prev = curr;
			curr = curr->next;
		}
	}

	void MoveToNodeFront(Node* other)
	{
		Node* node = next->next;
		other->InsertNode(this->next);
		this->next = node;
		size--;

	}
};
void Print_Node(string name, Node* root)
{
	cout << name << endl;
	for (int i = 0; i < root->size; ++i)
		cout << "────";
	cout << endl;
	for (int i = 0; i < NUM_RING; ++i) {
		cout << "|";
		Node* curr = root;
		while (curr->next != nullptr)
		{
			curr = curr->next;
			for (int j = 0; j < NUM_DISK; ++j) {
				if (NUM_RING - 1 - i < curr->v.vvi_Board[j].size())
					cout << curr->v.vvi_Board[j][NUM_RING - 1 - i];
				else
					cout << "'";
			}
			cout << "|";
		}
		cout << endl;
	}
	for (int i = 0; i < 3; ++i) {
		cout << "|";
		Node* curr = root;
		while (curr->next != nullptr)
		{
			curr = curr->next;
			if (i == 0)
				cout << "F=" << curr->v.getFs();
			else if (i == 1)
				cout << "G=" << curr->v.getGs();
			else
				cout << "H=" << curr->v.getHs();
			cout << "|";

		}
		cout << endl;
	}
	for (int i = 0; i < root->size; ++i)
		cout << "────";
	cout << endl;
}
bool Check_Overlap(Node* N_Open, Node* N_Close, Node* N_NextLevelNode, Hanoi_node& next, Node* parent)
{
	if (next.check_OverlapBoard(parent->v.vvi_Board)) return false;
	Node* node = N_Open;
	while (node->next != nullptr)
	{
		node = node->next;
		if (next.check_OverlapBoard(node->v.vvi_Board)) {

			if (node->v.getFs() > next.getFs()) {
				Node* temp = node;
				node = node->next;
				N_Open->DeleteFindNode(temp);
				if (node == nullptr) break;
			}
			else return false;
		}
	}
	node = N_Close;
	while (node->next != nullptr)
	{
		node = node->next;
		if (next.check_OverlapBoard(node->v.vvi_Board)) {
			return false;
		}
	}
	node = N_NextLevelNode;
	while (node->next != nullptr)
	{
		node = node->next;
		if (next.check_OverlapBoard(node->v.vvi_Board)) {
			if (node->v.getFs() > next.getFs()) {
				Node* temp = node;
				node = node->next;
				N_NextLevelNode->DeleteFindNode(temp);
				if (node == nullptr) break;
			}
			else return false;
		}
	}

	return true;
}
void print_Answer(Hanoi_node node, Node* root)
{
	cout << "정답입니다." << endl;

	Node* Path = new Node;
	Path->next = nullptr;

	Path->InsertNode(node);

	while (root->parent != nullptr)
	{
		Path->InsertFront(root);
		root = root->parent;
	}
	Node* start = new Node;
	start->v = Hanoi_node();
	Path->InsertFront(start);
	Print_Node("경로", Path);
}




void Astar_Honoi(Hanoi_node Root)
{
	Node* N_Open = new Node;
	Node* N_Close = new Node;
	Node* N_NextLevelNodeen = new Node;

	N_Open->next = nullptr;
	N_Close->next = nullptr;
	N_NextLevelNodeen->next = nullptr;

	N_Close->InsertNode(Root);
	for (auto node : Root.possible_NextNode()) {
		Node* New = new Node;
		New->v = node;
		New->parent = N_Close->next;
		N_Open->InsertNode(New);
	}

	Print_Node("vOpen", N_Open);
	Print_Node("vClose", N_Close);
	while (true) {

		for (auto next : N_Open->next->v.possible_NextNode()) {
			if (next.getHs() == 0) {
				print_Answer(next, N_Open->next);
				return;
			}
			if (Check_Overlap(N_Open, N_Close, N_NextLevelNodeen, next, N_Open->next)) {
				Node* New = new Node;
				New->v = next;
				New->parent = N_Open->next;
				N_NextLevelNodeen->InsertNode(New);
			}
		}


		N_Open->MoveToNodeFront(N_Close);
		while (N_NextLevelNodeen->next != nullptr) {
			N_NextLevelNodeen->MoveToNodeFront(N_Open);
		}

		cout << "===========================" << endl;
		Print_Node("N_Open", N_Open);
		Print_Node("N_Close", N_Close);
		cout << "===========================" << endl;

	}
}

int main()
{
	Hanoi_node Root;

	Astar_Honoi(Root);
}