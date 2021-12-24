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

import sys
import os

################################################################################
# ARGUMENT PARSING AND USAGES
################################################################################


def printUsage(binName: str):
    message = "<direct|indirect|both> <num of test repetition> <gcc|clang|both> [verbose-options]"
    print("Usage: ./" + binName + " " + message)
    sys.exit(1)


def parseTechniqueOption(technique: str, techniques: list) -> list:
    if technique == "both":
        return techniques
    elif technique in techniques:
        return [technique]
    else:
        raise Exception("The technique <" + technique + "> is not supported.")


def parseRepeatOption(number: str) -> int:
    return int(number)


def parseCompilerOption(compiler: str, compilers: list) -> list:
    if compiler == "both":
        return compilers
    elif compiler in compilers:
        return [compiler]
    else:
        raise Exception("The compiler <" + compiler + "> is not supported.")


def parseVerboseOption(arg: str, options: dict) -> dict:
    if arg == "--only-summary":
        options['print-ok'] = False
        options['print-some'] = False
        options['print-fail'] = False
    elif arg == "--not-ok":
        options['print-ok'] = False
    elif arg == "--only-ok":
        options['print-some'] = False
        options['print-fail'] = False
    elif arg == "--not-fail":
        options['print-fail'] = False
    elif arg == "--only-fail":
        options['print-ok'] = False
        options['print-some'] = False
    elif arg == "--only-some":
        options['print-ok'] = False
    else:
        raise Exception("Verbose option <" + arg + "> is not supported")

    return options

################################################################################
# HANDLE FILES AND FORMATTING
################################################################################


def getParamsString(params) -> str:
    return "-t %8s -l %5s -c %18s -i %16s -f %8s" % params


def fileReadLine(filename: str) -> str:
    return open(filename, "r").read()


def makeDir(dirName: str):
    if not os.path.exists(dirName):
        os.system("mkdir " + dirName)


def fileExist(fileName: str) -> bool:
    return os.path.exists(fileName)


def rmFile(fileName: str):
    if fileExist(fileName):
        os.system("rm " + fileName)


def dumpOntoFile(line: str, fileName: str):
    os.system("echo " + line + " >> " + fileName)


def dumpParamsOntoFile(params, fileName: str):
    paramsStr = getParamsString(params)
    dumpOntoFile(paramsStr, fileName)
