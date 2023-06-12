#include "llvm/IR/Function.h"
#include "llvm/IR/PassManager.h"
#include "llvm/Pass.h"
#include "llvm/IR/Operator.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Instruction.h"
#include<unordered_set>
#include "llvm/IR/Module.h"
using namespace llvm;

namespace {
    class OurDGEPass : public ModulePass {
    public:
        static char ID;
        std::vector<GlobalVariable*> deletable; //variables to be deleted

        OurDGEPass() : ModulePass(ID) {}

        bool isReachable(GlobalVariable* gv) {

            //recursively check if global var is contained in instructions
            for(User* u : gv->users()) {
                if(Instruction *inst = dyn_cast<Instruction>(u)) {
                    //for(auto vstart=inst->op_begin(), vend=inst->op_end(); vstart!=vend; vstart++ ) {
                        //if(GlobalVariable* curr = dyn_cast<GlobalVariable>(vstart)) {
                            return true;
                        //}
                    //}
                }
            }
            return false;
        }

        bool runOnModule(Module &M) override {

            bool changed=false; //indicates if changes to IR were made
            GlobalVariable* gv =nullptr;
            for(Module::global_iterator begin = M.global_begin(), end=M.global_end(); begin!=end; begin++) {
                gv = &(*begin);
                if((gv->getName()).starts_with("."))
                    continue;
                //ignore iteration if global is reachable
                if(isReachable(gv)) {
                    continue;
                }
                deletable.push_back(gv);
                changed=true;

            }
            for(GlobalVariable* elem : deletable) {
                M.eraseGlobalVariable(elem);
            }

            return changed;
        }

    };
}


char OurDGEPass::ID = 0;
static RegisterPass<OurDGEPass> X("our-dge", "our dead global elimination");
