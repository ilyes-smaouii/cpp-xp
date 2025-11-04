int _start(void) {
  const char hello[] = "Hello world !\n";

  asm volatile("mov $1, %%rax\n\t"  // 1 >> rax --> syscall = write
               "mov $1, %%rdi\n\t"  // 1 >> rdi --> stdout (i.e. 1)
               "lea %0, %%rsi\n\t"  // 0 >> rsi --> str address
               "mov $15, %%rdx\n\t" // 15 >> rdx --> str length
               "syscall\n\t"
               :
               : "m"(hello)
               : "%rax", "%rdi", "%rsi", "%rdx");

  asm volatile("mov $60, %%rax\n\t" // SYS_exit
               "mov $1, %%rdi\n\t"  // exit code = 0
               "syscall\n\t"        //
               ::
                   : "%rax", "%rdi");

  return 0;
}