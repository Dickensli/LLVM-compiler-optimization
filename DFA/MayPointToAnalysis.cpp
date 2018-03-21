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
	typedef pair<char, unsigned> pointerInfo;

	class MayPointToInfo : public Info {
	public:
		map<pointerInfo, set<pointerInfo>> info;

		MayPointToInfo() {}

		void print() {
			for (auto &pointer : info) {
				errs() << pointer.first.first << pointer.first.second << "->(";
				for (auto &pointee : pointer.second) {
					errs() << pointee.first << pointee.second << "/";
				}
				errs() << ")|";
			}
			errs() << "\n";
		}

		static bool equals(MayPointToInfo * info1, MayPointToInfo * info2) {
			auto l = info1->info;
			auto r = info2->info;
			return l.size() == r.size() && std::equal(l.begin(), l.end(), r.begin());
		}

		static MayPointToInfo* join(MayPointToInfo * info1, MayPointToInfo * info2, MayPointToInfo * result) {
			result->info.insert(info1->info.begin(), info1->info.end());
			for (auto pair : info2->info) {
				for (auto pointee : pair.second) {
					result->addSingleInfo(pair.first, pointee);
				}
			}
			return result;
		}

		static void join(MayPointToInfo * info1, MayPointToInfo * result) {
			for (auto pair : info1->info) {
				for (auto pointee : pair.second) {
					result->addSingleInfo(pair.first, pointee);
				}
			}
		}

		void addSingleInfo(pointerInfo pointer, pointerInfo pointee) {
			auto it = this->info.find(pointer);
			if (it != this->info.end()) {
				it->second.insert(pointee);
			}
			else
				this->info[pointer].insert(pointee);
		}

	};


	class MayPointToAnalysis : public DataFlowAnalysis<MayPointToInfo, true> {
	public:
		MayPointToAnalysis(MayPointToInfo & bottom, MayPointToInfo & initialState) :
			DataFlowAnalysis(bottom, initialState) {}

		void flowfunction(Instruction * I, std::vector<unsigned> & IncomingEdges,
			std::vector<unsigned> & OutgoingEdges,
			std::vector<MayPointToInfo *> & Infos) {

			string op = I->getOpcodeName();
			int type;
			unsigned idx = this->InstrToIndex[I];
			pointerInfo Ri = make_pair('R', idx);
			map<string, int> typeMap = { { "alloca", 1 },{ "load", 4 },{ "getelementptr", 3 },
			{ "select", 6 },{ "store", 5 },{ "phi", 7 },{ "bitcast", 2 } };
			//Get type of op
			if (typeMap.count(op))
				type = typeMap[op];

			//incoming info
			MayPointToInfo * curInfo = new MayPointToInfo();
			for (unsigned src : IncomingEdges) {
				Edge edge = Edge(src, idx);
				MayPointToInfo::join(this->EdgeToInfo[edge], curInfo);
			}

			//if not pointer type or store
			if (!I->getType()->isPointerTy() && strcmp(I->getOpcodeName(), "store")) {
				for (unsigned i = 0; i < OutgoingEdges.size(); i++) {
					Infos.push_back(curInfo);
				}
				return;
			}

			//alloca
			if (type == 1) {
				pointerInfo Mi = make_pair('M', idx);
				curInfo->addSingleInfo(Ri, Mi);
			}

			//getelementptr or bitcast
			if (type == 2 or type == 3) {
				pointerInfo Rv = make_pair('R', this->InstrToIndex[dyn_cast<Instruction>(I->getOperand(0))]);
				if (curInfo->info.count(Rv) > 0) {
					for (auto x : curInfo->info[Rv]) {
						curInfo->addSingleInfo(Ri, x);
					}
				}
			}

			//load
			if (type == 4) {
				pointerInfo Rp = make_pair('R', this->InstrToIndex[dyn_cast<Instruction>(I->getOperand(0))]);
				if (curInfo->info.count(Rp) > 0) {
					for (auto x : curInfo->info[Rp]) {
						if (curInfo->info.count(x) > 0) {
							for (auto y : curInfo->info[x]) {
								curInfo->addSingleInfo(Ri, y);
							}
						}
					}
				}
			}

			//store
			if (type == 5) {
				pointerInfo Rv = make_pair('R', this->InstrToIndex[dyn_cast<Instruction>(I->getOperand(0))]);
				pointerInfo Rp = make_pair('R', this->InstrToIndex[dyn_cast<Instruction>(I->getOperand(1))]);
				if (curInfo->info.count(Rv) > 0) {
					for (auto x : curInfo->info[Rv]) {
						if (curInfo->info.count(Rp) > 0) {
							for (auto y : curInfo->info[Rp]) {
								curInfo->addSingleInfo(y, x);
							}
						}
					}
				}
			}

			//select
			if (type == 6) {
				pointerInfo Rt = make_pair('R', this->InstrToIndex[dyn_cast<Instruction>(I->getOperand(1))]);
				pointerInfo Rf = make_pair('R', this->InstrToIndex[dyn_cast<Instruction>(I->getOperand(2))]);
				if (curInfo->info.count(Rt) > 0) {
					for (auto x : curInfo->info[Rt]) {
						curInfo->addSingleInfo(Ri, x);
					}
				}
				if (curInfo->info.count(Rf) > 0) {
					for (auto x : curInfo->info[Rf]) {
						curInfo->addSingleInfo(Ri, x);
					}
				}
			}

			//type 7
			if (type == 7) {
				Instruction *firstNonPhi = I->getParent()->getFirstNonPHI();
				unsigned fNPIdx = this->InstrToIndex[firstNonPhi];
				for (unsigned i = idx; i < fNPIdx; ++i) {
					Instruction *instr = this->IndexToInstr[i];
					for (unsigned j = 0; j < instr->getNumOperands(); ++j) {
						Instruction *operand = (Instruction *)instr->getOperand(j);
						pointerInfo Rk = make_pair('R', this->InstrToIndex[operand]);
						if (curInfo->info.count(Rk)) {
							for (auto x : curInfo->info[Rk]) {
								curInfo->addSingleInfo(Ri, x);
							}
						}
					}
				}
			}

			//add to outgoingedges
			for (unsigned i = 0; i < OutgoingEdges.size(); ++i) {
				Infos.push_back(curInfo);
			}
		}

	};

	struct MayPointToAnalysisPass : public FunctionPass {
		static char ID;
		MayPointToAnalysisPass() : FunctionPass(ID) {}

		bool runOnFunction(Function &F) override {
			MayPointToInfo bottom;
			MayPointToInfo initialState;
			MayPointToAnalysis * rda = new MayPointToAnalysis(bottom, initialState);

			rda->runWorklistAlgorithm(&F);
			rda->print();

			return false;
		}
	};// end of struct ReachingDefinitionAnalysisPass
}  // end of anonymous namespace

char MayPointToAnalysisPass::ID = 0;
static RegisterPass<MayPointToAnalysisPass> X("cse231-maypointto", "MayPointTo Analysis",
	false /* Only looks at CFG */,
	false /* Analysis Pass */);