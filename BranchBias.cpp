#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include <vector>

using namespace llvm;
using namespace std;
 
namespace {
struct Branch : public FunctionPass {
  static char ID;
  Branch() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
	Module *module = F.getParent(); 
	Function *print = cast<Function>(module->getOrInsertFunction("printOutBranchInfo",
															Type::getVoidTy(module->getContext())));
	Function *update = cast<Function>(module->getOrInsertFunction("updateBranchInfo",
															Type::getVoidTy(module->getContext()),
															Type::getInt1Ty(module->getContext())));
	for (BasicBlock &BB : F) {
			BasicBlock::iterator i = BB.end();
			IRBuilder<> builder(&*(--i));
			int flag = 0;
			for (Instruction &I : BB) {
				int code = I.getOpcode();
				if (code == 2 && I.getNumOperands() > 1) {
					vector<Value *> v = {I.getOperand(0)};
					builder.CreateCall(update, v);
				}
				if (code == 1) {
					flag = 1;
					break;
				}
			}
			if (flag == 1) 
				builder.CreateCall(print);
	}
    return true;
    }
}; // end of struct TestPass
}  // end of anonymous namespace

char Branch::ID = 0;
static RegisterPass<Branch> X("cse231-bb", "Developed to test LLVM and docker",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

