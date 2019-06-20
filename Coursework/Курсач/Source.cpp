//	���������� ����������.
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <string>

//	���������� ����������� ������������ ���.
using namespace std;

//	�������� ��������� ��� ������ �����.
void read_file_for_me(string&, string, bool&);

//	�������� ������� ��� ������ ������������ ����������� ������.
int max_nesting(string&);

//	�������� ��������� ��� ������ ��������� � ������.
void cancel_changes(string &);

/*	�������� ��������� ��� �������� ���-�� ����� �  ���-�� ��������,
�������� �� �������. */

void quantity(int&, int&, string);

//	�������� ��������� ��� �������� ��������� �� �����.
void delete_comments(string &);

int main()
{
	//	�������� ������� � ������� ������.
	setlocale(LC_ALL, "rus");

	//	��������� ����� ��� �������� ���������� �����.
	string code;

	//	��������� ���������� ��� �������� ������ �����.
	string address;

	//	���������� ��� ���������� �����.
	int quantity_of_lines = 0;

	//	���������� ��� ���������� ��������.
	int quantity_of_symbols = 0;

	//	���������� ��� �������� ��������� �������� �����.
	bool we_happy = false;

	//	����������� � ������������ ����� �����.
	cout << " ����������, ������� ����� �����: "; cin >> address; cout << endl;

	//	������� ����.
	read_file_for_me(code, address, we_happy);

	//	���� � ������ �� � �������, �� ����������.
	if (we_happy)
	{
		//	������� �� ����� ��������� �� ������� �������� �����.
		cout << "����������, ���� ������!\n\n";

		//	������� �� ����� �������������� ���.
		cout << "	������������ ���: \n\n" << code << endl;

		//	������� �� ����� ������� ������������ ����������� ������.
		cout << "\n ������� ������������ ����������� ������ � ��������� ����� " <<
			max_nesting(code) << ".\n" << endl;

		//	�������� ���-�� ����� � ��������.
		quantity(quantity_of_lines, quantity_of_symbols, code);

		//	� ������� ��� ���������� �� �����.
		cout << " ���������� ����� � ����: " << quantity_of_lines <<
			", ���������� �������� ��� ������� � ����: " << quantity_of_symbols <<
			'\n' << endl;

		//	������� ��������.
		delete_comments(code);

		//	������� �� ����� ���������� ���.
		cout << "	��� ��� ���������: \n\n" << code << '\n' << endl;
	}

	//	����� ������� ��������� �� ������.
	else
		cout << "	��������, � �� ���� ��������� ����, ���� ���� �� ������ :( \n\n";

	system("pause");
	return 0;
}

//	��������� ������ ��������� �� �����.
void read_file_for_me(string & code, string address, bool & we_happy)
{
	//	��������� ���������� ��� �������� ������ ����.
	string code_line;

	//	��������� ����� ��� ������.
	ifstream file(address);
	
	//	���� �� ������� ����, �� ���������� ��� ����������.
	if (file.is_open())
	{
		//	��� �� ��������, ��� � ������ �� � �������.
		we_happy = true;

		/*	� ����� ������ ��������� ������� �� ����� ������,
		����� ��� ������ ������ ��� ��������� ����� ���� � ������� ��� ���������� ����. */

		while (getline(file, code_line))
		{
			code += code_line + '\n';
		}
	}

	//	��������� ����� ������ � ������.
	file.close();

	return;
}

/*	�������, ������������ �������
������������ ����������� ������ � ���������. */

int max_nesting(string & code)
{
	//	��������� � �������������� ����������:
	
	//	- ��� ����. �����������;
	int maxy_nesty = 0;

	//	- ��� �������� ����� � ������.
	int loops_quantity = 0;
	int tabs = 0;
	int dos = 0;
	int dos_last_tabs = 0;
	int first_loop_tabs = 1;

	//	������� ���.
	for (int i = 0; i < code.length(); i++)
	{
		/*	���� �� ���������� �� ���������,
		�� �������������� ����� ���������.*/

		if (code[i] == '\t')
			tabs++;

		/*	�����, ���� ���������� �� ����� ������,
		�� �������� ����� ���������.*/

		else
			if (code[i] == '\n')
				tabs = 0;
		
		else
		{
			//	�����, ���� �� ���������� �� ����:
			if ((i == code.find("for")) ||
				(i == code.find("while")) || (i == code.find("do")))
			{
				/* - ���� ���-�� ��������� �� ����� ������ ���������,
				�� �� �������� �������� ���������;
				*/
				if (first_loop_tabs >= tabs)
					first_loop_tabs = tabs;

				//	�����:
				else
				{
					/*	- ���� ���-�� ��������� ���������� ���������� ����
					������ ���������, �� �������������� �����
					��������� ������.
					*/

					if (tabs > first_loop_tabs)
						loops_quantity++;
				}

				//	-	���� �� ��������� do, ��
				if ((i == code.find("do")))
				{
					//	* ���������, �� ������ �� ��� do,
					if (dos == 0)
						dos_last_tabs = tabs;

					//	�����
					else
					
						/*	* ����, ��������� ��������� do
						���������� ������, ��� ����������, ��
						������ ���������� ���������.*/

						if (tabs > dos_last_tabs)
							dos_last_tabs = tabs;
					
					dos++;
				}

				/*	- ���� �� ��������� ����� while, �� �� ���������,
				�������� �� �� ����������� ��� ��������� do.*/

				if ((i == code.find("while")) && (dos > 0) && 
					(tabs <= dos_last_tabs))
				{
					dos--;
					loops_quantity--;
				}

				//	- �����v ������ ������ ����� �� 0;
				code[i] = '0';
			}
			
			//	���� ������������ ����������� ������.
			if (tabs == first_loop_tabs)
				if (loops_quantity > maxy_nesty)
				{
					maxy_nesty = loops_quantity;
					loops_quantity = 0;
				}
		}
	}

	//	�������� ���� ��������� � ������.
	cancel_changes(code);

	return maxy_nesty;
}

//	��������� ��� ������ ��������� � ������.
void cancel_changes(string & code)
{
	for (int i = 0; i < code.length(); i++)
	{
		/*	���� �� ������� ���������� ���� �����,
		�� ������ ���������� 0 �� ������ �����.*/

		if (i == code.find("0or"))
			code[i] = 'f';

		else if (i == code.find("0hile"))
			code[i] = 'w';

		else if(i == code.find("0o"))
			code[i] = 'd';
	}

	return;
}

//	��������� ��� �������� ���-�� ����� � ��������.
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

//	���������, ��������� ��������.
void delete_comments(string &code)
{
	/*	��������� � �������������� ����������
	��������� ��� ����������� ����, ���������
	�� �� ������ �����������.*/

	bool simple_comment = false, complex_comment = false;

	//	������� ���.
	for (int i = 0; i < code.length(); i++)
	{
		//	���������� ����� ����� � ������� �����������.
		if (i == code.find("//"))
		{
			simple_comment = true;
			code[i] = ' ';
		}

		/*	���� �� ����� �� ����� ������,
		�� ��������, ��� ��� ����������� �����������.*/
		
		else
		{
			if (code[i] == '\n')
				simple_comment = false;
		}

		//	���������� ����� ����� � ������������� �����������.
		if (i == code.find("/*"))
		{
				complex_comment = true;
				code[i] = ' ';
		}

		/*	���� �� ����� �� ������� ����� ��������������
		�����������, �� ������� ������ �� �������� �����
		����������� � ��������, ��� ��� ����������� �����������.*/

		else
		{
			if (i == code.find("*/"))
			{
				code[i] = ' ';
				code[i + 1] = ' ';
				complex_comment = false;

			}
		}

		/*	���� ������ ����������� ������ ����,
		�� ������ ������ */

		if (simple_comment || complex_comment)
			code[i] = ' ';
	}

	return;
}