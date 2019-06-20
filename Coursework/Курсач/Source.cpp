//	Подключаем библиотеки.
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>

//	Используем стандартное пространство имён.
using namespace std;

//	Прототип процедуры для чтения файла.
void read_file_for_me(string&, string, bool&);

//	Прототип функции для поиска максимальной вложенности циклов.
int max_nesting(string&);

//	Прототип процедуры для отката изменений в строке.
void cancel_changes(string &);

/*	Прототип процедуры для подсчета кол-ва строк и  кол-ва символов,
отличных от пробела. */

void quantity(int&, int&, string);

//	Прототип процедуры для удаления комментов из файла.
void delete_comments(string &);

int main()
{
	//	знакомим консоль с русским языком.
	setlocale(LC_ALL, "rus");

	//	Объявляем место для хранения считанного файла.
	string code;

	//	Объявляем переменную для хранения адреса файла.
	string address;

	//	Переменная под количество строк.
	int quantity_of_lines = 0;

	//	Переменная под количество символов.
	int quantity_of_symbols = 0;

	//	Переменная для хранения состояния открытия файла.
	bool we_happy = false;

	//	Запрашиваем у пользователя адрес файла.
	cout << " Пожалуйста, введите адрес файла: "; cin >> address; cout << endl;

	//	Прочтем файл.
	read_file_for_me(code, address, we_happy);

	//	Если с файлом всё в порядке, то продолжаем.
	if (we_happy)
	{
		//	Выводим на экран сообщение об удачном открытии файла.
		cout << "Поздравляю, файл открыт!\n\n";

		//	Выводим на экран первоначальный код.
		cout << "	Оригинальный код: \n\n" << code << endl;

		//	Выводим на экран степень максимальной вложенности циклов.
		cout << "\n Степень максимальной вложенности циклов в программе равна " <<
			max_nesting(code) << ".\n" << endl;

		//	Получаем кол-ва строк и символов.
		quantity(quantity_of_lines, quantity_of_symbols, code);

		//	И выводим эту информацию на экран.
		cout << " Количество строк в коде: " << quantity_of_lines <<
			", количество символов без пробела в коде: " << quantity_of_symbols <<
			'\n' << endl;

		//	Удаляем комменты.
		delete_comments(code);

		//	Выводим на экран измененный код.
		cout << "	Код без комментов: \n\n" << code << '\n' << endl;
	}

	//	Иначе выводим сообщение об ошибке.
	else
		cout << "	Простите, я не могу прочитать файл, либо файл не найден :( \n\n";

	system("pause");
	return 0;
}

//	Процедура записи программы из файла.
void read_file_for_me(string & code, string address, bool & we_happy)
{
	//	Объявляем переменную для хранения строки кода.
	string code_line;

	//	Открываем поток для чтения.
	ifstream file(address);
	
	//	Если мы открыли файл, то записываем его содержимое.
	if (file.is_open())
	{
		//	Так же отмечаем, что с файлом всё в порядке.
		we_happy = true;

		/*	В конце строки добавляем переход на новую строку,
		чтобы при выводе строки она предстала перед нами в удобном для восприятия виде. */

		while (getline(file, code_line))
		{
			code += code_line + '\n';
		}
	}

	//	Закрываем поток работы с файлом.
	file.close();

	return;
}

/*	Функция, возвращающая степень
максимальной вложенности циклов в программе. */

int max_nesting(string & code)
{
	//	Объявляем и инициализируес переменные:
	
	//	- для макс. вложенности;
	int maxy_nesty = 0;

	//	- для подсчета табов и циклов.
	int loops_quantity = 0;
	int tabs = 0;
	int dos = 0;
	int dos_last_tabs = 0;
	int first_loop_tabs = 1;

	//	Шерстим код.
	for (int i = 0; i < code.length(); i++)
	{
		/*	Если мы наткнулись на табуляцию,
		то инкрементируем число табуляций.*/

		if (code[i] == '\t')
			tabs++;

		/*	Иначе, если натыкаемся на конец строки,
		то обнуляем число табуляций.*/

		else
			if (code[i] == '\n')
				tabs = 0;
		
		else
		{
			//	Иначе, если мы наткнулись на цикл:
			if ((i == code.find("for")) ||
				(i == code.find("while")) || (i == code.find("do")))
			{
				/* - если кол-во табуляций до цикла меньше стандарта,
				то мы заменяем значение стандарта;
				*/
				if (first_loop_tabs >= tabs)
					first_loop_tabs = tabs;

				//	Иначе:
				else
				{
					/*	- если кол-во табуляций последнего найденного цила
					больше стандарта, то инкрементируем число
					вложенных циклов.
					*/

					if (tabs > first_loop_tabs)
						loops_quantity++;
				}

				//	-	если мы встречаем do, то
				if ((i == code.find("do")))
				{
					//	* проверяем, не первый ли это do,
					if (dos == 0)
						dos_last_tabs = tabs;

					//	Иначе
					else
					
						/*	* если, последний найденный do
						расположен глубже, чем предыдущий, то
						меняем количество табуляций.*/

						if (tabs > dos_last_tabs)
							dos_last_tabs = tabs;
					
					dos++;
				}

				/*	- если мы встречаем новый while, то мы проверяем,
				является ли он закрывающим для открытого do.*/

				if ((i == code.find("while")) && (dos > 0) && 
					(tabs <= dos_last_tabs))
				{
					dos--;
					loops_quantity--;
				}

				//	- меняеv первый символ цикла на 0;
				code[i] = '0';
			}
			
			//	Ищем максимальную вложенность циклов.
			if (tabs == first_loop_tabs)
				if (loops_quantity > maxy_nesty)
				{
					maxy_nesty = loops_quantity;
					loops_quantity = 0;
				}
		}
	}

	//	Отменяем наши изменения в строке.
	cancel_changes(code);

	return maxy_nesty;
}

//	Процедура для отката изменений в строке.
void cancel_changes(string & code)
{
	for (int i = 0; i < code.length(); i++)
	{
		/*	Если мы находим измененные нами циклы,
		то просто исправляем 0 на нужные буквы.*/

		if (i == code.find("0or"))
			code[i] = 'f';

		else if (i == code.find("0hile"))
			code[i] = 'w';

		else if(i == code.find("0o"))
			code[i] = 'd';
	}

	return;
}

//	Процедура для подсчета кол-ва строк и символов.
void quantity(int & lines, int & symbols, string code)
{
	for (int i = 0; i < code.length(); i++)
	{
		if (code[i] == '\n')
			lines++;

		else if (code[i] != ' ')
			symbols++;
	}

	return;
}

//	Процедура, удаляющая комменты.
void delete_comments(string &code)
{
	/*	Объявляем и инициализируем логические
	перменные для обозначения того, находимся
	ли мы внутри комментария.*/

	bool simple_comment = false, complex_comment = false;

	//	Шерстим код.
	for (int i = 0; i < code.length(); i++)
	{
		//	Определяем точку входа в простой комментарий.
		if (i == code.find("//"))
		{
			simple_comment = true;
			code[i] = ' ';
		}

		/*	Если мы дошли до конца строки,
		то отмечаем, что наш комментарий закрывается.*/
		
		else
		{
			if (code[i] == '\n')
				simple_comment = false;
		}

		//	Определяем точку входа в многострочный комментарий.
		if (i == code.find("/*"))
		{
				complex_comment = true;
				code[i] = ' ';
		}

		/*	Если мы дошли до символа конца многострочного
		комментария, то очищаем строку от символов конца
		комментария и отмечаем, что наш комментарий закрывается.*/

		else
		{
			if (i == code.find("*/"))
			{
				code[i] = ' ';
				code[i + 1] = ' ';
				complex_comment = false;

			}
		}

		/*	Если открыт комментарий любого вида,
		то просто меняем */

		if (simple_comment || complex_comment)
			code[i] = ' ';
	}

	return;
}