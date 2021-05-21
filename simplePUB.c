
//  Espresso Pattern
//  This shows how to capture data using a pub-sub proxy

#include <czmq.h>
#include <json-c/json.h>

const char *endpoint_tcp = "tcp://127.0.0.1:6000";
const char *endpoint_inproc = "inproc://example";
#define ENDPOINT endpoint_tcp


static void
publisher_thread(zsock_t *pipe, void *args) {

    char *string_json_parameters;
    json_object *PARAM;

    PARAM = json_object_from_file("C:\\Users\\utente\\CLionProjects\\SampleThreadSafeQueue2\\parametersPUB.json");
    if(PARAM!=NULL){
        puts("PARAMETERS PUBLISHER: ");
        json_object_object_foreach(PARAM, key, val) {
            printf("\t%s: %s\n", key, json_object_to_json_string(val));
        }
    }


    zsock_t *pub = zsock_new_pub(ENDPOINT);
    /*void *publisher = zsock_new (ZMQ_PUB);
    zsock_bind (publisher, "tcp://127.0.0.1:6000");
    */
    int count = 0;
    puts("pub connected");
    while (!zctx_interrupted || count < 10) {

        char string[12];
        long timestamp = zclock_usecs();
        printf("TIMESTAMP: %ld\n", timestamp);
        zmsg_t *msg = zmsg_new();
        sprintf(string, "%ld", timestamp);
        int rc = zmsg_pushstr(msg, string);
        assert(rc == 0);

        if (zsock_send(pub, "sss", "ENGINE", "TIMESTAMP", string) == -1)
            break;              //  Interrupted
        zclock_sleep(2000);//  Wait for x seconds
        count++;
    }
    zsock_destroy(&pub);
}

int main(void) {
    //  Start child threads

    zactor_t *pub_actor = zactor_new(publisher_thread, NULL);

    zactor_destroy(&pub_actor);
    return 0;
}