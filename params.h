#pragma once

#include <paramkit.h>
using namespace paramkit;

#include "unpack_scanner.h"

#define DEFAULT_TIMEOUT 1000

#define PARAM_EXE "exe"
#define PARAM_TIMEOUT "timeout"
#define PARAM_OUT_DIR "dir"

#define PARAM_DATA "data"
#define PARAM_MINDUMP "minidmp"
#define PARAM_SHELLCODE "shellc"
#define PARAM_HOOKS "hooks"

typedef struct {
    char exe_path[MAX_PATH];
    char out_dir[MAX_PATH];
    DWORD timeout;
    UnpackScanner::t_unp_params hh_args;
} t_params_struct;

class UnpackParams : public Params
{
public:
    UnpackParams()
        : Params()
    {
        this->addParam(new StringParam(PARAM_EXE, true));
        this->setInfo(PARAM_EXE, "Input exe (to be run)");

        this->addParam(new IntParam(PARAM_TIMEOUT, true));
        this->setInfo(PARAM_TIMEOUT, "Timeout: ms");

        this->addParam(new StringParam(PARAM_OUT_DIR, false));
        this->setInfo(PARAM_OUT_DIR, "Output directory");

        this->addParam(new IntParam(PARAM_DATA, false));
        this->setInfo(PARAM_DATA, "Set if non-executable pages should be scanned\n"
            "\t0 - none: do not scan non-executable pages\n"
            "\t1 - .NET: scan non-executable in .NET applications\n"
            "\t2 - if no DEP: scan non-exec if DEP is disabled (or if is .NET)\n"
            "\t3 - always: scan non-executable pages unconditionally"
        );

        this->addParam(new BoolParam(PARAM_MINDUMP, false));
        this->setInfo(PARAM_MINDUMP, "Create a minidump of the detected process");

        this->addParam(new BoolParam(PARAM_SHELLCODE, false));
        this->setInfo(PARAM_SHELLCODE, "Detect shellcodes");

        this->addParam(new BoolParam(PARAM_HOOKS, false));
        this->setInfo(PARAM_HOOKS, "Detect hooks and patches");
    }

    void fillStruct(t_params_struct &ps)
    {
        StringParam *myExe = dynamic_cast<StringParam*>(this->getParam(PARAM_EXE));
        if (myExe) {
            myExe->copyToCStr(ps.exe_path, sizeof(ps.exe_path));
        }
        StringParam *myDir = dynamic_cast<StringParam*>(this->getParam(PARAM_OUT_DIR));
        if (myDir) {
            myDir->copyToCStr(ps.out_dir, sizeof(ps.out_dir));
        }
        IntParam *myTimeout = dynamic_cast<IntParam*>(this->getParam(PARAM_TIMEOUT));
        if (myTimeout) ps.timeout = myTimeout->value;

        //TODO: it should be an enum parameter
        IntParam *myData = dynamic_cast<IntParam*>(this->getParam(PARAM_DATA));
        if (myData) ps.hh_args.pesieve_args.data = (pesieve::t_data_scan_mode) myData->value;

        BoolParam *myMinidump = dynamic_cast<BoolParam*>(this->getParam(PARAM_MINDUMP));
        if (myMinidump) ps.hh_args.pesieve_args.minidump = myMinidump->value;

        BoolParam *myShellc = dynamic_cast<BoolParam*>(this->getParam(PARAM_SHELLCODE));
        if (myShellc) ps.hh_args.pesieve_args.shellcode = myShellc->value;

        BoolParam *myHooks = dynamic_cast<BoolParam*>(this->getParam(PARAM_HOOKS));
        if (myHooks) ps.hh_args.pesieve_args.no_hooks = !(myHooks->value);
    }
};
