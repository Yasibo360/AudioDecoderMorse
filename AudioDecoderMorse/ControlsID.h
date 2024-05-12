#pragma once

#include <vector>

const int IDreserv = 200;
const int IDreservMenu = IDreserv + 100;

const int IDMenuItem1 = IDreservMenu + 1;
const int IDMenuItem2 = IDreservMenu + 2;
const int IDMenuItem3 = IDreservMenu + 3;
const int IDMenuItem4 = IDreservMenu + 4;
const int IDMenuItem5 = IDreservMenu + 5;
const int IDMenuItem6 = IDreservMenu + 6;
const int IDMenuItem7 = IDreservMenu + 7;
const int IDMenuItem8 = IDreservMenu + 8;
const int IDMenuItem9 = IDreservMenu + 9;
const int IDMenuItem10 = IDreservMenu + 10;

const int IDPane1 = IDreserv + 10;
const int IDPane2 = IDreserv + 20;
const int IDPane3 = IDreserv + 30;
const int IDPane4 = IDreserv + 40;

const std::vector<int> IDPanes = { IDPane1, IDPane2, IDPane3, IDPane4 };

const int IDBgrMenu = IDreserv + 1;
const int IDButtPane1 = IDreserv + 2;
const int IDButtPane2 = IDreserv + 3;
const int IDButtPane3 = IDreserv + 4;
const int IDButtPane4 = IDreserv + 5;

const int IDPane1ButtonRec = IDPane1 + 1;
const int IDPane1EditRes = IDPane1 + 2;

const int IDPane2Plot = IDPane2 + 1;
const int IDPane2ButtPlay = IDPane2 + 2;
const int IDPane2ButtStop = IDPane2 + 3;
const int IDPane2ButtReset = IDPane2 + 4;
const int IDPane2EditCode = IDPane2 + 5;
const int IDPane2EditText = IDPane2 + 6;

const int IDPane3EditText = IDPane3 + 1;
const int IDPane3EditCode = IDPane3 + 2;
const int IDPane3ButtCopyText = IDPane3 + 3;
const int IDPane3ButtPlayCode = IDPane3 + 4;
const int IDPane3ButtCopyCode = IDPane3 + 5;
const int IDPane3ListDict = IDPane3 + 6;
const int IDPane3ButtAddDict = IDPane3 + 7;

const int IDPane4Picture = IDPane4 + 1;