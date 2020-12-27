#pragma once
#ifndef NFAH
#define NFAH

#include <global.h>


struct Path {
	char type;
	bool signal;
	string charSet;

	Path();
	Path(char type, string charSet, bool signal);

	bool operator == (const Path& p)const;
	void setSignal(bool signal);
	void setCharSet(string charSet);
	bool isMatch(char c);
};
class NFA_Node {
	public :
	static int idCnt;
	int id;
	int isEnd;
	vector<Path>PathSet;
	map<int, vector<NFA_Node*>>mp;
	NFA_Node();
	int findEdge(Path path);
	void addEdge(Path path, NFA_Node* node);
	void setEndNode(int x);
};

extern vector<NFA_Node*>nfa_nodes;

class NFA_Graph {
public:
	NFA_Node* startNode;
	NFA_Node* endNode;
	NFA_Graph();
	NFA_Graph(NFA_Node* s, NFA_Node* e);
	void addParallelGraph(NFA_Graph* g);
	void addSeriesGraph(NFA_Graph* g);
	void addExtraGraph(NFA_Graph* g);
	void StoE();
	void repeatStar();
	void repeatPlus();
};
NFA_Graph* regexToNFA(string regex);
#endif // !GLOBALH