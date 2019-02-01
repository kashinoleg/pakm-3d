#pragma once
#include <deque>
//#include <Python.h>
#include "mymath.h"

using namespace std;

/*class CPythonWrapper
{
private:
	int m_iHistoryPosition;
	PyObject *m_pConsoleInstance;
	PyObject *m_pCompleterInstance;
	deque<string> m_CommandHistory;
private:
	PyObject *CreateInstance(char szModule[], char szClass[], PyObject *pArgs = NULL);
	void DeleteInstance(PyObject *pInstance);
public:
	CPythonWrapper(void);
	~CPythonWrapper(void);

	bool SendCommand(string Command);
	bool SendCodeBlock(string Code);

	string Complete(string Text, long iState);
	vector<string> GetCompleteOptions(string Text);
	string GetPrevHistoryCommand();
	string GetNextHistoryCommand();

	bool ConsoleInstanceCreated() { return m_pConsoleInstance?true:false; }
	bool CompleterInstanceCreated() { return m_pCompleterInstance?true:false; }
	deque<string> &GetCommandHistory() { return m_CommandHistory; }
};
//*/