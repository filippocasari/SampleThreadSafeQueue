
#include <czmq.h>

static void
subscriber_thread(zsock_t *pipe, void *args) {
    //  Subscribe to "A" and "B"
    /*
    void *subscriber = zsock_new (ZMQ_SUB);
    zsock_connect (subscriber, "tcp://127.0.0.1:6000");*/
    zsock_t *sub = zsock_new_sub("tcp://127.0.0.1:6000", "ENGINE");
    assert(sub);
    puts("sub connected");

    int count = 0;
    while (count < 5) {
        char *topic;
        char *frame;
        zmsg_t *msg;
        zsock_recv(sub, "sm", &topic, &msg);
        zsys_info("Recv on %s", topic);
        while (zmsg_size(msg) > 0) {
            frame = zmsg_popstr(msg);
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
    zactor_t *sub_actor = zactor_new(subscriber_thread, NULL);

    zactor_destroy(&sub_actor);
    return 0;
}

