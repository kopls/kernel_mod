savedcmd_strutil_test.mod := printf '%s\n'   src/strutil_test.o | awk '!x[$$0]++ { print("./"$$0) }' > strutil_test.mod
