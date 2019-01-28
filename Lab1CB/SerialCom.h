#ifndef SERIALCOM_H_INCLUDED
#define SERIALCOM_H_INCLUDED

HANDLE handle_Init(void);
void dcb_Params(HANDLE hSerial);
void timeouts_Init(HANDLE hSerial);

#endif // SERIALCOM_H_INCLUDED
