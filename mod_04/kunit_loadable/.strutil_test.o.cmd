savedcmd_strutil_test.o := ld -m elf_x86_64 --fatal-warnings -z noexecstack --no-warn-rwx-segments   -r -o strutil_test.o @strutil_test.mod  ; /usr/src/linux-7.0.3/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --ibt --orc --static-call --uaccess  --link  --module strutil_test.o

strutil_test.o: $(wildcard /usr/src/linux-7.0.3/tools/objtool/objtool)
