#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Analysis/CFG.h"
#include<unordered_set>
using namespace llvm;

namespace {
    class OurDSEPass : public FunctionPass {
    public:
        static char ID;
        std::unordered_map<Value*, StoreInst*> ptr2inst; // Pointer value -> instruction
        std::vector<StoreInst*> instsDelete; // Worklist of instructions to delete
        std::unordered_set<Value*> unsafeptr; // set of pointer values that are used in load instructions

        OurDSEPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {

            bool changed=false; //indicates if changes to IR were made
            for(auto &BB : F) {
                //without this line, stores before if else branches would get deleted, without considering if we're gonna enter those basic blocks during runtime
                ptr2inst.clear();
                unsafeptr.clear();
                for(auto &inst : BB) {

                     if(StoreInst* si = dyn_cast<StoreInst>(&inst)) { //consider only store instructions
                         Value* ptrval = si->getPointerOperand(); //pointer value, doesnt change after alloca

                         if(ptr2inst.find(ptrval) == ptr2inst.end()) { //if not in map, add it
                             ptr2inst[ptrval] = si;
                         }
                         else { // not the first store to the pointer
                             //fetch previous store inst that assigned value to the already seen pointer
                             StoreInst* candidate = ptr2inst[ptrval];
                             if(unsafeptr.find(ptrval)==unsafeptr.end()) {
                                 instsDelete.push_back(candidate);

                             }
                             //ptr is not safe currently
                             else {
                                 unsafeptr.erase(ptrval);
                             }

                             ptr2inst[ptrval] = si;
                         }
                     }
                     else if(LoadInst* li = dyn_cast<LoadInst>(&inst)) {
                         Value* loadptr = li->getPointerOperand();

                         unsafeptr.insert(loadptr);
                     }

                }
            }
            //delete selected instructions
            for(StoreInst * inst : instsDelete) {
                inst->eraseFromParent();
                changed=true;
            }

            return changed;
        }
    };
}


char OurDSEPass::ID = 0;
static RegisterPass<OurDSEPass> X("our-dse", "our dead store elimination");

