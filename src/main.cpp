#include <cstdio>
#include <cstdlib>
#include <pthread.h>
#include <unistd.h>

#include "radio.h"

radio sbxp_app;

void* aircraft_run(void* thread_id) {
    long tid = (long) thread_id;
    printf("[GLO4993] Running with thread id: %ld\n", tid);
    
    while (1) {
        printf("GLO4993 transmitindo...\n");
        sbxp_app.transmit("GLO4993", "SBXP_APP", "Boa noite, GLO4993 com voce no FL330", "09L", CHECK_IN);
        printf("GLO4993 transmitiu\n");
        sleep(30);
        printf("GLO4993 ouvindo o radio\n");
        radio_message result = sbxp_app.listen("GLO4993");
        if (!result.blank) {
            printf("GLO4993 recebeu mensagem\n");
            printf("Origem: %s\nDestino: %s\nTipo: %d\nConteudo: %s\nArgs: %s\n", result.sender.c_str(), result.recipient.c_str(), result.type, result.content.c_str(), result.args.c_str());
        }
    }
}

void* tower_run(void* thread_id) {
    long tid = (long) thread_id;
    printf("[SBXP_APP] Running with thread id: %ld\n", tid);
    
    while (1) {
        sleep(10);
        printf("SBXP_APP ouvindo o radio\n");
        radio_message result = sbxp_app.listen("SBXP_APP");
        if (!result.blank) {
            printf("SBXP_APP recebeu mensagem\n");
            printf("Origem: %s\nDestino: %s\nTipo: %d\nConteudo: %s\nArgs: %s\n", result.sender.c_str(), result.recipient.c_str(), result.type, result.content.c_str(), result.args.c_str());
            
            printf("SBXP_APP transmitindo...\n");
            sbxp_app.transmit("SBXP_APP", "GLO4993", "Boa noite, GLO4993. Descida autorizada para o FL220", "", DESCEND_CLEARANCE);
            printf("SBXP_APP transmitiu\n");
        }
    }
    
    pthread_exit(0);
}

int main(int argc, char **argv) {

    pthread_t aircraft;
    pthread_t tower;

    pthread_create(&tower, NULL, tower_run, (void *) 123);
    pthread_create(&aircraft, NULL, aircraft_run, (void *) 145);

    pthread_exit(0);

    return 0;
}