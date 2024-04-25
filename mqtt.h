extern struct mosquitto* GetMqtt();

extern int InitMqtt(struct mosquitto *mosq1);

extern void on_message1(struct mosquitto *mosq, void *obj, const struct mosquitto_message *message);

extern void on_connect1(struct mosquitto *mosq, void *obj, int result);

extern void publish_message(struct mosquitto *mosq, const char *payload);

extern int DestroyMQTT(struct mosquitto *mosq1);