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

struct Goods {
	double price;
	int amount;
	chrono::year_month_day dateAgreements;
	chrono::year_month_day dateExpiration;
	Goods() {}
	Goods(const double& price, const int& n, const chrono::year_month_day& dateAgreements, const chrono::year_month_day& dateExpiration) : price(price), amount(n), dateAgreements(dateAgreements), dateExpiration(dateExpiration) {}
};

class SoldOut {
private:
	unordered_map<string, Goods> catalog;
	int n;
public:
	SoldOut();
	void AddToCatalog(const string&, const double&, const int&, const chrono::year_month_day&, const chrono::year_month_day&);
	void ShowCatalog();
	void GreedyChoice();
	double CalculatePriority(const Goods&, const chrono::year_month_day&);
	double CalculateThePrice(const Goods&, const chrono::year_month_day&);
};

SoldOut::SoldOut() {

	this->n = 0;
	auto today = chrono::floor<chrono::days>(chrono::system_clock::now());
	chrono::year_month_day date = chrono::year_month_day{ today };
	this->AddToCatalog("bread", 0.5, 100, chrono::year_month_day{ today + chrono::days(3) }, chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("milk", 1, 40, chrono::year_month_day{ today + chrono::days(3) }, chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("butter", 1.5, 20, chrono::year_month_day{ today + chrono::days(5) }, chrono::year_month_day{ today + chrono::days(14) });
	this->AddToCatalog("chips", 1.99, 30, chrono::year_month_day{ today + chrono::days(7) }, chrono::year_month_day{ today + chrono::days(30) });
	this->AddToCatalog("beef", 25, 20, chrono::year_month_day{ today + chrono::days(4) }, chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("pork", 15, 20, chrono::year_month_day{ today + chrono::days(4) }, chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("chicken", 7.5, 20, chrono::year_month_day{ today + chrono::days(4) }, chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("pizza", 8, 10, chrono::year_month_day{ today + chrono::days(3) }, chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("salmon", 45, 5, chrono::year_month_day{ today + chrono::days(1) }, chrono::year_month_day{ today + chrono::days(3) });
	this->AddToCatalog("sushi", 20, 10, chrono::year_month_day{ today + chrono::days(3) }, chrono::year_month_day{ today + chrono::days(5) });
	this->AddToCatalog("candy", 4, 100, chrono::year_month_day{ today + chrono::days(7) }, chrono::year_month_day{ today + chrono::days(60) });
	this->AddToCatalog("water", 1, 30, chrono::year_month_day{ today + chrono::days(3) }, chrono::year_month_day{ today + chrono::days(14) });
	this->AddToCatalog("soda", 5, 20, chrono::year_month_day{ today + chrono::days(7) }, chrono::year_month_day{ today + chrono::days(30) });
	this->AddToCatalog("potato", 0.33, 60, chrono::year_month_day{ today + chrono::days(2) }, chrono::year_month_day{ today + chrono::days(7) });
	this->AddToCatalog("tomato", 1.8, 40, chrono::year_month_day{ today + chrono::days(2) }, chrono::year_month_day{ today + chrono::days(6) });
	this->AddToCatalog("cucumber", 1.8, 40, chrono::year_month_day{ today + chrono::days(2) }, chrono::year_month_day{ today + chrono::days(6) });
	this->AddToCatalog("oliveOil", 10, 10, chrono::year_month_day{ today + chrono::days(5) }, chrono::year_month_day{ today + chrono::days(30) });
	this->AddToCatalog("sausage", 12, 30, chrono::year_month_day{ today + chrono::days(2) }, chrono::year_month_day{ today + chrono::days(14) });
}

void SoldOut::ShowCatalog() {
	
	double val = 0;
	cout << "-!Catalog : " << endl;
	cout << ANSI_COLOR_GREEN << setw(15) << left << "product name" << setw(18) << "estimated value" << setw(10) << "amount" << setw(15) << "50% discount" << setw(15) << "expiration date" << ANSI_COLOR_RESET << endl;
	for (const auto& [key, value] : this->catalog) {
		cout << setw(15) << key 
		<< ANSI_COLOR_LIGHTBLUE << setw(18) << value.price << ANSI_COLOR_RESET 
		<< setw(10)  << value.amount << setw(15) << value.dateAgreements
		<< ANSI_COLOR_RED << setw(15) << value.dateExpiration << ANSI_COLOR_RESET << endl;
		val += value.price * value.amount;
	}

	cout << "Estimate value : " << ANSI_COLOR_PURPLE << val << ANSI_COLOR_RESET << "$" << endl;
}

void SoldOut::AddToCatalog(const string& name, const double& price, const int& amount, const chrono::year_month_day& dateAgreements, const chrono::year_month_day& dateExpiration) {

	this->catalog[name] = Goods(price, amount, dateAgreements, dateExpiration);
	this->n++;
}

double SoldOut::CalculateThePrice(const Goods& good, const chrono::year_month_day& currentDate) {
	
	double price = 0.0;
	auto daysAgreement = chrono::sys_days(good.dateAgreements) - chrono::sys_days(currentDate);
	auto daysExpiration = chrono::sys_days(good.dateExpiration) - chrono::sys_days(currentDate);
	
	if (daysAgreement.count() > 0) {
		price = good.price * good.amount;
	}
	else if (daysExpiration.count() > 0) {
		cout << ANSI_COLOR_RED << "50% off" << ANSI_COLOR_RESET << endl;
		price = (good.price * good.amount) * 0.5;
	}
	else {
		cout << ANSI_COLOR_RED << "Expiration date has expired" << endl;
	}

	return price;
}

double SoldOut::CalculatePriority(const Goods& good, const chrono::year_month_day& currentDate) {

	double priority = 0.0;
	auto daysAgreement = chrono::sys_days(good.dateAgreements) - chrono::sys_days(currentDate);
	auto daysExpiration = chrono::sys_days(good.dateExpiration) - chrono::sys_days(currentDate);

	if (daysAgreement.count() > 0) {
		priority += good.price * good.amount / daysAgreement.count();
	}
	if (daysExpiration.count() > 0) {
		int days = 0;
		if (daysAgreement.count() > 0) days = daysExpiration.count() - daysAgreement.count();
		else days = daysExpiration.count();
		priority += 0.5 * (good.price * good.amount) / days;
	}

	return priority;
}

void SoldOut::GreedyChoice() {

	string name;
	double priority = 0.0;
	double profit = 0.0;
	double price = 0.0;
	auto today = chrono::floor<chrono::days>(chrono::system_clock::now());
	chrono::year_month_day date = chrono::year_month_day{ today };
	int countDays = 1;
	
	cout << endl;
	while (!this->catalog.empty()) {
		cout << "\n!-The day : " << ANSI_COLOR_LIGHTBLUE << date << ANSI_COLOR_RESET << endl;
		for (const auto& [key, value] : this->catalog) {
			if (this->CalculatePriority(value, date) > priority) {
				name = key;
				priority = this->CalculatePriority(value, date);
			}
		}
		cout << "Chosen the " << ANSI_COLOR_GREEN << name << ANSI_COLOR_RESET << " by priority " << priority << endl;
		price = this->CalculateThePrice(this->catalog[name], date);
		profit += price;
		cout << "Sold : " << ANSI_COLOR_PURPLE << price << ANSI_COLOR_RESET << "$ ";
		if (price == 0) cout << "Lost : " << ANSI_COLOR_RED << this->catalog[name].price * this->catalog[name].amount << ANSI_COLOR_RESET << "$" << endl;
		else cout << endl;
		this->catalog.erase(name);
		date = chrono::year_month_day{ today + chrono::days(countDays) };
		countDays++; priority = -1;
	}
	cout << "The total profit : " << ANSI_COLOR_PURPLE << profit << ANSI_COLOR_RESET << "$" << endl;
}

int main() {

	cout << "Task #" << ANSI_COLOR_GREEN << ((17 - 1) % 5) + 1 << ANSI_COLOR_RESET << endl;
	SoldOut s;
	s.ShowCatalog();
	s.GreedyChoice();

	return 0;
}