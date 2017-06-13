#include "view.h"
#include <QTimer>
#include <QColor>
View::View()
{


}

View::View(QGraphicsScene *scene)
{
    width = VYSKA*SIRKA_CTVERCE+150;
    height = SIRKA*SIRKA_CTVERCE;
    setScene(scene);
   // setFixedSize(width,height);
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // this->scene()->addRect(0,0,500,500,QPen(Qt::black),QBrush(QColor(100,230,100)));
    time = QTime::currentTime();
    qsrand((uint)time.msec());

    CreateTextItems();

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(Update()));
    timer->start(200);
}
void View::CreateTextItems()
{
    QFont font("Helvetica [Cronyx]", 18);
    font.setBold(true);

    levelItem = new QGraphicsTextItem("Level: 1");
    levelItem->setPos(-150,00);
    levelItem->setFont(font);
    this->scene()->addItem(levelItem);

    goalItem = new QGraphicsTextItem("Cíl: 10");
    goalItem->setPos(-150,30);
    goalItem->setFont(font);
    this->scene()->addItem(goalItem);

    scoreItem = new QGraphicsTextItem("Score: 0");
    scoreItem->setPos(-150,60);
    scoreItem->setFont(font);
    this->scene()->addItem(scoreItem);

    timeItem = new QGraphicsTextItem("Cas: 60");
    timeItem->setPos(-150,90);
    timeItem->setFont(font);
    this->scene()->addItem(timeItem);

    gameOverItem = new QGraphicsTextItem("Game Over");
    font.setPixelSize(119);
    gameOverItem->setFont(font);
    gameOverItem->setPos(-150,110);
    gameOverItem->setPlainText("");
    gameOverItem->setZValue(5);
    QColor red(255, 51, 130);
    gameOverItem->setDefaultTextColor(red);
    this->scene()->addItem(gameOverItem);
}


void View::Update()
{
    if (!bloky) {
        for (int i = 0; i < VYSKA; ++i) {// i jsou radky, j sloupce
            QList<Blok*> bloks;

            for (int j = 0; j < SIRKA; ++j) {

                Blok *blok = new Blok(this);
                this->scene()->addItem(blok);
                blok->setPos(j*SIRKA_CTVERCE,i*SIRKA_CTVERCE);
                blok->setBarva(randomBarva());
                blok->setZValue(1);

                bloks.append(blok);
                connect(blok,SIGNAL(BlockMove(QPointF,QPointF)),
                        this, SLOT(OnBlockMove(QPointF,QPointF)));

            }
            list_bloks.append(bloks);

        }
        this->centerOn(this->list_bloks[4][4]);
        bloky=true;
    }

    Smazani5();
    Smazani4();
    Smazani3();

    SchozeniBlokov();
    GenerovaniBlokov();
    Casovac();

}


void View::keyPressEvent(QKeyEvent *event)
{
}

void View::keyReleaseEvent(QKeyEvent *event)
{
}
void View::Casovac()
{
    sekunda++;
    //qDebug()<<sekunda;
    if (sekunda%5==0) {
        timeItem->setPlainText("Cas: " +
                               QString::number(timeItem->toPlainText().split(' ').at(1).toInt()-1));

    }
    if (sekunda%300==0) {
        GameOver();


        //Blok().setFlag(QGraphicsItem::ItemIsMovable, false);

    }
    if (skore>=cil) {
        goalItem->setPlainText("Cíl: " +
                               QString::number(goalItem->toPlainText().split(' ').at(1).toInt()+10));
        levelItem->setPlainText("Level: " +
                                QString::number(levelItem->toPlainText().split(' ').at(1).toInt()+1));
        scoreItem->setPlainText("Skore: 0");
        timeItem->setPlainText("Cas: 60");
        skore=0;
        sekunda=0;
        cil=cil+10;
    }
}
QColor View::randomBarva()
{
    QList  <QColor> barvicka;
    barvicka << Qt::blue << Qt::black << Qt::red << Qt::green<< Qt::cyan<< Qt::magenta<< Qt::yellow;
    int high=barvicka.length();
    int low=0;

    //time = QTime::currentTime();
    int randomValue =qrand() % ((high) - low) + low;
    // int randomValue = (qrand() % ((High ) - Low) + Low);
    return barvicka[randomValue];
}

void View::DeleteBlok(int i, int j)
{
    Blok *blok =list_bloks[i][j];
    list_bloks[i][j]=NULL;
    delete blok;
    scoreItem->setPlainText("Score: " +
                            QString::number(scoreItem->toPlainText().split(' ').at(1).toInt()+1));
    skore++;

}

void View::OnBlockMove(QPointF prvni_pred, QPointF druhy_pred)
{


    qDebug()<<prvni_pred.x();
    qDebug()<<prvni_pred.y();
    qDebug()<<druhy_pred.x();
    qDebug()<<druhy_pred.y();
    BlockProhozeniIndex(prvni_pred.y()/SIRKA_CTVERCE,prvni_pred.x()/SIRKA_CTVERCE,
                        druhy_pred.y()/SIRKA_CTVERCE,druhy_pred.x()/SIRKA_CTVERCE);

    //    qDebug() << list_bloks[posxprvni][posyprvni];
    //    qDebug() << list_bloks[posxdruhy][posydruhy];



}

void View::BlockProhozeniIndex(int posxprvni, int posyprvni, int posxdruhy, int posydruhy)
{
    if (list_bloks[posxprvni][posyprvni]!=NULL &&
            list_bloks[posxdruhy][posydruhy]!=NULL ) {
        //qDebug() << list_bloks[posxprvni][posyprvni]->getBarva().name();
        //qDebug() << list_bloks[posxdruhy][posydruhy]->getBarva().name();
    }
    qDebug()<<"druhy";
    qDebug()<<list_bloks[posxprvni][posyprvni];
    qDebug()<<"p";
    qDebug()<<list_bloks[posxdruhy][posydruhy];


    Blok *blok =list_bloks[posxprvni][posyprvni];
    list_bloks[posxprvni][posyprvni]=list_bloks[posxdruhy][posydruhy];
    list_bloks[posxdruhy][posydruhy]=blok;

    if (!NachaziSe3()) {
        Blok *blok =list_bloks[posxprvni][posyprvni];
        list_bloks[posxprvni][posyprvni]=list_bloks[posxdruhy][posydruhy];
        list_bloks[posxdruhy][posydruhy]=blok;

        list_bloks[posxprvni][posyprvni]->setPos(posyprvni*SIRKA_CTVERCE,posxprvni*SIRKA_CTVERCE);
        list_bloks[posxdruhy][posydruhy]->setPos(posydruhy*SIRKA_CTVERCE,posxdruhy*SIRKA_CTVERCE);
    }

    if (list_bloks[posxprvni][posyprvni]!=NULL &&
            list_bloks[posxdruhy][posydruhy]!=NULL ) {
        // qDebug() << list_bloks[posxprvni][posyprvni]->getBarva().name();
        // qDebug() << list_bloks[posxdruhy][posydruhy]->getBarva().name();
    }
    gameOverItem->setPos(-150,110);
    this->scene()->update();

}
void View::BlockProhozeniPoziceIndex(int posxprvni, int posyprvni, int posxdruhy, int posydruhy)
{
    Blok *blok =list_bloks[posxprvni][posyprvni];
    list_bloks[posxprvni][posyprvni]=list_bloks[posxdruhy][posydruhy];
    list_bloks[posxdruhy][posydruhy]=blok;

    qDebug() << "pred :"  << list_bloks[posxprvni][posyprvni];
    qDebug() << list_bloks[posxdruhy][posydruhy];

    list_bloks[posxdruhy][posydruhy]->setPos(posxprvni*SIRKA_CTVERCE,posyprvni*SIRKA_CTVERCE);
    list_bloks[posxprvni][posyprvni]->setPos(posxdruhy*SIRKA_CTVERCE,posydruhy*SIRKA_CTVERCE);

    this->scene()->update();

}


void View::SchozeniBlokov(){
    for (int i = 0; i < VYSKA; ++i) {
        for (int j = 0; j < SIRKA; ++j) {
            if (list_bloks[i][j]==NULL&&i>0) {//i je radek, j je soupec
                //qDebug() << "nad" << i << j;

                for (int x = i-1; x >= 0; --x) {
                    //                int x = i -1;
                    //                if(x >0 ) {
                    if (list_bloks[x][j]!=NULL) {
                        // list_bloks[x][j]->setPos((x*50),(j*50)+50);
                        list_bloks[x][j]->setPos(list_bloks[x][j]->pos() + QPointF(0,SIRKA_CTVERCE));
                        //list_bloks[x][j]->setScale(2);
                        //list_bloks[x][j]->setZValue(20);
                        list_bloks[x+1][j]=list_bloks[x][j];
                        list_bloks[x][j] = NULL;
                    }
                }
            }
        }
    }

}



void View::GenerovaniBlokov()
{
    for (int i = 0; i < VYSKA; ++i) {
        for (int j = 0; j < SIRKA; ++j) {
            if (list_bloks[i][j]==NULL) {
                Blok *blok = new Blok(this);
                this->scene()->addItem(blok);
                blok->setPos(j*SIRKA_CTVERCE,i*SIRKA_CTVERCE);
                blok->setBarva(randomBarva());
                blok->setZValue(1);

                list_bloks[i][j]=blok;
                if (list_bloks[i][j]==NULL) {
                    qDebug()<<"pes";
                }
                connect(blok,SIGNAL(BlockMove(QPointF,QPointF)),
                        this, SLOT(OnBlockMove(QPointF,QPointF)));

            }



        }
    }
}

void View::Smazani3()
{
    for (int i = 0; i < VYSKA; ++i) {
        for (int j = 0; j < SIRKA; ++j) {
            if (j<SIRKA-2) {
                if (list_bloks[i][j]!=NULL &&  list_bloks[i][j+1]!=NULL &&  list_bloks[i][j+2]!=NULL ) {

                    if (list_bloks[i][j]->getBarva() == list_bloks[i][j+1]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i][j+2]->getBarva() ) {
                        //qDebug() << "vodorovna";
                        DeleteBlok(i,j);
                        DeleteBlok(i,j+1);
                        DeleteBlok(i,j+2);
                    }
                }
            }
            if (i<VYSKA-2) {

                if (list_bloks[i][j]!=NULL && list_bloks[i+1][j]!=NULL && list_bloks[i+2][j]!=NULL ) {

                    if (list_bloks[i][j]->getBarva() == list_bloks[i+1][j]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i+2][j]->getBarva() ) {
                        //qDebug() << "svisla";
                        DeleteBlok(i,j);
                        DeleteBlok(i+1,j);
                        DeleteBlok(i+2,j);
                    }
                }
            }
        }
    }
}

void View::Smazani4()
{
    for (int i = 0; i < VYSKA; ++i) {
        for (int j = 0; j < SIRKA; ++j) {
            if (j<SIRKA-3) {
                if (list_bloks[i][j]!=NULL &&  list_bloks[i][j+1]!=NULL
                        &&  list_bloks[i][j+2]!=NULL &&  list_bloks[i][j+3]!=NULL  ) {

                    if (list_bloks[i][j]->getBarva() == list_bloks[i][j+1]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i][j+2]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i][j+3]->getBarva()) {
                        //qDebug() << "vodorovna";

                        DeleteBlok(i,j);
                        DeleteBlok(i,j+1);
                        DeleteBlok(i,j+2);
                        DeleteBlok(i,j+3);
                    }
                }
            }
            if (i<VYSKA-3) {

                if (list_bloks[i][j]!=NULL && list_bloks[i+1][j]!=NULL
                        && list_bloks[i+2][j]!=NULL&& list_bloks[i+3][j]!=NULL ) {

                    if (list_bloks[i][j]->getBarva() == list_bloks[i+1][j]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i+2][j]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i+3][j]->getBarva() ) {
                        //qDebug() << "svisla";
                        DeleteBlok(i,j);
                        DeleteBlok(i+1,j);
                        DeleteBlok(i+2,j);
                        DeleteBlok(i+3,j);
                    }
                }
            }
        }
    }
}

void View::Smazani5()
{
    for (int i = 0; i < VYSKA; ++i) {
        for (int j = 0; j < SIRKA; ++j) {
            if (j<SIRKA-4) {
                if (list_bloks[i][j]!=NULL &&  list_bloks[i][j+1]!=NULL
                        &&  list_bloks[i][j+2]!=NULL &&  list_bloks[i][j+3]!=NULL
                        &&  list_bloks[i][j+4]!=NULL ) {

                    if (list_bloks[i][j]->getBarva() == list_bloks[i][j+1]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i][j+2]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i][j+3]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i][j+3]->getBarva()) {
                        //qDebug() << "vodorovna";
                        DeleteBlok(i,j);
                        DeleteBlok(i,j+1);
                        DeleteBlok(i,j+2);
                        DeleteBlok(i,j+3);
                        DeleteBlok(i,j+4);
                    }
                }
            }
            if (i<VYSKA-4) {

                if (list_bloks[i][j]!=NULL && list_bloks[i+1][j]!=NULL
                        && list_bloks[i+2][j]!=NULL && list_bloks[i+3][j]!=NULL
                        && list_bloks[i+4][j]!=NULL) {

                    if (list_bloks[i][j]->getBarva() == list_bloks[i+1][j]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i+2][j]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i+3][j]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i+4][j]->getBarva() ) {
                       // qDebug() << "svisla";
                        DeleteBlok(i,j);
                        DeleteBlok(i+1,j);
                        DeleteBlok(i+2,j);
                        DeleteBlok(i+3,j);
                        DeleteBlok(i+4,j);
                    }
                }
            }
        }
    }
}


void View::GameOver()
{
    gameOverItem->setPlainText("Game over");
    scene()->update();
    timer->stop();
}

bool View::NachaziSe3()
{
    for (int i = 0; i < VYSKA; ++i) {
        for (int j = 0; j < SIRKA; ++j) {
            const char * name = list_bloks[i][j]->getBarva().name().toStdString().c_str();
           // qDebug() << list_bloks[i][j]->getBarva().name();
            //qDebug("%s ", name);
           // printf("%s ",list_bloks[i][j]->getBarva().name().toLatin1().data());
            if (j<SIRKA-2) {
                if (list_bloks[i][j]!=NULL &&  list_bloks[i][j+1]!=NULL &&  list_bloks[i][j+2]!=NULL ) {


                    if (list_bloks[i][j]->getBarva() == list_bloks[i][j+1]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i][j+2]->getBarva() ) {
                        qDebug()<<"trojca";
                        return true;

                    }
                }
            }
            if (i<VYSKA-2) {

                if (list_bloks[i][j]!=NULL && list_bloks[i+1][j]!=NULL && list_bloks[i+2][j]!=NULL ) {

                    if (list_bloks[i][j]->getBarva() == list_bloks[i+1][j]->getBarva() &&
                            list_bloks[i][j]->getBarva() == list_bloks[i+2][j]->getBarva() ) {
                        qDebug()<<"trojca";
                        return true;

                    }
                }
            }

        }
        //qDebug()<<" ";
    }
    qDebug()<<"neni trojca";
    return false;
}

