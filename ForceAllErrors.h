#pragma once

// Include modified error headers first, so we never get the original ones
#include <llvm/Support/Error.h>
#include <llvm/Support/ErrorOr.h>

#include <mutex>

static int InstanceCount = 0;
static int InstanceToBreak = 0;
std::mutex AccessInstanceCount;

bool TurnInstanceIntoError() {
  assert(!AccessInstanceCount.try_lock());
  return ++InstanceCount == InstanceToBreak;
}

template <class Functor_t>
int CountMutationPoints(const Functor_t &F) {
  std::lock_guard<std::mutex> lock(AccessInstanceCount);

  InstanceToBreak = 0; // disable breaking instances
  InstanceCount = 0;
  F();

  int C = InstanceCount;
  DEBUG(llvm::dbgs() << "             Detected " << C << " mutation points\n");

  InstanceCount = 0;
  return C;
}

template <class Functor_t>
void ForceAllErrors(int C, const Functor_t &F) {
  std::lock_guard<std::mutex> lock(AccessInstanceCount);

  for (int i = 1; i <= C; i++) {
    InstanceToBreak = i;
    InstanceCount = 0;
    F();
  }

  exit(0); // Success
}
