// testTraderApi.cpp : 定义控制台应用程序的入口点。
//

#include ".\ThostTraderApi\ThostFtdcMdApi.h"
#include "MdSpi.h"
#include <iostream>
#include <windows.h>

// UserApi对象
CThostFtdcMdApi* pUserApi;

// 配置参数

char FRONT_ADDR[] = "tcp://asp-sim2-md1.financial-trading-platform.com:26213";		// 前置地址
TThostFtdcBrokerIDType	BROKER_ID = "2030";				// 经纪公司代码

//char FRONT_ADDR[] = "tcp://shcifco-md1.financial-trading-platform.com:41213";		// 前置地址
//TThostFtdcBrokerIDType	BROKER_ID = "8070";				// 经纪公司代码

//char FRONT_ADDR[] = "tcp://gtja-md1.financial-trading-platform.com:41213";		// 前置地址
//TThostFtdcBrokerIDType	BROKER_ID = "7090";				// 经纪公司代码

TThostFtdcInvestorIDType INVESTOR_ID = "00092";			// 投资者代码
TThostFtdcPasswordType  PASSWORD = "888888";			// 用户密码
//char *ppInstrumentID[] = { "m1501","ru1501","cu1501","IF1411"};	// 行情订阅列表
//char *ppInstrumentID[] = {"IF1411"};	// 行情订阅列表 这样不可以 why 20141016 
//看起来像最后一个不被处理 因为观察到订阅四个只输出了三个，订阅一个直接退出了
char *ppInstrumentID[] = {"m1501","IF1411","IF1410","cu1501"};	// 行情订阅列表
int iInstrumentID = 4;									// 行情订阅数量

// 请求编号
int iRequestID = 0;

void main(void)
{
	HWND hwnd;
	if(hwnd=::FindWindow("ConsoleWindowClass",NULL)) //找到控制台句柄
	{
//	::ShowWindow(hwnd,SW_HIDE); //隐藏控制台窗口
	}
	// 初始化UserApi
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();			// 创建UserApi
	CThostFtdcMdSpi* pUserSpi = new CMdSpi();
	pUserApi->RegisterSpi(pUserSpi);						// 注册事件类
	pUserApi->RegisterFront(FRONT_ADDR);					// connect
	pUserApi->Init();

	pUserApi->Join();
//	pUserApi->Release();

}

