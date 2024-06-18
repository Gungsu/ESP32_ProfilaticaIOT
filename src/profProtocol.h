#ifndef PROFPROTOCOL_H
#define PROFPROTOCOL_H

#include "HardwareSerial.h"

class SerialProfisy {
    public:
        char arraytotal[150];
        char respToProfsys[20];
        char cmdNameList[4][25] = {"calibracaofluxometro", "mudanca1", "mudanca2","mudanca3"};
        uint8_t lengArray;
        bool existeValor = false;
        bool sendToazure = false;

        char usuario[25], nomedoproduto[25];
        int32_t Concetracao;
        float volume_de_agua, calibracao_fluxo, volume_de_quimico, opcao_de_dosagem, calibbomba;

        char *value[4];
        uint8_t value_leng[3];
        uint8_t values;

        uint8_t contLengRxSerialProf;

        bool lerDadosSerial();
        void decodeDadosSerial();
        void atualizarDadosParaAzure();
        uint8_t sizeVle(char *vle);
        uint16_t azureReadAndSendprofsys(char *cmd, char *vle);

        SerialProfisy(){
            memset(respToProfsys,0,20);
        }
};

void initSerialProf();
void enviarResposta(char *cmd, char *arrayvl, uint16_t leng);
void fw(uint8_t x);
void timestamp2Ser(time_t x);

#endif