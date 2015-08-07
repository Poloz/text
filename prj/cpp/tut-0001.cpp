/*
http://www.cyberforum.ru/cpp-beginners/thread1510810.html
Тема: Функции2.
3. В кассе есть в наличии только двух и пьятигривневи копюры. 
Написать программу, которая выплачивала любую сумму (более 6 грн) 
такими копюрамы.
*/
#include<iostream>
using namespace::std;
int func (int j);

int main() {

	int bablo=0;
	int five=0;
	
	setlocale(LC_ALL, "rus");
	cout << "Чо-как по баблу? ";
	cin >> bablo;
	if (bablo < 7) {
		cout << "Сдачи нет! Пиши 7 и больше.";
		return 0;
	}
	five=func(bablo);
	if (five > 0){
		cout << endl << "Пятёрок: " << five << endl;
	}
	cout << "Двушек: " << (bablo-five*5)/2 << endl;
	return 0;
}

int func(int j){
	return 0;
}