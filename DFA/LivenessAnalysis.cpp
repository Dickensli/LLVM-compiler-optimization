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

class LivenessInfo : public Info {
public:
	set<unsigned> info;

	LivenessInfo() {}

	void print() {
		for (auto def : info) {
			errs() << def << '|';
		}
		errs() << '\n';
	}

	static bool equals(LivenessInfo * info1, LivenessInfo * info2) {
		return info1->info == info2->info;
	}

	static LivenessInfo* join(LivenessInfo * info1, LivenessInfo * info2, LivenessInfo * result) {
		result->info.insert(info1->info.begin(), info1->info.end());
		result->info.insert(info2->info.begin(), info2->info.end());
		return result;
	}

	static void join(LivenessInfo * info1, LivenessInfo * result) {
		result->info.insert(info1->info.begin(), info1->info.end());
	}

};


class LivenessAnalysis : public DataFlowAnalysis<LivenessInfo, false> {
public:
	LivenessAnalysis(LivenessInfo & bottom, LivenessInfo & initialState) :
		DataFlowAnalysis(bottom, initialState) {}

	void addOperands(Instruction *I, LivenessInfo *liveinfo) {
		for (unsigned i = 0; i < I->getNumOperands(); ++i) {
			Instruction *operand = (Instruction *)I->getOperand(i);
			if (this->InstrToIndex.find(operand) != this->InstrToIndex.end()) {
				liveinfo->info.insert(this->InstrToIndex[operand]);
			}
		}

	}

	void flowfunction(Instruction * I, std::vector<unsigned> & IncomingEdges,
		std::vector<unsigned> & OutgoingEdges,
		std::vector<LivenessInfo *> & Infos) {

		string op = I->getOpcodeName();
		int type;
		unsigned idx = this->InstrToIndex[I];
		set<unsigned> operands;
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
		LivenessInfo * curInfo = new LivenessInfo();
		for (unsigned src : IncomingEdges) {
			Edge edge = Edge(src, idx);
			LivenessInfo::join(this->EdgeToInfo[edge], curInfo);
		}

		//type 1
		if (type == 1) {
			curInfo->info.erase(idx);
			this->addOperands(I, curInfo);
			for (size_t i = 0; i < OutgoingEdges.size(); ++i)
				Infos.push_back(curInfo);
		}

		//type 2
		if (type == 2) {
			this->addOperands(I, curInfo);
			for (size_t i = 0; i < OutgoingEdges.size(); ++i)
				Infos.push_back(curInfo);
		}

		//type 3
		if (type == 3) {
			BasicBlock *block = I->getParent();
			//erase all new assigned variable
			for (auto ii = block->begin(), ie = block->end(); ii != ie; ++ii) {
				Instruction *instr = &*ii;
				if (isa<PHINode>(instr))
					curInfo->info.erase(this->InstrToIndex[instr]);
			}

			Instruction *firstNonPhi = I->getParent()->getFirstNonPHI();
			unsigned fNPIdx = this->InstrToIndex[firstNonPhi];

			//get output for each outgoingedge
			for (size_t k = 0; k < OutgoingEdges.size(); ++k) {
				LivenessInfo *kthOut = new LivenessInfo();

				//enumerate all label_ij
				for (unsigned i = idx; i < fNPIdx; ++i) {
					Instruction *instr = this->IndexToInstr[i];
					for (unsigned j = 0; j < instr->getNumOperands(); ++j) {
						Instruction *operand = (Instruction *)instr->getOperand(j);
						if (this->InstrToIndex.find(operand) == this->InstrToIndex.end())
							continue;
						if (operand->getParent() == this->IndexToInstr[OutgoingEdges[k]]->getParent()) {
							kthOut->info.insert(this->InstrToIndex[operand]);
							break;
						}
					}
				}
				LivenessInfo::join(curInfo, kthOut);
				Infos.push_back(kthOut);
			}
		}
	}
	
};

struct LivenessAnalysisPass : public FunctionPass {
	static char ID;
	LivenessAnalysisPass() : FunctionPass(ID) {}

	bool runOnFunction(Function &F) override {
		LivenessInfo bottom;
		LivenessInfo initialState;
		LivenessAnalysis * rda = new LivenessAnalysis(bottom, initialState);

		rda->runWorklistAlgorithm(&F);
		rda->print();

		return false;
	}
};// end of struct ReachingDefinitionAnalysisPass
}  // end of anonymous namespace

char LivenessAnalysisPass::ID = 0;
static RegisterPass<LivenessAnalysisPass> X("cse231-liveness", "Liveness Analysis",
	false /* Only looks at CFG */,
	false /* Analysis Pass */);