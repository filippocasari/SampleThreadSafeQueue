
#include <czmq.h>

#define NUM_SUB 5
const char *endpoint_tcp = "tcp://127.0.0.1:6000";
const char *endpoint_inproc = "inproc://example";
#define ENDPOINT endpoint_tcp


static void
subscriber_thread(zsock_t *pipe, void *args) {
    //  Subscribe to "A" and "B"
    /*
    void *subscriber = zsock_new (ZMQ_SUB);
    zsock_connect (subscriber, "tcp://127.0.0.1:6000");*/

    zsock_signal(pipe, 1);
    zsock_t *sub = args;

    assert(sub);
    puts("sub connected");

    int count = 0;
    while (!zctx_interrupted) {
        char *topic;
        char *frame;
        zmsg_t *msg;
        zsock_recv(sub, "sm", &topic, &msg);
        zsys_info("Recv on %s", topic);

        char *end_pointer_string;
        long end;
        long start;
        long end_to_end_delay;
        while (zmsg_size(msg) > 0) {

            frame = zmsg_popstr(msg);
            if (strcmp(frame, "TIMESTAMP") == 0) {

                frame = zmsg_popstr(msg);
                zsys_info("> %s", frame);
                end = zclock_usecs();
                start = strtol(frame, &end_pointer_string, 10);
                end_to_end_delay = end - start;
                printf("END TO END DELAY : %ld [micro secs]\n", end_to_end_delay);

                break;
            }
            zsys_info("> %s", frame);
            free(frame);

        }

        free(topic);
        zmsg_destroy(&msg);
        count++;
    }
    zsock_destroy (&sub);

}


int main() {

    zactor_t *sub_threads[NUM_SUB];
    zsock_t *subscribers[NUM_SUB];
    for (int i = 0; i < NUM_SUB; i++) {
        subscribers[i]= zsock_new_sub(ENDPOINT, "ENGINE");
        char name[9];
        sprintf(name, "%s-%d", "ENGINE", i);
        printf("Starting new sub thread : %s\n", name);
        sub_threads[i] = zactor_new(subscriber_thread, subscribers[i]);


    }

    for (int i = 0; i < NUM_SUB; i++) {
        zactor_destroy(&sub_threads[i]);
        zsock_destroy(&subscribers[i]);
    }

    return 0;
}

