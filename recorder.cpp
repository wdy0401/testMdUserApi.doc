// testTraderApi.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"
#include <iostream>
#include <windows.h>

// UserApi����
CThostFtdcMdApi* pUserApi;

// ���ò���

char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// ǰ�õ�ַ
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// ���͹�˾����

//char FRONT_ADDR[] = "tcp://shcifco-md1.financial-trading-platform.com:41213";		// ǰ�õ�ַ
//TThostFtdcBrokerIDType	BROKER_ID = "8070";				// ���͹�˾����

//char FRONT_ADDR[] = "tcp://gtja-md1.financial-trading-platform.com:41213";		// ǰ�õ�ַ
//TThostFtdcBrokerIDType	BROKER_ID = "7090";				// ���͹�˾����

TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// Ͷ���ߴ���
TThostFtdcPasswordType  PASSWORD = "888888";			// �û�����
//char *ppInstrumentID[] = { "m1501","ru1501","cu1501","IF1411"};	// ���鶩���б�
//char *ppInstrumentID[] = {"IF1411"};	// ���鶩���б� ���������� why 20141016 
//�����������һ���������� ��Ϊ�۲쵽�����ĸ�ֻ���������������һ��ֱ���˳���
char *ppInstrumentID[] = {"m1501","IF1411","IF1410","cu1501"};	// ���鶩���б�
int iInstrumentID = 4;									// ���鶩������

// ������
int iRequestID = 0;

void main(void)
{
	HWND hwnd;
	if(hwnd=::FindWindow("ConsoleWindowClass",NULL)) //�ҵ�����̨���
	{
//	::ShowWindow(hwnd,SW_HIDE); //���ؿ���̨����
	}
	// ��ʼ��UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// ����UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// ע���¼���
	pUserApi->RegisterFront(FRONT_ADDR);					// connect
	pUserApi->Init();

	pUserApi->Join();
//	pUserApi->Release();

}

