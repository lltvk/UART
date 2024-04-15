#include <avr/io.h>
#include <util/delay.h>

void uart_init(unsigned int ubrr){
	UBRR0L = (unsigned char) ubrr;
	UBRR0H = (unsigned char) (ubrr >>8); /* �������� ubrr, ��������� �� 8 ������� ������
	��� ���������� �������� ����� �������� UBRR */
	UCSR0B |= (1<<RXEN0)|(1<<TXEN0); /* ��������� ����� ���������� ������ �������� RX � �����������
 TX*/
	UCSR0C |= (1<<UCSZ00) | (1<<UCSZ01) | (1<<USBS0); /* ������ ��������:
	8 ��� ������, 2 ����-����, ��� �������� ��������, ����������� �����
	*/ 
	UCSR0A &= ~(1<<U2X0);// ��������� ���� 
}

void uart_transmit(unsigned char data){
	while(!(UCSR0A & (1<<UDRE0)));// �������� ��������� ����� ������������ �������� ������ UDR
	UDR0 = data; // ������ ������������ ������ � ������� UDR 
}

unsigned char uart_receive(){
	while(!(UCSR0A & (1<<RXC0))); // �������� ��������� ����� ������������ ���������� �������� ��������
	return UDR0; // ������� �������� �������� UDR
}
//���������� ������� 1
void first(){
	while(1){
		char sign = uart_receive();
		uart_transmit(sign);
	}
}
//���������� ������� 2
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

//���������� ������� 4
void fourth(){
	while(1){
		uart_transmit(uart_receive());
	}
}
//���������� ������� 3
void third(){
	DDRB |= (1 << DDB5); // ��������� 5-� ����� ����� ��� �����, ���. ����� - ��� ����
	while(1){
		char c = uart_receive();
		int max = c - '0';
		for (int i = 0; i < max; i++){
			PORTB |= (1<<5); // ���������� ��� 1 �� ����� 5 ����� �
			_delay_ms(2000); //�������� 2�
			PORTB &= ~(1<<5); // ���������� ��� 0 �� ����� 5 ����� �
			_delay_ms(2000); //�������� 2�
		}
		max = 0;
		
	}
}


int main(){
	uart_init(16);
	third(); //����� �������
}



