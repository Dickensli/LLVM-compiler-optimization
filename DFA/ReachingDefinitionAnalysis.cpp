#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/InstIterator.h"
#include <set>
#include <algorithm>
#include <string>
#include "231DFA.h"

using namespace llvm;
using namespace std;

namespace llvm {

class ReachingInfo : public Info {
public:
	set<unsigned> info;

	ReachingInfo() {}

	void print() {
		for (auto def : info) {
			errs() << def << '|';
		}
		errs() << '\n';
	}

	static bool equals(ReachingInfo * info1, ReachingInfo * info2) {
		return info1->info == info2->info;
	}

	static ReachingInfo* join(ReachingInfo * info1, ReachingInfo * info2, ReachingInfo * result) {
		result->info.insert(info1->info.begin(), info1->info.end());
		result->info.insert(info2->info.begin(), info2->info.end());
		return result;
	}

	static void join(ReachingInfo * info1, ReachingInfo * result) {
		result->info.insert(info1->info.begin(), info1->info.end());
	}

};

class ReachingDefinitionAnalysis : public DataFlowAnalysis<ReachingInfo, true> {
public:
	ReachingDefinitionAnalysis(ReachingInfo & bottom, ReachingInfo & initialState) :
		DataFlowAnalysis(bottom, initialState) {}

	void flowfunction(Instruction * I, std::vector<unsigned> & IncomingEdges,
		std::vector<unsigned> & OutgoingEdges,
		std::vector<ReachingInfo *> & Infos) {
		string op = I->getOpcodeName();
		int type;
		unsigned idx = this->InstrToIndex[I];
		map<string, int> typeMap = { { "alloca", 1 },{ "load", 1 },{ "getelementptr", 1 },
		{ "icmp", 1 },{ "fcmp", 1 },{ "selecct", 1 },
		{ "br", 2 },{ "switch", 2 },{ "store", 2 },
		{ "phi", 3 } };
		//Get type of op
		if (typeMap.count(op))
			type = typeMap[op];
		else if (I->isBinaryOp())
			type = 1;
		else
			type = 2;


		//incoming info
		ReachingInfo * curInfo = new ReachingInfo();
		for (unsigned src : IncomingEdges) {
			Edge edge = Edge(src, idx);
			ReachingInfo::join(this->EdgeToInfo[edge], curInfo);
		}

		//type 1
		if (type == 1) {
			ReachingInfo* current = new ReachingInfo();
			current->info = { idx };
			ReachingInfo::join(current, curInfo);
		}

		//type 3
		if (type == 3) {
			ReachingInfo* current = new ReachingInfo();
			Instruction * firstNonPhi = I->getParent()->getFirstNonPHI();
			unsigned firstNonPhiIdx = InstrToIndex[firstNonPhi];
			for (unsigned i = idx; i < firstNonPhiIdx; ++i) {
				current->info.insert(i);
			}
			ReachingInfo::join(current, curInfo);
		}

		for (size_t i = 0; i < OutgoingEdges.size(); ++i)
			Infos.push_back(curInfo);
	}
	
};

struct ReachingDefinitionAnalysisPass : public FunctionPass {
	static char ID;
	ReachingDefinitionAnalysisPass() : FunctionPass(ID) {}

	bool runOnFunction(Function &F) override {
		ReachingInfo bottom;
		ReachingInfo initialState;
		ReachingDefinitionAnalysis * rda = new ReachingDefinitionAnalysis(bottom, initialState);

		rda->runWorklistAlgorithm(&F);
		rda->print();

		return false;
	}
};// end of struct ReachingDefinitionAnalysisPass
}  // end of anonymous namespace

char ReachingDefinitionAnalysisPass::ID = 0;
static RegisterPass<ReachingDefinitionAnalysisPass> X("cse231-reaching", "Reaching definition Analysis",
	false /* Only looks at CFG */,
	false /* Analysis Pass */);