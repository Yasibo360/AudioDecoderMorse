#pragma once

#include <vector>

const int IDreserv = 200;

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

const int IDPane3EditText = IDPane3 + 1;
const int IDPane3EditCode = IDPane3 + 2;
const int IDPane3ButtCopyText = IDPane3 + 3;
const int IDPane3ButtPlayCode = IDPane3 + 4;
const int IDPane3ButtCopyCode = IDPane3 + 5;
const int IDPane3ListDict = IDPane3 + 6;
const int IDPane3ButtAddDict = IDPane3 + 7;

const int IDPane4Picture = IDPane4 + 1;