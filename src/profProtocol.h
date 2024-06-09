#ifndef PROFPROTOCOL_H
#define PROFPROTOCOL_H

#include "HardwareSerial.h"

class SerialProfisy {
    public:
        char arraytotal[150];
        uint8_t lengArray;
        bool existeValor = false;

        char usuario[25], nomedoproduto[25];
        int32_t Concetracao;
        float volume_de_agua, calibracao_fluxo;

        char *value[4];
        uint8_t value_leng[3];
        uint8_t values;

        uint8_t contLengRxSerialProf;

        bool lerDadosSerial();
        void decodeDadosSerial();
        void atualizarDadosParaAzure();
};

void initSerialProf();
void enviarResposta(char *array, uint16_t leng);
void fw(uint8_t x);
void timestamp2Ser(time_t x);

#endif