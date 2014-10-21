#include "MdSpi.h"
#include <iostream>
#include "stdafx.h"
#include<fstream>
#include<sstream>
#include<string>
#include<vector>
#include "time.h"
#include "windows.h"
using namespace std;

time_t rawtime;
struct tm * timeinfo;
 
#pragma warning(disable : 4996)

// USER_API参数
extern CThostFtdcMdApi* pUserApi;

// 配置参数
extern char FRONT_ADDR[];		
extern TThostFtdcBrokerIDType	BROKER_ID;
extern TThostFtdcInvestorIDType INVESTOR_ID;
extern TThostFtdcPasswordType	PASSWORD;
extern char* ppInstrumentID[];	
extern int iInstrumentID;

// 请求编号
extern int iRequestID;

void CMdSpi::OnRspError(CThostFtdcRspInfoField *pRspInfo,
		int nRequestID, bool bIsLast)
{
	cerr << "--->>> "<< "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

void CMdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}
		
void CMdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void CMdSpi::OnFrontConnected()
{
	cerr << "--->>> " << "OnFrontConnected" << endl;
	///用户登录请求
	ReqUserLogin();
}

void CMdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, BROKER_ID);
	strcpy(req.UserID, INVESTOR_ID);
	strcpy(req.Password, PASSWORD);
	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> 发送用户登录请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin,
		CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///获取当前交易日
		cerr << "--->>> 获取当前交易日 = " << pUserApi->GetTradingDay()<< endl;
		// 请求订阅行情
		SubscribeMarketData();	
	}
}

void CMdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	cerr << "--->>> 发送行情订阅请求: " << ((iResult == 0) ? "成功" : "失败") << endl;
}

void CMdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{ 
	cerr << "OnRspSubMarketData" << endl;
}

void CMdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField *pSpecificInstrument, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "OnRspUnSubMarketData" << endl;
}

void CMdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField *pDepthMarketData)
{
//	cout<<pDepthMarketData->Volume  << "\t" << pDepthMarketData->InstrumentID << "\t" <<pDepthMarketData->AskPrice1<<endl;
 
	time(&rawtime);
	timeinfo=localtime(&rawtime);
	cout<<timeinfo->tm_hour<<' '<<timeinfo->tm_min<<' '<< timeinfo->tm_sec<<endl;
//	cout<< "," << pDepthMarketData->BidPrice2;cout<<endl;
//	cout<< "," << pDepthMarketData->AskPrice2;cout<<endl;
//	cout<< "," << pDepthMarketData->BidVolume2;cout<<endl;
//	cout<< "," << pDepthMarketData->AskVolume2;cout<<endl;
	ostringstream oname;
	oname << "d://record//"<< pDepthMarketData->TradingDay<<'_'<<timeinfo->tm_hour<<'_'<<timeinfo->tm_min<<'_'<<timeinfo->tm_sec<<"_record"<<".txt";

	string outname;
	outname=oname.str();


	int sz=outname.size();
		for(int i=4;i<sz;i++)
		{
			if (outname[i]==':')
			{
				outname[i]='_';
			}
		}

	//replace(tpname.begin(),tpname.end(),":","_");
	//cout << outname;
	static ofstream file(outname,ios_base::out);

//file.open("d://1.txt");
file<< pDepthMarketData->TradingDay;
file<<","<<timeinfo->tm_hour;
file<<':'<<timeinfo->tm_min;
file<<':'<< timeinfo->tm_sec;
file<< "," << pDepthMarketData->UpdateTime;
file<< ":" << pDepthMarketData->UpdateMillisec;
file<< "," << pDepthMarketData->InstrumentID;
file<< "," << pDepthMarketData->BidPrice1;
file<< "," << pDepthMarketData->AskPrice1;
file<< "," << pDepthMarketData->BidVolume1;
file<< "," << pDepthMarketData->AskVolume1;
file<< "," << pDepthMarketData->LastPrice;
file<< "," << pDepthMarketData->HighestPrice;
file<< "," << pDepthMarketData->LowestPrice;
file<< "," << pDepthMarketData->Volume;
file<< endl;
//file<<" "<<'bid ' << pDepthMarketData->BidPrice1;


//filesell<<'bid' << pDepthMarketData->BidPrice1;

//filesell.close();
}

bool CMdSpi::IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo)
{
	// 如果ErrorID != 0, 说明收到了错误的响应
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}