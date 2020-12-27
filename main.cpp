
#include <dfa.h>

const int regexCnt = 3;

string regex[3] = {
	"[123a-b]*c?(ab)+\\d",
	"[123]*",
	"\d*[cx]"
};

NFA_Graph* MergeGraph() {
	NFA_Graph* result= regexToNFA(regex[0]);
	result->endNode->setEndNode(1);
	//return result;//TODO 
	for (int i = 1;i < regexCnt;i++) {
		NFA_Graph* graph = regexToNFA(regex[i]);
		graph->endNode->setEndNode(i + 1);
		result->addExtraGraph(graph);
	}
	return result;
}

int main() {

	NFA_Graph* graph = MergeGraph();
	/*for (int i = 0;i < nfa_nodes.size();i++) {
		printf("%d\n", nfa_nodes[i]->id);
	}*/
	convertNFAtoDFA(graph);
	for (int i = 0;i < DFA_Graph_Count;i++) {
		//printf("->>%d\n", i);
		dfa_node[i].outPutNodeAndEdge(i);
	}
	//graph->endNode->setEndNode();
	return 0;
}