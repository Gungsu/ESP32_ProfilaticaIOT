// Copyright (c) Microsoft Corporation. All rights reserved.
// SPDX-License-Identifier: MIT

// Wifi
#define IOT_CONFIG_WIFI_SSID "0"
#define IOT_CONFIG_WIFI_PASSWORD "0"

// Enable macro IOT_CONFIG_USE_X509_CERT to use an x509 certificate to authenticate the IoT device.
// The two main modes of authentication are through SAS tokens (automatically generated by the
// sample using the provided device symmetric key) or through x509 certificates. Please choose the
// appropriate option according to your device authentication mode.

// #define IOT_CONFIG_USE_X509_CERT

#ifdef IOT_CONFIG_USE_X509_CERT

/*
 * Please set the define IOT_CONFIG_DEVICE_CERT below with
 * the content of your device x509 certificate.
 *
 * Example:
 * #define IOT_CONFIG_DEVICE_CERT "-----BEGIN CERTIFICATE-----\r\n" \
 * "MIIBJDCBywIUfeHrebBVa2eZAbouBgACp9R3BncwCgYIKoZIzj0EAwIwETEPMA0G\r\n" \
 * "A1UEAwwGRFBTIENBMB4XDTIyMDMyMjazMTAzN1oXDTIzMDMyMjIzMTAzN1owGTEX\r\n" \
 * "MBUGA1UEAwwOY29udG9zby1kZXZpY2UwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNC\r\n" \
 * .......
 * "YmGzdaHTb6P1W+p+jmc+jJn1MAoGCXqGSM49BAMCA0gAMEUCIEnbEMsAdGFroMwl\r\n" \
 * "vTfQahwsxN3xink9z1gtirrjQlqDAiEAyU+6TUJcG6d9JF+uJqsLFpsbbF3IzGAw\r\n" \
 * "yC+koNRC0MU=\r\n" \
 * "-----END CERTIFICATE-----"
 *
 */
#define IOT_CONFIG_DEVICE_CERT "Device Certificate"

/*
 * Please set the define IOT_CONFIG_DEVICE_CERT_PRIVATE_KEY below with
 * the content of your device x509 private key.
 *
 * Example:
 *
 * #define IOT_CONFIG_DEVICE_CERT_PRIVATE_KEY "-----BEGIN EC PRIVATE KEY-----\r\n" \
 * "MHcCAQEEIKGXkMMiO9D7jYpUjUGTBn7gGzeKPeNzCP83wbfQfLd9obAoGCCqGSM49\r\n" \
 * "AwEHoUQDQgAEU6nQoYbjgJvBwaeD6MyAYmOSDg0QhEdyyV337qrlIbDEKvFsn1El\r\n" \
 * "yRabc4dNp2Jhs3Xh02+j9Vvqfo5nPoyZ9Q==\r\n" \
 * "-----END EC PRIVATE KEY-----"
 *
 * Note the type of key may different in your case. Such as BEGIN PRIVATE KEY
 * or BEGIN RSA PRIVATE KEY.
 *
 */
#define IOT_CONFIG_DEVICE_CERT_PRIVATE_KEY "Device Certificate Private Key"

#endif // IOT_CONFIG_USE_X509_CERT

// Azure IoT Central
#define ESPPROF

#ifdef ESPAMA
#define DPS_ID_SCOPE "0ne00A2AF58"
#define IOT_CONFIG_DEVICE_ID "1zcq38lrvrk"
// Use device key if not using certificates
#ifndef IOT_CONFIG_USE_X509_CERT
#define IOT_CONFIG_DEVICE_KEY "R3dGRAFSfj6PRhtbpu4RKtK2ljyPmMfalNp876gRyT4="
#endif // IOT_CONFIG_USE_X509_CERT
#endif

#ifdef ESPPROF
#define DPS_ID_SCOPE "0ne009F0365"
#define IOT_CONFIG_DEVICE_ID "2cv8jqk9t7d"
// Use device key if not using certificates
#ifndef IOT_CONFIG_USE_X509_CERT
#define IOT_CONFIG_DEVICE_KEY "9lRG59V+AMIN7k82lb30fggdqEW1Eh+/B0vLr2Ksrwk="
#endif // IOT_CONFIG_USE_X509_CERT
#endif

// User-agent (url-encoded) provided by the MQTT client to Azure IoT Services.
// When developing for your own Arduino-based platform,
// please update the suffix with the format '(ard;<platform>)' as an url-encoded string.
#define AZURE_SDK_CLIENT_USER_AGENT "c%2F" AZ_SDK_VERSION_STRING "(ard%3Besp32)"

// Publish 1 message every 10 seconds.
#define TELEMETRY_FREQUENCY_IN_SECONDS 10

// For how long the MQTT password (SAS token) is valid, in minutes.
// After that, the sample automatically generates a new password and re-connects.
#define MQTT_PASSWORD_LIFETIME_IN_MINUTES 60
