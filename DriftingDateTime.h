#ifndef DRIFTINGDATETIME_H
#define DRIFTINGDATETIME_H

#include <QObject>
#include <QDateTime>
#include <QMutex>


/**
 * JS8Call allows the user to manipulate the clock.
 *
 * One intention is: If a QSO partner's clock is off too much, we can
 * set our clock to correspond to that partner's clock.  This increases
 * the probability of successful decodes on both sides.
 *
 * Alternatively, we may not be able to set our own computer's clock
 * with appropriate precision.  If so, we can manipulate our
 * JS8Call-internal clock, e.g., based on incoming JS8 signals, to fit
 * other folks' clocks.
 *
 * There is only one, central clock for the entire application.
 *
 * The manipulated clock is ubiquitously used throughout JS8Call.  This
 * includes timestamps logged to ALL.TXT or via Qt logging.  This makes
 * it easier to judge, especially when reading the Qt logs, whether
 * JS8Call's timing is as it should be.
 *
 * There is exactly one object of class DriftingDateTimeObject
 * (singleton pattern).
 *
 * Drift specified as a qint64 is always in ms.
 * A positive drift means that the JS8Call internal clock
 * is that many milliseconds later than the system clock,
 * a negative drift that many milliseconds earlier.
 *
 * This functionality is (intended to be) thread-safe.
 **/

class DriftingDateTimeSingleton: public QObject
{
Q_OBJECT

private:
    DriftingDateTimeSingleton();
    qint64 driftMS;
    QMutex mutex;

    static DriftingDateTimeSingleton singleton;

private:
    void setDriftInner(qint64 ms);

public:
    inline static DriftingDateTimeSingleton & getSingleton() {
        return singleton;
    }

    qint64 drift();

    /** Various ways of retrieving "now": */
    inline QDateTime currentDateTimeUtc() {
        return QDateTime::currentDateTimeUtc().addMSecs(drift());
    }

    inline QDateTime currentDateTimeLocal() {
        return QDateTime::currentDateTime().addMSecs(drift());
    }

    inline qint64 currentMSecsSinceEpoch() {
        return QDateTime::currentMSecsSinceEpoch() + drift();
    }

    inline qint64 currentSecsSinceEpoch() {
        return currentMSecsSinceEpoch() / 1000;
    }

public slots:
    void setDrift(qint64 ms);

signals:
    /**
     * This outgoing signal is the main motivation
     * for introducing this class in the first place:
     */
    void driftChanged(qint64 new_drift);
};
                              

namespace DriftingDateTime
{
    /** Some convenience functions: */
    inline qint64 drift() {
        return DriftingDateTimeSingleton::getSingleton().drift();
    }

    inline void setDrift(qint64 ms) {
        DriftingDateTimeSingleton::getSingleton().setDrift(ms);
    }

    inline QDateTime currentDateTimeUtc() {
        return DriftingDateTimeSingleton::getSingleton().currentDateTimeUtc();
    }

    inline QDateTime currentDateTimeLocal() {
        return DriftingDateTimeSingleton::getSingleton().currentDateTimeLocal();
    }

    inline qint64 currentMSecsSinceEpoch() {
        return DriftingDateTimeSingleton::getSingleton().currentMSecsSinceEpoch();
    }

    inline qint64 currentSecsSinceEpoch() {
        return DriftingDateTimeSingleton::getSingleton().currentSecsSinceEpoch();
    }
};

#endif // DRIFTINGDATETIME_H
