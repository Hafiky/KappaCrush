#ifndef VIEW_H
#define VIEW_H
#include <QGraphicsView>
#include <QKeyEvent>
#include <QTime>
#include <QDebug>


#include "blok.h"

const int SIRKA=10;
const int VYSKA=10;

class View : public QGraphicsView
{
    Q_OBJECT
public:
    View();
    View(QGraphicsScene *scene);
    QList<QList<Blok*> > list_bloks;
    //int skore=0;


protected:
    //void timerEvent(QTimerEvent * event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    QColor randomBarva();
    void DeleteBlok(int i,int j);

public slots:
    void OnBlockMove(QPointF prvni_pred,QPointF druhy_pred);
    void BlockProhozeniIndex(int posxprvni, int posyprvni, int posxdruhy, int posydruhy);
    void BlockProhozeniPoziceIndex(int posxprvni, int posyprvni, int posxdruhy, int posydruhy);
    void SchozeniBlokov();
    void GenerovaniBlokov();
    void Smazani3();
    void Smazani4();
    void Smazani5();
    bool NachaziSe3();
    void CreateTextItems();
    void Casovac();
    void GameOver();
    void Update();
signals:
    DeleteBlokov();
private:
    QGraphicsTextItem *scoreItem;
    QGraphicsTextItem *timeItem;
    QGraphicsTextItem *levelItem;
    QGraphicsTextItem *goalItem;
    QGraphicsTextItem *gameOverItem;
    Blok *blok;
    QTime time;
    bool NachaziNull=false;
    int sekunda=0;
    int width;
    int height;
    int skore=0;
    int cil=10;
    QTimer *timer;
    bool bloky=false;




};
#endif // VIEW_H
