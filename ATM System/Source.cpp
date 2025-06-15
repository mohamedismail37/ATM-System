#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
using namespace std;

struct stClientData
{
	string AccountNumber;
	short PINCode;
	float Balance;
};

enum enATMMenuChoices
{
	QuickWithdraw = 1, NormalWithDraw = 2, Deposit = 3, CheckBalance = 4,ChangePINCode = 5, Logout = 6
};

enum enQuickWithdrawChoice
{
	Twenty = 1, Fifty = 2, OneHundred = 3, TwoHundred = 4, FourHundred = 5, SixHundred = 6, EightHundred = 7, OneThousand = 8, Exit = 9
};

stClientData CurrentClient;

const string ClientsFileName = "ClientsData.txt";

vector <string> Splitter(string Line, string Separator = "#//#")
{
	short Position = 0;
	vector <string> vWords;
	string Word;

	while ((Position = Line.find(Separator)) != std::string::npos)
	{
		Word = Line.substr(0, Position);
		Line.erase(0, Position + Separator.length());
		vWords.push_back(Word);
	}
	if (Line != "")
	{
		vWords.push_back(Line);
	}
	return vWords;
}

stClientData FromVectorOfStringTostClientData(vector <string> vWords)
{
	stClientData ClientData;

	ClientData.AccountNumber = vWords[0];
	ClientData.PINCode = stoi(vWords[1]);
	ClientData.Balance = stod(vWords[2]);

	return ClientData;
}

void ReadFromFileToVectorOfstCleintData(vector <stClientData>& vOfstClientData, string FileName = ClientsFileName)
{
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	
	if (MyFile.is_open())
	{
		string Line;
		
		while (getline(MyFile, Line))
		{
			vOfstClientData.push_back(FromVectorOfStringTostClientData(Splitter(Line)));
		}
		MyFile.close();
	}
}

string Joinner(stClientData OneClient, string Separator = "#//#")
{
	string Line = "";
	Line += OneClient.AccountNumber + Separator;
	Line += to_string(OneClient.PINCode) + Separator;
	Line += to_string(OneClient.Balance);

	return Line;
}

void PrintvOfstClientDataInFile(vector <stClientData>& vOfstClientData)
{
	fstream MyFile;
	MyFile.open(ClientsFileName, ios::out);

	if (MyFile.is_open())
	{
		for (stClientData& C : vOfstClientData)
		{
			MyFile << Joinner(C) << endl;
		}
		MyFile.close();
	}
}

void Login();

void GoBackToMainMenu()
{
	cout << "\nPress any key to go back to Main Menu...";
	system("pause>0");
}

bool IsClientValid(stClientData &ClientData, vector <stClientData> vOfstClientData)
{
	for (stClientData &C : vOfstClientData)
	{
		if (C.AccountNumber == ClientData.AccountNumber && C.PINCode == ClientData.PINCode)
		{
			ClientData = C;
			return true;
		}
	}
	return false;
}

stClientData LoginScreen(vector <stClientData> vOfstClientData)
{
	stClientData ClientData;
	short i = 0;
	do
	{

		system("cls");
		cout << "-------------------------------\n";
		cout << "          Login Screen\n";
		cout << "-------------------------------\n";

		if (!IsClientValid(ClientData, vOfstClientData) && i > 0)
			cout << "Invalid Account Number/PINCode!\n";

		cout << "Enter Account Number? ";
		cin >> ClientData.AccountNumber;
		cout << "Enter PIN? ";
		cin >> ClientData.PINCode;

		i++;
	}while(!IsClientValid(ClientData, vOfstClientData));

	return ClientData;
}

enATMMenuChoices ATMMainMenuScreen(stClientData TheCurrentClient = CurrentClient)
{
	short Choice = 0;
	system("cls");
	cout << "============================================\n";
	cout << "        ATM Main Menu Screen\n";
	cout << "============================================\n";
	cout << "     [1] Quick Withdraw.\n";
	cout << "     [2] Normal Withdraw.\n";
	cout << "     [3] Deposit.\n";
	cout << "     [4] Check Balance.\n";
	cout << "     [5] Change Password.\n";
	cout << "     [6] Logout.\n";
	cout << "============================================\n";
	do
	{
		cout << "Choose what do you want to do? [1 to 6]? ";
		cin >> Choice;
	} while (Choice < 0 || Choice > 6);

	return enATMMenuChoices(Choice);
}

bool IsClientHaveEnoughMoney( float AmountWillTake, stClientData ClientData = CurrentClient)
{
	return (ClientData.Balance >= AmountWillTake) ? true : false;
}

bool PerformTransaction()
{
	char Perform;
	cout << "\nAre you sure you want to perform this transaction? [Y/N]? ";
	cin >> Perform;

	return (Perform == 'y' || Perform == 'Y') ? true : false;
}

void TheAmountExceeds()
{
	cout << "\nThe amount exceeds your balance, make another choice.\n";
	cout << "Press Anykey to continue...";
	system("pause>0");
}

void DoneSuccessfully(stClientData ClientData = CurrentClient)
{
	cout << "\nDone Successfully. New balance is: " << CurrentClient.Balance << endl;
}

void CallQuickWtihdrawForward(enQuickWithdrawChoice Choice);

void QuickWithdrawScreen(stClientData ClientData = CurrentClient)
{
	short Choice = 0;
	system("cls");
	cout << "============================================\n";
	cout << "        Quick Withdraw\n";
	cout << "============================================\n";
	cout << "     [1] 20             [2] 50\n";
	cout << "     [3] 100            [4] 200\n";
	cout << "     [5] 400            [6] 600\n";
	cout << "     [7] 800            [8] 1000\n";
	cout << "     [9] Exit\n";
	cout << "============================================\n";
	cout << "Your Balance is " << ClientData.Balance << endl;
	do
	{
		cout << "Choose from withdraw from[1] to [9] ? ";
		cin >> Choice;
	} while (Choice < 1 || Choice > 9);

	
	CallQuickWtihdrawForward(enQuickWithdrawChoice(Choice));

}

void QuickWithDrawForward(enQuickWithdrawChoice Choice, stClientData &ClientData = CurrentClient)
{
	switch (Choice)
	{
	case Twenty:
		if (IsClientHaveEnoughMoney(20))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 20;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case Fifty:
		if (IsClientHaveEnoughMoney(50))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 50;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case OneHundred:
		if (IsClientHaveEnoughMoney(100))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 100;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case TwoHundred:
		if (IsClientHaveEnoughMoney(200))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 200;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case FourHundred:
		if (IsClientHaveEnoughMoney(400))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 400;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case SixHundred:
		if (IsClientHaveEnoughMoney(600))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 600;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case EightHundred:
		if (IsClientHaveEnoughMoney(800))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 800;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case OneThousand:
		if (IsClientHaveEnoughMoney(1000))
		{
			if (PerformTransaction())
			{
				CurrentClient.Balance -= 1000;
				DoneSuccessfully();
				GoBackToMainMenu();
			}
		}
		else
		{
			TheAmountExceeds();
			QuickWithdrawScreen();
		}
		break;
	case Exit:
		GoBackToMainMenu();
		ATMMainMenuScreen();
		break;
	}
}

void CallQuickWtihdrawForward(enQuickWithdrawChoice Choice)
{
	QuickWithDrawForward(enQuickWithdrawChoice(Choice));
}

void CheckBalanceScreen(stClientData ClientData = CurrentClient)
{
	
	system("cls");
	cout << "============================================\n";
	cout << "        Check Balance Screen\n";
	cout << "============================================\n";
	cout << "Your Balance is " << ClientData.Balance << endl;
	GoBackToMainMenu();
}

void LogoutScreen()
{
	Login();
}

void AssignToVectorTheCurrentClientAfterUpdate(vector <stClientData>& vOfstClientData, stClientData ClientData = CurrentClient)
{
	for (stClientData& C : vOfstClientData)
	{
		if (CurrentClient.AccountNumber == C.AccountNumber)
			C = CurrentClient;
	}
}

void NormalWithdrawScreen(stClientData ClientData = CurrentClient)
{
	int Number = 0;
	system("cls");
	cout << "============================================\n";
	cout << "        Normal Withdraw Screen\n";
	cout << "============================================\n";
	
	do
	{
		cout << "\nEnter an amount multiple of 5's ? ";
		cin >> Number;
	} while (Number % 5 != 0);

	if (IsClientHaveEnoughMoney(Number))
	{
		if (PerformTransaction())
		{
			CurrentClient.Balance -= Number;
			DoneSuccessfully();
			GoBackToMainMenu();
		}
	}
	else
	{
		TheAmountExceeds();
		NormalWithdrawScreen();
	}

}

void DepositScreen(stClientData ClientData = CurrentClient)
{
	int Number = 0;
	system("cls");
	cout << "============================================\n";
	cout << "        Deposite Screen\n";
	cout << "============================================\n";

	do
	{
		cout << "\nEnter a positive Deposite Amount ? ";
		cin >> Number;
	} while (Number <= 0);

	if (PerformTransaction())
	{
		CurrentClient.Balance += Number;
		DoneSuccessfully();
	}

	GoBackToMainMenu();
}

void ChangePINCodeScreen(stClientData ClientData = CurrentClient)
{
	int Number = 0;
	system("cls");
	cout << "============================================\n";
	cout << "        Change PIN Code Screen\n";
	cout << "============================================\n";

	do
	{
		cout << "\nEnter The new PIN Code ? ";
		cin >> Number;
	} while (Number <= 0);

	CurrentClient.PINCode = Number;
}

void ClientChoiceForward(enATMMenuChoices ClientChoice, vector <stClientData> &vOfstClientData)
{
	switch (ClientChoice)
	{
	case QuickWithdraw:
		QuickWithdrawScreen();
		AssignToVectorTheCurrentClientAfterUpdate(vOfstClientData);
		PrintvOfstClientDataInFile(vOfstClientData);
		ClientChoiceForward(ATMMainMenuScreen(), vOfstClientData);
		break;
	case NormalWithDraw:
		NormalWithdrawScreen();
		AssignToVectorTheCurrentClientAfterUpdate(vOfstClientData);
		PrintvOfstClientDataInFile(vOfstClientData);
		ClientChoiceForward(ATMMainMenuScreen(), vOfstClientData);
		break;
	case Deposit:
		DepositScreen();
		AssignToVectorTheCurrentClientAfterUpdate(vOfstClientData);
		PrintvOfstClientDataInFile(vOfstClientData);
		ClientChoiceForward(ATMMainMenuScreen(), vOfstClientData);
		break;
	case CheckBalance:
		CheckBalanceScreen();
		ClientChoiceForward(ATMMainMenuScreen(), vOfstClientData);
		break;
	case ChangePINCode:
		ChangePINCodeScreen();
		AssignToVectorTheCurrentClientAfterUpdate(vOfstClientData);
		PrintvOfstClientDataInFile(vOfstClientData);
		ClientChoiceForward(ATMMainMenuScreen(), vOfstClientData);
		break;
	case Logout:
		LogoutScreen();
		break;
	}
}

void Login()
{
	vector <stClientData> vOfstClientData;

	ReadFromFileToVectorOfstCleintData(vOfstClientData);
	
	CurrentClient = LoginScreen(vOfstClientData);

	ClientChoiceForward(ATMMainMenuScreen(), vOfstClientData);

}

int main()
{
	Login();

	return 0;

}
