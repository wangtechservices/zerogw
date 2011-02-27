#ifndef _H_MAIN
#define _H_MAIN

#define SHIFT(ptr, typ, mem) ((typ *)((char *)(ptr) - offsetof(typ, mem)))
#define RANDOM_LENGTH 16

#include <website.h>
#include <pthread.h>
#include <sys/queue.h>

#include "config.h"
#include "sieve.h"
#include "zutils.h"
#include "uidgen.h"
#include "polling.h"

typedef struct ev_loop *evloop_t;
typedef struct ev_io watch_t;

typedef enum {
    HYBI_WEBSOCKET,
    HYBI_COMET
} hybi_enum;

typedef struct connection_s {
    ws_connection_t ws;
    struct hybi_s *hybi;
} connection_t;

typedef struct hybi_s {
    hybi_enum type;
    char uid[UID_LEN];
    config_Route_t *route;
    LIST_HEAD(conn_subscribers_s, subscriber_s) subscribers;
    connection_t *conn;
    comet_t comet[]; // tiny hack, to use less memory, but be efficient
} hybi_t;

typedef struct statistics_s {
    size_t connects;
    size_t disconnects;
    size_t http_requests;
    size_t http_replies;
    size_t zmq_requests;
    size_t zmq_retries;
    size_t zmq_replies;
    size_t zmq_orphan_replies;
    size_t websock_connects;
    size_t websock_disconnects;
    size_t comet_connects;
    size_t comet_disconnects;
    size_t comet_acks;
    size_t comet_empty_replies;
    size_t comet_aborted_replies;
    size_t comet_received_messages;
    size_t comet_received_batches;
    size_t comet_sent_messages;
    size_t comet_sent_batches;
    size_t topics_created;
    size_t topics_removed;
    size_t websock_subscribed;
    size_t websock_unsubscribed;
    size_t websock_published;
    size_t websock_sent;
    size_t websock_received;
    size_t websock_backend_queued;
    size_t websock_backend_unqueued;
    size_t disk_requests;
    size_t disk_reads;
    size_t disk_bytes_read;
} statistics_t;

typedef struct serverroot_s {
    ws_server_t ws;
    void *zmq;
    void *disk_socket;
    evloop_t loop;
    struct ev_io disk_watch;
    struct ev_async disk_async;
    pthread_t *disk_threads;
    char instance_id[IID_LEN];
    char random_data[RANDOM_LENGTH];
    config_main_t *config;
    config_Route_t **wsock_routes;
    sieve_t *request_sieve;
    sieve_t *hybi_sieve;
    LIST_HEAD(hybi_devices_s, streamer_s) hybi_devices;
    statistics_t stat;
} serverroot_t;

extern serverroot_t root;

#endif
