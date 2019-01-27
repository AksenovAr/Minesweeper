#include "CellData.h"

CellData::CellData(int i, int j)
    :  m_i(i), m_j(j), m_value(0), m_isMine(false)
    , m_isOpen(false), m_isFlag(false), m_isBoom (false)
{
}

int CellData::i() { return m_i; }
int CellData::j() { return m_j; }
int CellData::value() { return m_value; }

void CellData::incValue() { ++m_value; }
bool CellData::isMine() { return m_isMine; }
void CellData::setMine() { m_isMine = true;  }
bool CellData::isOpen() { return m_isOpen; }
void CellData::setOpen() { m_isOpen = true; }
void CellData::setBoom() { m_isBoom = true; }
void CellData::clear()
{
    m_value = 0;
    m_isMine = false;
    m_isOpen = false;
    m_isFlag = false;
    m_isBoom = false;
}
