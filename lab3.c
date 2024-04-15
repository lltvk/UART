#include <avr/io.h>
#include <util/delay.h>

void uart_init(unsigned int ubrr){
	UBRR0L = (unsigned char) ubrr;
	UBRR0H = (unsigned char) (ubrr >>8); /* значение ubrr, сдвинутое на 8 позиций вправо
	дл€ заполнени€ старшего байта регистра UBRR */
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0); /* установка битов разрешени€ работы приЄмника RX и передатчика
 TX*/
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01) | (1<<USBS0); /* формат проверки:
	8 бит данных, 2 стоп-бита, нет проверки чЄтности, асинхронный режим
	*/ 
	UCSR0A &= ~(1<<U2X0);// обнуление бита 
}

void uart_transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));// ожидание установки флага освобождени€ регистра данных UDR
	UDR0 = data; // запись передаваемых данных в регистр UDR 
}

unsigned char uart_receive(){
	while(!(UCSR0A & (1<<RXC0))); // ожидание установки флага освобождени€ сдвигового регистра приЄмника
	return UDR0; // возврат значени€ регистра UDR
}
//¬ыполнение задани€ 1
void first(){
	while(1){
		char sign = uart_receive();
		uart_transmit(sign);
	}
}
//¬ыполнение задани€ 2
void second(){
	char str[19] = {'L','I','T','O','V','K','O',' ','E','L','I','Z','A','V','E','T','A',0x0D,0x0A};
	int lenght = sizeof(str);
	while(1){
		for (int i = 0;i < lenght; i++){
			uart_transmit(str[i]);
			_delay_ms(100);
		}
	}
}

//¬ыполнение задани€ 4
void fourth(){
	while(1){
		uart_transmit(uart_receive());
	}
}
//¬ыполнение задани€ 3
void third(){
	DDRB |= (1 << DDB5); // настроить 5-й вывод порта как выход, ост. линии - как вход
	while(1){
		char c = uart_receive();
		int max = c - '0';
		for (int i = 0; i < max; i++){
			PORTB |= (1<<5); // установить лог 1 на линии 5 порта ¬
			_delay_ms(2000); //задержка 2с
			PORTB &= ~(1<<5); // установить лог 0 на линии 5 порта ¬
			_delay_ms(2000); //задержка 2с
		}
		max = 0;
		
	}
}


int main(){
	uart_init(16);
	third(); //смена функции
}



