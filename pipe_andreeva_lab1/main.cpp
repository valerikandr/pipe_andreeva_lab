#include <iostream>
#include <string>
#include <fstream>
#include "Utils.h"
#include <ctime>
#include <iomanip>
#include <sstream>
#include "GTS.h"

#include <windows.h>

using namespace std;

void Menu() {
	cout << "1. Добавить трубу\n"
		<< "2. Добавить КС\n"
		<< "3. Просмотр всех объектов\n"
		<< "4. Редактировать трубу\n"
		<< "5. Редактировать КС\n"
		<< "6. Сохранить\n"
		<< "7. Загрузить\n"
		<< "8. Удалить трубу\n"
		<< "9. Удалить КС\n"
		<< "10. Поиск труб\n"
		<< "11. Поиск КС\n"
		<< "12. Пакетное редактирование\n"
		<< "13. Соединить КС трубой\n"
		<< "14. Отсоединить трубу\n"
		<< "0. Выход\n";
}


int main() {
	SetConsoleCP(65001);
	SetConsoleOutputCP(65001);

	time_t now = time(nullptr);
	tm t{};
	localtime_s(&t, &now);

	ostringstream filename;
	filename << "log_" << put_time(&t, "%d_%m_%Y_%H_%M_%S") << ".txt";

	ofstream logfile(filename.str());
	if (!logfile) {
		cout << "Не удалось создать журнал\n";
		return 1;
	}

	redirect_output_wrapper log(cerr);
	log.redirect(logfile);

	GTS gts;

	int  number;

	while (true) {
		Menu();
		cout << "Введите команду: ";
		number = GetCorrectNumber(0, 14);

		switch (number) {
		case 1:
			gts.AddPipe();
			break;

		case 2:
			gts.AddCS();
			break;

		case 3:
			gts.Show();
			break;

		case 4:
			gts.EditPipe();
			break;

		case 5:
			gts.EditCS();
			break;

		case 6: 
			gts.SaveFile();
			break;

		case 7:
			gts.Zagruzka();
			break;

		case 8:
			gts.DeletePipe();
			break;

		case 9:
			gts.DeleteCS();
			break;

		case 10:
			gts.PoiskPipe();
			break;

		case 11:
			gts.PoiskStation();
			break;

		case 12:
			gts.PaketPipe();
			break;

		case 13:
			gts.Connect(); 
			break;

		case 14:
			gts.Disconnect();
			break;

		case 0:
			cout << "Выход\n";
			return 0;
		}
	} 
}