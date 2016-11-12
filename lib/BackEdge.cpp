#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/ADT/DepthFirstIterator.h"
#include "llvm/ADT/SmallPtrSet.h"
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
		
	};


	bool BackEdge::runOnFunction(Function &F) {
		errs() << "FUNCTION ";
		errs().write_escaped(F.getName()) << "\n";


		// map variable map a basic block to its dominator set
		std::map<BasicBlock*, std::set<BasicBlock*>*> map;

		// Mark reachable blocks.
		SmallPtrSet<BasicBlock*, 8> visitedBlocks;
  		for (df_ext_iterator<Function*, SmallPtrSet<BasicBlock*, 8>> currBB = df_ext_begin(&F, visitedBlocks), 
			endBlock = df_ext_end(&F, visitedBlocks); 
			currBB != endBlock; 
			currBB++) {			
		}

		// set dominator set of all unreachable basic blocks to be itself
		std::set<BasicBlock*> unreachableBB;
		for (Function::iterator currBB = F.begin(); currBB != F.end(); currBB++) {
			if (visitedBlocks.count(currBB) == 0) {
				unreachableBB.insert(currBB);
			}
		}

		// initialize dominator sets
		for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
			if(blk == F.begin() || unreachableBB.find(blk) != unreachableBB.end()) {
				std::set<BasicBlock*> *set = new std::set<BasicBlock*>();
				set->insert(blk);
				map[blk] = set;
			}
			else {
				std::set<BasicBlock*> *set = new std::set<BasicBlock*>();
				for (Function::iterator eachBlk = F.begin(); eachBlk != blkEnd; ++eachBlk) {
					set->insert(eachBlk);
				}
				map[blk] = set;
			}
		}

		// compute dominator set
		bool changed = true;
		std::set<BasicBlock*> *reachableBB = new std::set<BasicBlock*>();
		reachableBB->insert(&F.getEntryBlock());
		while(changed) {
			changed = false;
			for (Function::iterator blk = F.begin(), blkEnd = F.end(); blk != blkEnd; ++blk) {
				const TerminatorInst *TInst = blk->getTerminator();
				for (unsigned I = 0, NSucc = TInst->getNumSuccessors(); I < NSucc; ++I) {
				  	BasicBlock *succ = TInst->getSuccessor(I);
				  	reachableBB->insert(succ);
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


		// detect and print back edge
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
		errs() << "\n";
		
		return false;
	}
	
}

char backedge::BackEdge::ID = 0;
static RegisterPass<backedge::BackEdge> X("BackEdge", "(CS 565) - Back Edge LLVM IR", false, false);
