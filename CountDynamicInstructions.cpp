#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include <map>

using namespace llvm;
using namespace std;
 
namespace {
struct CountDynamicInstr : public FunctionPass {
  static char ID;
  CountDynamicInstr() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
	Module *module = F.getParent(); 
	Function *print = cast<Function>(module->getOrInsertFunction("printOutInstrInfo",
															Type::getVoidTy(module->getContext())));
	Function *update = cast<Function>(module->getOrInsertFunction("updateInstr",
															Type::getVoidTy(module->getContext()),
															Type::getInt32Ty(module->getContext()),
															Type::getInt32Ty(module->getContext())));
	for (BasicBlock &BB : F) {
			BasicBlock::iterator i = BB.end();
			IRBuilder<> builder(&*(--i));
			map<int, int> m;
			int flag = 0;
			for (Instruction &I : BB) {
				int code = I.getOpcode();
				m[code] += 1;
				if (code == 1) {
					flag = 1;
					break;
				}
			}
			for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it) {
				vector<Value *> v = {builder.getInt32(it->first), builder.getInt32(it->second)};
				builder.CreateCall(update, v);
			}
			if (flag == 1) 
				builder.CreateCall(print);
	}
    return true;
    }
}; // end of struct TestPass
}  // end of anonymous namespace

char CountDynamicInstr::ID = 0;
static RegisterPass<CountDynamicInstr> X("cse231-cdi", "Developed to test LLVM and docker",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

