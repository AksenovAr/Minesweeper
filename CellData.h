#ifndef CELLDATA_H
#define CELLDATA_H

//! одна клетка поля
class CellData
{
public:
    CellData(int i, int j);
    int i();
    int j();
    bool isMine();
    void setMine();
    void incValue();
    int  value();
    void setOpen();
    bool isOpen();
    void clear();
    void setBoom();

    int m_i, m_j;
    int m_value;
    bool m_isMine;
    bool m_isOpen;
    bool m_isFlag;
    bool m_isBoom;
};

#endif // END iF
