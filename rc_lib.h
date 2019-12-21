#ifndef RCLIB_H
#define RCLIB_H

#include <stdint.h>
#include <stdbool.h>

#ifndef DATA_BUFFER_SIZE
#define DATA_BUFFER_SIZE 64
#endif

extern uint8_t rc_lib_global_package_uid;
extern uint8_t rc_lib_transmitter_id;
extern uint8_t rc_lib_error_count;

typedef struct {
    uint16_t channel_data[DATA_BUFFER_SIZE];
    uint8_t buffer[DATA_BUFFER_SIZE];
    uint8_t buf_count;

    uint8_t uid; ///< Unique (package) id
    uint8_t tid; ///< Unique transmitter (device) id
    uint16_t channel_count; ///< Number of saved channels
    uint16_t resolution; ///< Resolution of each channel in pixels
    uint8_t error;

    uint8_t checksum;

    uint8_t mesh;
    uint8_t routing_length;
    uint8_t discover_state; ///< Zero means no discovery-message, one is a discovery message, two a discovery response
    uint8_t _receive_state_machine_state;
    uint8_t _data_byte_count;
} rc_lib_package_t;

void rc_lib_init_rx(rc_lib_package_t *package);
void rc_lib_init_tx(rc_lib_package_t *package, uint16_t resolution, uint8_t channel_count);
uint8_t rc_lib_encode(rc_lib_package_t *package);
uint8_t rc_lib_decode(rc_lib_package_t *package, uint8_t data);
uint8_t rc_lib_calculate_checksum(const rc_lib_package_t *package);
uint8_t rc_lib_is_discover_message(const rc_lib_package_t *package);
uint8_t rc_lib_is_discover_response(const rc_lib_package_t *package);
void rc_lib_set_discover_message(rc_lib_package_t *package);
void rc_lib_make_discover_response(rc_lib_package_t *new_package, const rc_lib_package_t *responses, uint8_t len);

#endif //RCLIB_H
