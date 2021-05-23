#ZMQ PATTERN SAMPLEs

1) PUSH-PULL sample 
    2 configurations: inproc and tcp. You can change the endpoint by just changing variable called ENDPOINT
2) PUB SUB sample
    same as PUSH-PULL sample.
    You can set how many subs you want to create
   
##PUB-SUB pattern

###PUBLISHER
-It takes a specific configuration from a json file called "parameters.json" ,
and push, on a specific topic, 12 bytes for the timestamp plus 
the rest of the bytes (=bytes of config - bytes of timestamp)

-It can work with a tcp or inproc connection.


-It will stop when the count is equal to the size of messages set by the json config file.

