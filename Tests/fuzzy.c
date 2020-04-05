#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../rc_lib.h"

void print_info(const rc_lib_package_t *pkg) {
    printf("Resolution: %d\tChannel Count: %d\n", pkg->resolution, pkg->channel_count);
    printf("Transmitter ID: %d\tUnique ID: %d\n", pkg->tid, pkg->uid);
    for (int c=0; c<pkg->channel_count; ++c) {
        printf("\tChannel %d:\t%d\n", c, pkg->channel_data[c]);
    }
    printf("Raw Data (%d bytes):\n", pkg->buf_count);
    for (int c=0; c<pkg->buf_count; ++c) {
        printf("\tByte %d:\t%d\n", c, pkg->buffer[c]);
    }
}

int main(void) {
    srand(time(NULL));

    for (int count=0; ; ++count) {

        rc_lib_package_t send_package;
        uint16_t resolution = 1u << (rand() % 8u + 5u);
        uint16_t channel_count = 1u << (rand() % 7u);
        if (channel_count == 128) {
            channel_count = 256;
        }

        rc_lib_transmitter_id = rand() % 256;

        rc_lib_init_tx(&send_package, resolution, channel_count);

        for (int channel = 0; channel < channel_count; ++channel) {
            send_package.channel_data[channel] = rand() % resolution;
        }

        // Encode & Decode
        rc_lib_package_t receive_package;
        rc_lib_init_rx(&receive_package);

        int len = rc_lib_encode(&send_package);
        for (int c=0; c<len; ++c) {
            bool decode_finished = rc_lib_decode(&receive_package, send_package.buffer[c]);
            bool is_last_byte = (c == len - 1);

            if (decode_finished != is_last_byte) {
                if (decode_finished) {
                    printf("Decode finished at byte %d, but message is %d long\n", c, len);
                } else if (is_last_byte) {
                    printf("Last Byte but decoding not finished\n");
                }
                print_info(&send_package);
                return 1;
            }
        }

        bool success = true;
        if (receive_package.channel_count != send_package.channel_count) {
            printf("Channel count mismatch!\n");
            success = false;
        }

        if (receive_package.resolution != send_package.resolution) {
            printf("Resolution mismatch!\n");
            success = false;
        }

        if (receive_package.uid != send_package.uid) {
            printf("UID mismatch!\n");
            success = false;
        }

        if (receive_package.tid != send_package.tid) {
            printf("TID mismatch!\n");
            success = false;
        }

        for (int c=0; c<channel_count; ++c) {
            if (receive_package.channel_data[c] != send_package.channel_data[c]) {
                printf("Channel data mismatch at channel %d", c);
            }
        }

        if (!success) {
            printf("\nSend:\n");
            print_info(&send_package);
            printf("\nReceive:\n");
            print_info(&receive_package);
            return 1;
        }

        printf("Package %d: Success\n", count);
    }
}
