
//  Espresso Pattern
//  This shows how to capture data using a pub-sub proxy

#include <czmq.h>
#include <json-c/json.h>

const char *endpoint_tcp = "tcp://127.0.0.1:6000";
const char *endpoint_inproc = "inproc://example";
#define ENDPOINT endpoint_tcp


static void
publisher_thread(zsock_t *pipe, void *args) {

    const char *string_json_path = "/home/filippocasari/CLionProjects/SampleThreadSafeQueue/parametersPUB.json";
    json_object *PARAM;

    int payload_size = 10;
    int num_mex = 10;
    const char *topic;
    int msg_rate_sec = 1;
    PARAM = json_object_from_file(string_json_path);
    zsock_t *pub;
    if (PARAM != NULL) {
        puts("PARAMETERS PUBLISHER: ");
        const char *type_connection;
        const char *port;
        const char *ip;
        const char *output_file;
        int int_value;

        const char *value;
        json_object_object_foreach(PARAM, key, val) {
            if (json_object_is_type(val, json_type_int)) {
                int_value = (int) json_object_get_int64(val);
                if (strcmp(key, "number_of_messages") == 0)
                    num_mex = int_value;
                if (strcmp(key, "payload_size_bytes") == 0)
                    payload_size = int_value;
                if (strcmp(key, "msg_rate_sec") == 0) {
                    msg_rate_sec = int_value;
                }
            } else {
                value = json_object_get_string(val);
            }


            printf("\t%s: %s\n", key, json_object_to_json_string(val));
            if (strcmp(key, "connection_type") == 0) {
                type_connection = value;
                printf("connection type found: %s\n", type_connection);

            }
            if (strcmp(key, "ip") == 0) {

                ip = value;
                printf("ip found: %s\n", ip);

            }
            if (strcmp(key, "port") == 0) {
                port = value;
                printf("port found: %s\n", port);
            }
            if (strcmp(key, "metric output file") == 0) {
                output_file = value;
                printf("output file found: %s\n", output_file);
            }
            if (strcmp(key, "topic") == 0) {
                topic = value;
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


    //size_of_payload = (int) strtol(payload_size, NULL, 10);
    //max_mex = strtol(num_mex, NULL, 10);

    printf("PAYLOAD SIZE: %d\n", payload_size);
    while (!zctx_interrupted && count < num_mex) {
        long double milli_secs_of_sleeping= (1.0/msg_rate_sec)*1000;

        zclock_sleep((int) milli_secs_of_sleeping); //  Wait for x seconds
        char string[12];
        char string_residual_payload[(payload_size - strlen(string))];

        long timestamp = zclock_usecs();
        printf("TIMESTAMP: %ld\n", timestamp);
        zmsg_t *msg = zmsg_new();
        sprintf(string, "%ld", timestamp);
        int rc = zmsg_pushstr(msg, string);
        assert(rc == 0);

        for (int i = 0; i < (payload_size - strlen(string)); i++) {
            string_residual_payload[i] = '0';
        }
        string_residual_payload[payload_size] = '\0';
        printf("String of zeros: %s\n", string_residual_payload);
        assert(rc == 0);
        rc = zmsg_addstr(msg, string_residual_payload);

        if (zsock_send(pub, "ssss", topic, "TIMESTAMP", string, string_residual_payload) == -1)
            break;              //  Interrupted
        zclock_log("Message No. %d", count);
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