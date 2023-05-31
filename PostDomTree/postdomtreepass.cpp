#include "llvm/ADT/Statistic.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"

#include "postdomtree.h"

using namespace llvm;

namespace {

struct postDomTreePass : public FunctionPass {
  static char ID;
  postDomTreePass() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    postDomTree* domTree = new postDomTree(F);
    domTree->dumpToFile();

    delete domTree;
    return false;
  }
};
}

char postDomTreePass::ID = 0;
static RegisterPass<postDomTreePass> X("our-postdominator-tree", "Our dominator tree pass");