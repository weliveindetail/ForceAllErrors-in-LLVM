// Include modified error headers first, so we never get the original ones
#include <llvm/Support/Error.h>
#include <llvm/Support/ErrorOr.h>

#include <llvm/Object/Binary.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/raw_ostream.h>

static int InstanceCount = 0;
static const int InstanceToBreak = 2; // raise Error in first invokation
                                      // of llvm::object::createBinary()

bool TurnInstanceIntoError() {
  return ++InstanceCount == InstanceToBreak;
}

int main(int argc, char **argv) {
  using namespace llvm;
  using namespace llvm::object;

  sys::PrintStackTraceOnErrorSignal(argv[0]);
  PrettyStackTraceProgram X(argc, argv);

  // Parse -debug and -debug-only options.
  cl::ParseCommandLineOptions(argc, argv, "ForceAllErrors tool\n");

  // Handle error case correct
  {
    Expected<OwningBinary<Binary>> bin = createBinary(argv[0]);
    if (!bin)
      outs() << toString(bin.takeError()) << "\n";
    else
      outs() << bin->getBinary()->getFileName() << "\n";
  }

  // Handle error case incorrect
  {
    Expected<OwningBinary<Binary>> bin = createBinary(argv[0]);
    if (!bin)
      outs() << toString(bin.takeError()) << "\n";

    outs() << bin->getBinary()->getType() << "\n";
  }

  outs() << "\n\n------------------------------------------------------\n\n";
  outs() << "Total instances: " << InstanceCount << "\n\n";

  outs().flush();
  return 0;
}
