sbit bit_PWM at RB0_bit;
unsigned int PWM=0;
unsigned int Pulso_PWM=0;
unsigned int valor=0;
unsigned int nuevo=0;
unsigned int giro_iz=0;
unsigned int giro_der=0;
const float numero=0.195;

void interrupt() {
    if(TMR0IF_bit){
        Pulso_PWM++;              //contador de interrupciones cada 10us
        if(Pulso_PWM==2000){      //si llega a 20ms
            Bit_PWM=1;
            Pulso_PWM=0;

        }
        if(Pulso_PWM==PWM){       //si Pulso_PWM es igual a PWM
            Bit_PWM=0;

        }
        TMR0=255;                 //cargamos TMR0 con 255
        TMR0IF_bit=0;             //salimos de interrupcion
    }
}

void main() {
ADCON1=0;           //Activo las entradas analógicas
PORTB=0;            // Puerto B como salida
TRISB=0x00;
TRISC=0x00;
PORTC=0x00;
PORTA=0x00;            // Puerto A como entrada
TRISA=0xC1;
OPTION_REG=0x83;    //prescaler del TMR0 = 1:16
GIE_bit=1;          // activamos las interrupciones globales
TMR0IE_bit=1;       // activamos interrupcion del TMR0
TMR0IF_bit=0;       // borramos el flag de desbordamiento del TMR0
TMR0=255;           //cargamos el TMR0=255 para generar 10us
valor=45;
nuevo=valor;
while(1){
    //si se desea mover con potenciometro, se debe de activar lo que las sentencias analog_pot_ser y desactivar
    //las sentencias iz_der_servo
//analog_pot_serv:
    //valor=ADC_read(0);   // leemos el canal analógico 0, por cada 1% aumenta 10
    //valor=numero*valor;  // multiplicamos el valor de ADC por 0.195
    //valor=valor+50;
    //PWM=valor;           // cargamos la variable PWM con el valor calculado
iz_der_servo:
     if((porta==0x40)&&(nuevo>=45)&&(nuevo!=0x95)){ //giro derecha
         delay_ms(3);
         valor=nuevo+1;
         nuevo=valor;
         portc=nuevo;
         valor=valor+50;
         giro_der=1;
         giro_iz=0;
         PWM=valor;           // cargamos la variable PWM con el valor calculado
     }
      if((porta==0x80)&&(nuevo<=0x95)&&(nuevo!=45)){  //giro izquierda
         delay_ms(3);
         valor=nuevo-1;
         portc=valor;
         nuevo=valor;
         portc=nuevo;
         valor=valor+50;
         giro_iz=1;
         giro_der=0;
         PWM=valor;           // cargamos la variable PWM con el valor calculado
     }
     if((nuevo>=0x95)&&(giro_der==1))
         nuevo=0x94;                  //-->0x95-1 para garantizar el retorno hacia la izquierda
     if((nuevo<=45)&&(giro_iz==1))
         nuevo=46;                    //-->45+1 para garantizar el retorno hacia la derecha

 }
}