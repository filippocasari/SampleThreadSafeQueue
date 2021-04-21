

//  Espresso Pattern
//  This shows how to capture data using a pub-sub proxy

#include <czmq.h>

//  The subscriber thread requests messages starting with
//  A and B, then reads and counts incoming messages.



//  .split publisher thread
//  The publisher sends random messages starting with A-J:

static void
publisher_thread(zsock_t *pipe, void *args) {
    zsock_t *pub = zsock_new_pub("tcp://127.0.0.1:6000");
    /*void *publisher = zsock_new (ZMQ_PUB);
    zsock_bind (publisher, "tcp://127.0.0.1:6000");
    */
    puts("pub connected");
    while (!zctx_interrupted) {
        char string[10];
        sprintf(string, "%c-%05d", randof (10) + 'A', randof (100000));
        //printf("String: %s\n", string);
        zmsg_t *msg = zmsg_new();
        int rc = zmsg_pushstr(msg, string);
        assert(rc == 0);
        if (zsock_send(pub, "sss", "ENGINE", "MESSAGE PART", string) == -1)
            break;              //  Interrupted
        zclock_sleep(100);     //  Wait for 1/10th second
    }
    zsock_destroy(&pub);
}

//  .split listener thread
//  The listener receives all messages flowing through the proxy, on its
//  pipe. In CZMQ, the pipe is a pair of ZMQ_PAIR sockets that connect
//  attached child threads. In other languages your mileage may vary:

static void
listener_thread(zsock_t *pipe, void *args) {
    //  Print everything that arrives on pipe
    while (true) {
        zframe_t *frame = zframe_recv(pipe);
        if (!frame)
            break;              //  Interrupted
        zframe_print(frame, NULL);
        zframe_destroy(&frame);
    }
}

//  .split main thread
//  The main task starts the subscriber and publisher, and then sets
//  itself up as a listening proxy. The listener runs as a child thread:

int main(void) {
    //  Start child threads

    zactor_t *pub_actor = zactor_new(publisher_thread, NULL);

    zactor_destroy(&pub_actor);
    return 0;
}