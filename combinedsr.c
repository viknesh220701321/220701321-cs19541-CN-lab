#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_SEQ 7
#define BUFFER (MAX_SEQ + 1) / 2
#define WINDOW_BOUND BUFFER

struct frame {
  int seq_no;
  int ack_no;
  char info;
};

void send_frame(struct frame s) {
  printf("Sending frame %d: seq_no=%d, ack_no=%d, info=%c\n", s.seq_no, s.ack_no, s.info);
}
int main() {
  int frame_exp = 0,
       next_frame_to_send = 0,
       ack_expected = 0;
  char ch[MAX_SEQ];
  bool arrived[BUFFER];

  printf("Enter the characters: ");
  scanf("%s", ch);

  // Initialize arrived array to false
  for (int i = 0; i < BUFFER; i++) {
    arrived[i] = false;
  }

  // Simulate sending and receiving frames
  while (ack_expected <= MAX_SEQ) {
    // Send frames within the window
    while (next_frame_to_send < WINDOW_BOUND && !arrived[next_frame_to_send % BUFFER]) {
      struct frame s;
      s.seq_no = frame_exp;
      s.ack_no = (frame_exp + MAX_SEQ) % (MAX_SEQ + 1);
      s.info = ch[next_frame_to_send];

      send_frame(s);
      next_frame_to_send=(next_frame_to_send+1)%BUFFER;
    }

    // Receive ACKs and update window
    int ack_received;
    printf("Waiting for ACK\n");
    scanf("%d", &ack_received);

    if (ack_received >= 0 && ack_received <= MAX_SEQ && !arrived[ack_received % BUFFER]) {
      arrived[ack_received % BUFFER] = true;
      frame_exp = (ack_received + 1) % (MAX_SEQ + 1);

      // Resend any unacknowledged frames within the window
      for (int i = 0; i < BUFFER; i++) {
        if (!arrived[i]) {
          struct frame s;
          s.seq_no = i;
          s.ack_no = (frame_exp + MAX_SEQ) % (MAX_SEQ + 1);
          s.info = ch[i];
          send_frame(s);
        }
      }

      // Check if all frames are acknowledged
      bool all_acknowledged = true;
      for (int i = 0; i < BUFFER; i++) {
        if (!arrived[i]) {
          all_acknowledged = false;
          break;
        }
      }

      if (all_acknowledged) {
        break; // Exit the loop if all frames are acknowledged
      }
    } else {
      printf("Invalid or duplicate ACK received\n");
    }
  }

  printf("All frames sent and acknowledged\n");

  return 0;
}
