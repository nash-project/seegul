
section .text
global _start
_start:
mov eax, [msg]
mov eax, [msg]
mov eax, 1
int 80h


section .data
msg: db "hello world", 0