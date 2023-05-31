#ifndef LLVM_PROJECT_postDomTree_H
#define LLVM_PROJECT_postDomTree_H

#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/CFG.h"

#include <unordered_map>
#include <vector>
#include <unordered_set>
#include <climits>

using namespace llvm;

class postDomTree {
private:
  std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> adjacencyList;
  std::unordered_map<BasicBlock*, std::vector<BasicBlock*>> reverseAdjacencyList;
  std::vector<BasicBlock*> visitedOrder;
  std::unordered_map<BasicBlock*, int> visitedTime;
  std::unordered_map<BasicBlock*, BasicBlock*> Sdom;
  std::unordered_map<BasicBlock*, BasicBlock*> Idom;
  std::unordered_map<BasicBlock*, BasicBlock*> ancestors;
  std::unordered_map<BasicBlock*, BasicBlock*> parents;
  std::unordered_set<BasicBlock*> visited;

  std::string functionName;
  int time;

  void addEdge(BasicBlock*, BasicBlock*);
  void dfs(BasicBlock*);
  void semiDominators();
  BasicBlock* semiDominator(BasicBlock *);
  void semiDominatorCandidates(BasicBlock*, BasicBlock*, std::vector<BasicBlock*>&);
  void immediateDominators();
public:
  postDomTree(Function &F);
  void dumpToFile();
};

#endif // LLVM_PROJECT_postDomTree_H
