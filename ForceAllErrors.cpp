#include <llvm/Object/Binary.h>
#include <llvm/Support/CommandLine.h>
#include <llvm/Support/PrettyStackTrace.h>
#include <llvm/Support/Signals.h>
#include <llvm/Support/raw_ostream.h>

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

  outs().flush();
  return 0;
}
