#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"

using namespace llvm;

namespace {
    class OurDSEPass : public FunctionPass {
    public:
        static char ID;
        std::unordered_map<Value*, StoreInst*> ptr2inst; // Pointer value -> instruction
        std::unordered_map<StoreInst*, int> inst2num; // Instruction -> number of uses for a value
        std::vector<StoreInst*> instsDelete; // Worklist of instructions to delete

        OurDSEPass() : FunctionPass(ID) {}

        bool runOnFunction(Function &F) override {

            bool changed=false; //indicates if changes to IR were made
            for(auto &BB : F) {
                for(auto &inst : BB) {
                     if(StoreInst* si = dyn_cast<StoreInst>(&inst)) { //consider only store instructions
                         Value * val = si->getValueOperand(); //actual number
                         Value* ptrval = si->getPointerOperand(); //pointer value, doesnt change after alloca

                         int numUses = val->getNumUses();
                         inst2num[si] = numUses;

                         if(ptr2inst.find(ptrval) == ptr2inst.end()) { //if not in map, add it
                             ptr2inst[ptrval] = si;
                         }
                         else { // in map -> 2nd or later store instruction for same pointer, consider deletion
                             StoreInst* candidate = ptr2inst[ptrval];
                             // usage number 1 means the value was never used outside of store.
                             // check if it's the same value as previous store (i.e. two lines of int x=5)
                             if (inst2num[candidate]==1 || candidate->getValueOperand()==val) {
                                 instsDelete.push_back(candidate);
                             }
                             ptr2inst[ptrval] = si;
                         }
                     }
                }
            }
            //delete selected instructions
            for(StoreInst * inst : instsDelete) {
                inst->eraseFromParent();
                changed=true;
            }

            return changed;
            //NOTE: C code (which you run passes on) returns 0 in main. IR always allocates value 0 to a pointer value %1, so in theory with this
            // implementation you should make an edge case when the value stored is 0. (decrement twice when checking, once for return value, once for IR start)
        }
    };
}


char OurDSEPass::ID = 0;
static RegisterPass<OurDSEPass> X("our-dse", "our dead store elimination");
