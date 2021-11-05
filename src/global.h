#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

#ifdef Q_OS_LINUX
#define COMMAND_TO_RUN "scripts/run.sh";
#elif defined(Q_OS_WINDOWS)
//QString commandToRun = "wsl \"scripts/run.sh\"";
#define COMMAND_TO_RUN "ASLI.exe";
#endif

#ifdef Q_OS_LINUX
#define WORKING_DIR "scripts/run.sh";
#elif defined(Q_OS_WINDOWS)
#define WORKING_DIR "bin\\";
#endif

#endif // GLOBAL_H
