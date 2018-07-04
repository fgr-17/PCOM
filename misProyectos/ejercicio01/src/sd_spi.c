/* Copyright 2014, ChaN
 * Copyright 2016, Matias Marando
 * Copyright 2016, Eric Pernia
 * All rights reserved.
 *
 * This file is part of Workspace.
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

/*==================[inlcusiones]============================================*/

#include <string.h>

#include "../../ejercicio01/inc/sd_spi.h"   // <= own header (optional)

#include "sapi.h"     // <= sAPI header

#include "ff.h"       // <= Biblioteca FAT FS

#include "../inc/rtc_lib.h"

/*==================[definiciones y macros]==================================*/

#define FILENAME "hola.txt"

/*==================[definiciones de datos internos]=========================*/

static FATFS fs;           // <-- FatFs work area needed for each volume
static FIL fp;             // <-- File object needed for each open file
extern int inicializarRTC (void);
/*==================[definiciones de datos externos]=========================*/

/*==================[declaraciones de funciones internas]====================*/

/*==================[declaraciones de funciones externas]====================*/

// FUNCION que se ejecuta cada vezque ocurre un Tick
void diskTickHook( void *ptr );


/*==================[funcion principal]======================================*/

// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE ENCENDIDO O RESET.
int main( void ){

	char fechaHora [32];
	bool_t val;
   /* Estructura RTC */
   rtc_t rtc;
   // ---------- CONFIGURACIONES ------------------------------
   // Inicializar y configurar la plataforma
   boardConfig();

   // SPI configuration
   spiConfig( SPI0 );

   // Inicializar el conteo de Ticks con resolucion de 10ms,
   // con tickHook diskTickHook
   tickConfig( 10 );
   tickCallbackSet( diskTickHook, NULL );


   /* inicializo RTC */
   inicializarRTC();

   // ------ PROGRAMA QUE ESCRIBE EN LA SD -------

   UINT nbytes;

   uartWriteString( UART_USB, "Ejercicio 01:\r\n" );
   uartWriteString( UART_USB, "Inicializando SD..." );


   // Give a work area to the default drive
   if( f_mount( &fs, "", 0 ) != FR_OK ){
	   uartWriteString( UART_USB, "no se pudo inicializar la SD, esta conectada?\r\n" );
	   while(1);
      // If this fails, it means that the function could
      // not register a file system object.
      // Check whether the SD card is correctly connected
   }

   // Create/open a file, then write a string and close it

   uint8_t i=0;



   if( f_open( &fp, FILENAME, FA_WRITE | FA_OPEN_APPEND ) == FR_OK ){

	   for( i=0; i<5; i++ ) {


    	  val = rtcRead( &rtc );
    	  fechaHoraAString(&rtc, fechaHora, 32);
    	  f_write( &fp, fechaHora, strlen(fechaHora), &nbytes );
         f_write( &fp, "Hola mundo\r\n", 12, &nbytes );

         f_close(&fp);

         if( nbytes == 12 ){
            // Turn ON LEDG if the write operation was successful
            gpioWrite( LEDG, ON );
         }
	   }

   }
   else {
		// Turn ON LEDR if the write operation was fail
		uartWriteString( UART_USB, "No escribir la memoria SD, esta conectada?\r\n" );
		gpioWrite( LEDR, ON );
   }

   // ---------- REPETIR POR SIEMPRE --------------------------
   while( TRUE )
   {
      sleepUntilNextInterrupt();
   }

   // NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa se ejecuta
   // directamenteno sobre un microcontroladore y no es llamado/ por ningun
   // Sistema Operativo, como en el caso de un programa para PC.
   return 0;
}

/*==================[definiciones de funciones internas]=====================*/

/*==================[definiciones de funciones externas]=====================*/

// FUNCION que se ejecuta cada vezque ocurre un Tick
void diskTickHook( void *ptr ){
   disk_timerproc();   // Disk timer process
}


/*==================[fin del archivo]========================================*/
