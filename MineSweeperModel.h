#ifndef MINESWEEPERMODEL_H
#define MINESWEEPERMODEL_H

#include <QAbstractTableModel>
#include "CellData.h"

class MineSweeperModel : public QAbstractTableModel
{
    Q_OBJECT

    Q_PROPERTY(bool BOOM READ getBOOM WRITE setBOOM NOTIFY BOOMChanged)
    Q_PROPERTY(bool WIN READ getWIN WRITE setWIN NOTIFY WINChanged)

    Q_ENUMS(Roles)
public:

    enum Roles {
        CellRole
    };

    QHash<int, QByteArray> roleNames() const override {
        return {
            { CellRole, "value" }
        };
    }
signals:
    void BOOMChanged();
    void WINChanged();
public:
    explicit MineSweeperModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    QVariant calculateImage( int column, int row ) const;
    Qt::ItemFlags flags(const QModelIndex &index) const override;
    int  getBOOM()const;
    void setBOOM(const bool &i);
    int  getWIN()const;
    void setWIN(const bool &i);

    Q_INVOKABLE void clear();
    Q_INVOKABLE void init();
    Q_INVOKABLE void timeElapsed();

private:
    // Открывает ячейку
    void openPlace( int column, int index );
    // Устанавливает флаг в ячейку
    void setFlag( int column, int row );
    //  проверяет выигрыш
    bool is_win();
    // Конвертирует координаты из двухмерных в одномерные
    size_t  two2one(int x, int y ) const;
    // проверяет является ли i,j координатами
    bool isValidCoord(int i, int j);
    // открывает ячейку и все ячейку вокруг
    void openCell(int i, int j);
    // Открыть игровое поле
    void openAll();
private:
    static constexpr int width = 9;
    static constexpr int height = 9;
    static constexpr int size = width * height;
    static constexpr int Mines = 10; // number of mines on the board
    using StateContainer = std::vector<CellData>;
    // игровое поле
    StateContainer m_currentState;
    // Поражение
    bool BOOM;
    // Победа
    bool WIN;
    // Количество оставшихся врагов
    int FlagsCount;
    // Initially the game is not over
    bool m_isGameActive;

};

#endif // GAMEOFLIFEMODEL_H
