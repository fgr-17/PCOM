/* Copyright 2016, Eric Pernia.
 * All rights reserved.
 *
 * This file is part sAPI library for microcontrollers.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 */

/*
 * Date: 2016-07-03
 */

/*==================[inclusions]=============================================*/


#include <stdint.h>
#include <string.h>
#include "sapi.h"    // <= sAPI header
#include "rtc_lib.h"   // <= own header (optional)

/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/* Buffer */
static char uartBuff[10];

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukás Chmela
 * Released under GPLv3.

 */
char* itoa(int value, char* result, int base) {
   // check that the base if valid
   if (base < 2 || base > 36) { *result = '\0'; return result; }

   char* ptr = result, *ptr1 = result, tmp_char;
   int tmp_value;

   do {
      tmp_value = value;
      value /= base;
      *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
   } while ( value );

   // Apply negative sign
   if (tmp_value < 0) *ptr++ = '-';
   *ptr-- = '\0';
   while(ptr1 < ptr) {
      tmp_char = *ptr;
      *ptr--= *ptr1;
      *ptr1++ = tmp_char;
   }
   return result;
}


/* Enviar fecha y hora en formato "DD/MM/YYYY, HH:MM:SS" */
void showDateAndTime( rtc_t * rtc ){
   /* Conversion de entero a ascii con base decimal */
   itoa( (int) (rtc->mday), (char*)uartBuff, 10 ); /* 10 significa decimal */
   /* Envio el dia */
   if( (rtc->mday)<10 )
      uartWriteByte( UART_USB, '0' );

   uartWriteString( UART_USB, uartBuff );
   uartWriteByte( UART_USB, '/' );

   /* Conversion de entero a ascii con base decimal */
   itoa( (int) (rtc->month), (char*)uartBuff, 10 ); /* 10 significa decimal */
   /* Envio el mes */
   if( (rtc->month)<10 )
      uartWriteByte( UART_USB, '0' );
   uartWriteString( UART_USB, uartBuff );
   uartWriteByte( UART_USB, '/' );

   /* Conversion de entero a ascii con base decimal */
   itoa( (int) (rtc->year), (char*)uartBuff, 10 ); /* 10 significa decimal */
   /* Envio el año */
   if( (rtc->year)<10 )
      uartWriteByte( UART_USB, '0' );
   uartWriteString( UART_USB, uartBuff );


   uartWriteString( UART_USB, ", ");


   /* Conversion de entero a ascii con base decimal */
   itoa( (int) (rtc->hour), (char*)uartBuff, 10 ); /* 10 significa decimal */
   /* Envio la hora */
   if( (rtc->hour)<10 )
      uartWriteByte( UART_USB, '0' );
   uartWriteString( UART_USB, uartBuff );
   uartWriteByte( UART_USB, ':' );

   /* Conversion de entero a ascii con base decimal */
   itoa( (int) (rtc->min), (char*)uartBuff, 10 ); /* 10 significa decimal */
   /* Envio los minutos */
  // uartBuff[2] = 0;    /* NULL */
   if( (rtc->min)<10 )
      uartWriteByte( UART_USB, '0' );
   uartWriteString( UART_USB, uartBuff );
   uartWriteByte( UART_USB, ':' );

   /* Conversion de entero a ascii con base decimal */
   itoa( (int) (rtc->sec), (char*)uartBuff, 10 ); /* 10 significa decimal */
   /* Envio los segundos */
   if( (rtc->sec)<10 )
      uartWriteByte( UART_USB, '0' );
   uartWriteString( UART_USB, uartBuff );


   /* Envio un 'enter' */
   uartWriteString( UART_USB, "\r\n");
}


/**
 * @fn paso fecha y hora a string
 *
 */

#define FECHAHORA_L 32
#define BUF_L	16
int fechaHoraAString ( rtc_t *rtc, char* cadena, int n) {

	char fechaHora [FECHAHORA_L];
	char buf [BUF_L];


	/* Conversion de entero a ascii con base decimal */
	itoa( (int) (rtc->mday), (char*)buf, BUF_L ); /* 10 significa decimal */
	strcpy(fechaHora, buf);
	/* Conversion de entero a ascii con base decimal */
	itoa( (int) (rtc->month), (char*)buf, 10 ); /* 10 significa decimal */
	strcat(fechaHora, buf);
	/* Conversion de entero a ascii con base decimal */
	itoa( (int) (rtc->year), (char*)buf, 10 ); /* 10 significa decimal */
	strcat(fechaHora, buf);
	/* Conversion de entero a ascii con base decimal */
	itoa( (int) (rtc->hour), (char*)buf, 10 ); /* 10 significa decimal */
	strcat(fechaHora, buf);
	/* Conversion de entero a ascii con base decimal */
	itoa( (int) (rtc->min), (char*)buf, 10 ); /* 10 significa decimal */
	strcat(fechaHora, buf);
	/* Conversion de entero a ascii con base decimal */
	itoa( (int) (rtc->sec), (char*)buf, 10 ); /* 10 significa decimal */
	strcat(fechaHora, buf);
	strcat(fechaHora, "\r\n");
	return 0;
}


// Guarda en str la fecha y hora convertido a string en formato "DD/MM/YYYY HH:MM:SS"
void rtcToString( char* rtcString, rtc_t* rtc ) {

   char rtcMemberString[10];
   rtcString[0] = 0;

   // "DD/MM/YYYY "

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->mday), rtcMemberString, 10 );
   if( (rtc->mday)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+mday+"/"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, "/", strlen("/") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->month), rtcMemberString, 10 );
   if( (rtc->month)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+month+"/"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, "/", strlen("/") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->year), rtcMemberString, 10 );
   // Concateno rtcString+year+" "
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, " ", strlen(" ") );

   // "HH:MM:SS"

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->hour), rtcMemberString, 10 );
   if( (rtc->hour)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+hour+":"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, ":", strlen(":") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->min), rtcMemberString, 10 );
   if( (rtc->min)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+min+":"
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
   strncat( rtcString, ":", strlen(":") );

   // Conversion de entero a string con base 10 (decimal)
   int64ToString( (rtc->sec), rtcMemberString, 10 );
   if( (rtc->sec)<10 ){
      // Concateno rtcString+"0"
      strncat( rtcString, "0", strlen("0") );
   }
   // Concateno rtcString+sec
   strncat( rtcString, rtcMemberString, strlen(rtcMemberString) );
}
/* FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET. */
int inicializarRTC (void) {

   /* ------------- INICIALIZACIONES ------------- */


   /* Estructura RTC */
   rtc_t rtc;

   rtc.year = 2018;
   rtc.month = 7;
   rtc.mday = 4;
   rtc.wday = 1;
   rtc.hour = 18;
   rtc.min = 26;
   rtc.sec= 0;

   bool_t val = 0;
   uint8_t i = 0;

   /* Inicializar RTC */
   val = rtcConfig( &rtc );

   delay_t delay1s;
   delayConfig( &delay1s, 1000 );

   delay(2000); // El RTC tarda en setear la hora, por eso el delay


   /*for( i=0; i<10; i++ ){
       Leer fecha y hora
      val = rtcRead( &rtc );
       Mostrar fecha y hora en formato "DD/MM/YYYY, HH:MM:SS"
      showDateAndTime( &rtc );
      delay(1000);
   }*/

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return val ;
}

/*==================[end of file]============================================*/
