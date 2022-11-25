#include <iostream>
#include <windows.h>
#include <stdio.h>
#include <math.h>
using namespace std;

#define Pi 3.1459

#define DonutRadius 2
#define SquareSideLenght 0.5
#define ScreenOffset 5
#define ScreenWidth 64
#define ScreenHeight 128
#define SquareSpacing 0.004
#define DonutSpacing 0.007


const float K1 = ScreenWidth * ScreenOffset * 3 / (8 * (DonutRadius + SquareSideLenght));

void gotoxy(SHORT x, SHORT y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}


int main()
{
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO CursorInfo;
	GetConsoleCursorInfo(out, &CursorInfo);
	CursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &CursorInfo);

	float A = 0, B = 0;
	float HalfSideLenght = SquareSideLenght / 2;


	float ZBuffer[ScreenWidth][ScreenHeight];
	char Output[ScreenWidth][ScreenHeight];

	int F;
	int K;
	cout << "\x1b[23";
	while (true)
	{
		for (int i = 0; i < ScreenWidth; i++)
		{
			memset(Output[i], 32, ScreenHeight);
			memset(ZBuffer[i], 0, ScreenHeight * 4);
		}

		float CosA = cos(A), SinA = sin(A);
		float CosB = cos(B), SinB = sin(B);

		for (float Alfa = 0; Alfa < SquareSideLenght; Alfa += SquareSpacing)
		{
			//cout << Alfa << endl;
			for (float Beta = 0; Beta < 2 * Pi; Beta += DonutSpacing)
			{
				float CosBeta = cos(Beta), SinBeta = sin(Beta);

				float Square_Y = Alfa - HalfSideLenght;
				float Square_PY_X = DonutRadius - HalfSideLenght;
				float Square_NY_X = DonutRadius + HalfSideLenght;

				float PY_X = Square_PY_X * (CosB * CosBeta + SinA * SinB * SinBeta) - Square_Y * CosA * SinB;
				float PY_Y = Square_PY_X * (CosBeta * SinB - CosB * SinA * SinBeta) + Square_Y * CosA * CosB;
				float PY_Z = ScreenOffset + Square_PY_X * CosA * SinBeta + Square_Y * SinA;
				float OOZ_PY = 1 / PY_Z;
				int PY_X_P = (int)(ScreenWidth / 2 + K1 * OOZ_PY * PY_X);
				int PY_Y_P = (int)(ScreenHeight / 2 - K1 * OOZ_PY * PY_Y);

				float NY_X = Square_NY_X * (CosB * CosBeta + SinA * SinB * SinBeta) - Square_Y * CosA * SinB;
				float NY_Y = Square_NY_X * (CosBeta * SinB - CosB * SinA * SinBeta) + Square_Y * CosA * CosB;
				float NY_Z = ScreenOffset + Square_NY_X * CosA * SinBeta + Square_Y * SinA;
				float OOZ_NY = 1 / NY_Z;
				int NY_X_P = (int)(ScreenWidth / 2 + K1 * OOZ_NY * NY_X);
				int NY_Y_P = (int)(ScreenHeight / 2 - K1 * OOZ_NY * NY_Y);

				float Square_X = DonutRadius - HalfSideLenght + Alfa;
				float Square_PX_Y = -HalfSideLenght;
				float Square_NX_Y = HalfSideLenght;
				
				float PX_X = Square_X * (CosB * CosBeta + SinA * SinB * SinBeta) - Square_PX_Y * CosA * SinB;
				float PX_Y = Square_X * (CosBeta * SinB - CosB * SinA * SinBeta) + Square_PX_Y * CosA * CosB;
				float PX_Z = ScreenOffset + Square_X * CosA * SinBeta + Square_PX_Y * SinA;
				float OOZ_PX = 1 / PX_Z;
				int PX_X_P = (int)(ScreenWidth / 2 + K1 * OOZ_PX * PX_X);
				int PX_Y_P = (int)(ScreenHeight / 2 - K1 * OOZ_PX * PX_Y);

				float NX_X = Square_X * (CosB * CosBeta + SinA * SinB * SinBeta) - Square_NX_Y * CosA * SinB;
				float NX_Y = Square_X* (CosBeta * SinB - CosB * SinA * SinBeta) + Square_NX_Y * CosA * CosB;
				float NX_Z = ScreenOffset + Square_X * CosA * SinBeta + Square_NX_Y * SinA;
				float OOZ_NX = 1 / NX_Z;
				int NX_X_P = (int)(ScreenWidth / 2 + K1 * OOZ_NX * NX_X);
				int NX_Y_P = (int)(ScreenHeight / 2 - K1 * OOZ_NX * NX_Y);


				float Luminance = Alfa * SinA * (1 + CosB);
				//cout << Luminance * 8 + 1 << endl;
				if (OOZ_PY > ZBuffer[PY_X_P][PY_Y_P])
				{
					ZBuffer[PY_X_P][PY_Y_P] = OOZ_PY;
					int LuminanceIndex = Luminance * 8;
					Output[PY_X_P][PY_Y_P] = ".,-~:;=!*?0123#$@"[LuminanceIndex];
					//cout << LuminanceIndex  << "<-" << endl;
				}
				if (OOZ_NY > ZBuffer[NY_X_P][NY_Y_P])
				{
					ZBuffer[NY_X_P][NY_Y_P] = OOZ_NY;
					int LuminanceIndex = Luminance * 8;
					Output[NY_X_P][NY_Y_P] = ".,-~:;=!*?0123#$@"[LuminanceIndex];
					//cout << LuminanceIndex << "<-" << endl;
				}
				if (OOZ_PX > ZBuffer[PX_X_P][PX_Y_P])
				{
					ZBuffer[PX_X_P][PX_Y_P] = OOZ_PX;
					int LuminanceIndex = Luminance * 8 + 1;
					//cout << PX_X_P << "-----" << PX_Y_P << endl;
					//cout << LuminanceIndex << "<-" << endl;
					Output[PX_X_P][PX_Y_P] = ".,-~:;=!*?0123#$@"[LuminanceIndex];
					
					
				}
				if (OOZ_PY > ZBuffer[PY_X_P][PY_Y_P])
				{
					ZBuffer[PY_X_P][PY_Y_P] = OOZ_PY;
					int LuminanceIndex = Luminance * 8;
					Output[PY_X_P][PY_Y_P] = ".,-~:;=!*?0123#$@"[LuminanceIndex];
					//cout << LuminanceIndex << "<-" << endl;
				}
				
			}
		}

		cout << "\x1b[H";
		
		for (F = 0; F < ScreenWidth; F++)
		{
			for (K = 0; K < ScreenHeight; K++)
			{
				cout << Output[F][K];
				//cout << "adasa";
				A += 0.00004;
				B += 0.00002;
			}
			cout << '\n';
		};
		gotoxy(0, 0);

	}
	
	
	
}              
