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

ripeLogFile = "results/log.tmp"
ripeErrFile = "results/err.tmp"
ripeAttFile = "/tmp/ripe-eval/f_xxxx"

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
    "longjmpstackvar",
    "longjmpstackparam",
    "longjmpheap",
    "longjmpbss",
    "longjmpdata",
    "structfuncptrstack",
    "structfuncptrheap",
    "structfuncptrdata",
    "structfuncptrbss"
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
    "nonop",
    "simplenop",
    "simplenopequival",
    "createfile",
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

techniques = util.parseTechniqueOption(sys.argv[1])
repeat_times = util.parseRepeatOption(sys.argv[2])
compilers = util.parseCompilerOption(sys.argv[3])

util.makeDir("results")

for compiler in compilers:
    for technique in techniques:
        for location in locations:
            for ptr in code_ptr:
                for attack in attacks:
                    for function in funcs:

                        attempts = 0
                        successfulAttempts = 0
                        attackPossible = True

                        while attempts < repeat_times:
                            attempts += 1
                            util.rmFile(ripeLogFile)

                            params = (compiler, technique, location,
                                      ptr, attack, function)

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

                            logInfo = term.parseLogInfo(logLine)

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
                                logInfo += term.parseLogError(logLine)
                                term.printLogOk(
                                    compiler, params, successfulAttempts, repeat_times, logInfo)

                        # FAIL
                        elif successfulAttempts == 0:
                            results[compiler]['fail'] += 1
                            if verbose["print-fail"]:
                                logInfo += term.parseLogError(logLine)
                                term.printLogFail(
                                    compiler, params, successfulAttempts, repeat_times, logInfo)

                        # SOME
                        else:
                            results[compiler]['some'] += 1
                            if verbose["print-some"]:
                                term.printLogSome(
                                    compiler, params, successfulAttempts, repeat_times, logInfo)


# Benchmark finished, print out results

for compiler in results:
    print("\n" + term.bold("||Summary " + compiler + "||"))

    attacks = results[compiler]["ok"] + \
        results[compiler]["some"] + results[compiler]["fail"]

    print("OK: %s SOME: %s FAIL: %s NP: %s Total Attacks: %s\n\n" % (
        results[compiler]["ok"], results[compiler]["some"], results[compiler]["fail"], results[compiler]["np"], attacks))
