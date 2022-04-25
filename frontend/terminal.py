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

################################################################################
# TERMINAL READ/WRITE/FLUSH
################################################################################


def tWrite(line: str):
    sys.stdout.write("... Running %s ...\r" % line)


def tFlush():
    sys.stdout.flush()


################################################################################
# TERMINAL COLORING
################################################################################


def _coloredString(string, color, size=0):
    padding = ' ' * (size - len(string)) if size else ''
    return color + string + '\033[0m' + padding


def red(string, size=0):
    return _coloredString(string, '\033[91m', size)


def green(string, size=0):
    return _coloredString(string, '\033[92m', size)


def orange(string, size=0):
    return _coloredString(string, '\033[93m', size)


def blue(string, size=0):
    return _coloredString(string, '\033[94m', size)


def bold(string, size=0):
    return _coloredString(string, '\033[1m', size)


def underline(string, size=0):
    return _coloredString(string, '\033[4m', size)

################################################################################
# LOG ANALYZING / DUMPING
################################################################################


def parseLogInfo(logLine: str) -> list:
    """Analyzes the given logline and searches for additional info to report it"""

    additionalInfo = []

    if logLine.find("jump buffer is between") != -1:
        additionalInfo += [orange('SpecialPayload')]

    if logLine.find("Overflow pointer contains terminating char") != -1:
        additionalInfo += [orange("TermCharInOverflowPtr")]

    if logLine.find("in the middle") != -1:
        additionalInfo += [orange('TermCharInPayload')]

    if logLine.find("Unknown choice of") != -1:
        additionalInfo += [red('UnknownChoice')]

    if logLine.find("Could not build payload") != -1:
        additionalInfo += [red('BuildPayloadFailed')]

    if logLine.find("find_gadget") != -1:
        additionalInfo += [red('FindGadgetFail')]

    if logLine.find("Unable to allocate heap") != -1:
        additionalInfo += [red('HeapAlloc')]

    if logLine.find("the wrong order") != -1:
        additionalInfo += [red('HeapAllocOrder')]

    if logLine.find("Target address is lower") != -1:
        additionalInfo += [red('Underflow')]

    if logLine.find("AddressSanitizer") != -1:
        additionalInfo += [red('ASAN')]

    return additionalInfo


def parseLogError(logLine: str) -> list:
    """Analyzes the given logline and searches for standard faults (i.e. SEGFAULT, ..)"""

    additionalInfo = []

    if logLine.find("Segmentation fault") != -1:
        additionalInfo += [red('SEGFAULT')]
    elif logLine.find("Bus error") != -1:
        additionalInfo += [red('BUSERROR')]
    elif logLine.find("Illegal instruction") != -1:
        additionalInfo += [red('SIGILL')]

    return additionalInfo


def printLog(message: str, colorFn: callable, compiler: str, params: str, successfulAttempts: int, repeatTimes: int, additionalInfo: list):
    print("%5s %s %6s (%s/%s) %s" % (compiler, params, colorFn(message, 4),
          successfulAttempts, repeatTimes, ' '.join(additionalInfo)))


def printLogFail(compiler: str, params: str, successfulAttempts: int, repeatTimes: int, additionalInfo: list):
    printLog("FAIL", red, compiler, params,
             successfulAttempts, repeatTimes, additionalInfo)


def printLogOk(compiler: str, params: str, successfulAttempts: int, repeatTimes: int, additionalInfo: list):
    printLog("OK", green, compiler, params,
             successfulAttempts, repeatTimes, additionalInfo)


def printLogSome(compiler: str, params: str, successfulAttempts: int, repeatTimes: int, additionalInfo: list):
    printLog("SOME", orange, compiler, params,
             successfulAttempts, repeatTimes, additionalInfo)
