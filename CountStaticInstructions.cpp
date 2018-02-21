#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include <map>

using namespace llvm;
using namespace std;
 
namespace {
struct PA1 : public FunctionPass {
  static char ID;
  PA1() : FunctionPass(ID) {}

  bool runOnFunction(Function &F) override {
    map<string,int> m;
    for (inst_iterator I = inst_begin(F), E = inst_end(F); I != E; ++I){
	m[I -> getOpcodeName()] += 1;
    }
    for (map<string,int>::iterator it=m.begin(); it!=m.end();++it){
	errs() << it -> first << "\t" << it -> second << "\n";
    }
    return false;
  }
}; // end of struct TestPass
}  // end of anonymous namespace

char PA1::ID = 0;
static RegisterPass<PA1> X("cse231-csi", "Developed to test LLVM and docker",
                             false /* Only looks at CFG */,
                             false /* Analysis Pass */);

