
#include <czmq.h>
#define NUM_SUB 2


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


    zactor_t *sub_threads[NUM_SUB];
    for(int i=0; i<NUM_SUB; i++){
        sub_threads[i]=zactor_new(subscriber_thread, NULL);
    }
    for(int i=0; i<NUM_SUB;i++){
        zactor_destroy(&sub_threads[i]);
    }

    return 0;
}

