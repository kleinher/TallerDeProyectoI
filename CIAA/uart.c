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
 * Date: 2016-04-26
 */

/*==================[inclusions]=============================================*/

#include "sapi.h"     // <= sAPI header
#include "cJSON.h"
/*==================[macros and definitions]=================================*/

/*==================[internal data declaration]==============================*/

/*==================[internal functions declaration]=========================*/

/*==================[internal data definition]===============================*/

/*==================[external data definition]===============================*/

/*==================[internal functions definition]==========================*/

/*==================[external functions definition]==========================*/


/**
 * C++ version 0.4 char* style "itoa":
 * Written by Lukas Chmela
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


// FUNCION PRINCIPAL, PUNTO DE ENTRADA AL PROGRAMA LUEGO DE RESET.
int main(void){

   // ------------- INICIALIZACIONES -------------

   // Inicializar la placa
   boardConfig();

   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 115200 );
   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_232, 115200 );

   uint8_t dato  = 0;



   char miTexto[] = "Hola de nuevo\r\n";

   uartWriteString( UART_USB, miTexto ); // Envi­a "Hola de nuevo\r\n"

   uartWriteString( UART_USB, "\r\n" ); // Enviar un Enter

   int test = 1;
   int i = 0;
   char* palabra =  (char *) calloc(50 , sizeof(char));
   const cJSON *luz_1 = NULL;
   // ------------- REPETIR POR SIEMPRE -------------
   while(1) {
      
      // Si recibe un byte de la UART_USB lo guardarlo en la variable dato.
      if(palabra != NULL){
         if(  uartReadByte( UART_232, &dato ) ){
            uartReadByte( UART_USB, dato);
            if(dato != '\r'){
               palabra[i]=dato;
               i++;
            }
            else{
               palabra[i]=dato;
               i++;
               palabra[i]='\0';
               
                  
               cJSON *json = cJSON_Parse(palabra);
               const char *error_ptr = cJSON_GetErrorPtr();
               if (error_ptr != NULL || json != NULL)
                {
                    
                  luz_1 = cJSON_GetObjectItemCaseSensitive(json, "luz_1");
                  if(cJSON_IsTrue(luz_1)){
                     uartWriteString( UART_USB, "\r\n" );
                     uartWriteString( UART_USB, "ES Verdader" );
                     uartWriteString( UART_USB, "\r\n" );
                  }
                  char *string = cJSON_Print(json);
                  uartWriteString( UART_USB,  string);
                  uartWriteString( UART_USB, "\r\n" );

                  i=0;
                }

               free(palabra);
               cJSON_Delete(json);
               cJSON_Delete(luz_1);
               palabra =  (char *) calloc(50 , sizeof(char));
            }
            // Se reenvia el dato a la UART_USB realizando un eco de lo que llega
            //uartWriteByte( UART_USB, dato );
            
            
         }
      }
      else{
       palabra =  (char *) calloc(50 , sizeof(char));  
      }
      
      
   }

   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   return 0 ;
}

/*==================[end of file]============================================*/
