#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>
#include <error.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#define MAX_DEVICE_COUNT 256
#define MAX_NAME_LENGTH 256
#define BD_ADDR_LENGTH 18
#define CHAR_CASE 32
#define INPUT_SIZE 64
#define DEFAULT_DISCOVER_PERIOD 1
#define TAB "  "

void byebye(char *message);
void *alloc(size_t size);
char lower(char ch);

// https://people.csail.mit.edu/albert/bluez-intro/c404.html
int main(int argc, char **argv) {
  // devices list
  inquiry_info *devices = NULL;
  int device_count;
  int module_id, module, flags;
  // 1.28 * x seconds
  int discover_period;
  char *input = alloc(sizeof(char) * 64);

  printf("Enter discover period [%d is default]: ", DEFAULT_DISCOVER_PERIOD);
  fgets(input, INPUT_SIZE, stdin);
  discover_period = atoi(input);
  if (discover_period <= 0)
    discover_period = DEFAULT_DISCOVER_PERIOD;

  module_id = hci_get_route(NULL);
  module = hci_open_dev(module_id);
  if (module_id < 0 || module < 0)
    byebye("opening socket");

  flags = IREQ_CACHE_FLUSH;
  devices = malloc(MAX_DEVICE_COUNT * sizeof(inquiry_info));

  printf("Discovering devices...\n");
  device_count = hci_inquiry(module_id, discover_period, MAX_DEVICE_COUNT, NULL,
                             &devices, flags);
  if (device_count < 0)
    byebye("hci_inquiry (discovery)");

  printf("Found %d device%s\n", device_count, device_count == 1 ? "" : "s");
  for (int i = 0; i < device_count; i++) {
    inquiry_info device = *(devices + i);
    char *addr = alloc(sizeof(char) * BD_ADDR_LENGTH);
    char *name = alloc(sizeof(char) * MAX_NAME_LENGTH);

    ba2str(&device.bdaddr, addr);
    printf("%d. [%s] ...", i, addr);
    fflush(stdout);

    int read_result =
        hci_read_remote_name(module, &device.bdaddr, MAX_NAME_LENGTH, name, 0);
    if (read_result < 0)
      strcpy(name, "unknown");
    printf("\r%d. [%s] %s\n", i, addr, name);

    printf("%sConnect? [y/N] ", TAB);
    memset(input, 0, INPUT_SIZE);
    fgets(input, INPUT_SIZE, stdin);
    if (lower(input[0]) != 'y') {
      printf("%sSkipping\n", TAB);
      continue;
    }

    printf("%sConnecting...\n", TAB);
    break;
  }

  free(devices);
  close(module);

  return EXIT_SUCCESS;
}

void byebye(char *message) { error(EXIT_FAILURE, 0, "%s", message); }

void *alloc(size_t size) {
  void *memory = malloc(size);
  if (memory == NULL)
    byebye("failed to allocate memory");
  memset(memory, 0, size);
  return memory;
}

char lower(char ch) { return ch | CHAR_CASE; }
