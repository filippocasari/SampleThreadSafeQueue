
//  Espresso Pattern
//  This shows how to capture data using a pub-sub proxy

#include <czmq.h>
#include <json-c/json.h>

const char *endpoint_tcp = "tcp://127.0.0.1:6000";
const char *endpoint_inproc = "inproc://example";
#define ENDPOINT endpoint_tcp


static void
publisher_thread(zsock_t *pipe, void *args) {

    const char *string_json_path="/home/filippocasari/CLionProjects/SampleThreadSafeQueue/parametersPUB.json";

    json_object *PARAM;

    const char *payload_size="10";
    const char *num_mex="10";

    PARAM = json_object_from_file(string_json_path);
    zsock_t *pub;
    if (PARAM != NULL) {
        puts("PARAMETERS PUBLISHER: ");
        const char *type_connection;
        const char *port;
        const char *ip;
        const char *output_file;

        json_object_object_foreach(PARAM, key, val) {
            const char *value = json_object_get_string(val);

            printf("\t%s: %s\n", key, json_object_to_json_string(val));
            if (strcmp(key, "connection type") == 0) {
                type_connection = value;
                printf("connection type found: %s\n", type_connection);

            }
            if (strcmp(key, "ip") == 0) {

                ip = value;
                printf("ip found: %s\n",ip);

            }
            if (strcmp(key, "port") == 0) {
                port = value;
                printf("port found: %s\n",port);
            }
            if (strcmp(key, "metric output file") == 0) {

                output_file = value;
                printf("output file found: %s\n",output_file);
            }
            if(strcmp(key, "payload size")==0){
                payload_size=value;

            }
            if(strcmp(key, "number of messages")==0){
                num_mex=value;

            }


        }
        char endpoint[30];
        char *endpoint_customized = strcat(endpoint, type_connection);
        endpoint_customized = strcat(endpoint_customized, "://");
        endpoint_customized = strcat(endpoint_customized, ip);
        endpoint_customized = strcat(endpoint_customized, ":");
        endpoint_customized = strcat(endpoint_customized, port);
        printf("string for endpoint (from json file): %s\t", endpoint_customized);


        pub = zsock_new_pub(endpoint_customized);

    } else {
        pub = zsock_new_pub(ENDPOINT);

    }




    /*void *publisher = zsock_new (ZMQ_PUB);
    zsock_bind (publisher, "tcp://127.0.0.1:6000");
    */
    int count = 0;
    puts("pub connected");
    long max_mex;
    long size_of_payload;
    size_of_payload= strtol(payload_size, NULL, 10);
    max_mex=strtol(num_mex, NULL, 10);

    while (!zctx_interrupted || count < max_mex) {
        //metrica errori, message size,
        char string[11];

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