savedcmd_strutil_module.o := ld -m elf_x86_64 --fatal-warnings -z noexecstack --no-warn-rwx-segments   -r -o strutil_module.o @strutil_module.mod  ; /usr/src/linux-7.0.3/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --ibt --orc --static-call --uaccess  --link  --module strutil_module.o

strutil_module.o: $(wildcard /usr/src/linux-7.0.3/tools/objtool/objtool)
