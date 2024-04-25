extern int InitMqtt();

extern void on_message1(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);

extern void on_connect1(struct mosquitto *mosq, void *obj, int result);

extern void publish_message(struct mosquitto *mosq, const char *topic, const char *payload, int qos);