#include <czmq.h>

int main(){


    long time_to_create_push_socket;
    long time_to_create_pull_socket;
    long time_end_to_end;
    long start;
    long end;
    start=zclock_usecs();
    zsock_t *push = zsock_new_push ("inproc://example");
    end=zclock_usecs();
    time_to_create_push_socket=end-start;
    start=zclock_usecs();
    zsock_t *pull = zsock_new_pull ("inproc://example");
    end=zclock_usecs();
    time_to_create_pull_socket=end-start;
    start=zclock_usecs();
    zstr_send (push, "just few bytes");
    char *string = zstr_recv (pull);
    end=zclock_usecs();
    time_end_to_end=end-start;

    puts (string);
    zstr_free (&string);
    printf("Time to create a new push socket: %ld [micro secs]\n", time_to_create_push_socket);
    printf("Time to create a new pull socket: %ld [micro secs]\n", time_to_create_pull_socket);
    printf("Time to send and receive a request : %ld [micro secs]\n", time_end_to_end);
    zsock_destroy (&pull);
    zsock_destroy (&push);


    return 0;




}



