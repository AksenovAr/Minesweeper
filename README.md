1.Игра сапер реализует принципы модель-представление
2.Модель реализована в классе MinesweeperModel.Также этот класс содержит логику работы с полем.
3.Класс CellData содержит свояства ячейки.

4.Вид сделан на типе TableView и имеет два делегата.
Первый msdelegate который является представлением для ячейки
второй делегат boomdelegate который является делегатом ячейки где происходит взрыв. 
Там же реализоана простенькая анимация на основе ParticleSystem имитирующая взрыв.
5.Также используется всплывающее окно которое появляется в случаю проигрыша по истечению времени или в случае выигрыша.
6. Изначально есть 10 мин и 10 флажков.
7. Поле 9х9
8. Время 180 секунд.
9. Сама модель реализована так что-бы при небходимости при загрузке приложения можно было бы устанавливать размер поля програмно.
Но не на лету !!!
