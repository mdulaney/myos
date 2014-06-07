gcc -o test_scheduler -g task.c test_scheduler.c scheduler.c -lcmocka
nasm -f elf64 -o syscall.o syscall.asm
nasm -f elf64 -o syscall.o syscall.asm
