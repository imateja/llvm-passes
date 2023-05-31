add_llvm_library( LLVMOurDGEPass MODULE
        dge.cpp

        PLUGIN_TOOL
        opt
        )
