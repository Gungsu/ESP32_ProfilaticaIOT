#include "profProtocol.h"

HardwareSerial Serialprofisys(1);

void initSerialProf()
{
    Serialprofisys.begin(9600, SERIAL_8N1, 27, 26);
}

void enviarResposta(char *array, uint16_t leng) {
    Serialprofisys.write(array);
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
        enviarResposta(toSend, 6);
    } // WIFI WAIT SSID & PASS
    else if (x == 1)
    {
        char toSend[] = "FW:1\n";
        enviarResposta(toSend, 6);
    } // WIFI ON
    else if (x == 2)
    {
        char toSend[] = "FW:2\n";
        enviarResposta(toSend, 6);
    } // WIFI OFF
    else if (x == 3)
    {
        char toSend[] = "FW:3\n";
        enviarResposta(toSend, 6);
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

