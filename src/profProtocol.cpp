#include "profProtocol.h"

HardwareSerial Serialprofisys(1);

enum {
    calibracaofluxometro,
    mudanca1,
    mudanca2,
    mudanca3,
    lotepos
};

void initSerialProf()
{
    Serialprofisys.begin(9600, SERIAL_8N1, 27, 26);
}

void enviarResposta(char *cmd, char *arrayvl, uint16_t vl_leng)
{
    Serialprofisys.write(cmd);
    Serialprofisys.write(':');
    Serialprofisys.write(arrayvl,vl_leng-1);
    Serialprofisys.println();
}

void enviarResposta(char *cmd, String arrayvl)
{
    Serialprofisys.write(cmd);
    Serialprofisys.write(':');
    Serialprofisys.println(arrayvl);
}

void timestamp2Ser(time_t time)
{
    struct tm *lt = localtime(&time);
    char str[32];
    delay(5);
    Serialprofisys.print("TS:");
    delay(5);
    strftime(str, sizeof str, "%Y.", lt);
    Serialprofisys.print(str);
    delay(5);
    strftime(str, sizeof str, "%m.%d", lt);
    Serialprofisys.print(str);
    delay(5);
    strftime(str, sizeof str, ":%H.%M", lt);
    Serialprofisys.print(str);
    delay(5);
    strftime(str, sizeof str, ".%S", lt);
    Serialprofisys.println(str);
    Serial.println(time);
}

void fw(uint8_t x)
{ // FAULT CONEXOES WIFI AZURE
    if (x == 0)
    {
        char toSend[] = "FW:0\n";
        Serialprofisys.print(toSend);
    } // WIFI WAIT SSID & PASS
    else if (x == 1)
    {
        char toSend[] = "FW:1\n";
        Serialprofisys.print(toSend);
    } // WIFI ON
    else if (x == 2)
    {
        char toSend[] = "FW:2\n";
        Serialprofisys.print(toSend);
    } // WIFI OFF
    else if (x == 3)
    {
        char toSend[] = "FW:3\n";
        Serialprofisys.print(toSend);
    } // AZURE CONNECTED
}

void SerialProfisy::decodeDadosSerial()
{
    for (uint16_t i = 0; i < this->lengArray; i++)
    {
        if (this->arraytotal[i] != 0x0A || this->arraytotal[i] == 0x00 || this->arraytotal[i] > 0xA9)
        {
            this->lengArray = i;
            break;
        }
    }
    uint8_t cont;
    char *pch;
    pch = strtok(this->arraytotal, ":\n");
    while (pch != NULL)
    {
        this->value[cont] = pch;
        pch = strtok(NULL, ":\n"); // SS
        cont++;
    }
    this->values = cont;
}

bool SerialProfisy::lerDadosSerial()
{
    // memset(serialprofi.arrayPoint,0x00,255);
    while (Serialprofisys.available())
    {
        this->arraytotal[contLengRxSerialProf] = Serialprofisys.read();
        if (this->arraytotal[contLengRxSerialProf] == char('\n'))
        {
            this->lengArray = contLengRxSerialProf;
            this->arraytotal[contLengRxSerialProf + 1] = 0x00;
            this->arraytotal[contLengRxSerialProf + 2] = 0x00;
            this->contLengRxSerialProf = 0;
            this->existeValor = true;
            return true;
        }
        else
        {
            this->contLengRxSerialProf++;
            if (this->contLengRxSerialProf > 149)
            {
                this->contLengRxSerialProf = 0;
            }
            this->existeValor = true;
            return false;
        }
    }
    this->existeValor = false;
    return false;
}

void SerialProfisy::atualizarDadosParaAzure(){
    String toCompar = this->value[0];
    String vl = this->value[1];
    if (toCompar == "DO")
    {
        Serial.println("DO");
        Concetracao = atoi(vl.c_str());
        Serial.println(Concetracao);
        this->existeValor = false;
    }
    else if (toCompar == "VO")
    {
        Serial.println("VO");
        this->volume_de_agua = atof(vl.c_str());
        Serial.println(this->volume_de_agua);
        this->existeValor = false;
    }
    else if (toCompar == "VQ")
    {
        Serial.println("VQ");
        this->volume_de_quimico = atof(vl.c_str());
        Serial.println(this->volume_de_quimico);
        this->existeValor = false;
    }
    else if (toCompar == "CB")
    {
        Serial.println("CB");
        this->calibbomba = atof(vl.c_str());
        Serial.println(this->calibbomba);
        this->existeValor = false;
    }
    else if (toCompar == "OD")
    {
        Serial.println("OD");
        this->opcao_de_dosagem = atof(vl.c_str());
        Serial.println(this->opcao_de_dosagem);
        this->existeValor = false;
    }
    else if (toCompar == "CF")
    {
        Serial.println("CF");
        this->calibracao_fluxo = atof(vl.c_str());
        Serial.println(this->calibracao_fluxo);
        this->existeValor = false;
    }
    else if (toCompar == "US")
    {
        Serial.print("User: ");
        strcpy(usuario,vl.c_str());
        Serial.println(this->usuario);
        this->existeValor = false;
        this->sendToazure = true;
    }
    else if (toCompar == "PU")
    {
        Serial.print("Produto: ");
        strcpy(nomedoproduto, vl.c_str());
        Serial.println(this->nomedoproduto);
        this->existeValor = false;
    }
    else
    {
        Serial.println("Fim de linha");
        this->existeValor = false;
    }
}

uint8_t SerialProfisy::sizeVle(char *vle)
{
    uint8_t cont;
    for(cont=1;cont<20;cont++){
        if (vle[cont] == '"'){
            return cont;
        }
    }
    return 0;
}

uint16_t SerialProfisy::azureReadAndSendprofsys(char *cmd, char *vle) {
    uint8_t pos;
    char cmdH[4] = {'\0'};
    int x;
    for (int i = 0; i < numCmds; i++)
    {
        x = strcmp(cmdNameList[i], cmd);
        if (x == 0)
        {
            pos = uint8_t(i);
            break;
        }
    }
    switch (pos)
    {
        case calibracaofluxometro:
            strncpy(cmdH,"CF",sizeof(cmdH)-1);
            enviarResposta(cmdH,vle+1,this->sizeVle(vle));
            return 202;
            break;

        case mudanca1:
            strncpy(cmdH, "D1", sizeof(cmdH) - 1);
            enviarResposta(cmdH, vle + 1, this->sizeVle(vle));
            return 202;
            break;

        case mudanca2:
            strncpy(cmdH, "D2", sizeof(cmdH) - 1);
            enviarResposta(cmdH, vle + 1, this->sizeVle(vle));
            return 202;
            break;

        case mudanca3:
            strncpy(cmdH, "D3", sizeof(cmdH) - 1);
            enviarResposta(cmdH, vle + 1, this->sizeVle(vle));
            return 202;
            break;

        case lotepos:
            strncpy(cmdH, "LO", sizeof(cmdH) - 1);
            enviarResposta(cmdH, vle + 1, this->sizeVle(vle));
            return 202;
            break;

        default:
            return 404;
            break;
    }
}
