//===--- StmtOpenMP.cpp - Classes for OpenMP directives -------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file implements the subclesses of Stmt class declared in StmtOpenMP.h
//
//===----------------------------------------------------------------------===//

#include "clang/AST/StmtOpenMP.h"

#include "clang/AST/ASTContext.h"

using namespace clang;

void OMPExecutableDirective::setClauses(ArrayRef<OMPClause *> Clauses) {
  assert(Clauses.size() == getNumClauses() &&
         "Number of clauses is not the same as the preallocated buffer");
  std::copy(Clauses.begin(), Clauses.end(), getClauses().begin());
}

void OMPLoopDirective::setCounters(ArrayRef<Expr *> A) {
  assert(A.size() == getCollapsedNumber() &&
         "Number of loop counters is not the same as the collapsed number");
  std::copy(A.begin(), A.end(), getCounters().begin());
}

void OMPLoopDirective::setPrivateCounters(ArrayRef<Expr *> A) {
  assert(A.size() == getCollapsedNumber() && "Number of loop private counters "
                                             "is not the same as the collapsed "
                                             "number");
  std::copy(A.begin(), A.end(), getPrivateCounters().begin());
}

void OMPLoopDirective::setInits(ArrayRef<Expr *> A) {
  assert(A.size() == getCollapsedNumber() &&
         "Number of counter inits is not the same as the collapsed number");
  std::copy(A.begin(), A.end(), getInits().begin());
}

void OMPLoopDirective::setUpdates(ArrayRef<Expr *> A) {
  assert(A.size() == getCollapsedNumber() &&
         "Number of counter updates is not the same as the collapsed number");
  std::copy(A.begin(), A.end(), getUpdates().begin());
}

void OMPLoopDirective::setFinals(ArrayRef<Expr *> A) {
  assert(A.size() == getCollapsedNumber() &&
         "Number of counter finals is not the same as the collapsed number");
  std::copy(A.begin(), A.end(), getFinals().begin());
}

OMPParallelDirective *OMPParallelDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt, bool HasCancel) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPParallelDirective *Dir =
      new (Mem) OMPParallelDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setHasCancel(HasCancel);
  return Dir;
}

OMPParallelDirective *OMPParallelDirective::CreateEmpty(const ASTContext &C,
                                                        unsigned NumClauses,
                                                        EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPParallelDirective(NumClauses);
}

OMPSimdDirective *
OMPSimdDirective::Create(const ASTContext &C, SourceLocation StartLoc,
                         SourceLocation EndLoc, unsigned CollapsedNum,
                         ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
                         const HelperExprs &Exprs) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), numLoopChildren(CollapsedNum, OMPD_simd)));
  OMPSimdDirective *Dir = new (Mem)
      OMPSimdDirective(StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  return Dir;
}

OMPSimdDirective *OMPSimdDirective::CreateEmpty(const ASTContext &C,
                                                unsigned NumClauses,
                                                unsigned CollapsedNum,
                                                EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, numLoopChildren(CollapsedNum, OMPD_simd)));
  return new (Mem) OMPSimdDirective(CollapsedNum, NumClauses);
}

OMPForDirective *
OMPForDirective::Create(const ASTContext &C, SourceLocation StartLoc,
                        SourceLocation EndLoc, unsigned CollapsedNum,
                        ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
                        const HelperExprs &Exprs, bool HasCancel) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), numLoopChildren(CollapsedNum, OMPD_for)));
  OMPForDirective *Dir =
      new (Mem) OMPForDirective(StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setIsLastIterVariable(Exprs.IL);
  Dir->setLowerBoundVariable(Exprs.LB);
  Dir->setUpperBoundVariable(Exprs.UB);
  Dir->setStrideVariable(Exprs.ST);
  Dir->setEnsureUpperBound(Exprs.EUB);
  Dir->setNextLowerBound(Exprs.NLB);
  Dir->setNextUpperBound(Exprs.NUB);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  Dir->setHasCancel(HasCancel);
  return Dir;
}

OMPForDirective *OMPForDirective::CreateEmpty(const ASTContext &C,
                                              unsigned NumClauses,
                                              unsigned CollapsedNum,
                                              EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, numLoopChildren(CollapsedNum, OMPD_for)));
  return new (Mem) OMPForDirective(CollapsedNum, NumClauses);
}

OMPForSimdDirective *
OMPForSimdDirective::Create(const ASTContext &C, SourceLocation StartLoc,
                            SourceLocation EndLoc, unsigned CollapsedNum,
                            ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
                            const HelperExprs &Exprs) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), numLoopChildren(CollapsedNum, OMPD_for_simd)));
  OMPForSimdDirective *Dir = new (Mem)
      OMPForSimdDirective(StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setIsLastIterVariable(Exprs.IL);
  Dir->setLowerBoundVariable(Exprs.LB);
  Dir->setUpperBoundVariable(Exprs.UB);
  Dir->setStrideVariable(Exprs.ST);
  Dir->setEnsureUpperBound(Exprs.EUB);
  Dir->setNextLowerBound(Exprs.NLB);
  Dir->setNextUpperBound(Exprs.NUB);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  return Dir;
}

OMPForSimdDirective *OMPForSimdDirective::CreateEmpty(const ASTContext &C,
                                                      unsigned NumClauses,
                                                      unsigned CollapsedNum,
                                                      EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, numLoopChildren(CollapsedNum, OMPD_for_simd)));
  return new (Mem) OMPForSimdDirective(CollapsedNum, NumClauses);
}

OMPSectionsDirective *OMPSectionsDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt, bool HasCancel) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPSectionsDirective *Dir =
      new (Mem) OMPSectionsDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setHasCancel(HasCancel);
  return Dir;
}

OMPSectionsDirective *OMPSectionsDirective::CreateEmpty(const ASTContext &C,
                                                        unsigned NumClauses,
                                                        EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPSectionsDirective(NumClauses);
}

OMPSectionDirective *OMPSectionDirective::Create(const ASTContext &C,
                                                 SourceLocation StartLoc,
                                                 SourceLocation EndLoc,
                                                 Stmt *AssociatedStmt,
                                                 bool HasCancel) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(0, 1));
  OMPSectionDirective *Dir = new (Mem) OMPSectionDirective(StartLoc, EndLoc);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setHasCancel(HasCancel);
  return Dir;
}

OMPSectionDirective *OMPSectionDirective::CreateEmpty(const ASTContext &C,
                                                      EmptyShell) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(0, 1));
  return new (Mem) OMPSectionDirective();
}

OMPSingleDirective *OMPSingleDirective::Create(const ASTContext &C,
                                               SourceLocation StartLoc,
                                               SourceLocation EndLoc,
                                               ArrayRef<OMPClause *> Clauses,
                                               Stmt *AssociatedStmt) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPSingleDirective *Dir =
      new (Mem) OMPSingleDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPSingleDirective *OMPSingleDirective::CreateEmpty(const ASTContext &C,
                                                    unsigned NumClauses,
                                                    EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPSingleDirective(NumClauses);
}

OMPMasterDirective *OMPMasterDirective::Create(const ASTContext &C,
                                               SourceLocation StartLoc,
                                               SourceLocation EndLoc,
                                               Stmt *AssociatedStmt) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(0, 1));
  OMPMasterDirective *Dir = new (Mem) OMPMasterDirective(StartLoc, EndLoc);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPMasterDirective *OMPMasterDirective::CreateEmpty(const ASTContext &C,
                                                    EmptyShell) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(0, 1));
  return new (Mem) OMPMasterDirective();
}

OMPCriticalDirective *OMPCriticalDirective::Create(
    const ASTContext &C, const DeclarationNameInfo &Name,
    SourceLocation StartLoc, SourceLocation EndLoc,
    ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPCriticalDirective *Dir =
      new (Mem) OMPCriticalDirective(Name, StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPCriticalDirective *OMPCriticalDirective::CreateEmpty(const ASTContext &C,
                                                        unsigned NumClauses,
                                                        EmptyShell) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPCriticalDirective(NumClauses);
}

OMPParallelForDirective *OMPParallelForDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    unsigned CollapsedNum, ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
    const HelperExprs &Exprs, bool HasCancel) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(),
                                                               numLoopChildren(CollapsedNum, OMPD_parallel_for)));
  OMPParallelForDirective *Dir = new (Mem)
      OMPParallelForDirective(StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setIsLastIterVariable(Exprs.IL);
  Dir->setLowerBoundVariable(Exprs.LB);
  Dir->setUpperBoundVariable(Exprs.UB);
  Dir->setStrideVariable(Exprs.ST);
  Dir->setEnsureUpperBound(Exprs.EUB);
  Dir->setNextLowerBound(Exprs.NLB);
  Dir->setNextUpperBound(Exprs.NUB);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  Dir->setHasCancel(HasCancel);
  return Dir;
}

OMPParallelForDirective *
OMPParallelForDirective::CreateEmpty(const ASTContext &C, unsigned NumClauses,
                                     unsigned CollapsedNum, EmptyShell) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses,
                                                               numLoopChildren(CollapsedNum, OMPD_parallel_for)));
  return new (Mem) OMPParallelForDirective(CollapsedNum, NumClauses);
}

OMPParallelForSimdDirective *OMPParallelForSimdDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    unsigned CollapsedNum, ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
    const HelperExprs &Exprs) {
  void *Mem = C.Allocate(
      totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(),
                                            numLoopChildren(CollapsedNum, OMPD_parallel_for_simd)));
  OMPParallelForSimdDirective *Dir = new (Mem) OMPParallelForSimdDirective(
      StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setIsLastIterVariable(Exprs.IL);
  Dir->setLowerBoundVariable(Exprs.LB);
  Dir->setUpperBoundVariable(Exprs.UB);
  Dir->setStrideVariable(Exprs.ST);
  Dir->setEnsureUpperBound(Exprs.EUB);
  Dir->setNextLowerBound(Exprs.NLB);
  Dir->setNextUpperBound(Exprs.NUB);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  return Dir;
}

OMPParallelForSimdDirective *
OMPParallelForSimdDirective::CreateEmpty(const ASTContext &C,
                                         unsigned NumClauses,
                                         unsigned CollapsedNum, EmptyShell) {
  void *Mem = C.Allocate(
      totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses,
                                            numLoopChildren(CollapsedNum, OMPD_parallel_for_simd)));
  return new (Mem) OMPParallelForSimdDirective(CollapsedNum, NumClauses);
}

OMPParallelSectionsDirective *OMPParallelSectionsDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt, bool HasCancel) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPParallelSectionsDirective *Dir =
      new (Mem) OMPParallelSectionsDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setHasCancel(HasCancel);
  return Dir;
}

OMPParallelSectionsDirective *
OMPParallelSectionsDirective::CreateEmpty(const ASTContext &C,
                                          unsigned NumClauses, EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPParallelSectionsDirective(NumClauses);
}

OMPTaskDirective *
OMPTaskDirective::Create(const ASTContext &C, SourceLocation StartLoc,
                         SourceLocation EndLoc, ArrayRef<OMPClause *> Clauses,
                         Stmt *AssociatedStmt, bool HasCancel) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPTaskDirective *Dir =
      new (Mem) OMPTaskDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setHasCancel(HasCancel);
  return Dir;
}

OMPTaskDirective *OMPTaskDirective::CreateEmpty(const ASTContext &C,
                                                unsigned NumClauses,
                                                EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPTaskDirective(NumClauses);
}

OMPTaskgroupDirective *OMPTaskgroupDirective::Create(const ASTContext &C,
                                                     SourceLocation StartLoc,
                                                     SourceLocation EndLoc,
                                                     Stmt *AssociatedStmt) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(0, 1));
  OMPTaskgroupDirective *Dir =
      new (Mem) OMPTaskgroupDirective(StartLoc, EndLoc);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPTaskgroupDirective *OMPTaskgroupDirective::CreateEmpty(const ASTContext &C,
                                                          EmptyShell) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(0, 1));
  return new (Mem) OMPTaskgroupDirective();
}

OMPCancelDirective *
OMPCancelDirective::Create(const ASTContext &C, SourceLocation StartLoc,
                           SourceLocation EndLoc, ArrayRef<OMPClause *> Clauses,
                           OpenMPDirectiveKind CancelRegion) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 0));
  OMPCancelDirective *Dir =
      new (Mem) OMPCancelDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setCancelRegion(CancelRegion);
  return Dir;
}

OMPCancelDirective *OMPCancelDirective::CreateEmpty(const ASTContext &C,
                                                    unsigned NumClauses,
                                                    EmptyShell) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 0));
  return new (Mem) OMPCancelDirective(NumClauses);
}

OMPFlushDirective *OMPFlushDirective::Create(const ASTContext &C,
                                             SourceLocation StartLoc,
                                             SourceLocation EndLoc,
                                             ArrayRef<OMPClause *> Clauses) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 0));
  OMPFlushDirective *Dir =
      new (Mem) OMPFlushDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  return Dir;
}

OMPFlushDirective *OMPFlushDirective::CreateEmpty(const ASTContext &C,
                                                  unsigned NumClauses,
                                                  EmptyShell) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 0));
  return new (Mem) OMPFlushDirective(NumClauses);
}

OMPOrderedDirective *OMPOrderedDirective::Create(const ASTContext &C,
                                                 SourceLocation StartLoc,
                                                 SourceLocation EndLoc,
                                                 ArrayRef<OMPClause *> Clauses,
                                                 Stmt *AssociatedStmt) {
  void *Mem =
  C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPOrderedDirective *Dir =
      new (Mem) OMPOrderedDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPOrderedDirective *OMPOrderedDirective::CreateEmpty(const ASTContext &C,
                                                      unsigned NumClauses,
                                                      EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPOrderedDirective(NumClauses);
}

OMPAtomicDirective *OMPAtomicDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt, Expr *X, Expr *V,
    Expr *E, Expr *UE, bool IsXLHSInRHSPart, bool IsPostfixUpdate) {
  void *Mem = C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(),
                                                               5));
  OMPAtomicDirective *Dir =
      new (Mem) OMPAtomicDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setX(X);
  Dir->setV(V);
  Dir->setExpr(E);
  Dir->setUpdateExpr(UE);
  Dir->IsXLHSInRHSPart = IsXLHSInRHSPart;
  Dir->IsPostfixUpdate = IsPostfixUpdate;
  return Dir;
}

OMPAtomicDirective *OMPAtomicDirective::CreateEmpty(const ASTContext &C,
                                                    unsigned NumClauses,
                                                    EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 5));
  return new (Mem) OMPAtomicDirective(NumClauses);
}

OMPTargetDirective *OMPTargetDirective::Create(const ASTContext &C,
                                               SourceLocation StartLoc,
                                               SourceLocation EndLoc,
                                               ArrayRef<OMPClause *> Clauses,
                                               Stmt *AssociatedStmt) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPTargetDirective *Dir =
      new (Mem) OMPTargetDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPTargetDirective *OMPTargetDirective::CreateEmpty(const ASTContext &C,
                                                    unsigned NumClauses,
                                                    EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPTargetDirective(NumClauses);
}

OMPTargetDataDirective *OMPTargetDataDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPTargetDataDirective *Dir =
      new (Mem) OMPTargetDataDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPTargetDataDirective *OMPTargetDataDirective::CreateEmpty(const ASTContext &C,
                                                            unsigned N,
                                                            EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(N, 1));
  return new (Mem) OMPTargetDataDirective(N);
}

OMPTeamsDirective *OMPTeamsDirective::Create(const ASTContext &C,
                                             SourceLocation StartLoc,
                                             SourceLocation EndLoc,
                                             ArrayRef<OMPClause *> Clauses,
                                             Stmt *AssociatedStmt) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(Clauses.size(), 1));
  OMPTeamsDirective *Dir =
      new (Mem) OMPTeamsDirective(StartLoc, EndLoc, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  return Dir;
}

OMPTeamsDirective *OMPTeamsDirective::CreateEmpty(const ASTContext &C,
                                                  unsigned NumClauses,
                                                  EmptyShell) {
  void *Mem =
      C.Allocate(totalSizeToAlloc<OMPClause *, Stmt *>(NumClauses, 1));
  return new (Mem) OMPTeamsDirective(NumClauses);
}

OMPTaskLoopDirective *OMPTaskLoopDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    unsigned CollapsedNum, ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
    const HelperExprs &Exprs) {
  unsigned Size = llvm::RoundUpToAlignment(sizeof(OMPTaskLoopDirective),
                                           llvm::alignOf<OMPClause *>());
  void *Mem =
      C.Allocate(Size + sizeof(OMPClause *) * Clauses.size() +
                 sizeof(Stmt *) * numLoopChildren(CollapsedNum, OMPD_taskloop));
  OMPTaskLoopDirective *Dir = new (Mem)
      OMPTaskLoopDirective(StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setIsLastIterVariable(Exprs.IL);
  Dir->setLowerBoundVariable(Exprs.LB);
  Dir->setUpperBoundVariable(Exprs.UB);
  Dir->setStrideVariable(Exprs.ST);
  Dir->setEnsureUpperBound(Exprs.EUB);
  Dir->setNextLowerBound(Exprs.NLB);
  Dir->setNextUpperBound(Exprs.NUB);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  return Dir;
}

OMPTaskLoopDirective *OMPTaskLoopDirective::CreateEmpty(const ASTContext &C,
                                                        unsigned NumClauses,
                                                        unsigned CollapsedNum,
                                                        EmptyShell) {
  unsigned Size = llvm::RoundUpToAlignment(sizeof(OMPTaskLoopDirective),
                                           llvm::alignOf<OMPClause *>());
  void *Mem =
      C.Allocate(Size + sizeof(OMPClause *) * NumClauses +
                 sizeof(Stmt *) * numLoopChildren(CollapsedNum, OMPD_taskloop));
  return new (Mem) OMPTaskLoopDirective(CollapsedNum, NumClauses);
}

OMPTaskLoopSimdDirective *OMPTaskLoopSimdDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    unsigned CollapsedNum, ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
    const HelperExprs &Exprs) {
  unsigned Size = llvm::RoundUpToAlignment(sizeof(OMPTaskLoopSimdDirective),
                                           llvm::alignOf<OMPClause *>());
  void *Mem = C.Allocate(Size + sizeof(OMPClause *) * Clauses.size() +
                         sizeof(Stmt *) *
                             numLoopChildren(CollapsedNum, OMPD_taskloop_simd));
  OMPTaskLoopSimdDirective *Dir = new (Mem)
      OMPTaskLoopSimdDirective(StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setIsLastIterVariable(Exprs.IL);
  Dir->setLowerBoundVariable(Exprs.LB);
  Dir->setUpperBoundVariable(Exprs.UB);
  Dir->setStrideVariable(Exprs.ST);
  Dir->setEnsureUpperBound(Exprs.EUB);
  Dir->setNextLowerBound(Exprs.NLB);
  Dir->setNextUpperBound(Exprs.NUB);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  return Dir;
}

OMPTaskLoopSimdDirective *
OMPTaskLoopSimdDirective::CreateEmpty(const ASTContext &C, unsigned NumClauses,
                                      unsigned CollapsedNum, EmptyShell) {
  unsigned Size = llvm::RoundUpToAlignment(sizeof(OMPTaskLoopSimdDirective),
                                           llvm::alignOf<OMPClause *>());
  void *Mem = C.Allocate(Size + sizeof(OMPClause *) * NumClauses +
                         sizeof(Stmt *) *
                             numLoopChildren(CollapsedNum, OMPD_taskloop_simd));
  return new (Mem) OMPTaskLoopSimdDirective(CollapsedNum, NumClauses);
}

OMPDistributeDirective *OMPDistributeDirective::Create(
    const ASTContext &C, SourceLocation StartLoc, SourceLocation EndLoc,
    unsigned CollapsedNum, ArrayRef<OMPClause *> Clauses, Stmt *AssociatedStmt,
    const HelperExprs &Exprs) {
  unsigned Size = llvm::RoundUpToAlignment(sizeof(OMPDistributeDirective),
                                           llvm::alignOf<OMPClause *>());
  void *Mem = C.Allocate(Size + sizeof(OMPClause *) * Clauses.size() +
                         sizeof(Stmt *) *
                             numLoopChildren(CollapsedNum, OMPD_distribute));
  OMPDistributeDirective *Dir = new (Mem)
      OMPDistributeDirective(StartLoc, EndLoc, CollapsedNum, Clauses.size());
  Dir->setClauses(Clauses);
  Dir->setAssociatedStmt(AssociatedStmt);
  Dir->setIterationVariable(Exprs.IterationVarRef);
  Dir->setLastIteration(Exprs.LastIteration);
  Dir->setCalcLastIteration(Exprs.CalcLastIteration);
  Dir->setPreCond(Exprs.PreCond);
  Dir->setCond(Exprs.Cond);
  Dir->setInit(Exprs.Init);
  Dir->setInc(Exprs.Inc);
  Dir->setIsLastIterVariable(Exprs.IL);
  Dir->setLowerBoundVariable(Exprs.LB);
  Dir->setUpperBoundVariable(Exprs.UB);
  Dir->setStrideVariable(Exprs.ST);
  Dir->setEnsureUpperBound(Exprs.EUB);
  Dir->setNextLowerBound(Exprs.NLB);
  Dir->setNextUpperBound(Exprs.NUB);
  Dir->setCounters(Exprs.Counters);
  Dir->setPrivateCounters(Exprs.PrivateCounters);
  Dir->setInits(Exprs.Inits);
  Dir->setUpdates(Exprs.Updates);
  Dir->setFinals(Exprs.Finals);
  return Dir;
}

OMPDistributeDirective *
OMPDistributeDirective::CreateEmpty(const ASTContext &C, unsigned NumClauses,
                                    unsigned CollapsedNum, EmptyShell) {
  unsigned Size = llvm::RoundUpToAlignment(sizeof(OMPDistributeDirective),
                                           llvm::alignOf<OMPClause *>());
  void *Mem = C.Allocate(Size + sizeof(OMPClause *) * NumClauses +
                         sizeof(Stmt *) *
                             numLoopChildren(CollapsedNum, OMPD_distribute));
  return new (Mem) OMPDistributeDirective(CollapsedNum, NumClauses);
}
