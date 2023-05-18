add_llvm_library( LLVMOurDSEPass MODULE
        dse.cpp

        PLUGIN_TOOL
        opt
        )
