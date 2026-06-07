savedcmd_strutil_module.mod := printf '%s\n'   src/strutil_module.o | awk '!x[$$0]++ { print("./"$$0) }' > strutil_module.mod
