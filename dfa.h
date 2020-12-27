#pragma once
#ifndef DFAH
#define DFAH
#include <global.h>
#include <nfa.h>
struct Edge {
	Path path;
	int dfa_node;
};

const static int M = 10000;
const static int P = 1000000007;
void convertNFAtoDFA(NFA_Graph* graph);
struct DFA_Node {
	vector<int>NfaNode;
	vector<Edge>Edges;
	bool end;
	int hash();
	void expand();
	void outPut();
	void outPutNodeAndEdge(int id);
};
extern DFA_Node dfa_node[M];
extern int DFA_Graph_Count;

extern map<int, int>DFA_mp;
#endif