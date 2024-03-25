volatile unsigned char *video = 0xB8000;
int next_text_position = 0;
int curr_line = 0;

void print(char *);
void println(void);
void printi(int);

void kernel_entry(void) {
  print("We are Now in protected mode");
  println();
  printi(45);
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
