add_llvm_library( LLVMOurDominatorTreePass MODULE
        postdomtree.cpp
        postdomtreepass.cpp

  PLUGIN_TOOL
  opt
  )
