#include <czmq.h>


const char *endpoint_tcp= "tcp://127.0.0.1:5000";

const char *endpoint_inproc= "inproc://example";

#define ENDPOINT endpoint_inproc

// "tcp://127.0.0.1:5000"
// "inproc://example"


int main() {

    long time_to_create_push_socket;
    long time_to_create_pull_socket;
    long time_end_to_end;
    long start;
    long end;


    start = (long) zclock_usecs();

    zsock_t *push = zsock_new_push (ENDPOINT);
    end = (long) zclock_usecs();
    time_to_create_push_socket = end - start;
    start = zclock_usecs();
    zsock_t *pull = zsock_new_pull (ENDPOINT);
    end = zclock_usecs();
    time_to_create_pull_socket = end - start;
    start = zclock_usecs();
    zstr_send(push, "1234567890"); //10 bytes
    char *string = zstr_recv(pull);
    int size = (int) strlen(string);

    printf("SIZE OF PAYLOAD: %d bytes\n", size);
    end = zclock_usecs();
    time_end_to_end = end - start;

    puts(string);
    zstr_free(&string);
    printf("Time to create a new push socket: %ld [micro secs]\n", time_to_create_push_socket);
    printf("Time to create a new pull socket: %ld [micro secs]\n", time_to_create_pull_socket);
    printf("Time end to end : %ld [micro secs]\n", time_end_to_end);
    zsock_destroy (&pull);
    zsock_destroy (&push);

    return 0;


}



