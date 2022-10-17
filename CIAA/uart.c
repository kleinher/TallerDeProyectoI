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


   int size_json= 50 * sizeof(char);
int leerJson(char* palabra){
   uint8_t size_json= 200 * sizeof(char);
   uint8_t dato  = 0;
   int i = 0;
   
   while( (i < size_json) && (dato != '\n')){
      if(uartReadByte( UART_232, &dato)){
         palabra[i]=dato;
         i++;
      }
   }
   palabra[i-1]='\0';
}
int imprimirJson(char* palabra){
   const cJSON *luz_1 = NULL;
   cJSON *json = cJSON_Parse(palabra);
   const char *error_ptr = cJSON_GetErrorPtr();
   if (error_ptr != NULL || json != NULL)
   {
        luz_1 = cJSON_GetObjectItemCaseSensitive(json, "luz_1");
        if(cJSON_IsTrue(luz_1)){
            uartWriteString( UART_USB, "\r\n" );
            uartWriteString( UART_USB, "ES Verdader" );
            uartWriteString( UART_USB, "\r\n" );
        }else{
            uartWriteString( UART_USB, "\r\n" );
            uartWriteString( UART_USB, "ES false" );
            uartWriteString( UART_USB, "\r\n" );
        }
        char *string = cJSON_Print(json);
        uartWriteString( UART_USB,  string);
        uartWriteString( UART_USB, "\r\n" );
   }
      cJSON_Delete(json);
   free(luz_1);
   return 1;
}
int setup(){
   // ------------- INICIALIZACIONES -------------

   // Inicializar la placa
   boardConfig();

   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_USB, 9600 );
   // Inicializar UART_USB a 115200 baudios
   uartConfig( UART_232, 9600 );


   char miTexto[] = "Hola de nuevo\r\n";
   uartWriteString( UART_USB, miTexto ); // Envi­a "Hola de nuevo\r\n"
   uartWriteString( UART_USB, "\r\n" ); // Enviar un Enter
}

int main(void){

   setup();


   // ------------- REPETIR POR SIEMPRE -------------
   char* palabra;
   char* json_final;

  uint8_t dato  = 0;
  int print = 0;


   palabra =  (char *) calloc(300 , sizeof(char));  
   json_final =  (char *) calloc(50 , sizeof(char));  
   

   
   leerJson(palabra);
   imprimirJson(palabra);


   free(palabra);  
   free(json_final);
   


   /* NO DEBE LLEGAR NUNCA AQUI, debido a que a este programa no es llamado
      por ningun S.O. */
   


}

/*==================[end of file]============================================*/
