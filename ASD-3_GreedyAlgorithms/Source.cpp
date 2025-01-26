#include <iostream>
#include <unordered_map>
#include <iomanip>
#include <chrono>
#include <tuple>
#include <queue>
#include <fstream>

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

struct Node {
	char symbol;
	int freq;
	Node* left;
	Node* right;

	Node(char symbol, int freq) : symbol(symbol), freq(freq), left(nullptr), right(nullptr) { }
};

struct Compare {
	bool operator()(Node* left, Node* right) {
		return left->freq > right->freq;
	}
};

class Haffman {
private:
	unordered_map<char, int> frequencies;
	unordered_map<char, string> codesTable;
	Node* root;
public:
	void SetFrequenciesLetter(const string&);
	void ShowFreequenciesLetter();
	void BuildHaffmanTree();
	void BuildTableCodes(Node*, const string&);
	void EncodingFileData(const string&, const string&, const string&);
	void DecodingFileData(const string&);
	void DecodingFileDataBin(const string&);
	void SerializeTree(Node*, ofstream&);
	void SerializeTreeBin(Node*, ofstream&);
	Node* DeserializeTree(ifstream&);
	Node* DeserializeTreeBin(ifstream&);
	void ShowHaffmanTree(Node*);
	~Haffman() { this->DeleteHaffmanTree(this->root); }
	void DeleteHaffmanTree(Node*);
};

void Haffman::SetFrequenciesLetter(const string& txt) {

	for (const char& s : txt) {
		this->frequencies[s]++;
	}
}

void Haffman::ShowFreequenciesLetter() {

	cout << "\n!-The frequencies table of symbols:" << endl;
	for (const auto& [key, value] : this->frequencies) {
		if (key == '\n') cout << "The symbol : " << ANSI_COLOR_RED << "/n" << ANSI_COLOR_RESET;
		else if (key == '\t') cout << "The symbol : " << ANSI_COLOR_RED << "/t" << ANSI_COLOR_RESET;
		else cout << "The symbol : " << ANSI_COLOR_RED << key << ANSI_COLOR_RESET;
		cout << " has frequencies - " << ANSI_COLOR_GREEN << value << ANSI_COLOR_RESET << endl;
	} cout << endl;
}

void Haffman::BuildHaffmanTree() {

	priority_queue<Node*, vector<Node*>, Compare> pQueue;

	for (const auto& [key, value] : this->frequencies) {
		pQueue.push(new Node(key, value));
	}

	while (pQueue.size() != 1) {
		Node* left = pQueue.top(); pQueue.pop();
		Node* right = pQueue.top(); pQueue.pop();
		Node* parent = new Node('\0', left->freq + right->freq);
		parent->left = left;
		parent->right = right;
		pQueue.push(parent);
	}
	this->root = pQueue.top();
}

void Haffman::ShowHaffmanTree(Node* parent) {

	if (!parent) return;
	if (parent->symbol == '\n') cout << "The parent \"" << ANSI_COLOR_PURPLE << "/n" << ANSI_COLOR_RESET;
	else if (parent->symbol == '\t') cout << "The parent \"" << ANSI_COLOR_PURPLE << "/t" << ANSI_COLOR_RESET;
	else cout << "The parent \"" << ANSI_COLOR_PURPLE << parent->symbol << ANSI_COLOR_RESET;
	if (parent->left && parent->right) {
		if (parent->left->symbol == '\n') cout << " \" has chieldren : \"" ANSI_COLOR_RED << "/n" << ANSI_COLOR_RESET << "\"";
		else if (parent->left->symbol == '\t') cout << " \" has chieldren : \"" ANSI_COLOR_RED << "/t" << ANSI_COLOR_RESET << "\"";
		else cout << " \" has chieldren : \"" ANSI_COLOR_RED << parent->left->symbol << ANSI_COLOR_RESET << "\"";
		
		if (parent->right->symbol == '\n')  cout << " (left) \"" << ANSI_COLOR_BLUE << "/n" << ANSI_COLOR_RESET << "\" (right) " << endl;
		else if (parent->right->symbol == '\t') cout << " (left) \"" << ANSI_COLOR_BLUE << "/t" << ANSI_COLOR_RESET << "\" (right) " << endl;
		else cout << " (left) \"" << ANSI_COLOR_BLUE << parent->right->symbol << ANSI_COLOR_RESET << "\" (right) " << endl;
		
		this->ShowHaffmanTree(parent->left);
		this->ShowHaffmanTree(parent->right);
	}
	else cout << "\" hasn`t children " << endl;
}

void Haffman::DeleteHaffmanTree(Node* node) {

	if (!node) return;
	DeleteHaffmanTree(node->left);
	DeleteHaffmanTree(node->right);
	delete node;
}

void Haffman::BuildTableCodes(Node* node, const string& code) {

	if (!node) return;
	if (node->symbol != '\0') {
		this->codesTable[node->symbol] = code;
		if (node->symbol == '\n') cout << "The letter : " << ANSI_COLOR_RED << "/n" << ANSI_COLOR_RESET;
		else if (node->symbol == '\t') cout << "The letter : " << ANSI_COLOR_RED << "/t" << ANSI_COLOR_RESET;
		else cout << "The letter : " << ANSI_COLOR_RED << node->symbol << ANSI_COLOR_RESET;
		cout << " has code - " << ANSI_COLOR_GREEN << code << ANSI_COLOR_RESET << endl;
	}
	this->BuildTableCodes(node->left, code + "0");
	this->BuildTableCodes(node->right, code + "1");
}

void Haffman::SerializeTree(Node* node, ofstream& ofs) {

	if (!node) return;
	if (!node->left && !node->right) {
		if (node->symbol == ' ') ofs << "L S ";
		else if (node->symbol == '\n') ofs << "L E ";
		else if (node->symbol == '\t') ofs << "L T ";
		else ofs << "L " << node->symbol << " ";
	}
	else ofs << "I ";
	this->SerializeTree(node->left, ofs);
	this->SerializeTree(node->right, ofs);
}

void Haffman::SerializeTreeBin(Node* node, ofstream& ofs) {

	if (!node) return;
	if (!node->left && !node->right) {
		uint8_t type = 0; 
		ofs.write(reinterpret_cast<char*>(&type), sizeof(type));
		ofs.put(node->symbol);
	}
	else {
		uint8_t type = 1;
		ofs.write(reinterpret_cast<char*>(&type), sizeof(type));
	}
	this->SerializeTreeBin(node->left, ofs);
	this->SerializeTreeBin(node->right, ofs);
}

Node* Haffman::DeserializeTree(ifstream& ifs) {

	string type;
	ifs >> type;

	//cout << type << endl;
	if (type == "L") {
		char symbol;
		ifs >> symbol;
		//cout << "Symbol : " << symbol << endl;
		if (symbol == 'S') return new Node(' ', 0);
		else if (symbol == 'E') return new Node('\n', 0);
		else if (symbol == 'T') return new Node('\t', 0);
		else return new Node(symbol, 0);
	}
	else if (type == "I") {
		Node* node = new Node('\0', 0);
		node->left = this->DeserializeTree(ifs);
		node->right = this->DeserializeTree(ifs);
		return node;
	}

	return nullptr;
}

Node* Haffman::DeserializeTreeBin(ifstream& ifs) {

	uint8_t type;
	ifs.read(reinterpret_cast<char*>(&type), sizeof(type));

	if (ifs.eof()) return nullptr;
	if (type == 0) {
		char symbol;
		ifs.get(symbol);
		return new Node(symbol, 0);
	}
	else if (type == 1) {
		Node* node = new Node('\0', 0);
		node->left = this->DeserializeTreeBin(ifs);
		node->right = this->DeserializeTreeBin(ifs);
		return node;
	}

	return nullptr;
}

void Haffman::EncodingFileData(const string& pathFileInput, const string& pathFileOutput, const string& pathFileOutputBin) {

	ifstream inFile(pathFileInput);
	if (!inFile) throw "Error : <The file isn`t found>";
	else cout << "The data file - " << pathFileInput << " is load..." << endl;
	string text((istreambuf_iterator<char>(inFile)), istreambuf_iterator<char>());
	inFile.close();

	cout << "The data : " << ANSI_COLOR_GREEN << text << ANSI_COLOR_RESET << endl;
	this->SetFrequenciesLetter(text);
	this->ShowFreequenciesLetter();
	this->BuildHaffmanTree();
	cout << "!-The Haffman tree:" << endl;
	this->ShowHaffmanTree(this->root);
	cout << "\n!-The Haffman table codes:" << endl;
	this->BuildTableCodes(this->root, "");

	ofstream outFile(pathFileOutput);
	string bits;
	cout << "Encoding data..." << endl;
	this->SerializeTree(this->root, outFile);
	outFile << endl;
	for (const auto& symbol : text) {
		outFile << this->codesTable[symbol];
		bits += this->codesTable[symbol];
	}
	outFile.close();
	
	ofstream outFileBin(pathFileOutputBin, ios::binary);
	uint8_t buffer = 0;
	int bitCount = 0;
	this->SerializeTreeBin(this->root, outFileBin);
	for (const char& bit : bits) {
		buffer = (buffer << 1) | (bit - '0');
		bitCount++;
		if (bitCount == 8) {
			outFileBin.put(buffer);
			buffer = 0;
			bitCount = 0;
		}
	}
	if (bitCount > 0) {
		buffer = buffer << (8 - bitCount);
		outFileBin.put(buffer);
	}
	outFileBin.close();
	cout << "Encoding data finish" << endl;
}

void Haffman::DecodingFileData(const string& pathFileInput) {
	
	if (this->root) {
		cout << "Delete Haffman tree" << endl;
		this->DeleteHaffmanTree(this->root);
	}
	ifstream inFile(pathFileInput);
	if(!inFile) throw "Error : <The encoded file isn`t found>";
	else cout << "The encoded data file - " << pathFileInput << " is load..." << endl;

	this->root = this->DeserializeTree(inFile);
	if (!this->root) throw "Error: <Failed to deserialize the Huffman tree>";
	string encodedData;
	inFile >> encodedData;
	inFile.close();
	if (encodedData.empty()) throw "Error: <The encoded data is empty>";
	cout << "Encode data : " << ANSI_COLOR_PURPLE << encodedData << ANSI_COLOR_RESET << endl;

	string decodedData = "";
	Node* currentNode = this->root;
	for (const char& bit : encodedData) {
		if (bit == '0') currentNode = currentNode->left;
		else if (bit == '1') currentNode = currentNode->right;
		if (!currentNode->left && !currentNode->right) {
			decodedData += currentNode->symbol;
			currentNode = this->root;
		}
	}
	cout << "Decode data : " << ANSI_COLOR_LIGHTBLUE << decodedData << ANSI_COLOR_RESET << endl;
}

void Haffman::DecodingFileDataBin(const string& pathFileInput) {

	if (this->root) {
		cout << "Delete Haffman tree" << endl;
		this->DeleteHaffmanTree(this->root);
	}
	ifstream inFile(pathFileInput, ios::binary);
	if(!inFile) throw "Error : <The encoded file isn`t found>";
	else cout << "The encoded data file - " << pathFileInput << " is load..." << endl;

	this->root = this->DeserializeTreeBin(inFile);
	string encodedData;

	char byte;
	while (inFile.get(byte)) {
		for (int i = 7; i >= 0; i--) {
			encodedData += ((byte >> i) & 1) ? '1' : '0';
		}
	}
	inFile.close();
	if (encodedData.empty()) throw "Error: <The encoded data is empty>";
	cout << "Encode data : " << ANSI_COLOR_PURPLE << encodedData << ANSI_COLOR_RESET << endl;

	string decodedData = "";
	Node* currentNode = this->root;
	for (const char& bit : encodedData) {
		if (bit == '0') currentNode = currentNode->left;
		else if (bit == '1') currentNode = currentNode->right;
		if (!currentNode->left && !currentNode->right) {
			decodedData += currentNode->symbol;
			currentNode = this->root;
		}
	}
	cout << "Decode data : " << ANSI_COLOR_LIGHTBLUE << decodedData << ANSI_COLOR_RESET << endl;
}

int main() {

	SoldOut s;
	Haffman h;
	
	int task = 99, ltask = 99, vtask = 99;

	while (task != 0) {
		switch (task) {
		case 1:
			while (ltask != 0) {
				switch (ltask) {
				case 1:
					s.ShowCatalog();
					break;
				case 2:
					s.GreedyChoice();
					break;
				}
				cout << "\n________Work with Task 1 var 17________" << endl;
				cout << "Task #" << ANSI_COLOR_GREEN << ((17 - 1) % 5) + 1 << ANSI_COLOR_RESET << endl;
				cout << setw(35) << left << "Show catalog of goods" << " / Enter - 1" << endl;
				cout << setw(35) << left << "Launch greedy choice" << " / Enter - 2" << endl;
				cout << setw(35) << left << "Back " << " / Enter - 0" << endl;
				if (ltask != 0) {
					try {
						cout << "\nChoose the Task: ";
						cin >> ltask;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							throw "Error: <Incorrect Input Data>";
						}
					}
					catch (const char* err) {
						cerr << endl << err << endl;
						ltask = 99;
					}
				}
			}
			break;
		case 2:
			while (vtask != 0) {
				switch (vtask) {
				case 1:
					try {
						h.EncodingFileData("./data/text.txt", "./data/encode_text.txt", "./data/encode_text.bin");
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				case 2:
					try {
						h.DecodingFileData("./data/encode_text.txt");
						h.DecodingFileDataBin("./data/encode_text.bin");
					}
					catch (const char* err) {
						cerr << ANSI_COLOR_RED << err << ANSI_COLOR_RESET << endl;
					}
					break;
				}
				cout << "\n________Work with Haffman algorithm________" << endl;
				cout << setw(45) << left << "Encode data from file" << " / Enter - 1" << endl;
				cout << setw(45) << left << "Decode data from file" << " / Enter - 2" << endl;
				cout << setw(45) << left << "Back " << " / Enter - 0" << endl;
				if (vtask != 0) {
					try {
						cout << "\nChoose the Task: ";
						cin >> vtask;
						if (cin.fail()) {
							cin.clear();
							cin.ignore(numeric_limits<streamsize>::max(), '\n');
							throw "Error: <Incorrect Input Data>";
						}
					}
					catch (const char* err) {
						cerr << endl << err << endl;
						vtask = 99;
					}
				}
			}
			break;
		case 0:
			break;
		}
		cout << "\n________________Menu of action________________" << endl;
		cout << setw(35) << left << "Work with Task 1 var 17 " << " / Enter - 1" << endl;
		cout << setw(35) << left << "Work with Haffman algorithm  " << " / Enter - 2" << endl;
		cout << setw(35) << left << "Exit " << " / Enter - 0" << endl;

		if (task != 0) {
			ltask = 99;
			vtask = 99;
			try {
				cout << "\nChoose the Task: ";
				cin >> task;
				if (cin.fail()) {
					cin.clear();
					cin.ignore(numeric_limits<streamsize>::max(), '\n');
					throw "Error: <Incorrect Input Data>";
				}
			}
			catch (const char* err) {
				cerr << endl << err << endl;
				task = 99;
			}
		}
	}

	return 0;
}