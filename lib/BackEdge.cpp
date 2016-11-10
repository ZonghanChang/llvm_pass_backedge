#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Analysis/CFG.h"
#include <map>
#include <set>

using namespace llvm;

namespace backedge {

	struct BackEdge : public FunctionPass {
		static char ID;
		BackEdge() : FunctionPass(ID) {}
		
		virtual bool runOnFunction(Function &F);
		
		//virtual void getAnalysisUsage(AnalysisUsage &Info) const;
	};


	bool BackEdge::runOnFunction(Function &F) {
		errs() << "FUNCTION ";
		errs().write_escaped(F.getName()) << "\n";

		for(Function::arg_iterator arg = F.arg_begin(), argEnd = F.arg_end(); arg != argEnd; ++arg) {
			errs() << arg->getName() <<"\n";
			errs() << arg->getType()->getTypeID() <<"\n";
		}
		// map variable map a basic block to its dominator set
		std::map<BasicBlock*, std::set<BasicBlock*>*> map;

		// initialize all sets
		for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
			if(blk == F.begin()) {
				//errs() << blk->getName() << " haha1-> " << "\n";
				std::set<BasicBlock*> *set = new std::set<BasicBlock*>();
				set->insert(blk);
				map[blk] = set;
			}
			else {
				//errs() << blk->getName() << " haha2-> " << "\n";
				std::set<BasicBlock*> *set = new std::set<BasicBlock*>();
				for (Function::iterator eachBlk = F.begin(); eachBlk != blkEnd; ++eachBlk) {
					set->insert(eachBlk);
				}
				map[blk] = set;
			}
		}

		// for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
		// 	  	std::set<BasicBlock*> *succSet = map[blk];
		// 	  	for(std::set<BasicBlock *>::iterator bbitem = succSet->begin(); bbitem != succSet->end(); bbitem++) {
		// 	  		errs() << blk->getName() << " -> " << (*bbitem)->getName() << "\n";
		// 	  	}
			
		// }

		//print
		// int t = 0;
		// for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
		// 	t++;
		// 	const TerminatorInst *TInst = blk->getTerminator();
		// 	for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
		// 	  	BasicBlock *succ = TInst->getSuccessor(I);
		// 	  	std::set<BasicBlock*> *succSet = map[succ];
		// 	  	for(std::set<BasicBlock *>::iterator bbitem = succSet->begin(); bbitem != succSet->end(); bbitem++) {
		// 	  		errs() << blk->getName() << " -> " << t << (*bbitem)->getName() << "\n";
		// 	  	}
		// 	}
		// }

		// algorithm
		bool changed = true;
		while(changed) {
			changed = false;
			for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
				const TerminatorInst *TInst = blk->getTerminator();
				for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
				  	BasicBlock *succ = TInst->getSuccessor(I);

					//errs() << blk->getName() << " -> " << succ->getName() << "\n";

				  	std::set<BasicBlock*> *succSet = map[succ];
				  	std::set<BasicBlock*> *currset = map[blk];
				  	std::set<BasicBlock*> *del = new std::set<BasicBlock*>();
				  	for(std::set<BasicBlock *>::iterator bbitem = succSet->begin(); bbitem != succSet->end(); bbitem++) {
				  		if(currset->find(*bbitem) == currset->end() && *bbitem != blk) {
				  			del->insert(*bbitem);
				  		}
				  	}
				  	if(del->size() != 0) {
				  		changed = true;
				  	}
				  	for(std::set<BasicBlock*>::iterator bbitem = del->begin(); bbitem != del->end(); bbitem++) {
				  		succSet->erase(*bbitem);
				  	}
				  	delete(del);
				}
			}
		}


		// 再次遍历blk, 检查每个blk的succ是否在这个blk的set里，在的话就是back edge
		for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
			const TerminatorInst *TInst = blk->getTerminator();
			for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
			  	BasicBlock *succ = TInst->getSuccessor(I);
			  	std::set<BasicBlock*> *succSet = map[blk];
			  	if(succSet->find(succ) != succSet->end()) {
			  		errs() << blk->getName() << " -> " << succ->getName() << "\n";
			  	}
			}
		}
		//Function does not modify anything
		return false;
	}
	
}

char backedge::BackEdge::ID = 0;
static RegisterPass<backedge::BackEdge> X("BackEdge", "(CS 565) - Back Edge LLVM IR", false, false);
