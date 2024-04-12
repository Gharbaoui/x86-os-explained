volatile unsigned char *video = 0xB8000;
int next_text_position = 0;
int curr_line = 0;

void print(char *);
void println(void);
void printi(int);

typedef enum process_state { READY, RUNNING } process_state_t;

typedef struct process_context {
  int eax, ecx, edx, ebx, esp, ebp, esi, edi, eip;
} process_context_t;

typedef struct process {
  int pid;
  process_context_t context;
  enum process_state state;
  int *base_address;
} process_t;

process_t *processes[10];

int processes_count;
int current_process_index;

void process_create(int *base_address, process_t *process, int esp) {
  process->pid = processes_count;
  process->context.eax = 0;
  process->context.ecx = 10;
  process->context.edx = 20;
  process->context.ebx = 30;
  process->context.esp = esp;
  process->context.ebp = 0;
  process->context.esi = 0;
  process->context.edi = 0;
  process->context.eip = base_address;

  process->state = READY;
  process->base_address = base_address;
  processes[processes_count] = process;

  ++processes_count;
}

int next_process_to_run_index, current_process_running_index;
process_t *next_process;

process_t *get_next_process(void) {
  process_t *np = processes[next_process_to_run_index];
  current_process_running_index = next_process_to_run_index;
  ++next_process_to_run_index;
  next_process_to_run_index = next_process_to_run_index % processes_count;
  return np;
}

void scheduler(int eip, int edi, int esi, int ebp, int esp, int ebx, int edx,
               int ecx, int eax) {
  print("scheduler called");
  println();
  process_t *current_process;

  current_process = processes[current_process_running_index];
  next_process = get_next_process();

  if (current_process->state == RUNNING) {
    current_process->context.eax = eax;
    current_process->context.ecx = ecx;
    current_process->context.edx = edx;
    current_process->context.ebx = ebx;
    current_process->context.esp = esp;
    current_process->context.ebp = ebp;
    current_process->context.esi = esi;
    current_process->context.edi = edi;
    current_process->context.eip = eip;
  }
  current_process->state = READY;

  asm("mov %0, %%eax; \
        mov %0, %%ecx; \
        mov %0, %%edx; \
        mov %0, %%ebx; \
        mov %0, %%esi; \
        mov %0, %%edi;"
      :
      : "r"(next_process->context.eax), "r"(next_process->context.ecx),
        "r"(next_process->context.edx), "r"(next_process->context.ebx),
        "r"(next_process->context.esi), "r"(next_process->context.edi));

  next_process->state = RUNNING;
}

void run_next_process(void) {
  // asm("mov %0, %%ebp; \
  //     mov %0, %%esp;"
  //     :
  //     : "r"(next_process->context.ebp), "r"(next_process->context.esp));
  //
  //
  // asm("mov %[var], %%eax" : : [var] "r"(next_process->context.esp) : "%eax");
  asm("mov %[var], %%esp" : : [var] "r"(next_process->context.esp) : "%esp");
  asm("sti; \
    jmp *%0"
      :
      : "r"(next_process->context.eip));
}
#define wait_constant 10000000

void processA(void) {
  asm("mov $5390, %eax");
  int a = 8;
  print("Process A, starts at: ");
  printi(a);
  println();
  while (1) {
    a += 1;
    print("PROCESS A ");
    printi(a);
    println();
    for (unsigned long i = 0; i < wait_constant; ++i)
      ; // like wait
  }
}

void processB(void) {
  asm("mov $5391, %eax");
  int a = 4000;
  print("Process B, starts at: ");
  printi(a);
  println();
  while (1) {
    a += 1;
    print("PROCESS B ");
    printi(a);
    println();
    for (unsigned long i = 0; i < wait_constant; ++i)
      ; // like wait
  }
}

void processC(void) {

  asm("mov $5392, %eax");
  int a = 800;
  print("Process C, starts at: ");
  printi(a);
  println();
  while (1) {
    a += 1;
    print("PROCESS C ");
    printi(a);
    println();
    for (unsigned long i = 0; i < wait_constant; ++i)
      ; // like wait
  }
}

void processD(void) {
  print("Process D,");
  while (1)
    asm("mov $5393, %eax");
}

void kernel_entry(void) {
  print("We are Now in protected mode");
  println();
  processes_count = 0;
  current_process_index = 0;
  next_process_to_run_index = 1;
  current_process_running_index = 0;

  process_t p1, p2, p3, p4;
  process_create(&processA, &p1, 0x5000);
  process_create(&processB, &p2, 0x6000);
  process_create(&processC, &p3, 0x7000);

  while (1)
    ;
}

void interrupt_handler(int interrupt_id) {
  println();
  print("Interrupt received ");
  printi(interrupt_id);
}

void print(char *str) {
  int char_location_in_video_memory;
  int char_color_location_in_video_memory;

  while (*str != 0) {
    char_location_in_video_memory = next_text_position * 2;
    char_color_location_in_video_memory = char_location_in_video_memory + 1;

    video[char_location_in_video_memory] = *str;
    video[char_color_location_in_video_memory] = 15;

    ++next_text_position;
    ++str;
  }
}

void println(void) {
  ++curr_line;
  next_text_position = curr_line * 80;
}

void printi(int number) {
  char *digitToStr[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

  if (number >= 0 && number <= 9) {
    print(digitToStr[number]);
    return;
  } else {
    int remaining = number % 10;
    number = number / 10;

    printi(number);
    printi(remaining);
  }
}
