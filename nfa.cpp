#include <nfa.h>

int token;
int line = 1;

char* src;

vector<NFA_Node*>nfa_nodes;
Path::Path() {
	this->type = 'E';
	this->signal = false;
	this->charSet = "";
}
Path::Path(char type,string charSet="",bool signal=false) {
	this->type = type;
	this->charSet = charSet;
	this->signal = signal;
}
bool Path::operator == (const Path& p)const {
	return type == p.type && signal == p.signal && charSet == p.charSet;
}
void Path::setSignal(bool signal) {
	signal = signal;
}
void Path::setCharSet(string charSet) {
	if (charSet[0] == '^')setSignal(false);
	this->charSet = charSet.substr(1,charSet.length()-1);
}
bool Path::isMatch(char c) {
	bool res = false;
	switch (type) {
	case 'D'://数字边
		res = c >= '0' && c <= '9';
		if (!signal)res = !res;
		break;
	case 'E':
		res = true;
		break;
	case 'W':
		res = (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
		if (!signal)res = !res;
		break;
	case 'S':
		int lenth = charSet.length();
		for (int i = 0;i < lenth;i++) {
			if (charSet[i] == '^')continue;
			if (charSet[i] == '-') {
				if (charSet[i - 1] <= c && c <= charSet[i + 1]) {
					res = true;
					break;
				}
				i++;
			}
			else if (charSet[i] == c) {
				res = true;
				break;
			}
		}
		if (!signal)res = !res;
		break;
	}
	return true;
}
//不支持重复{n,m}次


//class NFA_Node {
//	public:
//	static int idCnt;
//	int id;
//	bool isEnd;
//	vector<Path>PathSet;
//	map<int, vector<NFA_Node*>>mp;
NFA_Node::NFA_Node() {
	this->id = this->idCnt++;
	this->isEnd = false;
	nfa_nodes.push_back(this);
}
int NFA_Node::findEdge(Path path) {
	for (int i = 0;i < PathSet.size();i++) {
		if (PathSet[i] == path)return i;
	}
	return -1;
}
void NFA_Node::addEdge(Path path, NFA_Node* node) {
	printf("%d--%c--%d  ", this->id, path.type, node->id);
	cout << path.charSet << endl;
	int index = findEdge(path);
	if (index == -1) {
		PathSet.push_back(path);
		vector<NFA_Node*>tmp;
		mp[PathSet.size()-1] = tmp;
		index = PathSet.size() - 1;
	}
	mp[index].push_back(node);
}

void NFA_Node::setEndNode(int x) {
	isEnd = x;
}

int NFA_Node:: idCnt=0;

NFA_Graph::NFA_Graph() {
	this->startNode = NULL;
	this->endNode = NULL;
}
NFA_Graph::NFA_Graph(NFA_Node* s, NFA_Node* e) {
	startNode = s;
	endNode = e;
}

void NFA_Graph::addParallelGraph(NFA_Graph* g) {
	NFA_Node* newStart = new NFA_Node;
	NFA_Node* newEnd = new NFA_Node;
	Path path;
	newStart->addEdge(path, this->startNode);
	newStart->addEdge(path, g->startNode);
	this->endNode->addEdge(path, newEnd);
	g->endNode->addEdge(path, newEnd);
	this->startNode = newStart;
	this->endNode = newEnd;
}

void NFA_Graph::addExtraGraph(NFA_Graph* g) {
	NFA_Node* newStart = new NFA_Node;
	Path path;
	newStart->addEdge(path, this->startNode);
	newStart->addEdge(path, g->startNode);
	this->startNode = newStart;
}

void NFA_Graph::addSeriesGraph(NFA_Graph* g) {
	Path path;
	this->endNode->addEdge(path, g->startNode);
	this->endNode = g->endNode;
}
//?
void NFA_Graph::StoE() {
	Path path;
	this->startNode->addEdge(path, this->endNode);
}

//*
void NFA_Graph::repeatStar() {
	repeatPlus();
	Path path;
	this->startNode->addEdge(path, this->endNode);
}

//+
void NFA_Graph::repeatPlus() {
	NFA_Node* newStart = new NFA_Node;
	NFA_Node* newEnd = new NFA_Node;
	Path path;
		
	newStart->addEdge(path, this->startNode);
		
	this->endNode->addEdge(path, newEnd);
	this->endNode->addEdge(path, this->startNode);

	this->startNode = newStart;
	this->endNode = newEnd;
}


void NFA_ReapeatConvert(char x, NFA_Graph* g) {
	switch (x) {
	case '+':
		g->repeatPlus();
		break;
	case '?':
		g->StoE();
		break;
	case '*':
		g->repeatStar();
		break;
	}
}

char handleRepeat(string regex,int &k) {
	int lenth = regex.length();
	if ((k + 1 < lenth) && (regex[k + 1] == '+' || regex[k + 1] == '?' || regex[k + 1] == '*')) {
		k++;
		return regex[k];
	}
	return '0';
}

NFA_Graph* regexToNFA(string regex) {
	//括号匹配没有预处理，效率不高
	NFA_Graph* res=NULL;
	int lenth = regex.length();
	for (int i = 0;i < lenth;i++) {
		char tmp = 0;
		int k;
		int cnt;
		NFA_Graph* newGraph;
		Path* path = NULL;
		switch (regex[i]) {
		case '(':
			cnt = 1, k = i+1;
			while (k < lenth) {
				if (regex[k] == ')')cnt--;
				else if (regex[k] == '(')cnt++;
				if (cnt == 0)break;
				k++;
			}
			newGraph = regexToNFA(regex.substr(i + 1, k - i - 1));
			tmp = handleRepeat(regex, k);
			if (tmp != '0')NFA_ReapeatConvert(tmp, newGraph);
			if (res==NULL) {
				res = newGraph;
			}
			else res->addSeriesGraph(newGraph);
			i = k;
			break;
		case '|':
			newGraph = regexToNFA(regex.substr(i + 1, lenth - i - 1));
			if (res==NULL) {
				res = newGraph;
			}
			else res->addParallelGraph(newGraph);
			i = lenth;
			break;
		case '[':
			k = i;
			while (regex[k] != ']')k++;
			path = new Path('S',regex.substr(i + 1, k - i - 1));
			//printf("%d %d\n", i, k);
			i = k;
			break;
		case '\\':
			switch (regex[++i]) {
			case 'd':
				path = new Path('D');
				break;
			case 'w':
				path = new Path('W');
				break;
			}
			break;
		default: 
			path = new Path('S',regex.substr(i, 1));
			//if (path != NULL)printf("->>%c\n", path->type);
		}
		if (path != NULL) {
			//printf("(%c)\n", path->type);
			NFA_Node* start = new NFA_Node();
			NFA_Node* end = new NFA_Node();
			start->addEdge(*path, end);
			newGraph = new NFA_Graph(start,end);

			tmp = handleRepeat(regex, i);
			//printf("->>%d\n", i);
			if (tmp != '0')NFA_ReapeatConvert(tmp, newGraph);

			if (res == NULL)res = newGraph;
			else {
				res->addSeriesGraph(newGraph);
			}
		}
	}
	return res;
}



//void next() {
//	char* las_pos;
//	int hash;
//	int isComment = 0;
//	while (token = *src) {
//		++src;
//		switch (token) {
//		case '\n':
//			line++;
//			break;
//		case '\\':
//
//		}
//	}
//}

