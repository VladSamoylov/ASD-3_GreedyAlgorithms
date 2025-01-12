#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <tuple>

using namespace std;

#define ANSI_COLOR_BLUE "\033[34m"
#define ANSI_COLOR_RESET "\033[0m"
#define ANSI_COLOR_GREEN "\033[32m"
#define ANSI_COLOR_RED "\033[31m"
#define ANSI_COLOR_PURPLE "\033[35m"
#define ANSI_COLOR_LIGHTBLUE "\033[94m"

class SoldOut {
private:
	unordered_map<string, tuple<double, chrono::days, chrono::year_month_day>> catalog;
	int n;
public:
	SoldOut();
	void AddToCatalog(const string&, const double&, const chrono::days&, const chrono::year_month_day&);
	void ShowCatalog();
};

SoldOut::SoldOut() {

	this->n = 0;
	auto today = chrono::floor<chrono::days>(chrono::system_clock::now());
	chrono::year_month_day date = chrono::year_month_day{ today };
	this->AddToCatalog("bread", 0.5 * 100, chrono::days(3), chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("milk", 1 * 40, chrono::days(3), chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("butter", 1.5 * 20, chrono::days(5), chrono::year_month_day{ today + chrono::days(14) });
	this->AddToCatalog("chips", 1.99 * 30, chrono::days(7), chrono::year_month_day{ today + chrono::days(30) });
	this->AddToCatalog("beef", 25 * 20, chrono::days(4), chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("pork", 15 * 20, chrono::days(4), chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("chicken", 7.5 * 20, chrono::days(4), chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("pizza", 8 * 10, chrono::days(3), chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("salmon", 45 * 5, chrono::days(1), chrono::year_month_day{ today + chrono::days(3) });
	this->AddToCatalog("sushi", 20 * 10, chrono::days(3), chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("candy", 4 * 100, chrono::days(7), chrono::year_month_day{ today + chrono::days(60) });
	this->AddToCatalog("water", 1 * 30, chrono::days(3), chrono::year_month_day{ today + chrono::days(14) });
	this->AddToCatalog("soda", 5 * 20, chrono::days(7), chrono::year_month_day{ today + chrono::days(30) });
	this->AddToCatalog("potato", 0.33 * 60, chrono::days(2), chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("tomato", 1.8 * 40, chrono::days(2), chrono::year_month_day{ today + chrono::days(6) });
	this->AddToCatalog("cucumber", 1.8 * 40, chrono::days(2), chrono::year_month_day{ today + chrono::days(6) });
	this->AddToCatalog("oliveOil", 10 * 10, chrono::days(5), chrono::year_month_day{ today + chrono::days(30) });
	this->AddToCatalog("sausage", 12 * 30, chrono::days(2), chrono::year_month_day{ today + chrono::days(14) });
}

void SoldOut::ShowCatalog() {
	
	double val = 0;
	cout << "-!Catalog : " << endl;
	cout << ANSI_COLOR_GREEN << setw(15) << left << "product name" << setw(18) << "estimated value" << setw(15) << "50% discount" << setw(15) << "expiration date" << ANSI_COLOR_RESET << endl;
	for (const auto& [key, value] : this->catalog) {
		auto [price, duration, date] = value;
		auto sale = chrono::year_month_day{ chrono::sys_days(date) - duration};
		cout << setw(15) << key << ANSI_COLOR_LIGHTBLUE << setw(18) << price << ANSI_COLOR_RESET << setw(15) << sale << setw(15) << date << endl;
		val += price;
	}
	cout << "Estimate value : " << ANSI_COLOR_PURPLE << val << ANSI_COLOR_RESET << "$" << endl;
}

void SoldOut::AddToCatalog(const string& name, const double& price, const chrono::days& days, const chrono::year_month_day& date) {

	this->catalog[name] = make_tuple(price, days, date);
	this->n++;
}

int main() {

	cout << "Task #" << ANSI_COLOR_GREEN << ((17 - 1) % 5) + 1 << ANSI_COLOR_RESET << endl;
	SoldOut s;
	s.ShowCatalog();

	return 0;
}