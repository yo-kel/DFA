#include <dfa.h>




bool mark[M];//标记子集
bool mark_path[M][50];//标记NFA路径，默认每个节点的路径不超过50个




int DFA_Node::hash() {
	sort(NfaNode.begin(), NfaNode.end());
	LL res = 0;
	for (int i = 0;i < NfaNode.size();i++) {
		res = (res * P + NfaNode[i]) % P;
	}
	return (int)res;
}
void DFA_Node::expand() {
	bool mark[M];
	memset(mark, 0, sizeof(mark));
	for (int i = 0;i < NfaNode.size();i++)mark[NfaNode[i]] = 1;
	for (int i = 0;i < NfaNode.size();i++) {
		int id = NfaNode[i];
		//printf("(%d)\n", id);
		for (int k = 0;k < nfa_nodes[id]->PathSet.size();k++) {
			Path path = nfa_nodes[id]->PathSet[k];
			if (path.type != 'E')continue;
			int pathId = nfa_nodes[id]->findEdge(path);
			//printf("->>%d\n", pathId);
			if (pathId == -1)continue;
			for (int j = 0;j < nfa_nodes[id]->mp[pathId].size();j++) {
				int nodeId = nfa_nodes[id]->mp[pathId][j]->id;
				if (mark[nodeId])continue;
				//printf("....%d\n", nodeId);
				mark[nodeId] = 1;
				NfaNode.push_back(nodeId);
			}
		}
	}
	sort(NfaNode.begin(), NfaNode.end());
}
void DFA_Node::outPut() {
	puts("-----------------");
	for (int i = 0;i < NfaNode.size();i++) {
		printf("%d ", NfaNode[i]);
	}
	puts("");
	for (int i = 0;i < Edges.size();i++) {
		printf("%d %c ", Edges[i].dfa_node, Edges[i].path.type);
		cout << Edges[i].path.charSet << endl;
	}
	puts("-----------------");
}

void DFA_Node::outPutNodeAndEdge(int id) {
	for (int i = 0;i < Edges.size();i++) {
		string value{ Edges[i].path.type };
		if (Edges[i].path.type != 'S')value = "<"+value+">";
		else value = Edges[i].path.charSet;
		printf("%d %d ", id,Edges[i].dfa_node);
		cout << value << endl;
	}

}

DFA_Node dfa_node[M];
int DFA_Graph_Count = 0;

map<int, int>DFA_mp;


void convertNFAtoDFA(NFA_Graph* graph) {
	DFA_Node start;
	start.NfaNode.push_back(graph->startNode->id);
	start.expand();
	dfa_node[DFA_Graph_Count++]=start;
	DFA_mp[dfa_node[0].hash()] = DFA_Graph_Count - 1;
	for(int i=0;i<=DFA_Graph_Count;i++){
		DFA_Node &tmp = dfa_node[i];
		for (int k = 0;k < tmp.NfaNode.size();k++) {
			int nfa_id = tmp.NfaNode[k];
			for (int j = 0;j < nfa_nodes[nfa_id]->PathSet.size();j++) {
				if (mark_path[nfa_id][j])continue;
				Path path = nfa_nodes[nfa_id]->PathSet[j];
				DFA_Node result;
				if (path.type == 'E')continue;
				for (int t1 = 0;t1 < tmp.NfaNode.size();t1++) {
					int nfa_id = tmp.NfaNode[t1];
					for (int t2 = 0;t2 < nfa_nodes[nfa_id]->PathSet.size();t2++) {
						if (mark_path[nfa_id][t2] || !(nfa_nodes[nfa_id]->PathSet[t2] == path))continue;
						vector<NFA_Node*>to_nodes = nfa_nodes[nfa_id]->mp[nfa_nodes[nfa_id]->findEdge(path)];
						for (int t3 = 0;t3 < to_nodes.size();t3++) {
							result.NfaNode.push_back(to_nodes[t3]->id);
						}
					}
				}
				int hash = result.hash();
				int p;
				if (DFA_mp.find(hash) != DFA_mp.end()) {
					p = DFA_mp[hash];
					//if (p==tmp.hash())puts("YES");
				}
				else {
					dfa_node[DFA_Graph_Count] = result;
					dfa_node[DFA_Graph_Count].expand();
					DFA_mp[hash] = DFA_Graph_Count;
					p = DFA_Graph_Count;
					DFA_Graph_Count++;
				}
				tmp.Edges.push_back(Edge{ path,p });
			}
		}
	}
}

