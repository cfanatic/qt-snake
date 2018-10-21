#include "snake.h"
#include "ui_snake.h"

Snake::Snake(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Snake),
    m_state(GAME_INIT),
    m_direct(MOVE_NONE),
    m_timerLeft(new QTimer),
    m_timerRight(new QTimer),
    m_timerUp(new QTimer),
    m_timerDown(new QTimer),
    m_timeTrigger(new QTime),
    m_timeThreshold(new QTime)
{
    ui->setupUi(this);
    ui->labelTime->setText("");
    ui->labelCounter->setVisible(false);

    m_width = this->frameSize().width();
    m_seed = new quint16[m_width / m_step];

    this->setFixedSize(m_width, m_width + 20);

    connect(m_timerLeft, SIGNAL(timeout()), this, SLOT(moveLeft()));
    connect(m_timerRight, SIGNAL(timeout()), this, SLOT(moveRight()));
    connect(m_timerUp, SIGNAL(timeout()), this, SLOT(moveUp()));
    connect(m_timerDown, SIGNAL(timeout()), this, SLOT(moveDown()));
    connect(this, SIGNAL(exit()), this, SLOT(close()));

}

Snake::~Snake()
{
    delete ui;
    delete m_seed;
    delete m_timerLeft;
    delete m_timerRight;
    delete m_timerUp;
    delete m_timerDown;
    delete m_timeTrigger;
    delete m_timeThreshold;
}

void Snake::mousePressEvent(QMouseEvent* event)
{
    Q_UNUSED(event);
    qDebug() << "Mouse pressed.";
    startGame();
}

void Snake::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    qDebug() << "Mouse released.";
}

template<typename T>
T Snake::getNumber(T limit, T offset) const
{
    qsrand(QDateTime::currentMSecsSinceEpoch());
    T number = static_cast<T>(qrand() % limit + offset);
    return number;
}

void Snake::startGame(void)
{
    quint8 time = m_counter;

    ui->labelTime->setVisible(true);
    ui->labelTime->setText(QString::number(m_counter - 1));
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    m_timeTrigger->start();
    m_timeThreshold->restart();

    while (m_timeTrigger->elapsed() <= m_counter * 1000)
    {
        if (m_timeTrigger->elapsed() > (m_counter - 1) * 1000)
        {
            ui->labelTime->setText(QString("Go!"));
        }
        else if (m_timeThreshold->elapsed() < 1000)
        {
            ui->labelTime->setText(QString::number(time - 1));
        }
        else
        {
            time--;
            m_timeThreshold->restart();
        }
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

    ui->labelTime->setVisible(false);
    ui->labelCounter->setVisible(true);
    initGame();
    initTarget();
    m_state = GAME_RUN;

    QKeyEvent *event = new QKeyEvent(QEvent::KeyPress, getNumber(4, 16777234), Qt::NoModifier);
    QCoreApplication::postEvent(this, event);

    update();
}

void Snake::initGame(void)
{
    m_field.reserve((m_width / m_step) * (m_width / m_step));
    for (quint16 i = 1; i <= Snake::m_size; i++)
    {
        m_field.push_back(QPoint(Snake::m_step * i, Snake::m_step));
    }
    for (quint16 i = 1; i < m_width / m_step; i++)
    {
        m_seed[i] = i * 10;
    }
}

void Snake::evalGame(const QPoint &pointNext)
{
    if (m_field.contains(pointNext))
    {
        m_state = GAME_OVER;
        ui->labelCounter->setText(":(");
        m_timerLeft->stop();
        m_timerRight->stop();
        m_timerUp->stop();
        m_timerDown->stop();
    }
    else
    {
        m_state = GAME_RUN;
    }
}

void Snake::initTarget(void)
{
    static quint16 positionPre;
    QPoint topLeft;
    QPoint topRight;
    QPoint bottomLeft;
    QPoint bottomRight;
    quint16 index;
    quint16 position;
    quint16 positionX;
    quint16 positionY;
    bool success;

    success = false;

    do
    {
        do
        {
            index = getNumber(m_width / m_step - 1, 1);
            position = m_seed[index];
        }
        while (position == positionPre);

        positionPre = position;

        topLeft.setX(position);
        topLeft.setY(position);
        topRight.setX(position + 10);
        topRight.setY(position);
        bottomLeft.setX(position);
        bottomLeft.setY(position + 10);
        bottomRight.setX(position + 10);
        bottomRight.setY(position + 10);

        for (m_idx = m_field.begin(); m_idx != m_field.end(); ++m_idx)
        {
            if ((*m_idx) != topLeft && (*m_idx) != topRight && (*m_idx) != bottomLeft && (*m_idx) != bottomRight)
            {
                m_target.setTopLeft(topLeft);
                m_target.setTopRight(topRight);
                m_target.setBottomLeft(bottomLeft);
                m_target.setBottomRight(bottomRight);

                positionX = m_seed[getNumber(m_width / m_step - 1, 1)];
                positionY = m_seed[getNumber(m_width / m_step - 1, 1)];

                m_target.moveTo(positionX - 5, positionY - 5);

                success = true;
                break;
            }
            else
            {
                qDebug() << "Target collision.";
                success = false;
                break;
            }
        }
    }
    while (!success);
}

void Snake::evalTarget(void)
{
    QPoint point;
    quint8 counter;

    if (m_target.contains(m_field.last()))
    {
        counter = ui->labelCounter->text().toUInt();
        counter++;
        ui->labelCounter->setText(QString::number(counter));
        switch(m_direct)
        {
            case MOVE_LEFT:
            {
                point = QPoint(m_field.last().x() - Snake::m_step, m_field.last().y());
            }
            break;
            case MOVE_RIGHT:
            {
                point = QPoint(m_field.last().x() + Snake::m_step, m_field.last().y());
            }
            break;
            case MOVE_UP:
            {
                point = QPoint(m_field.last().x(), m_field.last().y() - Snake::m_step);
            }
            break;
            case MOVE_DOWN:
            {
                point = QPoint(m_field.last().x(), m_field.last().y() + Snake::m_step);
            }
            break;
            case MOVE_NONE:
            {

            }
            break;
        }
        m_field.push_back(point);
        initTarget();
    }
}

void Snake::moveLeft(void)
{
    QPoint point;
    QPoint pointNext;

    evalTarget();

    m_timerUp->stop();
    m_timerDown->stop();

    point = m_field.last();

    if (point.x() > 0 + Snake::m_step)
    {
        pointNext = QPoint(point.x() - Snake::m_step, point.y());
    }
    else
    {
        pointNext = QPoint(m_width - Snake::m_step, point.y());
    }

    evalGame(pointNext);

    if (m_state != GAME_OVER)
    {
        m_field.erase(m_field.begin());
        m_field.push_back(pointNext);
#ifndef SINGLESTEP
        m_timerLeft->start(Snake::m_delay);
#endif
    }
    update();
}

void Snake::moveRight(void)
{
    QPoint point;
    QPoint pointNext;

    evalTarget();

    m_timerUp->stop();
    m_timerDown->stop();

    point = m_field.last();

    if (point.x() < m_width - Snake::m_step)
    {
        pointNext = QPoint(point.x() + Snake::m_step, point.y());
    }
    else
    {
        pointNext = QPoint(0 + Snake::m_step, point.y());
    }

    evalGame(pointNext);

    if (m_state != GAME_OVER)
    {
        m_field.erase(m_field.begin());
        m_field.push_back(pointNext);
#ifndef SINGLESTEP
        m_timerRight->start(Snake::m_delay);
#endif
    }
    update();
}

void Snake::moveUp(void)
{
    QPoint point;
    QPoint pointNext;

    evalTarget();

    m_timerLeft->stop();
    m_timerRight->stop();

    point = m_field[m_field.size()-1];

    if (point.y() > 0 + Snake::m_step)
    {
        pointNext = QPoint(point.x(), point.y() - Snake::m_step);
    }
    else
    {
        pointNext = QPoint(point.x(), m_width - Snake::m_step);
    }

    evalGame(pointNext);

    if (m_state != GAME_OVER)
    {
        m_field.erase(m_field.begin());
        m_field.push_back(pointNext);
#ifndef SINGLESTEP
        m_timerUp->start(Snake::m_delay);
#endif
    }
    update();
}

void Snake::moveDown(void)
{
    QPoint point;
    QPoint pointNext;

    evalTarget();

    m_timerLeft->stop();
    m_timerRight->stop();

    point = m_field.last();

    if (point.y() < m_width - Snake::m_step)
    {
        pointNext = QPoint(point.x(), point.y() + Snake::m_step);
    }
    else
    {
        pointNext = QPoint(point.x(), 0 + Snake::m_step);
    }

    evalGame(pointNext);

    if (m_state != GAME_OVER)
    {
        m_field.erase(m_field.begin());
        m_field.push_back(pointNext);
#ifndef SINGLESTEP
        m_timerDown->start(Snake::m_delay);
#endif
    }
    update();
}

void Snake::keyPressEvent(QKeyEvent *event)
{
    if (m_state == GAME_RUN)
    {
        switch (event->key())
        {
            case Qt::Key_Left:
            {
                if (m_field.first().x() == Snake::m_step)
                {
                    std::reverse(m_field.begin(), m_field.end());
                }
                if (m_direct != MOVE_RIGHT)
                {
                    m_direct = MOVE_LEFT;
                    moveLeft();
                }
            }
            break;
            case Qt::Key_Right:
            {
                if (m_direct != MOVE_LEFT)
                {
                    m_direct = MOVE_RIGHT;
                    moveRight();
                }
            }
            break;
            case Qt::Key_Up:
            {
                if (m_direct != MOVE_DOWN)
                {
                    m_direct = MOVE_UP;
                    moveUp();
                }
            }
            break;
            case Qt::Key_Down:
            {
                if (m_direct != MOVE_UP)
                {
                    m_direct = MOVE_DOWN;
                    moveDown();
                }
            }
            break;
            case Qt::Key_Space:
            {
                m_timerUp->stop();
                m_timerDown->stop();
                m_timerLeft->stop();
                m_timerRight->stop();
                m_direct = MOVE_NONE;
            }
            default:
            {
                m_direct = MOVE_NONE;
            }
            break;
        }
    }
    if (event->key() == Qt::Key_Escape)
    {
        m_state = GAME_EXIT;
        emit exit();
    }
}

void Snake::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    static bool trigger = true;

    m_paint.begin(this);
    m_paint.setRenderHint(QPainter::Antialiasing);

    if (m_state == GAME_INIT)
    {
    }
    else if (m_state == GAME_RUN)
    {
        m_paint.setPen(QPen(Qt::black, Snake::m_step, Qt::SolidLine, Qt::FlatCap));
        m_paint.drawPoints(&m_field[0], m_field.size());
    }
    else if (m_state == GAME_OVER)
    {
        if (trigger)
        {
            m_paint.setPen(QPen(Qt::lightGray, Snake::m_step, Qt::SolidLine, Qt::FlatCap));
            m_paint.drawPoints(&m_field[0], m_field.size());
            trigger = false;
        }
        else
        {
            m_paint.setPen(QPen(Qt::transparent, Snake::m_step, Qt::SolidLine, Qt::FlatCap));
            m_paint.drawPoints(&m_field[0], m_field.size());
            m_paint.eraseRect(m_target);
            trigger = true;
        }
        QTimer::singleShot(500, this, SLOT(update()));
    }

    m_paint.setPen(Qt::red);
    m_paint.setBrush(Qt::red);
    m_paint.drawEllipse(m_target);

    m_paint.end();
}
