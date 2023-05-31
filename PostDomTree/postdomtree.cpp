#include "postdomtree.h"

void postDomTree::addEdge(BasicBlock *from, BasicBlock *to)
{
  adjacencyList[from].push_back(to);
  reverseAdjacencyList[to].push_back(from);
}

postDomTree::postDomTree(Function &F)
{
  functionName = F.getName().str();
  time = 1;

  for (BasicBlock& BB : F) {
    adjacencyList[&BB] = {};
    for (BasicBlock* predecessor : predecessors(&BB)) {
      addEdge(&BB, predecessor);
    }
  }

  dfs(&F.back());

  semiDominators();

  immediateDominators();
}

void postDomTree::dfs(BasicBlock* current)
{
  visited.insert(current);
  visitedTime[current] = time;
  time++;
  visitedOrder.push_back(current);

  for (BasicBlock* predecessor : adjacencyList[current]) {
    if (visited.find(predecessor) == visited.end()) {
      parents[predecessor] = current;
      dfs(predecessor);
    }
  }
}

void postDomTree::semiDominators()
{
  std::reverse(visitedOrder.begin(), visitedOrder.end());

  for (BasicBlock* current : visitedOrder) {
    Sdom[current] = semiDominator(current);
  }
}

BasicBlock* postDomTree::semiDominator(BasicBlock *current)
{
  visited.clear();
  std::vector<BasicBlock*> candidates;

  semiDominatorCandidates(current, current, candidates);

  int min = INT_MAX;
  BasicBlock* semiDominator = nullptr;

  for (BasicBlock* candidate : candidates) {
    if (visitedTime[candidate] < min) {
      min = visitedTime[candidate];
      semiDominator = candidate;
    }
  }

  return semiDominator;
}

void postDomTree::semiDominatorCandidates(BasicBlock *start, BasicBlock *current,
                                                   std::vector<BasicBlock *> &candidates)
{
  visited.insert(current);

  if (visitedTime[current] < visitedTime[start]) {
    candidates.push_back(current);
    return;
  }

  for (BasicBlock* ancestor : reverseAdjacencyList[current]) {
    if (visited.find(ancestor) == visited.end()) {
      semiDominatorCandidates(start, ancestor, candidates);
    }
  }
}

void postDomTree::immediateDominators()
{
  BasicBlock* parent;
  int min;

  for (BasicBlock* current : visitedOrder) {
    min = visitedTime[Sdom[current]];
    parent = parents[current];
    BasicBlock* ancestor = nullptr;

    while (parent != Sdom[current]) {
      if (visitedTime[Sdom[parent]] < min) {
        min = visitedTime[Sdom[parent]];
        ancestor = parent;
      }
      parent = parents[parent];
    }

    ancestors[current] = ancestor;
  }

  std::reverse(visitedOrder.begin(), visitedOrder.end());

  for (BasicBlock* current : visitedOrder) {
    if (ancestors[current] == nullptr) {
      Idom[current] = Sdom[current];
    }
    else {
      Idom[current] = Idom[ancestors[current]];
    }
  }
}

void postDomTree::dumpToFile()
{
  std::error_code error;
  std::string fileName = "postdomtree." + functionName + ".dot";
  raw_fd_ostream file(fileName, error);

  file << "digraph \"Post Dominator tree for '" << functionName << "' function\" {\n"
          "\tlabel=\"Post Dominator tree for '" << functionName << "' function\";\n\n";

  for (BasicBlock* current : visitedOrder) {
    file << "\tNode" << current << " [shape=record,label=\"{";
    current->printAsOperand(file, false);
    file << "}\"];\n";

    file << "\tNode" << Idom[current] << " -> Node" << current << ";\n";
  }
  file << "}\n";
  file.close();
}
