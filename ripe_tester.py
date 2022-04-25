#!/usr/bin/python3
#
# Updated version of the RIPE frontend to assist the automated testing using
# the RIPE evaluation tool. Works only with Python 3
#
# RIPE was originally developed by John Wilander (@johnwilander)
# and was debugged and extended by Nick Nikiforakis (@nicknikiforakis)
#
# Updated version was developed by Hubert Rosied (@hrosier) and rewritten for
# python3 by Gabriele Serra (@gabriserra)
#
# Released under the MIT license (see file named LICENSE)

import os
import sys
import frontend.terminal as term
import frontend.utility as util

################################################################################
# RIPE OPTIONS AND COSTANTS
################################################################################

ripeLogFile = "/tmp/ripe_log"
ripeErrFile = "/tmp/ripe_log2"
ripeAttFile = "/tmp/ripe-eval/f_xxxx"

# TODO: enable this only after new backend
# ripeLogFile = "results/log.tmp"
# ripeErrFile = "results/err.tmp"
# ripeAttFile = "/tmp/ripe-eval/f_xxxx"

compilers = [
    "gcc",
    "clang"
]

code_ptr = [
    "ret",
    "baseptr",
    "funcptrstackvar",
    "funcptrstackparam",
    "funcptrheap",
    "funcptrbss",
    "funcptrdata",
    "structfuncptrstack",
    "structfuncptrheap",
    "structfuncptrbss",
    "structfuncptrdata",
    "longjmpstackvar",
    "longjmpstackparam",
    "longjmpheap",
    "longjmpbss",
    "longjmpdata"
]

funcs = [
    "memcpy",
    "strcpy",
    "strncpy",
    "sprintf",
    "snprintf",
    "strcat",
    "strncat",
    "sscanf",
    "fscanf",
    "homebrew"
]

locations = [
    "stack",
    "heap",
    "bss",
    "data"
]

attacks = [
    # "nonop",
    # "simplenop",
    # "createfile",
    "simplenopequival",
    "r2libc",
    "rop"
]

techniques = [
    "direct",
    "indirect"
]

verbose = {
    "print-ok": True,
    "print-some": True,
    "print-fail": True,
}

################################################################################
# MAIN
################################################################################

results = {
    "gcc": {
        "ok": 0,
        "fail": 0,
        "some": 0,
        "np": 0
    },
    "clang": {
        "ok": 0,
        "fail": 0,
        "some": 0,
        "np": 0
    },
}

if len(sys.argv) < 4:
    util.printUsage(sys.argv[0])

techniques = util.parseTechniqueOption(sys.argv[1], techniques)
repeat_times = util.parseRepeatOption(sys.argv[2])
compilers = util.parseCompilerOption(sys.argv[3], compilers)

# TODO: enable this only after backend is complete.
# util.makeDir("results")

for compiler in compilers:
    for technique in techniques:
        for location in locations:
            for ptr in code_ptr:
                for attack in attacks:
                    for function in funcs:

                        attempts = 0
                        successfulAttempts = 0
                        attackPossible = True
                        logInfo = []

                        while attempts < repeat_times:
                            attempts += 1
                            util.rmFile(ripeLogFile)

                            params = (technique, location,
                                      ptr, attack, function)

                            term.tWrite(util.getParamsString(params))
                            term.tFlush()

                            util.dumpParamsOntoFile(params, ripeLogFile)

                            cmdline = "(echo \"touch /tmp/ripe-eval/f_xxxx\" | ./build/"+compiler + \
                                "_attack_gen " + util.getParamsString(params) + \
                                " >> /tmp/ripe_log 2>&1) 2> /tmp/ripe_log2" + \
                                str(attempts)
                            os.system(cmdline)

                            logLine = util.fileReadLine(ripeLogFile)

                            if logLine.find("Impossible") != -1:
                                attackPossible = False
                                break  # Not possible once, not possible always :)

                            logInfo += term.parseLogInfo(logLine)

                            if util.fileExist(ripeAttFile):
                                successfulAttempts += 1
                                util.rmFile(ripeAttFile)

                        # NOT POSSIBLE
                        if attackPossible == False:
                            results[compiler]['np'] += 1
                            continue

                        # SUCCESS
                        if successfulAttempts == repeat_times:
                            results[compiler]['ok'] += 1
                            if verbose["print-ok"]:
                                term.printLogOk(
                                    compiler, util.getParamsString(params), successfulAttempts, repeat_times, logInfo)

                        # FAIL
                        elif successfulAttempts == 0:
                            results[compiler]['fail'] += 1
                            if verbose["print-fail"]:

                                i = 1
                                while i < repeat_times:
                                    logInfo += term.parseLogError(
                                        util.fileReadLine(ripeErrFile+str(i)))
                                    i += 1

                                term.printLogFail(
                                    compiler, util.getParamsString(params), successfulAttempts, repeat_times, logInfo)

                        # SOME
                        else:
                            results[compiler]['some'] += 1
                            if verbose["print-some"]:

                                i = 1
                                while i < repeat_times:
                                    logInfo += term.parseLogError(
                                        util.fileReadLine(ripeErrFile+str(i)))
                                    i += 1

                                term.printLogSome(
                                    compiler, util.getParamsString(params), successfulAttempts, repeat_times, logInfo)


# Benchmark finished, print out results

for compiler in compilers:
    print("\n" + term.bold("||Summary " + compiler + "||"))

    attacks = results[compiler]["ok"] + \
        results[compiler]["some"] + results[compiler]["fail"]

    print("OK: %s SOME: %s FAIL: %s NP: %s Total Attacks: %s\n\n" % (
        results[compiler]["ok"], results[compiler]["some"], results[compiler]["fail"], results[compiler]["np"], attacks))
